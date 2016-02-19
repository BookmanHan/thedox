#include "lib_import.h"

template <typename T>
class cls_linear_fisher_bi_pat : public cls_classifier
{
private:
	cls_matrix<T> mat_means;
	cls_multi_array<T> ma_s;
	cls_matrix<T> mat_sw;
	cls_matrix<T> mat_isw;
	cls_matrix<T> mat_w;

	double d_thd;
	
	int n_feature;
	int n_pat;

public:
	cls_linear_fisher_bi_pat() 
		: cls_multi_array(2,1,1)
	{
		n_feature = 0;
		n_pat = 0;
	}

	cls_linear_fisher_bi_pat(int a_feature,int a_pat) 
		:mat_means(2,a_feature),
		 ma_s(2,a_feature,a_feature),
		 mat_sw(a_feature,a_feature),
		 mat_isw(a_feature,a_feature),
		 mat_w(a_feature,1)
	{
		n_feature = a_feature;
		n_pat = a_pat;
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des) 
	{
		int n_data = v_src.get_width();
		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_pat;j++)
			{
				mat_means(i,j) = 0;
			}
		}

		int ar_num[2];
		ar_num[0] = 0;
		ar_num[1] = 0;
		for(int cnt=0;cnt<n_feature;cnt++)
		{
			for(int i=0;i<n_data;i++)
			{
				mat_means(v_des(i,0),cnt) += v_src(i,cnt);
				ar_num[v_des(i,0)]++;
			}

			mat_means(0,cnt) /= ar_num[0];
			mat_means(1,cnt) /= ar_num[1];
		}

		for(int i=0;i<n_feature;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				double s[2];
				s[0] = 0;
				s[1] = 0;

				for(int cnt=0;cnt<n_data;cnt++)
				{
					s[v_des(cnt,0)] += (v_src(cnt,i) - mat_means(v_des(cnt,0),i)) *
									   (v_src(cnt,j) - mat_means(v_des(cnt,0),j));
				}

				s[0] /= ar_num[0];
				s[1] /= ar_num[1];

				ma_s[0][i][j] = s[0];
				ma_s[1][i][j] = s[1];
			}
		}

		for(int i=0;i<n_feature;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				mat_sw(i,j) = ma_s(0,i,j) + ma_s(1,i,j);
			}
		}

		mat_isw = mat_sw.inverse();

		for(int cnt=0;cnt<n_feature;cnt++)
		{
			mat_w(cnt,0) = mat_means(0,cnt) - mat_means(1,cnt);
		}

		mat_w = (mat_isw * mat_w.transpose()).transpose();

		double m[2];
		m[0] = 0.0;
		m[1] = 0.0;

		for(int cnt=0;cnt<n_data;cnt++)
		{
			cls_matrix mat_mode(n_feature,1);
			for(int i=0;i<n_feature;i++)
			{
				mat_mode(i,0) = v_src(cnt,i);
			}

			m(v_des(cnt,0)) += (mat_mode.transpose() * mat_w)(0,0); 
		}

		d_thd = (m[0] + m[1]) / (ar_num[0] + ar_num[1]);
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times) 
	{
		train(v_src,v_des);
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e ) 
	{
		train(v_src,v_des);
	}

	virtual void simulate( vector<T> & v_src,vector<T> & v_des ) 
	{
		cls_matrix<T> mat_tmp(n_feature,1);
		for(int cnt=0;cnt<n_feature;cnt++)
		{
			mat_tmp(cnt,0) = v_src[cnt];
		}

		double y = (mat_w.transpose() * mat_tmp)(0,0);

		v_des.resize(1);
		if (y >= d_thd)
			v_des[0] = 0;
		else 
			v_des[1] = 1;
	}
};