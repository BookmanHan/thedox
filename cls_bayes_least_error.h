#include "lib_import.h"

class cls_bayes_least_error
	:public cls_bayes
{
public:
	cls_bayes_least_error(int num_pat,int num_feature)
		:cls_bayes(num_pat,num_feature)
	{
		for(int cnt=0;cnt<num_pat;cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit() 
	{
		units.push_back(new cls_bayes_unit_least_error(get_feature_num()));
	}

	virtual int get_output_index( double* input )
	{
		vector<double> re;
		for(int cnt=0;cnt<num_feature;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output_index(re);
	}

	virtual int get_output_index(const vector<double>& input ) 
	{
		vector<double> d_pw(num_pat);

#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::iterator itor;
		typename vector<double>::iterator itor_pat;
#else
		vector<cls_bayes_unit*>::iterator itor;
		vector<double>::iterator itor_pat;
#endif

		for(itor=units.begin(),itor_pat=d_pw.begin()
			;itor_pat!=d_pw.end() && itor!=units.end();
			itor_pat++,itor++)
		{
			*itor_pat = (*itor)->get_output(input);
		}

		double max_val = -1;
		int max_indx = -1;
		int cnt = 0;
		for(itor_pat=d_pw.begin();itor_pat!=d_pw.end();itor_pat++,cnt++)
		{
			if (max_val < *itor_pat)
			{
				max_val = *itor_pat;
				max_indx = cnt;
			}
		}

		return max_indx;
	}
};
