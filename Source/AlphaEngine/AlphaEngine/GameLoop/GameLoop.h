#pragma once
#include "../AlphaStd.h"
#include "../EventManager/EventManager.h"
//Forward Declaration
class Clock;
class IWindow;
//========================================================================
#define MIN_DELTA_MS 4
//========================================================================
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
	EventManager m_globalEventManager;
};
//========================================================================