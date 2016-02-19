#include "lib_import.h"

template<typename T>
class cls_bayes_least_risk_old_time : public cls_classifier
{

private:
	vector<vector<T> > v_mean;
	vector<cls_matrix<T> > v_s;
	vector<cls_matrix<T> > v_is;
	vector<T> v_pre;
	vector<T> v_det;
	vector<T> v_loss;

	int n_pat;
	int n_feature;

public:
	cls_bayes_least_risk()
	{
		n_feature = 0;
		n_pat = 0;
	}

	cls_bayes_least_risk(vector<T> a_loss,int a_pat,int a_feature)
	{
		n_feature = a_feature;
		n_pat = a_pat;

		v_loss = a_loss;
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T>  & v_des)
	{
		int n_data = v_src.get_width();

		set<T> set_des;
		for(int cnt=0;cnt<n_data;cnt++)
		{
			set_des.insert(v_des[cnt][0]);
		}

		int n_pat = set_des.size();

		v_mean.resize(n_pat,0);
		for(int cnt=0;cnt<n_pat;cnt++)
		{
			v_mean[cnt].resize(n_feature);
		}

		vector<int> ar_num;
		ar_num.resize(n_pat,0);
		for(int i=0;i<n_feature;i++)
		{
			for(int j=0;j<n_data;j++)
			{
				ar_num[v_des[j][0]]++;
				v_mean[v_des[j][0]][j] += v_src(i,j);
			}
		}

		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				v_mean[i][j] /= ar_num[i];
			}
		}

		cls_matrix<T> v_s_tmp(n_feature,n_feature);
		v_s.resize(n_pat,v_s_tmp);
		for(int i=0;i<n_feature;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				vector<T> v_s(n_pat);

				for(int k=0;k<n_data;k++)
				{
					(v_s[v_des[k][0]])(i,j) += (v_src(k,i) - v_mean[i]) * (v_src(k,j) - v_mean[j]);
				}

				for(int cnt=0;cnt<n_pat;cnt++)
				{
					(v_s[cnt])(i,j) = (v_s[cnt])(i,j) / T(ar_num[cnt]);
				}
			}
		}

		v_pre.clear();
		v_is.clear();
		v_det.clear();
		for(int cnt=0;cnt<n_pat;cnt++)
		{
			v_pre.push_back(ar_num[cnt]/n_data);
			v_is.push_back(v_s[cnt].get_reverse());
			v_det.push_back(v_s[cnt].get_det());
		}
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T>  & v_des,int n_times)
	{
		train(v_src,v_des);
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T>  & v_des,double d_e)
	{
		train(v_src,v_des);
	}

	virtual void simulate(vector<T> & a_src,vector<T> & a_des)
	{
		int n_pat = v_mean.size();

		double max_value = 0;
		int max_num = -1;

		for(int t=0;t<n_pat;t++)
		{
			vector<T> v_src = a_src;
			for(int cnt=0;cnt<n_feature;cnt++)
			{
				v_src[cnt] -= v_mean[t][cnt];
			}

			cls_matrix<T> mat_src(v_src);
			cls_matrix<T> mat_res = mat_src * v_is[t] * mat_src.get_transpose();
			double d_1 = mat_res(0,0);
			double d_2 = log(v_pre[t]);
			double d_3 = log(v_det[t] + 1);

			double d_max = -d_1/2 + d_2 - d_3/2 ;
			for(int p=0;p<n_pat;p++)
				d_max *= v_loss[t][p];

			if (max_num == -1 || d_max > max_value)
			{
				max_value = d_max;
				max_num = t;
			}
		}

		a_des.resize(1);
		a_des[0] = max_num;
	}
};
