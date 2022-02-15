#include "GameBoard.h"
#include <assert.h>

GameBoard::GameBoard()
{
	clear();
}

//Check wheater pos is out of bound
bool GameBoard::checkValidPos(int x, int y)
{
	if (
		!(
			(0 <= x && x < N) && (0 <= y && y < N)
			)
		)
	{
		return false;
	}

	return true;
}

int GameBoard::putPiece(int x, int y, int who=-1)
{
	//optional check
	if (who != -1)
	{
		//0: black turn, 1:white turn
		assert(piecePos.size() % 2 == who);
		if (piecePos.size() % 2 != who)
		{
			return -1;
		}
	}

	if (checkValidPos(x,y)==false)
	{
		return -1;
	}

	//already has piece
	if (board[x][y]>0)
	{
		return -1;
	}

	board[x][y] = piecePos.size() + 1;
	piecePos.emplace_back(x,y);

	updateHashVal(x, y, getWhoPiece(x, y));

	return 0;
}

std::pair<std::pair<int, int>, int> GameBoard::popPiece()
{
	if (piecePos.size() == 0)
	{
		return { {-1,-1},-1 };
	}

	std::pair<int,int> pos = piecePos.back();
	int who = getWhoPiece(pos.first, pos.second);
	board[pos.first][pos.second] = 0;
	piecePos.pop_back();
	updateHashVal(pos.first, pos.second, who);
	
	return  { pos,who };
}

int GameBoard::fakePopPiece(int x, int y)
{
	auto ans = board[x][y];
	board[x][y] = 0;
	return ans;
}

void GameBoard::fakePutPiece(int x, int y, int id)
{
	board[x][y] = id;
}


//Who's turn: 0:black, 1:white
int GameBoard::getWho()
{
	return (piecePos.size())%2;
}


//0:black, 1:white, 2:none , -1:out of bound
int GameBoard::getWhoPiece(int x, int y)
{
	if (checkValidPos(x, y)==false)
	{
		return -1;
	}

	if (board[x][y]==0)
	{
		return 2;
	}

	return (board[x][y]+1)%2;
}

#include <memory.h>
void GameBoard::clear()
{
	//std::fill(board, board + N * N, 0);
	memset(board, 0, sizeof(board));
	piecePos.clear();
	hashVal = 0;
}

//Check who win at (x,y)
bool GameBoard::checkWin(int x, int y, int who)
{
	int who_piece = getWhoPiece(x, y);
	if (who_piece == who)
	{
		//vertical
		auto check_vertical = [&]() {
			int cnt = 0;

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx--)
			{
				cnt++;
			}

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx++)
			{
				cnt++;
			}

			return cnt - 1 >= 5;
		};

		//horizonal
		auto check_horizonal = [&]() {
			int cnt = 0;

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; yy--)
			{
				cnt++;
			}

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; yy++)
			{
				cnt++;
			}

			return cnt - 1 >= 5;
		};


		//diagonal1
		auto check_diagonal1 = [&]() {
			int cnt = 0;

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx--, yy--)
			{
				cnt++;
			}

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx++, yy++)
			{
				cnt++;
			}

			return cnt - 1 >= 5;
		};

		//diagonal2
		auto check_diagonal2 = [&]() {
			int cnt = 0;

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx--, yy++)
			{
				cnt++;
			}

			for (int xx = x, yy = y; getWhoPiece(xx, yy) == who_piece; xx++, yy--)
			{
				cnt++;
			}

			return cnt - 1 >= 5;
		};


		return check_vertical() || check_horizonal() || check_diagonal1() || check_diagonal2();
	}


	return false;
}

//Check whether this state is tie
bool GameBoard::checkTie()
{	
	return piecePos.size() == N * N;
}


void GameBoard::updateHashVal(int x, int y, int who)
{
	if (checkValidPos(x, y))
	{
		hashVal ^= hashTable::get(x, y, who);
	}
}

bool hashTable::flag = false;
ull hashTable::m_hashTable[N][N][2];

ull hashTable::get(int x, int y, int who)
{
	if (flag == false)
	{
		__generateTable();
		flag = true;
	}

	return hashTable::m_hashTable[x][y][who];
}

#include <unordered_set>
#include <ctime>
#include <random>
void hashTable::__generateTable()
{
	std::unordered_set<ull> s;
	std::mt19937_64 rnd(time(0));
	for (int i=0;i<N;i++)
	{
		for (int j = 0; j < N; j++)
		{
			for (int k = 0; k <= 1; k++)
			{
				ull random_number = 0;
				do
				{
					random_number = rnd();
				} while (s.count(random_number));

				s.insert(random_number);
				hashTable::m_hashTable[i][j][k] = random_number;
			}
		}
	}
}

