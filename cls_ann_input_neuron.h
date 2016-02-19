#include "ann_import.h"

template<typename FUNC>
class cls_ann_input_neuron : public cls_ann_neuron<FUNC>
{
private:
	void re_cache_error()
	{
		this->error_cache_valid = true;
	}

	void re_cache_output()
	{
		this->output_cache_valid = true;
	}

public:
	cls_ann_input_neuron(int label)
		:cls_ann_neuron<FUNC>(label)
	{

	}

	void set_value(double value)
	{
		this->activation_cache = value;
		this->output_cache = value;
		this->invalidate_output_cache();
		this->re_cache_output();
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_neuron<FUNC>::save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_input_neuron::save() - file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_neuron<FUNC>::load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_input_neuron::load() - file is bad for input.\n");
		}
	}
};
