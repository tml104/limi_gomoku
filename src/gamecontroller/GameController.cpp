#include "GameController.h"
#include "GameBoard.h"
#include "Player.h"
#include "GameView.h"
#include "Server.h"

//Check this move is valid
bool GameController::checkValidMove(int x, int y)
{
	if (gameBoard->getWhoPiece(x, y) == 2) //none
	{
		return true;
	}
	return false;
}

bool GameController::checkWin(int x, int y, int who)
{
	return gameBoard->checkWin(x,y,who);
}

bool GameController::checkTie()
{
	return gameBoard->checkTie();
}

//Lobby: ask for playmode
int GameController::state0()
{
	gameFlag = 0;
	playMode = 0;
	gameBoard->clear();

	playMode = gameView->askGameMode0();
	if (playMode == -1) return -1;

	if (playMode == 0)
	{
		//for (int i = 0; i < 2; i++)
		//{
		//	//players[i] = std::make_shared<Player>(playMode,i);
		//	players[i] = PlayerFactory::getPlayer(playMode, i);
		//}

		players[0] = std::make_shared<UserPlayer>(playMode, 0);
		//players[1] = std::make_shared<UserPlayer>(playMode, 1);
	}
	else
	{
		players[0] = std::make_shared<AIPlayer>(playMode, 0);
		//players[1] = std::make_shared<AIPlayer>(playMode, 1);
	}

	playMode = gameView->askGameMode1();
	if (playMode == -1) return -1;

	if (playMode == 0)
	{
		//for (int i = 0; i < 2; i++)
		//{
		//	//players[i] = std::make_shared<Player>(playMode,i);
		//	players[i] = PlayerFactory::getPlayer(playMode, i);
		//}

		//players[0] = std::make_shared<UserPlayer>(playMode, 0);
		players[1] = std::make_shared<UserPlayer>(playMode, 1);
	}
	else
	{
		//players[0] = std::make_shared<UserPlayer>(playMode, 0);
		players[1] = std::make_shared<AIPlayer>(playMode, 1);
	}


	return 0;
}

//Game playing

#ifdef SHOW_TIME

	#include <time.h>

#endif
int GameController::state1()
{
#ifdef SHOW_TIME
	clock_t start, end;//定义clock_t变量
	start = clock();  //开始时间
#endif

	while (true)
	{
		
		//player ask
		int now_who = gameBoard->getWho();


		while (true)
		{
			//print out game
			gameView->printGameState(gameBoard);
			auto do_res = players[now_who]->askPlayer(this);

			if (do_res.first == -1) //player quit
			{
				return -1;
			}

			if (checkValidMove(do_res.first, do_res.second))
			{
				//put piece
				gameBoard->putPiece(do_res.first, do_res.second, now_who);
				break;
			}
			else
			{
				gameView->printInfo("InValid Move.");
			}
		}
		
		auto do_res = gameBoard->piecePos.back();
		if (checkWin(do_res.first, do_res.second, now_who))
		{
			gameFlag = now_who;
			break;
		}

		if (checkTie())
		{
			gameFlag = 2; //Tie
			break;
		}
		
	}


#ifdef SHOW_TIME
	end = clock();   //结束时间
	std::cerr << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << std::endl;  //输出时间（单位：ｓ）
#endif

	return 0;
}

int GameController::state2()
{
	gameView->printGameState(gameBoard);

	if (gameFlag == 2)
	{
		gameView->printInfo("Tie.");
	}
	else if (gameFlag == 0)
	{
		gameView->printInfo("Black win.");
	}
	else if(gameFlag == 1)
	{
		gameView->printInfo("White win.");
	}

	gameBoard->clear();

	return 0;
}

GameController::GameController()
{
	gameBoard = std::make_shared<GameBoard>();
	gameServer = std::make_shared<Server>();
	gameView = std::make_shared<ReactGameView>();
	gameFlag = 0;
	playMode = 0;

	gameServer->run();

	//tmp for connection
	//int x;
	//std::cout << "C:" << std::endl;
	//std::cin >> x;
	//std::cout << "CA!" << std::endl;
	while (gameServer->conn == nullptr)
	{
		sleep(2);
		std::cout << "Waiting Frontend to connect..." << std::endl;
	}
	std::cout << "Connected! :)" << std::endl;



	gameView->conn = gameServer->conn;
}

void GameController::start()
{
	while (true)
	{
		if(state0()==-1) continue;
		if(state1()==-1) continue;
		if(state2() == -1) continue;
	}
}
