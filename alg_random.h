#include "lib_import.h"

inline 
double a_random()
{
	static bool b_inti = false;
	if (b_inti == false)
	{
		b_inti = true;
		srand(time(null));
	}

	return double(RAND_MAX - rand())/RAND_MAX;
}

inline 
double a_uniform_random()
{
	static bool b_inti = false;
	if (b_inti == false)
	{
		b_inti = true;
		srand(time(null));
	}

	return double(rand() - RAND_MAX/2.0)/RAND_MAX;
}

inline 
double a_unfirom_random(double d_min,double d_max)
{
	static bool b_inti = false;
	if (b_inti == false)
	{
		b_inti = true;
		srand(time(null));
	}

	return d_min + double(rand())/RAND_MAX*(d_max-d_min);
}