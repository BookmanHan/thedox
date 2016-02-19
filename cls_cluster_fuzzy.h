#include "lib_import.h"

template<typename FUNC>
class cls_cluster_fuzzy
	:cls_cluster
{
protected:
	int pat;

public:
	cls_cluster_fuzzy(int num_pat)
		:pat(num_pat),
	{
		;
	}

	virtual vector<int> train_example( cls_training_set & ts ) 
	{
		;
	}
};