#include "lib_import.h"

class cls_linear_unit_abstract 
	:public cls_linear_unit
{
public:
	vector<double> *inputs;
	double output;

	cls_linear_unit_abstract(int size)
		:cls_linear_unit(size)
	{
		inputs = null;
	}

	virtual void set_output(double output)
	{
		this->output = output;
	}

	virtual void set_input(vector<double> & input)
	{
		this->inputs = &input;
		invalidate_error();
		invalidate_output();
	}

	virtual void set_input(vector<double> * input)
	{
		this->inputs = input;
		invalidate_error();
		invalidate_output();
	}

	virtual void re_cache_error() 
	{
		if (error_cache_validate)
			return;

		double d_re = get_output();
		error_cache = output - d_re;

		error_cache_validate = true;
	}

	virtual void re_cache_output() 
	{
		if (output_cache_validate)
			return;

		if (inputs == null)
		{
			throw cls_exception_null_pointer("cls_linear_unit_simple::re_cache_output() - you must set the input.\n");
		}

		double re = 0;
		vector<double>::iterator itor = weight.begin();
		vector<double>::iterator itor_in = (*inputs).begin();
		for(itor = weight.begin(),itor_in = (*inputs).begin();itor != weight.end() && itor_in != (*inputs).end();itor_in++,itor++)
		{
			re += (*itor) * (*itor_in);
		}

		re += (*itor) * 1.0;

		output_cache = re;
		output_cache_validate = false;
	}
};