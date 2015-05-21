#ifndef GAMELOOP_H
#define GAMELOOP_H
#include "../AlphaStd.h"
#include "..\Time\Clock.h"
#include "..\Time\SystemTime.h"
#include "..\Graphics3D\GraphicsSystem.h"
#include "..\Window\GLWindow.h"
#include "..\Graphics3D\OpenGL\GLRenderer.h"
#include "../EventManager/EventManager.h"
#include "../Actor/RoleSystem.h"
#define MIN_DELTA_MS 1
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
#endif