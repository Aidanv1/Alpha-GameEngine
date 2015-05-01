#ifndef GAMELOOP_H
#define GAMELOOP_H
#include "..\Time\Clock.h"
#include "..\Time\SystemTime.h"
#include "..\Graphics\GraphicsSystem.h"
#ifdef _WIN32
#include "..\Window\GLWindow.h"
#include "..\Graphics\OpenGL\GLRenderer.h"
#endif
class GameLoop
{
public:
	GameLoop();
	~GameLoop();
	bool Init(string name = "AlphaEngine");
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