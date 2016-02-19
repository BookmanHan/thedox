#include "lib_import.h"

class cls_conject_linear
	:public cls_object
{
public:
	virtual double operator()(const vector<double> &x,const vector<double> &y) = 0;
};

class cls_conject_linear_euclid
	:public cls_conject_linear
{
public:
	virtual double operator()(const vector<double> &x,const vector<double> &y)
	{
		vector<double>::const_iterator itor_x,itor_y;

		double re = 0.0;
		for(itor_x = x.begin(),itor_y = y.begin();
		    itor_x != x.end() && itor_y != y.end();
			itor_x++,itor_y++)
		{
			re += (*itor_x - *itor_y) * (*itor_x - *itor_y);
		}

		return re;
	}
};