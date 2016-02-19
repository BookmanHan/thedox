#include "lib_import.h"

class cls_ci
	:public cls_object
{
public:
	virtual void run_once() = 0;
	virtual vector<double>& get_output_data() = 0;
};