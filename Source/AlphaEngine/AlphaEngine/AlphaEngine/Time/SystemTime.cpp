#include "SystemTime.h"
high_resolution_clock::time_point SystemTime::start;
bool SystemTime::init;
//----------------------------------------------------------------------
SystemTime& SystemTime::Instance()
{

	static SystemTime* instance = new SystemTime();
	if (!init)
	{
		start = high_resolution_clock::now();
	}
	init = true;
	return *instance;
}
//======================================================================