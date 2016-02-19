#include "ann_import.h"

template<typename FUNC>
class cls_ann_center_neuron : public cls_ann_abstract_neuron<FUNC>
{
protected:
	double *center;
	double *delta_center;

	int dim;
	virtual void re_cache_error()
	{
		this->error_cache_valid = true;
	}

	virtual void re_cache_output()
	{
		if (this->output_cache_valid)
			return;

		if (this->get_input_count() != get_dim())
			throw cls_exception_not_identical("cls_ann_center_neuron::re_cache_output() - the input count must match the dim.");

		this->activation_cache = 0.0;

		#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		int j=0;
		for(itor = this->input_links.begin();itor != this->input_links.end();itor++,j++)
		{
			assert(*itor);
			double tmp = (*itor)->get_source()->get_output() - center[j];
			this->activation_cache += (tmp*tmp);
		}

		this->activation_cache = (double)sqrt(1.0*this->activation_cache);
		this->output_cache = (this->func)(this->activation_cache);
		this->output_cache_valid = true;
	}

public:

	cls_ann_center_neuron(int label,int dim)
		:cls_ann_abstract_neuron<FUNC>(label)
	{
		this->dim = dim;
		center = new double[dim];
		for(int cnt=0;cnt<dim;cnt++)
		{
			center[cnt] = a_unfirom_random(-0.1,0.1);
		}
	}

	void calc_new_weights(double learningRate,double a)
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator k1,i;
		#else
        vector<cls_ann_link<FUNC>*>::iterator k1,i;
		#endif

		int k;
		double act,dact,factor;

		#ifdef __USING_GCC__
		cls_ann_link<FUNC> *inl,*outl;
		#else
		cls_ann_link<FUNC> *inl,*outl;
		#endif

		if (this->get_input_count() != get_dim())
		{
			throw cls_exception_not_identical("cls_ann_center_neuron::calc_new_center() - centers must match the dims.");
		}

		for (k1=this->input_links.begin(), k=0; k1!=this->input_links.end(); k1++,k++)
		{
			assert(*k1);
			inl=(cls_ann_link<FUNC>*)(*k1);
			delta_center[k] = (double)0.0;
			act = this->get_activation();
			dact = (double)(exp(-1*act*act)*2);
			factor = (inl->get_source()->get_output() - center[k])*dact;///act;
			for (i=this->output_links.begin(); i!=this->output_links.end(); i++)
			{
				assert(*i);
				outl=(cls_ann_link<FUNC>*)(*i);
				delta_center[k] += outl->get_destination()->get_error() * outl->get_weight();
			}

			delta_center[k] = delta_center[k] * factor * learningRate;
		}
	}

	void update()
	{
		int dim = get_dim();
		int i;
		for (i=0; i<dim; i++)
		{
			center[i] += delta_center[i];
			delta_center[i]=(double)0.0;
		}

		this->invalidate_output_cache();
		this->invalidate_error_cache();
	}

	cls_ann_center_neuron(int label,vector<double> center)
		:cls_ann_neuron<FUNC>(label)
	{
		this->dim = center.size();
		set_center(center);
	}

	cls_ann_center_neuron(int label,int dim,double center[])
		:cls_ann_neuron<FUNC>(label)
	{
		this->dim = dim;
		set_center(center);
	}

	virtual ~cls_ann_center_neuron()
	{
		delete [] center;
	}

	vector<double> get_center() const
	{
		vector<double> re;
		for(int cnt=0;cnt<dim;cnt++)
		{
			re.push_back(center[cnt]);
		}

		return re;
	}

	virtual void set_center(vector<double> center)
	{
		int dim = get_dim();
		for(int cnt=0;cnt<dim;cnt++)
			this->center[cnt] = center[cnt];
	}

	virtual void set_center(double center[])
	{
		int dim = get_dim();
		for(int cnt=0;cnt<dim;cnt++)
			this->center[cnt] = center[cnt];
	}

	virtual void connect(cls_ann_neuron<FUNC> *from)
	{
		disconnect(from);
		cls_ann_link<FUNC> *link = new cls_ann_link<FUNC>(from,this,1.0);
		this->input_links.push_back(link);
		from->output_links.push_back(link);
		this->invalidate_output_cache();
	}

	virtual unsigned get_dim() const
	{
		return dim;
	}

	virtual void set_desired_output( double desired )
	{
		throw cls_exception_io("The method or operation is not implemented.");
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_abstract_neuron<FUNC>::save(fout);

			fout.write((char*)&dim,sizeof(dim));
			for(int cnt=0;cnt<dim;cnt++)
			{
				fout.write((char*)&center[cnt],sizeof(double));
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_center_neuron::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		delete [] center;

		try
		{
			cls_ann_abstract_neuron<FUNC>::load(fin);

			fin.read((char*)&dim,sizeof(dim));

			center = new double[dim];
			for(int cnt=0;cnt<dim;cnt++)
			{
				fin.read((char*)&center[cnt],sizeof(double));
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_center_neuron::load() - the file is bad for input.\n");
		}
	}
};
