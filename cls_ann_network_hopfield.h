#include "ann_import.h"

inline
double a_check_positive(double x)
{
	return (x<=0.0)?0.0:1.0;
}

template<typename FUNC>
class cls_ann_network_hopfield_node_data
	: public cls_serialized_object
{
public:
	vector<double> biases;
	vector<double> outputs;

	void set_size(unsigned size)
	{
		biases.resize(size);
		outputs.resize(size);
	}

	cls_ann_network_hopfield_node_data(unsigned size)
	{
		set_size(size);
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_network_hopfield_node_data::save() - the file is bad for output.\n");

		int num;
		num = biases.size();
		fout.write((char*)&num,sizeof(int));
		num = outputs.size();
		fout.write((char*)&num,sizeof(int));

		vector<double>::const_iterator itor;
		for(itor = biases.begin();itor != biases.end();itor ++)
		{
			double d = *itor;
			fout.write((char*)&d,sizeof(double));
		}

		for(itor = outputs.begin();itor != outputs.end();itor ++)
		{
			double d = *itor;
			fout.write((const char*)&d,sizeof(double));
		}
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_ann_network_hopfield_node_data::load() - the file is bad for input.\n");

		int num = 0;
		fin.read((char*)&num,sizeof(int));
		biases.resize(num);

		fin.read((char*)&num,sizeof(int));
		outputs.resize(num);

		vector<double>::iterator itor;
		for(itor = biases.begin();itor != biases.end();itor ++)
		{
			double d;
			fin.read((char*)&d,sizeof(double));
			*itor = d;
		}

		for(itor = outputs.begin();itor != outputs.end();itor ++)
		{
			double d;
			fin.read((char*)&d,sizeof(double));
			*itor = d;
		}
	}
};

template<typename FUNC>
class cls_ann_network_hopfield_base : public cls_ann_network
{
protected:
	bool bipolar;
	int n_patterns;
	int time;
	cls_ann_network_hopfield_node_data<FUNC> neurons;
	FUNC func;

	int double_to_int(double r) const
	{
		if (bipolar)
			return r>0?1:-1;
		else
			return r>0.5?1:0;
	}

	double _get_bias(unsigned cnt)
	{
		return neurons.biases[cnt];
	}

	void _set_bias(unsigned cnt,double bias)
	{
		neurons.biases[cnt] = bias;
	}

	void set_neuron_output(unsigned cnt,double a)
	{
		neurons.outputs[cnt] = a;
	}

	double get_neuron_output(unsigned cnt) const
	{
		return neurons.outputs[cnt];
	}

	cls_ann_network_hopfield_base()
		:cls_ann_network(0,0),
		 neurons(0)
	{

	}

public:
	virtual void train_example(cls_training_set & ts)
	{
		throw cls_exception_not_sense("cls_ann_network_hopfield_base::train_example() - you must know how to use hopfield,the is of no sense.\n");
	}

	virtual double get_weight(unsigned from,unsigned to) = 0;
	virtual void set_weight(unsigned from ,unsigned to, double weight) = 0;

	cls_ann_network_hopfield_base(unsigned size,bool bias=false,bool bipolar=true)
		:cls_ann_network(0,size),
		 neurons(size)
	{
		n_patterns = 0;
		time = 0;
		this->bipolar = bipolar;

		for(unsigned cnt=0;cnt<size;cnt++)
		{
			set_neuron_output(cnt,0);
			set_bias(cnt,0);
		}
	}

	virtual ~cls_ann_network_hopfield_base()
	{

	}

	virtual double get_energy()
	{
		double d_energy = 0;
		for(unsigned i=0;i<get_size();i++)
		{
			for(unsigned j=0;j<get_size();j++)
			{
				double weight = get_weight(i,j);
				double out_i = neurons.outputs[i];
				double out_j = neurons.outputs[j];
				d_energy += weight * out_i * out_j;
			}
		}

		return -1 * d_energy;
	}

	virtual double get_energy(int pattern[])
	{
		double d_energy = 0.0;
		for(unsigned i=0;i<get_size();i++)
		{
			for(unsigned j=0;j<get_size();j++)
			{
				double weight = get_weight(i,j);
				d_energy += weight * pattern[i] * pattern[j];
			}
		}

		return -1 * d_energy;
	}

	virtual unsigned get_size() const
	{
		return get_output_count();
	}

	virtual void step()
	{
		unsigned size = get_size();
		for(unsigned i=0;i<size;i++)
		{
			double d_sum = 0;
			for(unsigned j=0;j<size;j++)
			{
				d_sum += get_neuron_output(j) * get_weight(i,j);
			}

			set_neuron_output(i,func(d_sum + get_bias(i)));
			time++;
		}
	}

	virtual int get_time()
	{
		return time;
	}

	void randomize()
	{
		vector<double> v_d(get_size());
		for(unsigned cnt=0;cnt<get_size();cnt++)
		{
			int r = rand()%3;
			if (bipolar && !r)
				r = -1;

			v_d[cnt] = r;
		}

		set_input(v_d);
	}

	virtual double get_bias(unsigned i)
	{
		if (i < 0 || i >=get_size())
		{
			throw cls_exception_overflow("cls_ann_network_hopfield::get_bias() - invalid neuron number.");
		}

		return _get_bias(i);
	}

	virtual void set_bias(unsigned i,double bias)
	{
		if (i < 0 || i >=get_size())
		{
			throw cls_exception_overflow("cls_ann_network_hopfield::get_bias() - invalid neuron number.");
		}

		_set_bias(i,bias);
	}

	virtual void set_input(vector<int> patten)
	{
		time = 0;
		int size = get_size();
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			set_neuron_output(cnt,patten[cnt]);
		}
	}

	virtual void set_input(int patten[])
	{
		time = 0;
		int size = get_size();
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			set_neuron_output(cnt,patten[cnt]);
		}
	}

	virtual void set_input(vector<double> patten)
	{
		time = 0;
		int size = get_size();
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			set_neuron_output(cnt,patten[cnt]);
		}
	}

	virtual void set_input(double patten[])
	{
		time = 0;
		int size = get_size();
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			set_neuron_output(cnt,patten[cnt]);
		}
	}

	virtual vector<double> get_output() const
	{
		int size =	get_output_count();
		vector<double> re(size);
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			re[cnt] = neurons.outputs[cnt];
		}

		return re;
	}

	virtual vector<int> get_output_int() const
	{
		vector<double> o = get_output();
		vector<int> answer(o.size());

		int size_output = o.size();
		for(unsigned cnt=0;cnt<size_output;cnt++)
		{
			answer[cnt] = double_to_int(o[cnt]);
 		}

		return answer;
	}

	virtual vector<double> get_next_output()
	{
		step();
		return get_output();
	}

	virtual vector<double> get_output(const vector<double> &input)
	{
		unsigned size = input.size();
		if (size != get_size())
		{
			throw cls_exception_io("dim dosn't match.");
		}

		set_input(input);
		step();

		return get_output();
	}

	virtual bool propagate(vector<int> &patten , unsigned timeout)
	{
		set_input(patten);
		vector<int> last;
		vector<int> curr;
		last = patten;
		for(unsigned time = 0;time <timeout;time++)
		{
			step();
			curr = get_output_int();
			if (curr == last)
				return true;
			last = curr;
		}

		return false;
	}

	virtual bool propagate(int * patten , unsigned timeout)
	{
		vector<int> p;
		int size = get_output_count();
		for(unsigned cnt=0;cnt<size;cnt++)
		{
			p.push_back(patten[cnt]);
		}

		return propagate(p,timeout);
	}

	virtual bool propagate(vector<double> &pattern,unsigned timeout)
	{
		set_input(pattern);
		vector<double> last;
		vector<double> curr;
		last = pattern;
		for(unsigned time = 0;time <timeout;time++)
		{
			step();
			curr = get_next_output();
			if (curr == last)
				return true;
			last = curr;
		}

		return false;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network::save(fout);

			fout.write((char*)&bipolar,sizeof(bipolar));
			fout.write((char*)&n_patterns,sizeof(n_patterns));
			fout.write((char*)&time,sizeof(time));

			neurons.save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io("cls_ann_network_hopfield_base::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_network::load(fin);

			fin.read((char*)&bipolar,sizeof(bipolar));
			fin.read((char*)&n_patterns,sizeof(n_patterns));
			fin.read((char*)&time,sizeof(time));

			neurons.load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io("cls_ann_network_hopfield_base::load() - the file is bad for input.\n");
		}
	}
};

template<typename FUNC>
class cls_ann_network_hopfield : public cls_ann_network_hopfield_base<FUNC>
{
protected:
	cls_matrix<double> mat_weight;

	virtual double get_weight(unsigned from,unsigned to)
	{
		return mat_weight(from,to);
	}

public:
	cls_ann_network_hopfield(unsigned size,bool bias=false,bool bipolar= true)
		:cls_ann_network_hopfield_base<FUNC>(size,bias,bipolar),
		 mat_weight(size,size)
	{
		for(int i=0;i<size;i++)
		{
			for(int j=0;j<size;j++)
			{
				mat_weight(i,j) = 0.0;
			}
		}
	}

	virtual void set_weight(unsigned from ,unsigned to, double weight)
	{
		mat_weight(from,to) = weight;
		mat_weight(to,from) = weight;
	}

	virtual void add_pattern(int pattern[])
	{
		int size=this->get_size();
		for(int i=0;i<size;i++)
		{
			if (this->bipolar && pattern[i] != -1 && pattern[i] != 1)
			{
				throw cls_exception_special_value("cls_ann_network_hopfield::add_pattern() - it can only accept 1 and -1.");
			}

			if (!this->bipolar && pattern[i] != 0 && pattern[i] !=1)
			{
				throw cls_exception_special_value("cls_ann_network_hopfield::add_pattern() - it can only accept 1 and 0.");
			}

			for(int j=i+1;j<size;j++)
			{
				double temp = get_weight(i,j);
				temp *= size;
				temp += pattern[i] * pattern[j];
				temp /= size;
				set_weight(i,j,temp);
			}
		}

		this->n_patterns++;
	}

	virtual unsigned get_patten_count()
	{
		return this->n_patterns;
	}

	cls_matrix<double> & get_mat_weight()
	{
		return mat_weight;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network_hopfield_base<FUNC>::save(fout);
			mat_weight.save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_hopfield::save() - file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_network_hopfield_base<FUNC>::load(fin);
			mat_weight.load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_hopfield::save() - file is bad for output.\n");
		}
	}
};
