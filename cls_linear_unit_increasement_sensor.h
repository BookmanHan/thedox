#include "lib_import.h"

class cls_linear_unit_increasement_sensor
	:public cls_linear_unit_sensor
{
protected:
	double d_thd;

public:
	virtual void save(ofstream& fout) const
	{
		cls_linear_unit_sensor::save(fout);

		fout.write((char*)&d_thd,sizeof(double));
	}

	virtual void load(ifstream& fin)
	{
		cls_linear_unit_sensor::load(fin);

		fin.read((char*)&d_thd,sizeof(double));
	}

	cls_linear_unit_increasement_sensor(int num,double thd)
		:cls_linear_unit_sensor(num)
	{
		d_thd = thd;
	}

public:
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
			if (output < 0)
			{
				if (d_re > -d_thd)
				{
					error_cache = -1.0;
				}
			}
			else if (output >= 0)
			{
				if (d_re < d_thd)
				{
					error_cache = 1.0;
				}
			}
			else
			{
				error_cache = 0.0;
			}
		}

		error_cache_validate = true;
	}
};