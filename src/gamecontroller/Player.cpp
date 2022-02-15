#include <memory>
#include "GameBoard.h"
#include "Player.h"
#include "GameView.h"
#include "GameController.h"

Player::Player(int playerType, int color) : playerType(playerType), color(color) {}

std::pair<int, int> UserPlayer::askPlayer(GameController* gameController)
{
	if(gameController->gameBoard->getWho() == 0)
		return gameController->gameView->askPlayerDo0();
	else
		return gameController->gameView->askPlayerDo1();
}

AIPlayer::AIPlayer(int playerType, int color) : Player(playerType, color)
{
	rootState = std::make_shared<State>(); 
	stateMap.clear();
	stateMap[0] = { rootState ,0};
	rootState->ans = { -1,-1 };
	search_id = 0;
}

#include <iostream>
#include <cstdio>
#include <assert.h>
std::pair<int, int> AIPlayer::askPlayer(GameController* gameController)
{
	search_id++;

	auto return_pair = Search_A(
		search_id,
		stateMap,
		gameController->gameBoard,
		rootState,
		color
	);

	rootState = return_pair.first;

	auto pos = return_pair.second;
	gameController->gameBoard->putPiece(pos.first, pos.second, gameController->gameBoard->getWho());
	assert(rootState->hashVal == gameController->gameBoard->hashVal);
	gameController->gameBoard->popPiece();

	cleanStateMap();
	std::cerr << "ACT SCORE:" << rootState->actualScore << std::endl;
	std::cerr << "EST SCORE:" << rootState->estimatedScore << std::endl;
	std::cerr << "MAP SIZE:"<< stateMap.size() << std::endl;

	char buf[200] = "";
	sprintf(buf, "ACT SCORE: %lld\n", rootState->actualScore);
	gameController->gameView->printInfo(buf);

	sprintf(buf, "EST SCORE: %lld\n", rootState->estimatedScore);
	gameController->gameView->printInfo(buf);

	sprintf(buf, "MAP SIZE: %lld\n", (ll)stateMap.size());
	gameController->gameView->printInfo(buf);

	sprintf(buf, "ANS: %d %d\n ---- \n", return_pair.second.first, return_pair.second.second);
	gameController->gameView->printInfo(buf);
	

	return return_pair.second;
}

void AIPlayer::cleanStateMap()
{
	std::vector<ull> index_vec;

	int now_cnt = rootState->pieceCnt;

	//if over 300000 then clear
	if (stateMap.size()>300000)
	{
		auto rootState_hashVal = rootState->hashVal;
		auto search_id = stateMap[rootState_hashVal].second;

		stateMap.clear();
		stateMap[rootState_hashVal] = { rootState,search_id };

		std::cerr << "MAP CLEANED." << std::endl;

		return;
	}

	for (auto it : stateMap)
	{
		if ((it.second.first)->pieceCnt < now_cnt)
		{
			index_vec.emplace_back(it.first);
		}
	}

	for (auto v : index_vec)
	{
		stateMap.erase(v);
	}
}

//auto PlayerFactory::getPlayer(int playerType, int color)
//{
//	if (playerType == 1)
//		return std::make_shared<AIPlayer>(playerType, color);
//	return std::make_shared<UserPlayer>(playerType, color);
//}
