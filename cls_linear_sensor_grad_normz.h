#include "lib_import.h"

class cls_linear_sensor_grad_normz
	:public cls_linear_sensor
{
public:
	cls_linear_sensor_grad_normz(int num,int num_per,double d_factor=0.85,int num_output=1)
		:cls_linear_sensor(num,num_per,d_factor,num_output,false)
	{
		for(int cnt=0;cnt<num;cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit()
	{
		int size = get_size();
		units.push_back(new cls_linear_unit_gradian(_size_per_unit+1));
	}
};