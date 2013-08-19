#include <sstream>
#include "board.h"

bool isPuttable(const Board & board,pos_t pos,state_t state)
{
	if(board[pos] != NONE)
		return false;
	else
	{
		for(auto d : dir)
		{
			bool isptb = false;
			pos_t q = pos;
			while(1)
			{
				q += d;
				if(board[q] == state)
				{
					if(isptb)
						return true;
					break;
				}
				else if(board[q] == invertState(state))
					isptb=true;
				else
					break;
			}
		}
		return false;
	}
}

std::vector<pos_t> getPuttable(const Board & board,state_t state)
{
	std::vector<pos_t> ans;
	ans.reserve(16);
	for(int i = 0;i < 8;++i)
	{
		for(int j = 0;j < 8;++j)
		{
			pos_t pos = xyToPos(i,j);
			if(isPuttable(board,pos,state))
				ans.push_back(pos);
		}
	}
	return ans;
}

std::vector<pos_t> getPuttable(const Board & board,state_t state,const std::vector<pos_t> & nonevec)
{
	std::vector<pos_t> ans;
	ans.reserve(16);
	for(auto pos : nonevec)
		if(isPuttable(board,pos,state))
			ans.push_back(pos);
	return ans;
}

Board put(const Board & board,pos_t pos,state_t state)
{
	Board ans(board);
	ans[pos] = state;
	for(auto d : dir)
	{
		bool isptb = false;
		pos_t q = pos;
		while(1)
		{
			q += d;
			if(ans[q] == state)
			{
				if(isptb)
				{
					q -= d;
					while(q != pos)
					{
						ans[q] = state;
						q -= d;
					}
				}
				break;
			}
			else if(ans[q] == invertState(state))
				isptb=true;
			else
				break;
		}
	}
	return ans;
}

std::vector<pos_t> getUnput(const Board & board)
{
	std::vector<pos_t> ans;
	for(int i = 0;i < 8;++i)
	{
		for(int j = 0;j < 8;++j)
		{
			pos_t pos = xyToPos(i,j);
			if(board[pos] == NONE)
				ans.push_back(pos);
		}
	}
	return ans;
}

std::string toStr(const Board & board)
{
	std::stringstream ss;
	for(int i = 0;i < 8;++i)
	{
		for(int j = 0;j < 8;++j)
		{
			if(j)ss << ' ';
			pos_t pos = xyToPos(i,j);
			switch(board[pos])
			{
				case NONE:
					ss << '.';
					break;
				case BLACK:
					ss << 'x';
					break;
				case WHITE:
					ss << 'o';
					break;
				default:;
			}
		}
		ss << '\n';
	}
	return ss.str();
}

Board toBoard(const std::string & str)
{
	std::stringstream ss;
	ss << str;
	char buf[20];
	ss.getline(buf,16);
	Board bd(DefaultBoard);
	for(int i = 0;i < 8;++i)
	{
		ss.getline(buf,16);
		for(int j = 0;j < 8;++j)
		{
			switch(buf[j*2])
			{
				case 'x':
					bd[xyToPos(i,j)] = BLACK;
					break;
				case 'o':
					bd[xyToPos(i,j)] = WHITE;
					break;
				case '.':
					bd[xyToPos(i,j)] = NONE;
					break;
			}
		}
	}
	return bd;
}

std::pair<int,int> countBoard(const Board & board)
{
	int b = 0;
	int w = 0;
	for(int i = 0;i < 8;++i)
	{
		for(int j = 0;j < 8;++j)
		{
			switch(board[xyToPos(i,j)])
			{
				case BLACK:
					++b;
					break;
				case WHITE:
					++w;
					break;
				default:;
			}
		}
	}
	return std::make_pair(b,w);
}
