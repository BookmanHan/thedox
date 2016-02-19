#include "lib_import.h"

class cls_bayes_unit
	:public cls_serialized_object
{
protected:
	double prob_pre;
	vector<double> prob_feature;
	int n_feature;
	int n_size;
	int n_total_size;

public:

	vector<double>& get_prob_feature()
	{
		return prob_feature;
	}

	virtual void save(ofstream & fout) const
	{

#ifdef __USING_GCC__
		typename vector<double>::const_iterator itor;
#else
		vector<double>::const_iterator itor;
#endif

		if (!fout)
			throw cls_exception_io(string(__FUNCTION__) + "() - the file is bad.\n");

		fout.write((char*)&prob_pre,sizeof(prob_pre));
		fout.write((char*)&n_feature,sizeof(n_feature));
		fout.write((char*)&n_size,sizeof(n_size));
		fout.write((char*)&n_total_size,sizeof(n_total_size));

		for(itor=prob_feature.begin();
			itor != prob_feature.end();
			itor++)
		{
			double tmp = *itor;
			fout.write((char*)&tmp,sizeof(double));
		}
	}

	virtual void load( ifstream & fin )
	{

#ifdef __USING_GCC__
		typename vector<double>::iterator itor;
#else
		vector<double>::iterator itor;
#endif

		if (!fin)
			throw cls_exception_io(string(__FUNCTION__) + "() - the file is bad.\n");

		fin.read((char*)&prob_pre,sizeof(prob_pre));
		fin.read((char*)&n_feature,sizeof(n_feature));
		fin.read((char*)&n_size,sizeof(n_size));
		fin.read((char*)&n_total_size,sizeof(n_total_size));

		prob_feature.resize(n_feature);

		for(itor=prob_feature.begin();
			itor != prob_feature.end();
			itor++)
		{
			double tmp = 0.0;
			fin.read((char*)&tmp,sizeof(double));
			*itor = tmp;
		}
	}

	void set_total(int num_total)
	{
		n_total_size = num_total;
	}

	cls_bayes_unit(int num_feautre)
		:prob_feature(num_feautre)
	{
		prob_pre = 0.0;
		n_feature = num_feautre;
		n_size = 0;

		fill(prob_feature.begin(),prob_feature.end(),0);
	}

	virtual void update_in(vector<double>& input)
	{
#ifdef __USING_GCC__
		typename vector<double>::iterator itor,itor_i;
#else
		vector<double>::iterator itor,itor_i;
#endif

		for(itor=prob_feature.begin(),itor_i=input.begin();
			itor != prob_feature.end() && itor_i != input.end();
			itor++,itor_i++)
		{
			*itor += *itor_i;
		}

		n_size++;
	}

	virtual void update()
	{

#ifdef __USING_GCC__
		typename vector<double>::iterator itor;
#else
		vector<double>::iterator itor;
#endif

		for(itor=prob_feature.begin();itor != prob_feature.end();itor++)
		{
			*itor /= (double)(n_size);
		}

		prob_pre = n_size*1.0 / n_total_size;
	}

	virtual double get_output(const vector<double>& input)
	{

#ifdef __USING_GCC__
		typename vector<double>::iterator itor;
		typename vector<double>::const_iterator itor_i;
#else
		vector<double>::iterator itor;
		vector<double>::const_iterator itor_i;
#endif
		double prob_re = 1.0;
		for(itor = prob_feature.begin(),itor_i=input.begin();
			itor != prob_feature.end() && itor_i != input.end();
			itor++,itor_i++)
		{
			prob_re *= (1-*itor_i)*(*itor) + (*itor_i)*(1-*itor);
		}

		return prob_re*prob_pre;
	}
};
