#include "lib_import.h"

class cls_linear_sensor_inctreasement
	:public cls_linear_sensor
{
protected:
	double d_thd;

public:
	cls_linear_sensor_inctreasement(int num,int per,double factor=0.85,double thd=0.85,int num_output=1)
		:cls_linear_sensor(num,per,factor,num_output,false)
	{
		d_thd = thd;

		for(int cnt=0;cnt<num;cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit() 
	{
		int size = get_size_per_unit();
		units.push_back(new cls_linear_unit_increasement_sensor(size,d_thd));
	}
};