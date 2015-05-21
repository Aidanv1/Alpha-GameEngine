#include "GameLoop\GameLoop.h"
#include "Memory/MemoryPool.h"
int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1280, 720);
	GameLoop gameLoop;
	gameLoop.Init(window);
	gameLoop.StartLoop();
	return 0;
}
	
