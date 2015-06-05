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
	shared_ptr<IRenderer> renderer(ALPHA_NEW Renderer_GL());
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


	//Populate Actors
	TiXmlDocument doc;
	doc.LoadFile("Actors.xml");
	TiXmlElement* elem = doc.FirstChildElement();	
	RoleSystem::Get().Populate(elem);

	//Load Graphics
	GraphicsSystem::Get().LoadScene();
	return true;
}
void GameLoop::StartLoop()
{
	float dt = 0;
	bool quit = false;
	unsigned int cycleCount = 0;
	while (!quit)
	{

		//display delta time
		//if (cycleCount%100 == 0)
		//{
		//	stringstream printText;
		//	printText << "FPS: " << to_string(MS_PER_SECOND / dt);
		//}
		
		GraphicsSystem::Get().Update(dt);
		quit = !m_window->VUpdate(dt);
		m_globalEventManager.VUpdate();
		
		dt = GetDeltaMs(m_systemTime, m_gameTime);
		cycleCount++;
	}
}

float GameLoop::GetDeltaMs(unsigned __int64& previousSystemTime, unsigned __int64& previousClockTime)
{
	float deltatime = 0;
	unsigned __int64 timenext = 0;
	unsigned __int64 nextime = 0;
	//nothing will happen if dT is zero
	while (deltatime < MIN_DELTA_MS)
	{
		timenext = SDL_GetTicks();
		ClockManager::Get().UpdateClocks((timenext - previousSystemTime));
		nextime = m_gameClock->GetTimeMilliSec();
		deltatime = nextime - previousClockTime;	
		previousSystemTime = timenext;
	}	
	previousClockTime = nextime;
	return deltatime;
}
