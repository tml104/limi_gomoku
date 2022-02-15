#pragma once

#include "GameBoard.h"
#include <map>
#include <unordered_map>
#include <memory>

using ll = long long;
using ull = unsigned long long;
using gb_pt = std::shared_ptr<GameBoard>;


const ll HUO_DRAGON_SCORE[6] = {
	0,
	2,
	250000000ll,
	500000000ll, //5e8
	1000000000ll, //1e9
	1000000000000000ll //1e15
};

//						 1000000000
//						 1000000000000000
const ll WIN_THRESHOLD = 1000000000000ll; //1e12
const ll PREWIN_THRESHOLD = 200000000000ll; //2e11

const ll FAST_THRESHOLD = 900000000ll; //9e8

const ll ALPHA_INIT = -PREWIN_THRESHOLD;
const ll BETA_INIT = PREWIN_THRESHOLD;
const ll ACT_INIT = 1e17;
extern int LIMIT_DEPTH;
extern int LIMIT_WIDTH;

const ll CHONG_DRAGON_SCORE[6] = {
	0,
	1,
	100,
	100000,
	500000000ll, //5e8
	1000000000000000ll //invaild val
};

struct DragonStatic
{
	int dragon[2][2][6]; //[black(0)/white(1)][huo(0)/chong(1)][1~5 cnt]
	int whoNext;
};

struct State
{
	DragonStatic dragonState;
	ll estimatedScore;
	ll actualScore;
	std::pair<int, int> ans;
	int pieceCnt;
	ull hashVal;
	ull ansHashVal;
	std::map<std::pair<int, int>, ll> nextStepMap[2];
	std::vector<std::pair<std::pair<int, int>, ll>> nextStepVec;
	//void __updateDragon(int x,int y);
	//void __updateEstimatedScore();

	//void init(gb_pt tmpGameBoard);

	State():
		dragonState(),
		estimatedScore(),
		actualScore(),
		ans(),
		pieceCnt(),
		hashVal(),
		ansHashVal(),
		nextStepMap(),
		nextStepVec()
	{} //must initialize all members
	State(const State& b);

	void updateEstimatedScore(gb_pt tmpGameBoard,int x,int y);
	void updateNextStepMap(gb_pt tmpGameBoard, int x, int y);
};

using st_pt = std::shared_ptr<State>;

using StateMap = std::unordered_map<ull, std::pair<st_pt,int>>;

ll judgeDragon_B(int piece_array[],int dragonState[2][2][6]=nullptr);
ll judgeDragon_A_0(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6] = nullptr);
ll judgeDragon_A_1(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6] = nullptr);
ll judgeDragon_A_2(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6] = nullptr);
ll judgeDragon_A_3(gb_pt tmpGameBoard, int x, int y, int dragonState[2][2][6] = nullptr);
ll judgeDragon_A(gb_pt tmpGameBoard, int x, int y, int mask=15, int dragonState[2][2][6] = nullptr);

#include <iostream>
struct Static
{
	int search_cnt;
	int alpha_beta_cnt;
	int already_exist_cnt;
	int state_searched_cnt;
	int fast_search_cnt;
	int pre_ret;

	Static(): 
		search_cnt(0), 
		alpha_beta_cnt(0), 
		already_exist_cnt(0), 
		state_searched_cnt(0),
		fast_search_cnt(0),
		pre_ret(0)
	{}

	void printCnt()
	{
		std::cerr << "search_cnt:" << search_cnt << std::endl;
		std::cerr << "fast_search_cnt:" << fast_search_cnt << std::endl;
		std::cerr << "alpha_beta_cnt:" << alpha_beta_cnt << std::endl;
		std::cerr << "already_exist_cnt:" << already_exist_cnt << std::endl;
		std::cerr << "state_searched_cnt:" << state_searched_cnt << std::endl;
		std::cerr << "pre_ret:" << pre_ret << std::endl;
	}

};

st_pt Search_B(
	int depth,
	int search_id,
	StateMap &stateMap,
	gb_pt tmpGameBoard, 
	st_pt fatherState,
	ll alpha, 
	ll beta, 
	ll limitDepth, 
	int newx, 
	int newy,
	int is_min,
	int fast_search,
	Static &stc
);

std::pair<st_pt, std::pair<int,int>> Search_A(
	int search_id,
	StateMap &stateMap,
	gb_pt tmpGameBoard,
	st_pt rootState,
	int is_white
);


ll calculateEstimatedScoreByDragonStatic(DragonStatic& ds);

void bruteforceCheckDragon(DragonStatic &ds, gb_pt &gb);