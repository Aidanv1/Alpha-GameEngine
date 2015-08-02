#include "GameLoop\GameLoop.h"
#include "Memory/MemoryPool.h"
#include "Window\GLWindow.h"
int main(int argc, char *argv[])
{
	IWindow* window = ALPHA_NEW GLWindow("AlphaEngine", 1920, 1080);
	GameLoop gameLoop;
	gameLoop.Init(window);
	gameLoop.StartLoop();
	return 0;
}
	
