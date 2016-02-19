#include "lib_import.h"

class cls_cluster
	:cls_object
{
public:
	virtual vector<int> train_example(cls_training_set & ts) = 0;
};