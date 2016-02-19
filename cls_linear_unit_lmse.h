#include "lib_import.h"

class cls_linear_unit_lmse
	:public cls_linear_unit_abstract
{
protected:
	int n_times;
	vector<vector<double>*> *input_1,*input_2;

public:
	virtual void save(ofstream& fout) const
	{
		cls_linear_unit_abstract::save(fout);

		fout.write((char*)&n_times,sizeof(n_times));
	}

	virtual void load(ifstream& fin)
	{
		cls_linear_unit_abstract::load(fin);

		fin.read((char*)&n_times,sizeof(n_times));
	}

	cls_linear_unit_lmse(int per)
		:cls_linear_unit_abstract(per)
	{
		n_times = 1;
		randomize_weight();
	}

	virtual void re_cache_error() 
	{
		if (error_cache_validate)
			return;

		error_cache = true;
	}

	virtual bool update(double d_study_factor)
	{
		vector<double>::iterator itor;
		vector<double>::iterator itor_in;
		assert(n_times);

		for(itor = weight.begin(),itor_in=inputs->begin();itor != weight.end() && itor_in != inputs->end();itor++,itor_in++)
		{

			(*itor) +=  (*itor_in) * (d_study_factor - get_output()) /n_times;
		}

		(*itor) += 1.0 * (d_study_factor - get_output()) /n_times;

		n_times++;

		invalidate_error();
		invalidate_output();

		return true;
	}

	virtual void re_cache_output() 
	{
		if (output_cache_validate)
			return;
	
		output_cache = 0;
		vector<double>::iterator itor;
		vector<double>::iterator itor_in;
		for(itor = weight.begin(),itor_in=inputs->begin();itor != weight.end() && itor_in != inputs->end();itor++,itor_in++)
		{
			output_cache += (*itor) * (*itor_in);
		}

		output_cache += (*itor);

		output_cache_validate = true;
	}
};