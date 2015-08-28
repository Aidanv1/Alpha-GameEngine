#include "GameContext.h"
#include "../AlphaStd.h"
#include "../Time/Clock.h"
#include "../Graphics3D/GraphicsSystem.h"
#include "../Actor/RoleSystem.h"
// -----------------------------------------------------------------------
void GameContext::InitContext(Clock* gameClock)
{
	if (!m_isInitialized)
	{
		ALPHA_ASSERT(gameClock);
		m_gameClock = gameClock;
		m_isInitialized = true;
	}
	m_paths.Load("Paths.xml");
}
// -----------------------------------------------------------------------
GameContext* GameContext::Get()
{
	static GameContext* context = ALPHA_NEW GameContext();
	return context;
}
// -----------------------------------------------------------------------
__int64 GameContext::GetGameTime() const
{
	if (m_gameClock)
	{
		return m_gameClock->GetTimeMilliSec();
	}
	return 0;
}
// -----------------------------------------------------------------------
Path GameContext::GetPath(PathID id)
{
	return m_paths.GetPath(id);
}
// -----------------------------------------------------------------------
IRenderer* GameContext::GetGraphicsRenderer() const
{
	return GraphicsSystem::Get().GetRenderer();
}
// -----------------------------------------------------------------------
RoleSystem* GameContext::GetRoleSystem() const
{
	return &RoleSystem::Get();
}
// -----------------------------------------------------------------------
GameContext::GameContext() :
m_isInitialized(false),
m_gameClock(NULL)
{

}
// -----------------------------------------------------------------------
GameContext::~GameContext()
{

}
// -----------------------------------------------------------------------
