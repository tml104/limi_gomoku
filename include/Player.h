#pragma once

//#include "GameView.h"
#include <memory>
#include "AIComponent.h"
//#include "GameController.h"

class GameController;

class Player
{
public:
	//wheater player is ai. playerType=1 is ai.
	int playerType; 
	//0: black, 1: white
	int color;

	Player(int playerType, int color);
	virtual std::pair<int,int> askPlayer(GameController *gameController) = 0;
};

class UserPlayer : public Player
{
public:
	UserPlayer(int playerType, int color): Player(playerType, color) {};
	std::pair<int, int> askPlayer(GameController* gameController) override;
};

class AIPlayer: public Player
{
	StateMap stateMap;
	st_pt rootState;
	int search_id;
	void cleanStateMap();
public:
	AIPlayer(int playerType, int color) ;
	std::pair<int, int> askPlayer(GameController* gameController) override;


};

//class PlayerFactory
//{
//public:
//	static auto getPlayer(int playerType, int color);
//};