#include "lib_import.h"

template<typename T>
class cls_point
{
public:
	T x;
	T y;

public:
	cls_point()
	{
		x = 0;
		y = 0;
	}

	cls_point(T ax,T ay)
	{
		x = ax;
		y = ay;
	}

	bool operator == (cls_point<T>& that)
	{
		return x==that.x && y==that.y;
	}
};