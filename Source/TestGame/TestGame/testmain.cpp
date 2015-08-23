#include "GameLoop\GameLoop.h"

int main(int argc, char *argv[])
{
	GameLoop gameLoop;
	if (gameLoop.Init())
	{
		gameLoop.StartLoop();
	}
	return 0;
}
