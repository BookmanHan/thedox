#include "lib_import.h"

class cls_linear_mutiply_lmse
	:public cls_linear_mutiply_abstract
{
public:
	cls_linear_mutiply_lmse(int num,int per)
		:cls_linear_mutiply_abstract(num,per)
	{
		for(int cnt=0;cnt<num*num-num;cnt++)
		{
			add_linear();
		}
	}

	virtual void add_linear() 
	{
		linears.push_back(new cls_linear_lmse(2,get_num_per_size()));
	}
};

class cls_linear_mutiply_sonser
	:public cls_linear_mutiply_abstract
{
public:
	cls_linear_mutiply_sonser(int num,int per)
		:cls_linear_mutiply_abstract(num,per)
	{
		for(int cnt=0;cnt<num*num-num;cnt++)
		{
			add_linear();
		}
	}

	virtual void add_linear() 
	{
		linears.push_back(new cls_linear_sensor(2,get_num_per_size()));
	}
};

class cls_linear_mutiply_sonser_gn
	:public cls_linear_mutiply_abstract
{
public:
	cls_linear_mutiply_sonser_gn(int num,int per)
		:cls_linear_mutiply_abstract(num,per)
	{
		for(int cnt=0;cnt<num*num-num;cnt++)
		{
			add_linear();
		}
	}

	virtual void add_linear() 
	{
		linears.push_back(new cls_linear_sensor_grad_normz(2,get_num_per_size()));
	}
};

class cls_linear_mutiply_sonser_inc
	:public cls_linear_mutiply_abstract
{
public:
	cls_linear_mutiply_sonser_inc(int num,int per)
		:cls_linear_mutiply_abstract(num,per)
	{
		for(int cnt=0;cnt<num*num-num;cnt++)
		{
			add_linear();
		}
	}

	virtual void add_linear() 
	{
		linears.push_back(new cls_linear_sensor_inctreasement(2,get_num_per_size()));
	}
};