#include "lib_import.h"

template<typename FUNC>
class cls_ann_abstract_neuron
    :public cls_ann_neuron<FUNC>
{
protected:
	bool has_bias;
	double bias;
	double delta_bias;

public:
	static double get_random_weight()
	{
		return a_uniform_random();
	}

	cls_ann_abstract_neuron(int label,bool a_has_bias = true)
		:cls_ann_neuron<FUNC>(label),
		has_bias(a_has_bias)
	{
		bias = 0.0;
		delta_bias = 0.0;
	}

	virtual void set_bias(double bias)
	{
		this->bias = bias;

		this->invalidate_output_cache();
	}

	virtual bool check_bias() const
	{
		return has_bias;
	}

	virtual double get_bias() const
	{
		return bias;
	}

	virtual void remove_bias()
	{
		has_bias = false;
		bias = 0.0;

		this->invalidate_output_cache();
		this->invalidate_error_cache();
	}

	virtual void set_desired_output(double desired) = 0;

	virtual void connect(cls_ann_neuron<FUNC> * from)
	{
		connect(from,get_random_weight());
	}

	virtual void connect(cls_ann_neuron<FUNC> & from)
	{
		connect(&from);
	}

	virtual void connect_from(cls_ann_neuron<FUNC> * from)
	{
		connect(from);
	}

	virtual void connect(cls_ann_neuron<FUNC> * from,double weight)
	{
		cls_ann_link<FUNC> * link = new cls_ann_link<FUNC>(from,this,weight);
		disconnect(from);
		this->input_links.push_back(link);
		from->output_links.push_back(link);
		this->invalidate_output_cache();
	}

	virtual void calc_new_weights(double d_study_factor,double d_momentum)
	{
		if (this->get_input_count() == 0)
		{
			throw cls_exception_io("there is no links.");
			return;
		}

        #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor = this->input_links.begin();itor != this->input_links.end();itor++)
		{
			double d_input = (*itor)->get_source()->get_output();
			(*itor)->set_delta_weight(this->get_error() * d_input * d_study_factor  +
				d_momentum * (*itor)->get_last_delta_weight());
		}

		if (has_bias)
		{
			delta_bias = this->get_error() * d_study_factor;
		}
	}

	virtual void update()
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor=this->input_links.begin();itor != this->input_links.end();itor++)
		{
			(*itor)->update_weight();
		}

		if (has_bias)
		{
			bias += delta_bias;
			delta_bias = 0.0;
		}

		this->invalidate_output_cache();
		this->invalidate_error_cache();
	}

	virtual double get_weight(cls_ann_neuron<FUNC> * from) const
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::const_iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::const_iterator itor;
		#endif

		for(itor=this->input_links.begin();itor != this->input_links.end();itor++)
		{
			if ((*itor)->get_source() == from)
				return (*itor)->get_weight();
		}

		return 0.0;
	}

	void randomize_weights()
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor=this->input_links.begin();itor != this->input_links.end();itor++)
		{
			assert(*itor);
			(*itor)->set_weight(get_random_weight());
		}

		if (this->has_bias)
			set_bias(get_random_weight());
	}

	virtual void save(ofstream& fout ) const
	{
		try
		{
			cls_ann_neuron<FUNC>::save(fout);

			fout.write((char*)&bias,sizeof(bias));
			fout.write((char*)&delta_bias,sizeof(delta_bias));
			fout.write((char*)&has_bias,sizeof(has_bias));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_abstract_neuron::save() - file is bad for output.\n");
		}
	}

	virtual void load(ifstream& fin )
	{
		try
		{
			cls_ann_neuron<FUNC>::load(fin);

			fin.read((char*)&bias,sizeof(bias));
			fin.read((char*)&delta_bias,sizeof(delta_bias));
			fin.read((char*)&has_bias,sizeof(has_bias));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_abstract_neuron::load() - file is bad for input.\n");
		}
	}
};
