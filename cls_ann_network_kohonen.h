#include "ann_import.h"

class cls_ann_param_kohonen
	:public cls_serialized_object
{
protected:
	cls_ann_public_values &control;
	double dnb;
	double dlp;

public:
	cls_ann_param_kohonen(double init_nb,double init_lp,cls_ann_public_values & ctrl = default_control)
		:control(ctrl)
	{
			set_nb(init_nb);
			set_lp(init_lp);
	}

	virtual void next_iterator() = 0;
	double get_neighborhood_size() const
	{
		return dnb;
	}

	double get_learnng_param() const
	{
		return dlp;
	}

protected:
	void set_nb(double nb)
	{
		dnb = nb;
	}

	void set_lp(double lp)
	{
		dlp = lp;
	}

public:
	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_param_kohonen::save() - the file is bad for output.\n");

		fout.write((char*)&dnb,sizeof(dnb));
		fout.write((char*)&dlp,sizeof(dlp));
	}

	virtual void load( ifstream& fin ) 
	{
		if (!fin)
			throw cls_exception_io("cls_ann_param_kohonen::load() - the file is bad for input.\n");

		fin.read((char*)&dnb,sizeof(dnb));
		fin.read((char*)&dlp,sizeof(dlp));	
	}
};

class cls_ann_param_kohonen_static : public cls_ann_param_kohonen
{
public:
	cls_ann_param_kohonen_static(double init_nb,double init_lp)
		:cls_ann_param_kohonen(init_nb,init_lp)
	{

	}

	virtual void next_iterator()
	{

	}
};

class cls_ann_param_kohonen_dynamic : public cls_ann_param_kohonen
{
protected:
	unsigned step;

public:
	cls_ann_param_kohonen_dynamic(double init_nb,double init_lp)
		:cls_ann_param_kohonen(init_nb,init_lp)
	{

	}

	virtual void next_iterator()
	{
		step++;
		re_compute();
	}

	virtual void re_compute()  = 0;

	unsigned get_step() const
	{
		return step;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_param_kohonen::save(fout);
			fout.write((char*)&step,sizeof(step));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_param_kohonen_dynamic::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin ) 
	{
		try
		{
			cls_ann_param_kohonen::load(fin);
			fin.read((char*)&step,sizeof(step));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_param_kohonen_dynamic::load() - the file is bad for input.\n");
		}
	}

protected:
	cls_ann_param_kohonen_dynamic() 
		:cls_ann_param_kohonen(1.0,1.0),
		 step(1)
	{

	}
};

class cls_ann_param_kohonen_standard 
	: public cls_ann_param_kohonen_dynamic
{
protected:
	double slope;
	double max_nb;

public:
	cls_ann_param_kohonen_standard(double a_max_nb,double slope)
		:slope(-slope),
		 max_nb(a_max_nb)
	{
		;
	}

	virtual void re_compute()
	{
		set_lp(exp(slope * (log((double)get_step()) * log((double)get_step())  )));
		set_nb(max_nb * get_learnng_param());
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_param_kohonen_dynamic::save(fout);
			fout.write((char*)&slope,sizeof(slope));
			fout.write((char*)&max_nb,sizeof(max_nb));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_param_kohonen_standard::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin ) 
	{
		try
		{
			cls_ann_param_kohonen_dynamic::load(fin);
			fin.read((char*)&slope,sizeof(slope));
			fin.read((char*)&max_nb,sizeof(max_nb));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_param_kohonen_standard::load() - the file is bad for input.\n");
		}
	}
};


template<typename FUNC>
class cls_ann_network_kohonen 
	: public cls_ann_network
{
protected:
	cls_ann_layer_input<FUNC> layer_input;
	cls_ann_layer<FUNC> layer_output;
	cls_ann_topology& topology;
	cls_ann_param_kohonen& params;
	cls_ann_public_values &control;
	mutable unsigned winner;
	mutable bool winner_valid;

	void connect_layer()
	{
		for(unsigned i=0;i<get_output_count();i++)
		{
			cls_ann_abstract_neuron<FUNC> &dest = (cls_ann_abstract_neuron<FUNC>&)(layer_output.get_neuron(i));

			for(unsigned j=0;j<get_input_count();j++)
			{
				cls_ann_neuron<FUNC> &src = layer_input.get_neuron(j);
				dest.connect_from(&src);
			}
		}
	}

	double get_nb_size() const
	{
		return params.get_neighborhood_size();
	}

	double get_learning_param() const
	{
		return params.get_learnng_param();
	}

	void set_input(const vector<double> & input)
	{
		winner_valid = false;
		layer_input.set_input(input);
	}

public:
	enum {en_input_layer,en_output_layer};


	cls_ann_network_kohonen(unsigned inputs,cls_ann_topology& atopoolohy,cls_ann_param_kohonen & aparam,cls_ann_public_values &ctrl = default_control)
		:cls_ann_network(inputs,atopoolohy.get_total_count()),
		 layer_input(en_input_layer),
		 layer_output(en_output_layer),
		 topology(atopoolohy),
		 params(aparam),
		 control(ctrl),
		 winner_valid(false)
	{
		for(unsigned i=0;i<inputs;i++)
		{
			layer_input.add_neuron(new cls_ann_input_neuron<FUNC>(i));
		}

		for(unsigned i=0;i<topology.get_total_count();i++)
		{
			layer_output.add_neuron(new cls_ann_neuron_kohonen<FUNC>(i));
		}

		connect_layer();

		for(unsigned i=0;i<topology.get_total_count();i++)
		{
			((cls_ann_neuron_kohonen<FUNC>&)layer_output[i]).randomize_weights();
		}
	}

	void train(cls_training_set &ts,double terminating_lp = const_term_in_atting)
	{
		params.next_iterator();
		int epoch = 0;
		for(epoch= 0;get_learning_param() >= terminating_lp;epoch++)
		{
			ts.initialize();
			while(!ts.epoch_over())
			{
				get_next_pair_for_ts(ts,i,o);
				train_example(i);
			}

			params.next_iterator();
		}
	}

	virtual void train_example(cls_training_set & ts)
	{
		params.next_iterator();

		ts.initialize();
		
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,i,o);
			train_example(i);
		}
	}

	virtual void train_example(const vector<double> & input)
	{
		for(unsigned cnt=0;cnt<get_output_count();cnt++)
		{
			double w= topology.get_neighbor_weight(winner,cnt,get_nb_size());

			if (w != 0)
			{
				((cls_ann_neuron_kohonen<FUNC>&)layer_output[cnt]).adapt_towards(input,w,get_learning_param());
			}
		}
	}

	unsigned get_winner_output(const vector<double> & input)
	{
		set_input(input);
		return get_winner();
	}

	unsigned get_winner_output(double *nput)
	{
		vector<double> input;
		for(int cnt=0;cnt<get_input_count();cnt++)
		{
			input.push_back(nput[cnt]);
		}

		set_input(input);
		return get_winner();
	}


	unsigned get_winner()
	{
		if (winner_valid)
		{
			return winner;
		}

		double mn = 1e100;
		int mn_i = -1;
		for(unsigned cnt=0;cnt<get_output_count();cnt++)
		{
			double o = layer_output[cnt].get_output();
			if (o < mn)
			{
				mn  = o;
				mn_i = (int)cnt;
			}
		}

		winner = mn_i;
		winner_valid = true;

		return winner;
	}

	virtual vector<double> get_output(const vector<double> & input)
	{
		throw cls_exception_not_sense("cls_ann_network_kohonen::get_output() - you must call the get_winner_output().");

		set_input(input);
		return layer_output.get_output();
	}

	virtual vector<double> get_output(double *input)
	{
		return cls_ann_network::get_output(input);
	}
	const cls_ann_layer<FUNC> & get_output_layer() const
	{
		return layer_output;
	}

	virtual vector<double> get_cluster(unsigned cluster_num) const
	{
		vector<double> out;
		layer_output[cluster_num].get_weights(out);

		return out;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network::save(fout);
			params.save(fout);
			topology.save(fout);
			fout.write((char*)&winner,sizeof(winner));
			fout.write((char*)&winner_valid,sizeof(winner_valid));
			layer_input.save(fout);
			layer_output.save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_kohonen::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin ) 
	{
		try
		{
			cls_ann_network::load(fin);
			params.load(fin);
			topology.load(fin);
			fin.read((char*)&winner,sizeof(winner));
			fin.read((char*)&winner_valid,sizeof(winner_valid));
			layer_input.load(fin);
			layer_output.load(fin);

			for(int i=0;i<layer_input.get_size();i++)
			{
				for(int j=0;j<layer_output.get_size();j++)
				{
					layer_input.get_neuron(i).connect_load(&layer_output.get_neuron(j),j);
				}
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_kohonen::load() - the file is bad for input.\n");
		}
	}
};