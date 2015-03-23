#include "Clock.h"
// -----------------------------------------------------------------------
Clock::Clock(float startTimeSeconds) :
m_timeMicroSec((unsigned __int64)(startTimeSeconds * US_PER_SECOND)),
m_timeScale(1.0f),
m_isPaused(false)
{
}
// -----------------------------------------------------------------------
Clock::~Clock()
{
	ClockManager::Get().RemoveClock(this);
}
// -----------------------------------------------------------------------
unsigned __int64 Clock::GetTimeMicroSec() const
{
	return m_timeMicroSec;
}
// -----------------------------------------------------------------------
float Clock::CalcDeltaSeconds(const Clock& other)
{
	unsigned __int64 dt = m_timeMicroSec - other.GetTimeMicroSec();
	return (float)(dt / US_PER_SECOND);
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
void Clock::Update(float dtRealSeconds)
{
	if (!m_isPaused)
	{
		unsigned __int64 dtScaledTime = dtRealSeconds * m_timeScale * US_PER_SECOND;
		m_timeMicroSec += dtScaledTime;
	}
}
// -----------------------------------------------------------------------
void Clock::Reset()
{
	m_timeMicroSec = 0;
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
		unsigned __int64 dtScaledTime = (IDEAL_FRAME_DT * m_timeScale) * US_PER_SECOND;
		m_timeMicroSec += dtScaledTime;
	}
}
// -----------------------------------------------------------------------

