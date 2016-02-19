#include "ann_import.h"

template<typename FUNC>
class cls_ann_neuron_gab //: public cls_ann_neuron_parametized<FUNC>
{
public:
	cls_ann_neuron_gab(int label,double init_param = 1.0,bool has_bias = true)
		//:cls_ann_neuron_parametized<FUNC>(label,init_param,has_bias)
	{
		;
	}
};
