#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "board.h"

char toBase64(unsigned int n)
{
	if(n < 26)
		return n + 'A';
	else if(n < 52)
		return n - 26 + 'a';
	else if(n < 62)
		return n - 52 + '0';
	else if(n == 62)
		return '+';
	else if(n == 63)
		return '/';
	return '\0';
}

constexpr timeval timelimit = {1,0};

int main(int argc,char** argv)
{
	if(argc < 3)
	{
		fprintf(stderr,"error: no input programs\nusage:\nothello PROG1 PROG2\n");
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	else
	{
		Board board(DefaultBoard);
		state_t state = BLACK;
		bool flag = false;
		state_t fault = NONE;
		const char * const PIPENAME = "othello.pipe";
		int e = mkfifo(PIPENAME,0666);
		std::string fault_detail = "";
		std::string record;
		char buf[1024];
		char* ret = getcwd(buf,1024);
		if(ret == nullptr)
		{
			std::cout<<"too long path name"<<std::endl;
			exit(EXIT_FAILURE);
		}
		while(1)
		{
			auto v = getPuttable(board,state);
			if(v.empty())
			{
				if(flag)
					break;
				else
				{
					flag = true;
					state = invertState(state);
					continue;
				}
			}
			char* str = (char*)malloc(strlen(argv[state])+11);
			sprintf(str,"%s > %s",argv[state],PIPENAME);
			FILE * pp = popen(str,"w");
			timeval s,e,d;
			gettimeofday(&s,NULL);
			std::string bstr = toStr(board);
			fprintf(pp,"%c\n%s",(state==BLACK)?'x':'o',bstr.c_str());
			printf("%c\n%s\n",(state==BLACK)?'x':'o',bstr.c_str());
			FILE * fp = fopen(PIPENAME,"r");
			pclose(pp);
			gettimeofday(&e,NULL);
			timersub(&e,&s,&d);
			if(timercmp(&d,&timelimit,>))
			{
				fault = state;
				fault_detail = "Time Limit Exceeded";
				break;
			}
			printf("%ldus\n",d.tv_usec);
			int x,y;
			fscanf(fp,"%d%d",&x,&y);
			fclose(fp);
			if(std::find(v.begin(),v.end(),xyToPos(x,y)) == v.end())
			{
				fault = state;
				fault_detail = "Wrong Output";
				break;
			}
			board = put(board,xyToPos(x,y),state);
			record.append(1,toBase64(x*8+y));
			state = invertState(state);
			flag = false;
		}
		printf("--<Game  Set>--\n%s\n",toStr(board).c_str());
		std::pair<int,int> p;
		switch(fault)
		{
			case NONE:
				p = countBoard(board);
				if(p.first > p.second)
					puts("Winner: Black");
				else if(p.first < p.second)
					puts("Winner: White");
				else
					puts("Draw");
				printf("Black: %2d White: %2d\n",p.first,p.second);
				printf("game record:%s\n",record.c_str());
				break;
			case BLACK:
				puts("Winner: White");
				puts("Black: Foul");
				puts(fault_detail.c_str());
				break;
			case WHITE:
				puts("Winner: Black");
				puts("White: Foul");
				puts(fault_detail.c_str());
				break;
			default:;
		}
		remove(PIPENAME);
	}
	return 0;
}
