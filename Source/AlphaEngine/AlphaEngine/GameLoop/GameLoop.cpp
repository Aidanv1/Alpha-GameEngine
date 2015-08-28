#include "GameLoop.h"
#include "../Physics/PhysicsSystem.h"
#include "..\Time\Clock.h"
#include "..\Time\SystemTime.h"
#include "..\Graphics3D\GraphicsSystem.h"
#include "..\Graphics3D\OpenGL\Renderer_GL.h"
#include "../Actor/RoleSystem.h"
#include "../Animation/AnimationSystem.h"
#include "../Window\IWindow.h"
#include "../EventManager/EventManager.h"
#include "../Window\SDLWindow.h"
#include "../Common/GameContext.h"
// -----------------------------------------------------------------------
GameLoop::GameLoop() :
m_window(NULL),
m_gameClock(NULL),
m_systemTime(0),
m_gameTime(0)
{
	m_globalEventManager = ALPHA_NEW EventManager("Global", true);
}
// -----------------------------------------------------------------------
GameLoop::~GameLoop()
{
	SAFE_DELETE(m_window);
	SAFE_DELETE(m_gameClock);
	SAFE_DELETE(m_globalEventManager);
}
// -----------------------------------------------------------------------
bool GameLoop::Init()
{
	//initialize gameclock
	m_gameClock = ALPHA_NEW Clock();
	m_systemTime = SDL_GetTicks();
	m_gameTime = 0;

	//initialize game context
	GameContext::Get()->InitContext(m_gameClock);

	//Window CHANGE THIS
	m_window = ALPHA_NEW SDLWindow("AlphaEngine", 1920, 1080);
	if (!m_window->VInit())
	{
		return false;
	}
	//Renderer
	shared_ptr<IRenderer> renderer(ALPHA_NEW Renderer_GL());
	//graphics system
	if (!GraphicsSystem::Get().Init(renderer, 60, 60))
	{
		return false;
	}
	//path of config directory
	string configPath = GameContext::Get()->GetPath("Config");
	//Physics
	PhysicsSystem::Get().Init(60);
	TiXmlDocument physicsDoc;
	//
	string physicsPath = configPath;
	physicsPath.append("Physics.xml");
	physicsDoc.LoadFile(physicsPath.c_str());
	TiXmlElement* physicsElem = physicsDoc.FirstChildElement();
	PhysicsSystem::Get().RigidBodyPhysics()->VConfigureXmlData(physicsElem);

	//Populate Actors
	string levelsPath = GameContext::Get()->GetPath("Levels");
	levelsPath.append("Actors.xml");
	TiXmlDocument doc;
	if (!doc.LoadFile(levelsPath.c_str()))
	{
		ALPHA_ERROR("Invalid path for levels");
		return false;
	}
	TiXmlElement* elem = doc.FirstChildElement();	
	RoleSystem::Get().Populate(elem);

	//Load Graphics
	GraphicsSystem::Get().LoadScene();
	//Load Physics
	PhysicsSystem::Get().LoadPhysics();
	return true;
}
// -----------------------------------------------------------------------
void GameLoop::StartLoop()
{
	float dt = 0;
	bool quit = false;
	unsigned int cycleCount = 0;
	while (!quit)
	{
		//RENDERING--------------------------
		//Graphics Render
		GraphicsSystem::Get().Render(dt);
		//Debug Physics Render
		PhysicsSystem::Get().RigidBodyPhysics()->VRenderDiagnostics();
		//ANIMATION--------------------------
		AnimationSystem::Get()->Update(dt);
		//-----------------------------------
		//UPDATES----------------------------
		//Dynamics
		PhysicsSystem::Get().RigidBodyPhysics()->VUpdate(dt);
		//Graphics Update
		GraphicsSystem::Get().Update(dt);
		//-----------------------------------
		//ROLESYSTEM UPDATE
		RoleSystem::Get().Update(dt);
		//-----------------------------------
		//run game logic for next frame
		VDoGameLogic();
		//-----------------------------------
		quit = !m_window->VUpdate(dt);
		m_globalEventManager->VUpdate();
		
		dt = GetDeltaMs(m_systemTime, m_gameTime);
		cycleCount++;
		if (cycleCount % 10 == 0)
		{
			string fps = "FPS :";
			fps.append(to_string(1000.0 / dt));
			GraphicsSystem::Get().GetRenderer()->VDrawText(fps.c_str());
		}


	}
}
// -----------------------------------------------------------------------
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
// -----------------------------------------------------------------------