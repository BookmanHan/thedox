#include "lib_import.h"

class cls_bayes_unit_least_error
	:public cls_bayes_unit
{
protected:
	vector<vector<double> > mode;
	cls_matrix<double> mat_s;
	double det;

public:
	cls_bayes_unit_least_error(int n_feature)
		:cls_bayes_unit(n_feature),
		 mat_s(n_feature,n_feature)
	{
		;
	}

	virtual void update_in(vector<double>& input)
	{
#ifdef __USING_GCC__
		typename vector<double>::iterator itor,itor_i;
#else
		vector<double>::iterator itor,itor_i;
#endif

		mode.push_back(input);

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
		int size = mode.size();

		for(int i=0;i<n_feature;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				double d_s = 0;
				for(int cnt=0;cnt<size;cnt++)
				{
					d_s = d_s + (mode[cnt][i] + 0.04 - prob_feature[i])*(mode[cnt][j] + 0.04 - prob_feature[j]);
				}

				mat_s(i,j) = d_s/(size-1);
			}
		}

		mat_s = mat_s.get_inverse();
		det = mat_s.get_det();
	}

	virtual double get_output(const vector<double>& input)
	{
		vector<double> v_d = input;

#ifdef __USING_GCC__
		typename vector<double>::iterator itor;
		typename vector<double>::iterator itor_i;
#else
		vector<double>::iterator itor;
		vector<double>::iterator itor_i;
#endif

		for(itor = prob_feature.begin(),itor_i=v_d.begin();
			itor != prob_feature.end() && itor_i != v_d.end();
			itor++,itor_i++)
		{
			*itor_i -= *itor;
		}

		cls_matrix<double> mat_d(v_d) ;
		cls_matrix<double> mat_re = mat_s * mat_d;
		cls_matrix<double> mat_trans_d = (mat_d.get_transpose());
		double d1 = (mat_re * mat_trans_d)(0,0);

		double d2 = log(prob_pre);
		double d3 = log(det+1);

		return -d1/2 + d2 - d3/2;
	}


};
