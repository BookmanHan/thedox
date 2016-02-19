const static double const_kohonen_neuron_random_weights_dev = 0.1;
const static double const_max_output = 1;
const static double const_min_output = 0;
const static double const_term_in_atting = 0.01;

template<typename FUNC>
class cls_ann_neuron_kohonen : public cls_ann_simple_neuron<FUNC>
{
public:
	cls_ann_neuron_kohonen(int label)
		:cls_ann_simple_neuron<FUNC>(label,false)
	{
		;
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
			double mid = (const_min_output + const_max_output)/2;
			(*itor)->set_weight(a_unfirom_random(mid-const_kohonen_neuron_random_weights_dev,mid + const_kohonen_neuron_random_weights_dev));
		}
	}

	virtual void re_cahce_output()
	{
		if (this->output_cache_valid)
		{
			return ;
		}

		this->activation_cache = 0;

		#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor=this->input_links.begin();itor != this->input_links.end();itor++)
		{
			assert(*itor);
			double diff = (*itor)->get_source()->get_output() - (*itor)->get_weight();
			this->activation_cache += diff * diff;
		}

		this->output_cache = this->activation_cache;
		this->output_cache_valid = true;
	}

	void adapt_towards(const vector<double>&input ,double d_weight,double learning_param)
	{
		vector<double> weights(this->get_input_count());
		this->get_weights(weights);

		vector<double> delta(this->get_input_count());
		int size = this->get_input_count();
		for(int cnt=0;cnt<size;cnt++)
		{
			delta[cnt] = input[cnt] - weights[cnt];
		}

		for(int cnt=0;cnt<weights.size();cnt++)
		{
			weights[cnt] += delta[cnt] * d_weight * learning_param;
		}

		set_weights(weights);
	}

	void set_weights(const vector<double> &w)
	{
		int cnt=0;
		#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor=this->input_links.begin();itor != this->input_links.end();itor++,cnt++)
		{
			assert(*itor);
			(*itor)->set_weight(w[cnt]);
		}
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_simple_neuron<FUNC>::save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron_kohone::save() - output file is bad.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_simple_neuron<FUNC>::load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron_kohone::load() - input file is bad.\n");
		}
	}
};

