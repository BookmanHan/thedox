#include "lib_import.h"

template<typename T>
unsigned a_get_max_index(T *beg,T *end)
{
	T max_t = *beg;
	T *p;
	int re = 0;
	int cnt = 0;
	for(p=beg;p<end;p++,cnt++)
	{
		if (max_t <= *p)
		{
			re = cnt;
			max_t = *p;
		}
	}

	return re;
}