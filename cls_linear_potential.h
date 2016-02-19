#include "lib_import.h"

//Uncompleted...

template<typename T,typename FUNC> 
class cls_linear_potential : public cls_classifier<T>
{
private:
	cls_matrix<T> mat_weight;
	cls_matrix<T> mat_des;

	int n_feature;
	int n_pat;
	double e;

	FUNC func;

public:
	cls_linear_potential()
	{
		n_feature = 0;
		n_pat = 0;
	}

	cls_linear_potential(int a_feature,int a_pat)
	{
		n_feature = a_feature;
		n_pat = a_pat;

		mat_weight.resize(a_pat,a_feature+1);
	}

	virtual void train(cls_matrix<T> & v_src,cls_matrix<T> & v_des) 
	{
		//...
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times) 
	{
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
		//...
	}

};