#include "ann_import.h"

template<typename FUNC1,typename FUNC2>
class cls_ann_network_radial_basis 
	: public cls_ann_network
{
protected:
	int n_centers;
	cls_ann_layer_input<FUNC1> * layer_input;
	cls_ann_layer<FUNC1> * layer_center;
	cls_ann_layer<FUNC2> * layer_output;

public:
	cls_ann_network_radial_basis(int inputs,int centers,int outputs)
		:cls_ann_network(inputs,outputs)
	{
		layer_input = new cls_ann_layer_input<FUNC1>(0,inputs);
		n_centers = centers;

		layer_center = new cls_ann_layer<FUNC1>(1);
		for(int i=0;i<centers;i++)
		{
			cls_ann_center_neuron<FUNC1> * c = new cls_ann_center_neuron<FUNC1>(const_max_layer_size*1 +i,inputs);
			for(int j=0;j<inputs;j++)
			{
				c->connect((cls_ann_neuron<FUNC1>*)&layer_input->get_neuron(j));
			}

			layer_center->add_neuron(c);
		}

		layer_output = new cls_ann_layer<FUNC2>(2);
		for(int i=0;i<outputs;i++)
		{
			cls_ann_simple_neuron<FUNC2> *n = new cls_ann_simple_neuron<FUNC2>(const_max_layer_size*2 + i ,true);
			for(int j=0;j<centers;j++)
			{
				n->connect((cls_ann_neuron<FUNC2>*)&layer_center->get_neuron(j));
			}

			layer_output->add_neuron(n);
		}
	}

	virtual ~cls_ann_network_radial_basis()
	{
		delete [] layer_input;
		delete [] layer_output;
		delete [] layer_center;
	}

	virtual void set_center(unsigned i,vector<double> & center)
	{
		cls_ann_center_neuron<FUNC1> & c = (cls_ann_center_neuron<FUNC1>&)layer_center->get_neuron(i);
		c.set_center(center);
	}

	virtual void set_center(unsigned i,double * center)
	{
		cls_ann_center_neuron<FUNC1> & c = (cls_ann_center_neuron<FUNC1>&)layer_center->get_neuron(i);
		c.set_center(center);
	}

	virtual vector<double> get_center(int i) const
	{
		vector<double> answer;
		answer = ((cls_ann_center_neuron<FUNC1>&)layer_center->get_neuron(i)).get_center();

		return answer;
	}

	virtual unsigned get_center_count() const
	{
		return n_centers;
	}

	virtual void set_bias(unsigned i,double bias)
	{
		cls_ann_simple_neuron<FUNC2> &n = (cls_ann_simple_neuron<FUNC2>&)(layer_output->get_neuron(i));
		n.set_bias(bias);
	}

	virtual double get_bias(unsigned i) const
	{
		cls_ann_simple_neuron<FUNC2> &n = (cls_ann_simple_neuron<FUNC2>&)(layer_output->get_neuron(i));

		return n.get_bias();
	}

	virtual void remove_bias(unsigned i)
	{
		cls_ann_simple_neuron<FUNC2> &o = (cls_ann_simple_neuron<FUNC2>&)layer_output->get_neuron(i);
		o.remove_bias();
	}

	virtual void set_weight(int center,int output,double weight)
	{
		cls_ann_center_neuron<FUNC1> &c = (cls_ann_center_neuron<FUNC1>&)layer_center->get_neuron(center);
		cls_ann_simple_neuron<FUNC2> &n = (cls_ann_simple_neuron<FUNC2>&)layer_output->get_neuron(output);
		n.connect((cls_ann_simple_neuron<FUNC2>*)&c,weight);
	}

	virtual double get_weight(int center,int output) const
	{
		cls_ann_center_neuron<FUNC1> &c = (cls_ann_center_neuron<FUNC1>&) layer_center->get_neuron(center);
		cls_ann_simple_neuron<FUNC2> &o = (cls_ann_simple_neuron<FUNC2>&) layer_center->get_neuron(output);

		return o.get_weight((cls_ann_simple_neuron<FUNC2>*)&c);
	}

	virtual vector<double> get_output(const vector<double> &input)
	{
		layer_input->set_input(input);
		return layer_output->get_output();
	}

	virtual vector<double> get_output(double *input)
	{
		return cls_ann_network::get_output(input);
	}

	virtual void train_example(cls_training_set &ts)
	{
		train_weights(ts);
	}

	void train_weights(cls_training_set &ts)
	{
		if (ts.get_input_size() != layer_input->get_size())
		{
			throw cls_exception_training_set("invalid training set.");
		}

		int size_data = ts.get_size();
		int size_center = get_center_count();
		int size_output = get_output_count();
		for(int output=0;output<size_output;output++)
		{
			ts.initialize();
			bool has_bias;
			int eh = size_center;

			cls_ann_simple_neuron<FUNC2> &output_nrn = (cls_ann_simple_neuron<FUNC2>&)layer_output->get_neuron(output);

			has_bias = output_nrn.check_bias();
			if (has_bias)
				eh++;

			cls_matrix<double> y(size_data,1);
			cls_matrix<double> w;
			cls_matrix<double> v(size_data,eh);
			cls_matrix<double> vt;

			if (size_data != eh)
				vt = cls_matrix<double>(eh,size_data);

			for(int i=0;i<size_data;i++)
			{
				get_next_pair_for_ts(ts,in,sy);
				get_output(in);

				for(int j=0;j<size_center;j++)
				{
					v(i,j) = layer_center->get_neuron(j).get_output();
					if (vt.get_width() != 0)
					{
						vt(j,i) = v(i,j);
					}
				}

				if (has_bias)
				{
					v(i,size_center) = 1.0;
					if (vt.get_width() != 0)
					{
						vt(size_center,i) = 1.0; 
					}
				}
				

				y(i,0) = sy[output];
			}

			if (vt.get_width() != 0)
			{
				cls_matrix<double> vtvinv,vty;
				cls_matrix<double> vtv;
				vtv = (vt) *( v);
				vtvinv = vtv.get_inverse();

				vty = (vt) * (y);
				w = vtvinv * vty;
			}
			else
			{
				cls_matrix<double> v_inv;
				v_inv = v.get_inverse();

				w = v_inv * (y);
			}

			for(int j=0;j<size_center;j++)
			{
				set_weight(j,output,w(j,0));
			}

			if (has_bias)	
				output_nrn.set_bias(w(size_center,0));
		}
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network::save(fout);

			fout.write((char*)&n_centers,sizeof(n_centers));

			int num = 0;
			num = layer_center->get_size();
			fout.write((char*)&num,sizeof(int));
			layer_center->save(fout);

			num = layer_input->get_size();
			fout.write((char*)&num,sizeof(int));
			layer_input->save(fout);

			num = layer_output->get_size();
			fout.write((char*)&num,sizeof(int));
			layer_output->save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_radial_basis::save() - the file is bad for input.\n");
		}
	}

	virtual void load( ifstream& fin ) 
	{
		delete layer_center;
		delete layer_input;
		delete layer_output;

		try
		{
			cls_ann_network::load(fin);

			fin.read((char*)&n_centers,sizeof(n_centers));

			int num = 0;
			fin.read((char*)&num,sizeof(int));
			layer_center = new cls_ann_layer<FUNC1>(0,0);
			for(int cnt=0;cnt<num;cnt++)
			{
				layer_center->add_neuron(new cls_ann_center_neuron<FUNC1>(0,0));
			}
			layer_center->load(fin);

			fin.read((char*)&num,sizeof(int));
			layer_input = new cls_ann_layer_input<FUNC1>(0,0);
			for(int cnt=0;cnt<num;cnt++)
			{
				layer_input->add_neuron(new cls_ann_input_neuron<FUNC1>(0));
			}
			layer_input->load(fin);

			fin.read((char*)&num,sizeof(int));
			layer_output = new cls_ann_layer<FUNC2>(0,0);
			for(int cnt=0;cnt<num;cnt++)
			{
				layer_output->add_neuron(new cls_ann_simple_neuron<FUNC2>(0,false));
			}
			layer_output->load(fin);

			for(int i=0;i<layer_input->get_size();i++)
			{
				for(int j=0;j<layer_center->get_size();j++)
				{
					layer_input->get_neuron(i).connect_load(&layer_center->get_neuron(j),j);
				}
			}

			for(int i=0;i<layer_center->get_size();i++)
			{
				for(int j=0;j<layer_output->get_size();j++)
				{
					((cls_ann_neuron<FUNC2>&)layer_center->get_neuron(i)).connect_load(&layer_output->get_neuron(j),j);
				}
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_radial_basis::save() - the file is bad for input.\n");
		}
	}
};