#ifndef CLOCK_H
#define CLOCK_H
#include "ClockManager.h"


class ClockManager;
//========================================================================
class Clock
{
public:
	unsigned __int64 GetTimeMilliSec() const;
	explicit Clock(float startTimeSeconds = 0.0f);
	~Clock();	
	void SetPaused(bool pause);
	bool IsPaused() const;
	void Update(float dtRealSeconds);
	void Reset();
	void SetTimeScale(float scale);
	float GetTimeScale() const;
	void SingleStep();

private:

private:
	unsigned __int64 m_timeMilliSec;
	float m_timeScale;
	bool m_isPaused;

};

#endif
//========================================================================
