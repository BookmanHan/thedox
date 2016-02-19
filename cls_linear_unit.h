#include "lib_import.h"

class cls_linear_unit
	:cls_serialized_object
{
protected:
	vector<double> weight;

	double output_cache;
	double error_cache;
	
	bool output_cache_validate;
	bool error_cache_validate;

public:
	virtual void save(ofstream& fout) const
	{
		if (!fout)
			throw cls_exception_io("cls_linear_unit::save() - the file is bad for output.");

		fout.write((char*)&output_cache,sizeof(output_cache));
		fout.write((char*)&error_cache,sizeof(error_cache));
		fout.write((char*)&output_cache_validate,sizeof(output_cache_validate));
		fout.write((char*)&error_cache_validate,sizeof(error_cache_validate));
		
		int num = weight.size();
		fout.write((char*)&num,sizeof(num));

		for(int cnt=0;cnt<num;cnt++)
		{
			fout.write((char*)&weight[cnt],sizeof(double));
		}
	}

	virtual void load(ifstream& fin)
	{
		if (!fin)
			throw cls_exception_io("cls_linear_unit::load() - the file is bad for input.");

		weight.clear();

		fin.read((char*)&output_cache,sizeof(output_cache));
		fin.read((char*)&error_cache,sizeof(error_cache));
		fin.read((char*)&output_cache_validate,sizeof(output_cache_validate));
		fin.read((char*)&error_cache_validate,sizeof(error_cache_validate));

		int num;
		fin.read((char*)&num,sizeof(num));

		for(int cnt=0;cnt<num;cnt++)
		{
			double d;
			fin.read((char*)&d,sizeof(double));

			weight.push_back(d);
		}
	}

	inline int get_input_count()
	{
		return weight.size() - 1;
	}

	cls_linear_unit(int dim)
		:weight(dim+1)
	{
		randomize_weight();

		output_cache = 0;
		error_cache = 0;

		output_cache_validate = false;
		error_cache_validate = false;
	}

	cls_linear_unit(vector<double> & re)
		:weight (re)
	{
		randomize_weight();

		output_cache = 0;
		error_cache = 0;

		output_cache_validate = false;
		error_cache_validate = false;
	}

	virtual void randomize_weight()
	{
		vector<double>::iterator itor;
		for(itor=weight.begin();itor != weight.end();itor++)
		{
			(*itor) = a_unfirom_random(0,1);
		}
	}

	virtual void re_cache_output() = 0;
	virtual void re_cache_error() = 0;

	virtual void invalidate_output()
	{
		output_cache_validate = false;
	}

	virtual void invalidate_error()
	{
		error_cache_validate = false;
	}

	virtual double get_output()
	{
		re_cache_output();
		return output_cache;
	}

	virtual double get_error()
	{
		re_cache_error();
		return error_cache;
	}

	virtual void set_output(double output) = 0;
	virtual void set_input(vector<double> & input) = 0;
	virtual void set_input(vector<double> * input) = 0;
	virtual bool update(double d_study_factor) = 0;
};

