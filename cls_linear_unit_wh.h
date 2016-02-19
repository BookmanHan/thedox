#include "lib_import.h"

class cls_linear_unit_wh
	:public cls_linear_unit_abstract
{

public:
	cls_linear_unit_wh(int num)
		:cls_linear_unit_abstract(num)
	{
		;
	}

public:
	virtual bool update(double d_study_factor)
	{
		double d_prod = 0.0;
		vector<double>::iterator itor;
		vector<double>::iterator itor_in;

		for(itor=weight.begin(),itor_in=inputs->begin();itor != weight.end() && itor_in != inputs->end();itor++,itor_in++)
		{
			*itor += d_study_factor*(output-get_output())*(*itor_in);
		}

		*itor += d_study_factor*(output - get_output())*1.0;


		invalidate_output();
		invalidate_error();

		return true;
	}
};