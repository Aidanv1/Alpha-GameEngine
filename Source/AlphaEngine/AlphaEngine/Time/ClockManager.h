#pragma once

#include "..\AlphaStd.h"
#include "Clock.h"
using namespace std;
class Clock;
//========================================================================
class ClockManager
{
public:
	static ClockManager& Get();
	void AddClock(Clock* clock);
	void RemoveClock(Clock* clock);
	void UpdateClocks(float deltaMs);
	int GetNumberOfClocks() const;

private:
	ClockManager();
	~ClockManager();
private:
	vector <Clock*>  m_ClockList;	
};
//========================================================================
