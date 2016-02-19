#include "ann_import.h"

#define  cls_ann_layer_recurrent TLayer<cls_ann_neuron_recurrent<FUNC>,FUNC>

template<typename FUNC>
class cls_ann_network_recurrent : public cls_ann_network
{

protected:
	cls_ann_layer_input<FUNC> inputs;
	cls_ann_layer_recurrent neurons;
	unsigned time;

public:
	cls_ann_network_recurrent(unsigned init_intput_count = 0,unsigned init_recurrent_count = 0)
		:cls_ann_network(0,0),
		 inputs(const_input_neurons_label_offset),
		 neurons(0)
	{
		time = 0;
		for(int cnt=0;cnt<init_intput_count;cnt++)
		{
			add_input(0.0);
		}

		for(int cnt=0;cnt<init_recurrent_count;cnt++)
		{
			add_neuron();
		}
	}

	void add_input(double init_value=0)
	{
		cls_ann_input_neuron<FUNC> *i = new cls_ann_input_neuron<FUNC>(get_input_count());
		i->set_value(init_value);
		inputs.add_neuron(i);
	}

	void add_input(cls_ann_input_neuron<FUNC> *in)
	{
		inputs.add_neuron(in);
	}

	void add_neuron(double ab = 0.0,double ac =0.0)
	{
		neurons.add_neuron(new cls_ann_neuron_recurrent<FUNC>(ab,ac));
	}

	void add_neuron(cls_ann_neuron_recurrent<FUNC> *n)
	{
		neurons.add_neuron(n);
	}

	virtual unsigned get_input_count() const
	{
		return inputs.size();
	}

	virtual unsigned get_output_count() const
	{
		return neurons.size();
	}

	virtual void set_input(const vector<double> &input)
	{
		for(int cnt=0;cnt<get_input_count();cnt++)
		{
			((cls_ann_input_neuron<FUNC>&) inputs[cnt]).set_value(input[cnt]);
		}
	}

	cls_ann_neuron_recurrent<FUNC> &get_neuron(unsigned index)
	{
		return neurons[index];
	}

	cls_ann_input_neuron<FUNC> & get_input_neuron(unsigned index)
	{
		return inputs[index];
	}

	virtual void reset_activations(const vector<double> & activations)
	{
		for(int cnt=0;cnt<get_output_count();cnt++)
		{
			neurons[cnt].reset(activations[cnt]);
		}
	}

	virtual void step()
	{
		for(unsigned cnt=0;cnt<get_output_count();cnt++)
		{
			neurons[cnt].update();
		}

		time++;
	}

	virtual double get_neuron_output(unsigned neuron)
	{
		return neurons[neuron].get_output();
	}

	virtual vector<double> get_output() const
	{
		vector<double> answer(get_output_count());
		for(unsigned cnt=0;cnt<get_output_count();cnt++)
		{
			answer[cnt] = neurons[cnt].get_output();
		}

		return answer;
	}

	virtual vector<double> get_output(const vector<double> &input)
	{
		set_input(input);
		step();

		return get_output();
	}

	cls_ann_neuron_recurrent<FUNC> &operator[] (unsigned index)
	{
		return get_neuron(index);
	}

	virtual void set_meta_data( string meta_data ) 
	{
		
	}

	virtual string get_meta_data() 
	{
		return "";
	}

	virtual void save( ofstream& fout ) const
	{
		throw std::exception("cls_ann_network_recurrent::save() - the method or operation is not implemented.\n");
	}

	virtual void load( ifstream& fin ) 
	{
		throw std::exception("cls_ann_network_recurrent::laod() - the method or operation is not implemented.\n");
	}
};
