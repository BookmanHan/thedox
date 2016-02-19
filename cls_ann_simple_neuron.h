#include "ann_import.h"

template<typename FUNC>
class cls_ann_simple_neuron : public cls_ann_abstract_neuron<FUNC>
{
public:
	cls_ann_simple_neuron(int label,bool has_bias)
		:cls_ann_abstract_neuron<FUNC>(label,has_bias)
	{
		;
	}

	virtual void set_desired_output(double desired)
	{
		if (this->get_output_count() != 0)
		{
			return;
		}

		this->invalidate_error_cache();
		this->error_cache = (desired - this->get_output()) *
			(this->func)[this->get_activation()];

		this->re_cache_error();
	}

	virtual void re_cache_error()
	{
		if (this->get_output_count() == 0)
		{
			this->error_cache_valid = true;
			return;
		}

		if (this->error_cache_valid)
			return;

        #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		this->error_cache = 0;
		for(itor=this->output_links.begin();itor != this->output_links.end();itor++)
		{
			assert(*itor);

			this->error_cache += (*itor)->get_weight() *
				(*itor)->get_destination()->get_error();
		}

		this->error_cache *= (this->func)[this->get_activation()];
		this->error_cache_valid = true;
	}

	virtual void re_cache_output()
	{
		if (this->output_cache_valid)
		{
			return;
		}

        #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		this->activation_cache = 0;
		for(itor = this->input_links.begin();itor != this->input_links.end();itor++)
		{
			assert(*itor);

			this->activation_cache += (*itor)->get_source()->get_output() * (*itor)->get_weight();
		}

		if (this->has_bias)
			this->activation_cache += this->bias;

		this->output_cache = (this->func)(this->activation_cache);
		this->output_cache_valid = true;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_abstract_neuron<FUNC>::save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_simple_neuron::save() - file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_abstract_neuron<FUNC>::load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_simple_neuron::load() - file is bad for input.\n");
		}
	}
};
