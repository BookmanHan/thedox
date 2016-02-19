#include "lib_import.h"

class cls_bayes_basis
	:public cls_bayes
{
public:
	cls_bayes_basis(int num_pat,int num_feature)
		:cls_bayes(num_pat,num_feature)
	{
		for(int cnt=0;cnt<num_pat;cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit() 
	{
		units.push_back(new cls_bayes_unit(get_feature_num()));
	}
};