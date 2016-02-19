#include "lib_import.h"

#ifndef __USING_GCC__
#include <windows.h>

class time_val 
{
public:
	long tv_sec;
	long tv_usec;
};

inline
int gettimeofday(struct time_val* tv) 
{
	union 
	{
		long long ns100;
		FILETIME ft;
	} now;

	GetSystemTimeAsFileTime (&now.ft);
	tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
	tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);
	return (0);
}
#else
#include <time.h> 
#endif
