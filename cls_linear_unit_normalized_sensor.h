#include "lib_import.h"

class cls_linear_unit_normalized_sensor 
	: public cls_linear_unit_sensor
{
public:
	cls_linear_unit_normalized_sensor(int num)
		:cls_linear_unit_sensor(num)
	{
		;
	}

	virtual bool update(double d_study_factor) 
	{
		assert(inputs);

		double d_error = get_error();

		vector<double>::iterator itor;
		vector<double>::iterator itor_in;

		double d_sum = 0.0;
		for(itor_in=inputs->begin();itor_in != inputs->end();itor_in++)
		{
			double d_tmp = (*itor_in);
			d_sum += d_tmp*d_tmp;
		}

		d_sum = sqrt(d_sum+0.01);

		for(itor=weight.begin(),itor_in = inputs->begin();itor != weight.end() && itor_in != inputs->end();itor++,itor_in++)
		{
			*itor += d_study_factor * d_error * (*itor_in) / d_sum;
		}

		*itor += d_study_factor * d_error * 1.0 / d_sum;

		invalidate_output();
		invalidate_error();

		if (d_error == 1.0)
			return true;
		else
			return false;
	}

	virtual void re_cache_error()
	{
		error_cache = get_output() - output;
	}
};