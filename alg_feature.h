#include "lib_import.h"

template<typename T>
void a_get_norm(vector<double> &des,vector<T>& src)
{
	T elem_max = abs(src[0]);
	for(int cnt=0;cnt<src.size();cnt++)
	{
		if (abs(src[cnt])>elem_max)
		{
			elem_max = abs(src[cnt]);
		}
	}

	for(int cnt=0;cnt<src.size();cnt++)
	{
		des.push_back(1.0 * src[cnt]/elem_max);
	}
}

template <typename T>
void a_trans_abs(vector<T> & src)
{
	for(int cnt=0;cnt<src.size();cnt++)
	{
		src[cnt] = abs(src[cnt]);
	}
}

template<typename T>
void a_get_norm(vector<double> &des,vector<T>& src,double inter)
{
	for(int cnt=0;cnt<src.size();cnt++)
	{
		des.push_back(1.0 * src[cnt]/inter);
	}
}
