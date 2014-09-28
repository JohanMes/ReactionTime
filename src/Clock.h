#ifndef CLOCK_INCLUDE
#define CLOCK_INCLUDE

#include <windows.h>

class Clock {
	__int64 starttime;
	double secpercount;
	public:
		Clock(bool start);
		~Clock();
		
		void Start();
		double GetTimeSec();
		double Reset();
};

#endif
