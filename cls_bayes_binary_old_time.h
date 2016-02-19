#include "lib_import.h"

template<typename T>
class cls_bayes_binary_old_time : public cls_classifier
{
private:
	vector<T> v_pre;
	vector<vector<T> > v_class_feature;
	vector<vector<T> > * v_des;
	T thd;

	int n_feature;
	int n_pat;

public:
	cls_bayes_binary()
	{
		n_pat = 0;
		n_feature = 0;

		thd = 0.5;
	}

	cls_bayes_binary(T a_thd,int a_feature,int a_pat)
	{
		n_pat = a_pat;
		n_feature = a_feature;

		thd = a_thd;
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T> & v_des)
	{
		int n_data = v_src.get_width();

		v_pre.resize(n_data);
		a_vector_resize(v_class_feature,n_data,n_feature);

		vector<int> num_class;
		num_class.resize(n_pat);

		for(int cnt=0;cnt<n_data;cnt++)
		{
			num_class[v_des(cnt,0)] ++;
		}

		for(int cnt=0;cnt<n_pat;cnt++)
		{
			v_pre[cnt] = num_class[cnt] / n_data;
		}

		for(int cnt_cls=0;cnt_cls<n_data;cnt_cls++)
		{
			for(int cnt_fea=0;cnt_fea<n_feature;cnt_fea++)
			{
				if (v_src(cnt_cls,cnt_fea) > thd)
					v_class_feature[cnt_cls][cnt_fea]++;
			}
		}

		for(int cnt_cls=0;cnt_cls<n_data;cnt_cls++)
		{
			for(int cnt_fea=0;cnt_fea<n_feature;cnt_fea++)
			{
				v_class_feature[cnt_cls][cnt_fea] = T(v_class_feature[cnt_cls][cnt_fea] + 1)/T(num_class[cnt_cls] + 2);
			}
		}
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times)
	{
		train(v_src,v_des);
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e)
	{
		train(v_src,v_des);
	}

	virtual void simulate(vector<T> & v_src,vector<T> & a_des)
	{
		a_des.resize(n_feature);

		vector<T> v_cls_pat;
		v_cls_pat.resize(n_pat);

		for(int cnt=0;cnt<n_pat;cnt++)
		{
			double p = 1;
			for(int i=0;i<n_feature;i++)
			{
				p *= (v_src[i] > thd)?v_class_feature[cnt][i]:1-v_class_feature[cnt][i];
			}

			v_cls_pat[cnt] = p;
		}

		double pro_base = 0;
		for(int cnt=0;cnt<n_pat;cnt++)
		{
			pro_base += v_pre[cnt] * v_cls_pat[cnt];
		}

		int num = -1;
		double pro_max = 0;

		for(int cnt=0;cnt<n_pat;cnt++)
		{
			double p = v_cls_pat[cnt] * v_pre[cnt] / pro_base;
			if (p > pro_max)
			{
				pro_max = p;
				num = cnt;
			}
		}

		a_des = v_des->at(num);
	}
};
