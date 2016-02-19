#include "lib_import.h"

template<typename T>
class cls_linear_fisher_old : public cls_classifier
{
private:
	vector<vector<cls_linear_fisher_bi_pat<T> > > vv_clfbp;
	int n_pat;

public:
	cls_linear_fisher()
	{
		n_pat = 0;
	}

	cls_linear_fisher(int a_feature,int a_pat)
	{
		n_pat = a_pat;

		a_vector_resize(vv_clfbp,a_pat,a_pat);
		for(int i=0;i<a_pat;i++)
		{
			for(int j=0;j<a_pat;j++)
			{
				vv_clfbp[i][j] = cls_linear_fisher_bi_pat(a_feature,a_pat);
			}
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des)
	{
		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_pat;j++)
			{
				vv_clfbp[i][j].train(v_src,v_des);
			}
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times)
	{
		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_pat;j++)
			{
				vv_clfbp[i][j].train(v_src,v_des,n_times);
			}
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e )
	{
		for(int i=0;i<n_pat;i++)
		{
			for(int j=0;j<n_pat;j++)
			{
				vv_clfbp[i][j].train(v_src,v_des,d_e);
			}
		}
	}

	virtual void simulate( vector<T> & v_src,vector<T> & v_des )
	{
		vector<int> v_n(n_pat,0);
		for(int i=0;i<n_pat;i++)
		{
			for(int j=i+1;j<n_pat;j++)
			{
				v_n[vv_clfbp[i][j].simulate(v_src,v_des)[0] == 0?i:j] ++;
			}
		}

		int max_index = 0;
		for(int cnt=0;cnt<n_pat;cnt++)
		{
			if (v_n[cnt] > v_n[max_index])
			{
				max_index = cnt;
			}
		}

		v_des.resize(0);
		v_des[0] = max_index;
	}
};
