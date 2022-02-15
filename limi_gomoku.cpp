// limi_gomoku.cpp: 定义应用程序的入口点。
//

#include "limi_gomoku.h"
#include "GameController.h"

using namespace std;

void work()
{
	GameController gc;

	gc.start();
}

int main()
{
	cout << "Hello limi_gomoku." << endl;

	work();

	return 0;
}
