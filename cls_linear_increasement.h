#include "lib_import.h"

template<typename T>
class cls_linear_increasement : public cls_classifier
{
private:
	cls_matrix<T> mat_weight;
	cls_matrix<T> mat_des;

	int n_feature;
	int n_pat;
	double e;

	int n_times;

public:
	cls_linear_increasement()
	{
		n_feature = 0;
		n_pat = 0;
	}

	cls_linear_increasement(int a_feature,int a_pat)
	{
		n_feature = a_feature;
		n_pat = a_pat;

		mat_weight.resize(a_pat,a_feature+1);
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T> & v_des) 
	{
		int n_data = v_src.get_width();

		mat_des.share_assign(v_src);

		e = 0;
		for(int i=0;i<mat_weight.get_width();i++)
		{
			for(int j=0;j<mat_weight.get_height();j++)
			{
				mat_weight(i,j) = 0;
			}
		}

		for(int cnt=0;cnt<n_data;cnt++)
		{
			vector<T> v_check(n_feature+1);

			fill(v_check.begin(),v_check.end(),0);

			for(int i=0;i<n_pat;i++)
			{
				for(int j=0;j<n_feature;j++)
				{
					v_check[i] += mat_weight(i,j)  * v_src(cnt,j);
				}

				v_check[i] = mat_weight(i,n_feature) * 1;
			}

			bool flag = true;
			for(int i=0;i<n_pat;i++)
			{
				if (i != v_des(cnt,0))
				{
					flag &= (v_check[v_des(cnt,0)] > v_check[i])?true:false;
				}
			}

			if (flag)
			{
				continue;
			}
			else
			{
				e += 1;
				for(int i=0;i<n_pat;i++)
				{
					for(int j=0;j<n_feature;j++)
					{
						if (i == v_des(cnt,0))
						{
							mat_weight(i,j) += v_src(cnt,j)/(n_times+1);
						}
						else if (v_check[i] > v_check[v_des(cnt,0)])
						{
							mat_weight(i,j) -= v_src(cnt,j)/(n_times+1);
						}
					}

					if (v_check[n_feature] > v_check[v_des(cnt,0)])
					{
						mat_weight(i,n_feature) -= 1;
					}
				}
			}
		}

		e /= n_data;
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int a_times) 
	{
		n_times = a_times;

		while(n_times--)
		{
			train(v_src,v_des);
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e ) 
	{
		while(e > d_e)
		{
			train(v_src,v_des);
		}
	}

	virtual void simulate( vector<T> & v_src,vector<T> & v_des ) 
	{
		vector<T> v_check(n_feature+1);

		fill(v_check.begin(),v_check.end(),0);

		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_feature;j++)
			{
				v_check[i] += mat_weight(i,j)  * v_src(cnt,j);
			}

			v_check[i] = mat_weight(i,n_feature) * 1;
		}

		int cnt_max = 0;
		for(int i=0;i<n_pat;i++)
		{
			if (v_check[i] > v_check[cnt_max])
			{
				cnt_max = i;
			}
		}

		v_des.resize(mat_des.get_height());
		for(int cnt=0;cnt<mat_des.get_height();cnt++)
		{
			v_des[cnt] = mat_des(cnt_max,cnt);
		}
	}
};