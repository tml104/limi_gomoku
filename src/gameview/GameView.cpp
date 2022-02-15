#include "GameView.h"
#include <iostream>
#include "GameBoard.h"
#include <string>
#include "Server.h"

using namespace std;

GameView::GameView()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	return;
}

int GameView::askGameMode()
{
	cout << "Please enter your game mode:" << endl;
	cout << "0: Multiplayer" << endl;
	cout << "1: AI" << endl;
	cout << "-1: Exit" << endl;
	cout << ">>>" <<flush;

	int mode;
	do
	{
		cin >> mode;
	} while (mode != 0 && mode != 1 && mode != -1);
	return mode;
}

void GameView::printGameState(std::shared_ptr<GameBoard> gameBoard)
{
	//first line
	cout << "\t";
	for (int i = 0; i < N; i++)
	{
		cout << i%10;
	}
	cout << endl;

	//next N line
	for (int i = 0; i < N; i++)
	{
		cout<<i%10<<"\t";

		for (int j=0;j<N;j++)
		{
			//black
			if (gameBoard->board[i][j] >0 && gameBoard->board[i][j] % 2 == 1)
			{
				cout << "O";
			}
			//white
			else if (gameBoard->board[i][j] >0 && gameBoard->board[i][j] % 2 == 0)
			{
				cout << "X";
			}
			//none
			else
			{
				cout << "~"; //🔵⚪⚫
			}
		}

		cout << endl;
	}
}

std::pair<int, int> GameView::askPlayerDo()
{
	cout << "It is your move. (x y): Put piece, -1: quit." << endl;

	int x, y;
	cin >> x;
	if (x == -1)
	{
		return {-1,-1};
	}

	cin >> y;


	return { x,y };
}

void GameView::printInfo(std::string info)
{
	cout << info << endl;
}

ReactGameView::ReactGameView()
{
	//todo...
	//important: conn needs to be initiailize
}


//consumers ↓
#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <string>

auto __Utility::split_str(const std::string& s, char split_char)
{
	vector<std::string> res;
	//lambda
	auto string_find_first_not = [](const std::string& s, size_t pos = 0, char split_char = ',') {
		for (size_t i = pos; i < s.size(); i++)
		{
			if (s[i] != split_char && s[i] != ' ' && s[i] != '\t')
				return i;
		}
		return std::string::npos;
	};

	size_t begin_pos = string_find_first_not(s, 0, split_char);
	size_t end_pos = s.find(split_char, begin_pos);

	while (begin_pos != std::string::npos)
	{
		size_t end_pos2 = end_pos - 1;
		while (begin_pos < end_pos2 && (s[end_pos2] == '\t' || s[end_pos2] == ' '))
		{
			end_pos2--;
		}
		res.emplace_back(s.substr(begin_pos, end_pos2 + 1 - begin_pos));
		begin_pos = string_find_first_not(s, end_pos, split_char);
		end_pos = s.find(split_char, begin_pos);
	}
	return res;
}

int ReactGameView::askGameMode0()
{
	const char name[] = "Q1";
	const char recv_cmd_name[] = "A1";
	warp_send(conn, std::string(name));

	//consumers
	std::unique_lock<std::mutex> lk(mvec_lock);
	while (true)
	{
		mvec_cv.wait(lk, [&]() {return mvec.size() > 0; }); //block

		auto str = mvec.front();
		mvec.pop();
		auto str_vec = __Utility::split_str(str);


		if (str_vec.front() == recv_cmd_name)
		{
			int ans = stoi(str_vec[1]);
			return ans;
		}
		else
		{
			while (mvec.size()) mvec.pop();
		}
	}

	return -1;
}

int ReactGameView::askGameMode1()
{
	const char name[] = "Q2";
	const char recv_cmd_name[] = "A2";
	warp_send(conn, std::string(name));

	//consumers
	std::unique_lock<std::mutex> lk(mvec_lock);
	while (true)
	{
		mvec_cv.wait(lk, [&]() {return mvec.size() > 0; }); //block

		auto str = mvec.front();
		mvec.pop();
		auto str_vec = __Utility::split_str(str);


		if (str_vec.front() == recv_cmd_name)
		{
			int ans = stoi(str_vec[1]);
			return ans;
		}
		else
		{
			while (mvec.size()) mvec.pop();
		}
	}

	return -1;
}



void ReactGameView::printGameState(std::shared_ptr<GameBoard> gameBoard)
{
	std::string name = "P ";

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			name += std::to_string(gameBoard->board[i][j]);
			name += " ";
		}
	}

	warp_send(conn, (name));

	//return -1;
}

std::pair<int, int> ReactGameView::askPlayerDo0()
{
	const char name[] = "QM1";
	const char recv_cmd_name[] = "AM1";
	warp_send(conn, std::string(name));

	//consumers
	std::unique_lock<std::mutex> lk(mvec_lock);
	while (true)
	{
		mvec_cv.wait(lk, [&]() {return mvec.size() > 0; }); //block

		auto str = mvec.front();
		mvec.pop();
		auto str_vec = __Utility::split_str(str);


		if (str_vec.front() == recv_cmd_name)
		{
			int ansx = stoi(str_vec[1]);
			int ansy = stoi(str_vec[2]);
			return { ansx,ansy };
		}
		else
		{
			while (mvec.size()) mvec.pop();
		}
	}

	return { -1,-1 };
}

std::pair<int, int> ReactGameView::askPlayerDo1()
{
	const char name[] = "QM2";
	const char recv_cmd_name[] = "AM2";
	warp_send(conn, std::string(name));

	//consumers
	std::unique_lock<std::mutex> lk(mvec_lock);
	while (true)
	{
		mvec_cv.wait(lk, [&]() {return mvec.size() > 0; }); //block

		auto str = mvec.front();
		mvec.pop();
		auto str_vec = __Utility::split_str(str);


		if (str_vec.front() == recv_cmd_name)
		{
			int ansx = stoi(str_vec[1]);
			int ansy = stoi(str_vec[2]);
			return { ansx,ansy };
		}
		else
		{
			while (mvec.size()) mvec.pop();
		}
	}

	return { -1,-1 };
}

void ReactGameView::forceChangeState(int state_id)
{
	std::string resp = "S " + std::to_string(state_id);
	warp_send(conn, resp);
}

void ReactGameView::printInfo(std::string info)
{
	std::string resp = "I " + info;
	warp_send(conn, resp);
}


