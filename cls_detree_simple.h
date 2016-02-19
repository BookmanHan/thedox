#include "lib_import.h"

template<typename T>
class cls_detree_simple
{
protected:
	vector<pair<cls_classifier*,vector<int> > > dec_tree;

	void train_sub(int h_node)
	{

	}

public:
	
	virtual void train_example(cls_training_set & ts)
	{

	}
};