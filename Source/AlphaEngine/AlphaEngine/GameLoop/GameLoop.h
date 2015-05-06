#ifndef GAMELOOP_H
#define GAMELOOP_H
#include "../AlphaStd.h"
#include "..\Time\Clock.h"
#include "..\Time\SystemTime.h"
#include "..\Graphics\GraphicsSystem.h"
#include "..\Window\GLWindow.h"
#include "..\Graphics\OpenGL\GLRenderer.h"

class GameLoop
{
public:
	GameLoop();
	~GameLoop();
	bool Init(IWindow* window);
	void StartLoop();
private:
	float GetDeltaMs(unsigned __int64& previousSystemTime, unsigned __int64& previousClockTime);
private:
	Clock* m_gameClock;
	unsigned __int64 m_systemTime;
	unsigned __int64 m_gameTime;
	IWindow* m_window;
};
#endif