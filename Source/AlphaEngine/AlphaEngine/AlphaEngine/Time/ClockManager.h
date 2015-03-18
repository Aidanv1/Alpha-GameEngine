#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H
#include "..\AlphaStd.h"
#include "Clock.h"
using namespace std;
class Clock;
//======================================================================
class ClockManager
{
public:
	static ClockManager& Instance();
	void AddClock(Clock* clock);
	void RemoveClock(Clock* clock);
	void UpdateClocks(float deltaSeconds);
	int GetNumberOfClocks() const;

private:
	ClockManager();
	~ClockManager();
private:
	vector <Clock*>  m_ClockList;	
};
#endif
//======================================================================