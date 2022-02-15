#pragma once

#include <memory>
//#include "GameBoard.h"
//#include "Player.h"
//#include "GameView.h"


struct GameBoard;
class GameView;
class ReactGameView;
class Player;
class Server;

class GameController
{
public:
	int gameFlag;
	int playMode;
	std::shared_ptr<GameBoard> gameBoard;
	std::shared_ptr<ReactGameView> gameView;
	std::shared_ptr<Server> gameServer;
	std::shared_ptr<Player> players[2];

	bool checkWin(int x, int y, int who);
	bool checkTie();
	bool checkValidMove(int x,int y);
	int state0();
	int state1();
	int state2();

	GameController();
	void start();
};