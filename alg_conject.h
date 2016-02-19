#include "lib_import.h"

class alg_conject
	:public cls_object
{
public:
protected:
private:
};

class alg_conject_neareast
	:public alg_conject
{
public:
	virtual double operator()(vector<double>& src,vector<double>& des) = 0;
};

class alg_conject_neareast_eulid
	:public alg_conject_neareast
{
	double operator()(vector<double>& src,vector<double>& des)
	{
		double d_re = 0;

		vector<double>::iterator itor_src;
		vector<double>::iterator itor_des;
		for(itor_src = src.begin(),itor_des = des.begin();
			itor_src != src.end() && itor_des != des.end();
			itor_des++,itor_src++)
		{
			d_re = (*itor_src - *itor_des) *  (*itor_src - *itor_des);
		}

		return sqrt(d_re);
	}
};