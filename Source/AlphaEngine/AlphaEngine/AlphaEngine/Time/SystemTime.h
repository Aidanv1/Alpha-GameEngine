#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H
//***************NOTE**************
//MUST REFRACTOR FOR MULTIPLATFORM COMPATIBILITY
//*********************************
#ifdef _WIN32
#include <chrono>
using namespace std::chrono;
#endif
//======================================================================
class SystemTime
{

public:
	
	static SystemTime& Instance();

	float GetTimeInMicroSeconds()
	{
#ifdef _WIN32
		auto finish = high_resolution_clock::now();
		auto time = (float)duration_cast<microseconds>(finish - start).count();
		return time;
#endif
	}
private:
	SystemTime(){}
	~SystemTime(){};
#ifdef _WIN32
	static high_resolution_clock::time_point start;
#endif
	static bool init;
};
#endif
//======================================================================