#include "lib_import.h"

template<typename T>
class cls_ci_function_peak
{
public:
	virtual double operator()(vector<T>& target) = 0;
};

template<typename T>
class cls_ci_function_peak_guass
	:public cls_ci_function_peak<T>
{
public:
	virtual double operator()(vector<T>& target)
	{
		double re = 0.0;
		int size = target.size();
		for(int i=0;i<size;i++)
		{
			for(int j=0;j<size;j++)
			{
				re += (target[i]-target[size/2])*(target[j]-target[size/2]);
			}
		}

		return exp(re);
	}
};

