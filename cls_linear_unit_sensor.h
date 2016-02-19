#include "lib_import.h"

class cls_linear_unit_sensor
	:public cls_linear_unit_abstract
{
public:
	cls_linear_unit_sensor(int num)
		:cls_linear_unit_abstract(num)
	{
		randomize_weight();
	}

	virtual bool update(double d_study_factor) 
	{
		assert(inputs);

		double d_error = get_error();

		vector<double>::iterator itor;
		vector<double>::iterator itor_in;
		for(itor=weight.begin(),itor_in = inputs->begin();itor != weight.end() && itor_in != inputs->end();itor++,itor_in++)
		{
			*itor += d_study_factor * d_error * (*itor_in);
		}

		*itor += d_study_factor * d_error * 1.0;

		invalidate_output();
		invalidate_error();

		if (d_error == 1.0)
			return true;
		else
			return false;
	}

	virtual void re_cache_error() 
	{
		if (error_cache_validate)
			return;

		double d_re = get_output();

		if (d_re >=0 && output<0)
		{
			error_cache = -1.0;
		}
		else if (d_re <0 && output>=0)
		{
			error_cache = 1.0;
		}
		else
		{
			error_cache = 0.0;
		}

		error_cache_validate = true;
	}
};