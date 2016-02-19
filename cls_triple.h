#include "lib_import.h"

template<typename T1,typename T2,typename T3>
class cls_triple
{
private:
	T1 t1;
	T2 t2;
	T3 t3;

public:
	inline T1& first()
	{
		return t1;
	}

	inline T2& second()
	{
		return t2;
	}

	inline T3& third()
	{
		return t3;
	}

	bool operator < (const cls_triple& that) const
	{
		return t1 < that.t1;
	}
};

template<typename T1,typename T2,typename T3>
cls_triple<T1,T2,T3>  a_make_triple(T1 a,T2 b,T3 c)
{
	cls_triple<T1,T2,T3> ct;
	ct.t1 = a;
	ct.t2 = b;
	ct.t3 = c;

	return  ct;
}