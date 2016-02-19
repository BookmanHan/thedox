#include "ann_import.h"

template<typename FUNC>
class cls_ann_layer_input
	: public TLayer<cls_ann_input_neuron<FUNC>,FUNC>
{
public:
	cls_ann_layer_input(int label,int size=0)
		:TLayer<cls_ann_input_neuron<FUNC>,FUNC>(label)
	{
		for(int cnt=0;cnt<size;cnt++)
		{
			add_neuron(new cls_ann_input_neuron<FUNC>(label*const_max_layer_size + cnt));
		}
	}

	virtual ~cls_ann_layer_input()
	{

	}

	virtual void set_input(const vector<double> & input)
	{
		if (input.size() != this->get_size())
		{
			throw cls_exception_not_identical("cls_ann_layer_input::set_input() - size is not the same.");
		}

        #ifdef __USING_GCC__
		typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#endif

		unsigned int cnt = 0;
		for(itor = this->neurons.begin();itor != this->neurons.end();itor++)
		{
			assert(*itor);
			((cls_ann_input_neuron<FUNC>*)(*itor))->set_value(input[cnt]);
			cnt++;
		}
	}

	virtual void set_input(const double *input)
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#endif

		int i=0;
		for(itor = this->neurons.begin();itor != this->neurons.end();itor++,i++)
		{
			assert(*itor);
			((cls_ann_input_neuron<FUNC>*)(*itor))->set_value(input[i]);
		}
	}

	virtual void add_neuron(cls_ann_neuron<FUNC> *nrn)
	{
		assert(nrn);
		cls_ann_layer<FUNC>::add_neuron(nrn);
	}
};
