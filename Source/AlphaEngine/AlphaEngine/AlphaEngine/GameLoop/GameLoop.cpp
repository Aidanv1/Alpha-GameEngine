#include "GameLoop.h"

GameLoop::GameLoop()
{
}
GameLoop::~GameLoop()
{
	delete m_window;
	delete m_gameClock;
}
bool GameLoop::Init(string name)
{
	
	#ifdef _WIN32
	//Window
	m_window = ALPHA_NEW GLWindow(name.c_str(), 1280, 720);
	if (!m_window->VInit())
	{
		return false;
	}
	//Renderer
	shared_ptr<IRenderer> renderer(new GLRenderer());
	#endif
	//graphics system
	if (!GraphicsSystem::Get().Init(renderer, 100, 100))
	{
		return false;
	}

	//initialize gameclock
	m_gameClock = ALPHA_NEW Clock();
	ClockManager::Get().AddClock(m_gameClock);
	m_systemTime = SystemTime::Get().GetTimeInMicroSeconds();
	m_gameTime = 0;

	return true;
}
void GameLoop::StartLoop()
{
	bool quit = false;
	while (!quit)
	{
		float dt = GetDeltaMs(m_systemTime, m_gameTime);
		GraphicsSystem::Get().Update();
		quit = !m_window->VUpdate();
	}
}

float GameLoop::GetDeltaMs(unsigned __int64& previousSystemTime, unsigned __int64& previousClockTime)
{
	unsigned __int64 timenext = SystemTime::Get().GetTimeInMicroSeconds();
	ClockManager::Get().UpdateClocks((timenext - previousSystemTime));
	previousSystemTime = timenext;
	unsigned __int64 nextime = m_gameClock->GetTimeMicroSec();
	float deltatime = nextime - previousClockTime;
	previousClockTime = nextime;
	return deltatime / MS_PER_MICROSECOND;
}
