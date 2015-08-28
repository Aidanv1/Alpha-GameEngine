#include "Clock.h"
// -----------------------------------------------------------------------
Clock::Clock(float startTimeSeconds) :
m_timeMilliSec((unsigned __int64)(startTimeSeconds * US_PER_SECOND)),
m_timeScale(1.0f),
m_isPaused(false)
{
	ClockManager::Get().AddClock(this);
}
// -----------------------------------------------------------------------
Clock::~Clock()
{
	ClockManager::Get().RemoveClock(this);
}
// -----------------------------------------------------------------------
unsigned __int64 Clock::GetTimeMilliSec() const
{
	return m_timeMilliSec;
}
// -----------------------------------------------------------------------
void Clock::SetPaused(bool pause)
{
	m_isPaused = pause;
}
// -----------------------------------------------------------------------
bool Clock::IsPaused() const
{
	return m_isPaused;
}
// -----------------------------------------------------------------------
void Clock::Update(float dtRealMs)
{
	if (!m_isPaused)
	{
		unsigned __int64 dtScaledTime = (unsigned __int64)dtRealMs * m_timeScale;
		if (dtScaledTime > MS_PER_SECOND / 2)
		{
			m_timeMilliSec += IDEAL_FRAME_DT;
		}
		else
		{
			m_timeMilliSec += dtScaledTime;
		}
	}
}
// -----------------------------------------------------------------------
void Clock::Reset()
{
	m_timeMilliSec = 0;
}
// -----------------------------------------------------------------------
void Clock::SetTimeScale(float scale)
{
	m_timeScale = scale;
}
// -----------------------------------------------------------------------
float Clock::GetTimeScale() const
{
	return m_timeScale;
}
// -----------------------------------------------------------------------
void Clock::SingleStep()
{
	if (m_isPaused)
	{
		unsigned __int64 dtScaledTime = (IDEAL_FRAME_DT * (unsigned __int64)m_timeScale) * US_PER_SECOND;
		m_timeMilliSec += dtScaledTime;
	}
}
// -----------------------------------------------------------------------

