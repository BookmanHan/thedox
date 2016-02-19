#include "lib_import.h"

class cls_linear_unit_margin
	:public cls_linear_unit_sensor
{
protected:
	double margin;

public:
	cls_linear_unit_margin(int num,double d_margin)
		:cls_linear_unit_sensor(num)
	{
		margin = d_margin;
	}


};