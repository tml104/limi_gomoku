#pragma once

//#include "GameBoard.h"
#include <memory>
//#include "Server.h"
#include "websocket.h"

class GameBoard;

class BaseView {};

//interface
class I_CmdView : public BaseView
{
public:
	virtual int askGameMode() = 0;
	virtual void printGameState(std::shared_ptr<GameBoard> gameBoard) = 0;
	virtual std::pair<int, int> askPlayerDo() = 0;
	virtual void printInfo(std::string info) = 0;
};

class GameView : public I_CmdView
{
public:
	GameView();
	int askGameMode() override;
	void printGameState(std::shared_ptr<GameBoard> gameBoard) override;
	std::pair<int,int> askPlayerDo() override;
	void printInfo(std::string info) override;
};


class Server;
using WSServer = websocket::WSServer<Server>;
using WSConn = WSServer::Connection;

class ReactGameView : public BaseView
{
public:
	

	WSConn* conn;

	ReactGameView();
	int askGameMode0();
	int askGameMode1();
	void printGameState(std::shared_ptr<GameBoard> gameBoard) ;
	std::pair<int, int> askPlayerDo0();
	std::pair<int, int> askPlayerDo1();
	void forceChangeState(int state_id);
	void printInfo(std::string info) ;
};

namespace __Utility
{
    //·Ö¸î×Ö·û´®
	auto split_str(const std::string& s, char split_char = ' ');
}