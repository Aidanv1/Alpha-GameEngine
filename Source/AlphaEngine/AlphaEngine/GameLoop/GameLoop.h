#pragma once
#include "../AlphaEngineDLL.h"
//Forward Declaration
class Clock;
class IWindow;
class EventManager;
//========================================================================
#define MIN_DELTA_MS 4
//========================================================================
class DLLExport GameLoop
{
public:
	GameLoop();
	~GameLoop();
	bool Init();
	void StartLoop();
private:
	float GetDeltaMs(unsigned __int64& previousSystemTime, unsigned __int64& previousClockTime);
private:
	Clock* m_gameClock;
	unsigned __int64 m_systemTime;
	unsigned __int64 m_gameTime;
	IWindow* m_window;
	EventManager* m_globalEventManager;
};
//========================================================================