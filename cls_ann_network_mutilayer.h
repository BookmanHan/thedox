#include "ann_import.h"

struct	cls_ann_error
{
	void operator+= (const cls_ann_error &e)
	{
		for(int cnt=0;cnt<_val.size();cnt++)
			_val[cnt] += e._val[cnt];
	}

	cls_ann_error(const vector<double> &diff)
		:_val(2)
	{
		zero();
		for(unsigned cnt=0;cnt<diff.size();cnt++)
			(*this) += cls_ann_error(diff[cnt]);
	}

	cls_ann_error() :_val(2)
	{
		zero();
	}

	cls_ann_error(double delta)
		:_val(2)
	{
		_val[0] = delta * delta;
		_val[1] = abs(delta);
 	}

	void zero()
	{
		_val[0] = 0;
		_val[1] = 0;
	}

	double get_sq() const
	{
		return _val[0];
	}

	double get_abs() const
	{
		return _val[1];
	}

	void publish(string name,double per=0, cls_ann_public_values &pv=default_control)
	{
		pv.init(name,get_abs());
		if (per != 0)
		{
			pv.init(string("normalized") + name, get_abs() / per);
		}
	}
protected:
	vector<double> _val;
};

template<typename FUNC>
class cls_ann_network_multilayer : public cls_ann_network
{
protected:
	cls_ann_error ex_er;
	unsigned n_layers;
	unsigned size_output;
	vector<cls_ann_layer<FUNC>*> layers;
	cls_ann_layer_input<FUNC> *input_layer()
	{
		return (cls_ann_layer_input<FUNC>*)layers[0];
	}

	cls_ann_layer<FUNC> * output_layer()
	{
		return layers[get_layer_count()];
	}

	void layer_valid(unsigned layer) const
	{
		if (layer <= 0 || layer > n_layers)
		{
			throw std::exception("out of scripts");
		}
	}

	void connect_layer(cls_ann_layer<FUNC> & src,cls_ann_layer<FUNC> & des)
	{
		unsigned size_src = src.get_size();
		unsigned size_des = des.get_size();
		for(unsigned i =0;i<size_des;i++)
		{
			cls_ann_simple_neuron<FUNC> &des_nrn = (cls_ann_simple_neuron<FUNC>&) des.get_neuron(i);
			for(unsigned j=0;j<size_src;j++)
			{
				des_nrn.connect(&src.get_neuron(j));
			}
		}
	}

	void connect_layer_load(unsigned src_lay)
	{
		unsigned size_src = layers[src_lay]->get_size();
		unsigned size_des = layers[src_lay + 1]->get_size();
		for(unsigned i =0;i<size_src;i++)
		{
			for(unsigned j=0;j<size_des;j++)
			{
				connect_load(src_lay,i,j);
			}
		}
	}
public:
	unsigned neuron_label_offset;

	cls_ann_network_multilayer(unsigned inputs)
		:cls_ann_network(inputs,0),
		 neuron_label_offset(0)
	{
		n_layers = 0;
		cls_ann_layer_input<FUNC> *input = new cls_ann_layer_input<FUNC>(neuron_label_offset,inputs);
		layers.push_back(input);
	}

	cls_ann_network_multilayer(unsigned inputs,unsigned _neuron_label_offset,cls_ann_public_values & pv)
		:cls_ann_network(inputs,0),
		 _control(&pv),
		 neuron_label_offset(_neuron_label_offset)
		 {
			 n_layers = 0;
			 cls_ann_layer_input<FUNC> *input = new cls_ann_layer_input<FUNC>(neuron_label_offset,inputs);
			 layers.push_back(input);
		 }

	virtual ~cls_ann_network_multilayer()
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_layer<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_layer<FUNC>*>::iterator itor;
		#endif

		for(itor = layers.begin();itor != layers.end();itor++)
		{
			delete (*itor);
		}
	}

	virtual void add_layer(int size)
	{
		n_layers++;
		cls_ann_layer<FUNC> * layer = new cls_ann_layer<FUNC>(neuron_label_offset + n_layers);
		for(int cnt=0;cnt<size;cnt++)
		{
			layer->add_neuron(new cls_ann_simple_neuron<FUNC>(layer->get_label() * const_max_layer_size + cnt,false));
		}

		size_output = size;
		cls_ann_network::n_output = size_output;
		layers.push_back(layer);
	}

	virtual void add_layer_load(int size)
	{
		cls_ann_layer<FUNC> * layer = new cls_ann_layer<FUNC>(neuron_label_offset + n_layers);
		for(int cnt=0;cnt<size;cnt++)
		{
			layer->add_neuron(new cls_ann_simple_neuron<FUNC>(layer->get_label() * const_max_layer_size + cnt,false));
		}

		size_output = size;
		cls_ann_network::n_output = size_output;
		layers.push_back(layer);
	}

	virtual void connect_layer(unsigned src)
	{
		if (src <0 || src > get_layer_count() - 1)
		{
			throw cls_exception_build_error("invalid source layer.");
		}

		connect_layer(*layers[src],*layers[src+1]);
	}

	virtual void connect(unsigned src,int src_nrn,int des_nrn)
	{
		connect(src,des_nrn,cls_ann_abstract_neuron<FUNC>::get_random_weight());
	}

	virtual void connect(unsigned src,int src_nrn,int des_nrn,double weight)
	{
		if (src <0 || src > get_layer_count() - 1)
		{
			throw cls_exception_build_error("invalid source layer.");
		}

		cls_ann_layer<FUNC> * lay_src = layers[src];
		cls_ann_layer<FUNC> * lay_des = layers[src+1];
		unsigned size_src = lay_src->get_size();
		unsigned size_des = lay_des->get_size();

		cls_ann_simple_neuron<FUNC> &destination = (cls_ann_simple_neuron<FUNC>&) lay_des->get_neuron(des_nrn);
		destination.connect(&lay_src->get_neuron(src_nrn),weight);
	}

	virtual void connect_load(unsigned src,int src_nrn,int des_nrn)
	{
		if (src <0 || src > get_layer_count() - 1)
		{
			throw cls_exception_build_error("invalid source layer.");
		}

		cls_ann_layer<FUNC> * lay_src = layers[src];
		cls_ann_layer<FUNC> * lay_des = layers[src+1];

		lay_src->get_neuron(src_nrn).connect_load(&lay_des->get_neuron(des_nrn),des_nrn);
	}

	cls_ann_public_values *_control;
	cls_ann_public_values &get_control()
	{
		return *_control;
	}

	void set_control(cls_ann_public_values &ctrl)
	{
		_control  = &ctrl;
	}

	void set_label_offset(unsigned first_label)
	{
		neuron_label_offset = first_label;
	}

	virtual void set_bias(unsigned layer,int nrn,double bias)
	{
		if (layer <0 || layer > get_layer_count())
		{
			throw cls_exception_build_error("out of scripts");
		}

		cls_ann_simple_neuron<FUNC> &n = (cls_ann_simple_neuron<FUNC>&)layers[layer]->get_neuron(nrn);
		n.set_bias(bias);
	}

	virtual unsigned get_layer_count() const
	{
		return n_layers;
	}

	virtual vector<double> get_output(const vector<double> & input)
	{
		if (get_layer_count() == 0)
		{
			throw cls_exception_build_error("there is no output layer.");
		}

		input_layer()->set_input(input);

		try
		{
			return output_layer()->get_output();
		}
		catch (std::exception exp)
		{
			throw exp;
		}
	}

	virtual vector<double> get_output(double *input)
	{
		return cls_ann_network::get_output(input);
	}

	virtual void train(cls_training_set & ts,cls_ann_public_real n_times,cls_ann_public_real d_learning_rate = const_default_learning_rate,cls_ann_public_real d_momentum = const_default_momentum)
	{
		if (ts.get_input_size() != get_input_count() ||
			ts.get_output_size() != get_output_count())
		{
			throw cls_exception_not_identical("not compatible.");
		}

		cls_ann_public_real teminate = get_control().get("terminate");
		cls_ann_public_real epoch = get_control().init("epoch",0);

		for(;epoch < n_times;++epoch)
		{
			cls_ann_error er;
			ts.initialize();

			unsigned examples = 0;

			while(! ts.epoch_over())
			{
				get_next_pair_for_ts(ts,input,desired);
				train_example(input,desired,d_learning_rate,d_momentum);
				er += ex_er;
				++examples;
			}

			er.publish("epoch error",examples,get_control());
		}
	}

	virtual void train_example(cls_training_set &ts)
	{
		cls_ann_error er;
		ts.initialize();

		while(! ts.epoch_over())
		{
			get_next_pair_for_ts(ts,input,desired);
			train_example(input,desired,const_default_learning_rate,const_default_momentum);
			er += ex_er;
		}
	}

	virtual void train(cls_training_set & ts,unsigned int n_times,double d_learning_rate = const_default_learning_rate,double d_momentum = const_default_momentum)
	{
		cls_ann_public_values pv;
		for(int cnt=0;cnt<n_times;cnt++)
		{
			cls_ann_error er;

			ts.initialize();

			while(! ts.epoch_over())
			{
				get_next_pair_for_ts(ts,input,desired);
				train_example(input,desired,d_learning_rate,d_momentum);
				er += ex_er;
			}
		}
	}

	virtual void train(cls_training_set & ts,cls_ann_public_values & p)
	{
			train(ts, p["epochs"], p["learningRate"], p["momentum"]);
	}

	virtual cls_ann_error get_error(cls_training_set & ts)
	{
		train(ts,1,0,0);
		return ex_er;
	}

	virtual unsigned get_output_count()
	{
		return size_output;
	}

	virtual unsigned get_input_count()
	{
		return cls_ann_network::get_input_count();
	}

	virtual void train_example(const vector<double> & input,const vector<double> & desired_ouput,double d_learning_reate = const_default_learning_rate,double d_momentum = const_default_momentum)
	{
		input_layer()->set_input(input);
		ex_er.zero();

		unsigned s_output = get_output_count();
		unsigned n_layer = get_layer_count();

		for(unsigned i=0;i<s_output;i++)
		{
			cls_ann_simple_neuron<FUNC> &n = (cls_ann_simple_neuron<FUNC>&)output_layer()->get_neuron(i);
			n.set_desired_output(desired_ouput[i]);
			double delta = n.get_output() - desired_ouput[i];
			ex_er += cls_ann_error(delta);
		}

		for(int i=(int)n_layers;i>0;i--)
		{
			unsigned size_layer = layers[i]->get_size();
			for(unsigned j=0;j<size_layer;j++)
			{
				cls_ann_simple_neuron<FUNC> &n = (cls_ann_simple_neuron<FUNC>&)layers[i]->get_neuron(j);
				n.calc_new_weights(d_learning_reate,d_momentum);
			}
		}

		for(unsigned i = 1;i<=n_layers;i++)
		{
			unsigned size_layer = layers[i]->get_size();
			for(unsigned j=0;j<size_layer;j++)
			{
				cls_ann_simple_neuron<FUNC> &n =(cls_ann_simple_neuron<FUNC>&)layers[i]->get_neuron(j);
				n.update();
			}
		}
	}

//	virtual void saved(string str_filename)
//	{
//		ofstream fout;
//		fout<<get_input_count()<<" ";
//		fout<<get_output_count()<<" ";
//		vector<cls_ann_layer<FUNC>*>::iterator itor;
//
//		for(itor=layers.begin();itor != layers.end();itor++)
//		{
//
//			fout<<(*itor)->get_size()<<" ";
//			for(unsigned cnt=0;cnt<(*itor)->get_size();cnt++)
//			{
//				cls_ann_simple_neuron<FUNC> &n = (cls_ann_simple_neuron<FUNC>&)((*itor)->get_neuron(cnt));
//				fout<<n.get_bias()<<" ";
//			}
//		}
//
//		for(itor=layers.begin();itor != layers.end();itor++)
//		{
//			unsigned size_layer = (*itor)->get_size();
//			for(int cnt=0;cnt<size_layer;cnt++)
//			{
//				cls_ann_neuron<FUNC> *nrn = &((*itor)->get_neuron(cnt));
//				vector<int> labels;
//				vector<double> weights;
//
//				unsigned conns = nrn->get_inputs(labels,weights);
//				for(unsigned j=0;j<conns;j++)
//				{
//					fout<<weights[j];
//				}
//			}
//		}
//	}

	void reset_weights()
	{
		for(unsigned i=0;i<get_layer_count();i++)
		{
			unsigned size_layer = layers[i]->get_size();
			for(unsigned j=0;j<size_layer;j++)
			{
				cls_ann_simple_neuron<FUNC>& n = (cls_ann_simple_neuron<FUNC>&) layers[i]->get_neuron(j);
				n.randomize_weights();
			}
		}
	}

	const cls_ann_layer<FUNC> & get_layer(unsigned layer) const
	{
		return get_layer(layer);
	}

	unsigned get_neurons_count() const
	{
		unsigned sum = 0;
		for(unsigned cnt=1;cnt<get_layer_count();cnt++)
		{
			sum += get_layer(cnt).get_size();
		}

		return sum;
	}

	unsigned get_links_count() const
	{
		unsigned sum = 0;
		for(unsigned i=0;i<get_layer_count();i++)
		{
			for(unsigned j=0;j<get_layer(i).get_size();j++)
			{
				sum += get_layer(i).get_neuron(j).get_input_count();
			}
		}

		return sum;
	}

	cls_ann_layer<FUNC> & get_layer(unsigned layer)
	{
		layer_valid(layer);
		return *layers[layer];
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network::save(fout);
			int num = layers[0]->get_size();

			fout.write((char*)&num,sizeof(int));
			fout.write((char*)&n_layers,sizeof(n_layers));
			fout.write((char*)&neuron_label_offset,sizeof(neuron_label_offset));
			fout.write((char*)&size_output,sizeof(size_output));

			layers[0]->save(fout);

			for(int cnt=1;cnt<=n_layers;cnt++)
			{
				int num = layers[cnt]->get_size();
				fout.write((char*)&num,sizeof(int));

				layers[cnt]->save(fout);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_multilayer::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_layer<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_layer<FUNC>*>::iterator itor;
		#endif

		for(itor = layers.begin();itor != layers.end();itor++)
		{
			delete (*itor);
		}

		layers.clear();

		try
		{
			cls_ann_network::load(fin);

			int num = 0;
			fin.read((char*)&num,sizeof(int));
			fin.read((char*)&n_layers,sizeof(n_layers));
			fin.read((char*)&neuron_label_offset,sizeof(neuron_label_offset));
			fin.read((char*)&size_output,sizeof(size_output));

			cls_ann_layer_input<FUNC> * li = new cls_ann_layer_input<FUNC>(neuron_label_offset,num);
			li->load(fin);
			layers.push_back(li);

			for(int cnt=1;cnt<=n_layers;cnt++)
			{
				int num_d;
				fin.read((char*)&num_d,sizeof(int));
				add_layer_load(num_d);

				layers[cnt]->load(fin);
			}

			for(int cnt=0;cnt<n_layers;cnt++)
			{
				connect_layer_load(cnt);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_multilayer::save() - the file is bad for output.\n");
		}
	}
};
