#ifndef CLOCK_H
#define CLOCK_H
#include "ClockManager.h"
#define MS_PER_SECOND 1000
#define US_PER_SECOND 1000000
#define NS_PER_SECOND 1000000000
#define IDEAL_FRAME_DT 1/60		//Deltatime for 60 FPS

class ClockManager;
//========================================================================
class Clock
{
public:
	unsigned __int64 GetTimeMicroSec() const;
	explicit Clock(float startTimeSeconds = 0.0f);
	~Clock();
	float CalcDeltaSeconds(const Clock& other);
	void SetPaused(bool pause);
	bool IsPaused() const;
	void Update(float dtRealSeconds);
	void Reset();
	void SetTimeScale(float scale);
	float GetTimeScale() const;
	void SingleStep();

private:

private:
	unsigned __int64 m_timeMicroSec;
	float m_timeScale;
	bool m_isPaused;

};

#endif
//========================================================================
