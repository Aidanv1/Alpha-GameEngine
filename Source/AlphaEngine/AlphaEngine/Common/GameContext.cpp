#include "GameContext.h"
#include "../AlphaStd.h"
#include "../Time/Clock.h"
// -----------------------------------------------------------------------
void GameContext::InitContext(Clock* gameClock)
{
	if (!m_isInitialized)
	{
		ALPHA_ASSERT(gameClock);
		m_gameClock = gameClock;
		m_isInitialized = true;
	}
}
// -----------------------------------------------------------------------
GameContext* GameContext::Get()
{
	static GameContext* context = ALPHA_NEW GameContext();
	return context;
}
// -----------------------------------------------------------------------
__int64 GameContext::GetGameTime()
{
	if (m_gameClock)
	{
		return m_gameClock->GetTimeMilliSec();
	}
	return 0;
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