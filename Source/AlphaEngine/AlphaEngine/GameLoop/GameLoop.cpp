#include "GameLoop.h"

GameLoop::GameLoop() :
m_globalEventManager("Global", true),
m_window(NULL),
m_gameClock(NULL),
m_systemTime(0),
m_gameTime(0)
{
}
GameLoop::~GameLoop()
{
	SAFE_DELETE(m_window);
	SAFE_DELETE(m_gameClock);
}
bool GameLoop::Init(IWindow* window)
{
	//Window
	m_window = window;
	if (!m_window->VInit())
	{
		return false;
	}
	//Renderer
	shared_ptr<IRenderer> renderer(ALPHA_NEW GLRenderer());
	//graphics system
	if (!GraphicsSystem::Get().Init(renderer, 100, 100))
	{
		return false;
	}

	//initialize gameclock
	m_gameClock = ALPHA_NEW Clock();
	ClockManager::Get().AddClock(m_gameClock);
	m_systemTime = SDL_GetTicks();
	m_gameTime = 0;
	return true;
}
void GameLoop::StartLoop()
{
	float dt = 0;
	bool quit = false;
	while (!quit)
	{		
		GraphicsSystem::Get().Update(dt);
		quit = !m_window->VUpdate(dt);
		m_globalEventManager.VUpdate();		
		while (dt == 0)
		{
			dt = GetDeltaMs(m_systemTime, m_gameTime);
		}
	}
}

float GameLoop::GetDeltaMs(unsigned __int64& previousSystemTime, unsigned __int64& previousClockTime)
{
	unsigned __int64 timenext = SDL_GetTicks();
	ClockManager::Get().UpdateClocks((timenext - previousSystemTime));
	previousSystemTime = timenext;
	unsigned __int64 nextime = m_gameClock->GetTimeMilliSec();
	float deltatime = nextime - previousClockTime;
	previousClockTime = nextime;
	return deltatime;
}
