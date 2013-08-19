#pragma once
#include <array>
#include <vector>
#include <string>
#include <utility>

enum state_t : int
{
	NONE,
	BLACK,
	WHITE,
	WALL
};

typedef int pos_t;

constexpr pos_t A1 = 11,B1 = 12,C1 = 13,D1 = 14,E1 = 15,F1 = 16,G1 = 17,H1 = 18;
constexpr pos_t A2 = 21,B2 = 22,C2 = 23,D2 = 24,E2 = 25,F2 = 26,G2 = 27,H2 = 28;
constexpr pos_t A3 = 31,B3 = 32,C3 = 33,D3 = 34,E3 = 35,F3 = 36,G3 = 37,H3 = 38;
constexpr pos_t A4 = 41,B4 = 42,C4 = 43,D4 = 44,E4 = 45,F4 = 46,G4 = 47,H4 = 48;
constexpr pos_t A5 = 51,B5 = 52,C5 = 53,D5 = 54,E5 = 55,F5 = 56,G5 = 57,H5 = 58;
constexpr pos_t A6 = 61,B6 = 62,C6 = 63,D6 = 64,E6 = 65,F6 = 66,G6 = 67,H6 = 68;
constexpr pos_t A7 = 71,B7 = 72,C7 = 73,D7 = 74,E7 = 75,F7 = 76,G7 = 77,H7 = 78;
constexpr pos_t A8 = 81,B8 = 82,C8 = 83,D8 = 84,E8 = 85,F8 = 86,G8 = 87,H8 = 88;

constexpr std::array<int,8> dir{{-11,-10,-9,-1,1,9,10,11}};

typedef std::array<state_t,100> Board;

constexpr Board DefaultBoard{{
 WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE,WHITE,BLACK, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE,BLACK,WHITE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, WALL,
 WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL
}};

constexpr pos_t xyToPos(int x,int y){return x*10+y+11;}
constexpr std::pair<int,int> posToXY(pos_t pos){return std::make_pair((pos-11)/10,(pos-11)%10);}
constexpr state_t invertState(state_t state){return static_cast<state_t>(WALL-state);}
bool isPuttable(const Board &,pos_t,state_t);
std::vector<pos_t> getPuttable(const Board &,state_t);
std::vector<pos_t> getPuttable(const Board &,state_t,const std::vector<pos_t> &);
std::vector<pos_t> getUnput(const Board &);
Board put(const Board &,pos_t,state_t);
std::string toStr(const Board &);
Board toBoard(const std::string &);
std::pair<int,int> countBoard(const Board &);
