#pragma once

#include <vector>
#include <memory>

const int N = 15;

using ull = unsigned long long;

struct GameBoard
{
	int board[N][N];
	//std::shared_ptr<int[N]> board[N];
	ull hashVal;
	std::vector<std::pair<int, int>> piecePos;

	GameBoard();
	static bool checkValidPos(int x, int y);
	int putPiece(int x, int y, int who);
	std::pair<std::pair<int, int>, int> popPiece();
	int fakePopPiece(int x,int y);
	void fakePutPiece(int x, int y, int id);
	int getWho();
	int getWhoPiece(int x,int y);
	void clear();
	bool checkWin(int x, int y, int who);
	bool checkTie();
	void updateHashVal(int x, int y, int who);
};

struct hashTable
{
	hashTable(hashTable const&) = delete;
	hashTable& operator=(hashTable const&) = delete;

	static ull get(int x,int y,int who);

private:
	static ull m_hashTable[N][N][2];
	static bool flag;
	static void __generateTable();
};

