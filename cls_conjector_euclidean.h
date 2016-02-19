#include "lib_import.h"

template<typename T>
class cls_conjector_euclidean
{
public:
	T operator()(vector<T> &src,vector<T> &des)
	{
		T re = 0;
		for(int cnt=0;cnt<min(src.size(),des.size());cnt++)
		{
			re += (src[cnt] - des[cnt]) * (src[cnt] - des[cnt]);
		}

		return re;
	}
};