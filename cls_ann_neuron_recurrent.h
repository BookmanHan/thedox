#include "ann_import.h"

template<typename FUNC>
class cls_ann_neuron_recurrent : public cls_ann_simple_neuron<FUNC>
{
public:
	cls_ann_neuron_recurrent(int label,double a_bias,double activation=0.0)
		:cls_ann_simple_neuron<FUNC>(label,true),
		bias(a_bias)
	{
		this->output_cache = (this->func)(activation);
	}

	virtual void update()
	{
		double sum = bias;

        #ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
		#endif

		for(itor = this->input_links.begin();itor != this->input_links.end();itor++)
		{
			sum += (*itor)->get_source()->get_output() * (*itor)->get_weight();
		}

		this->activation_cache = sum;
		this->output_cache = (this->func)(sum);
	}

	virtual void invalidate_output_cache()
	{

	}

	virtual double get_output() const
	{
		return this->output_cache;
	}

	virtual void reset(double init_activation)
	{
		this->activation_cache = init_activation;
		this->output_cache = (this->func)(this->activation_cache);
	}

protected:
	virtual void re_cache_output()
	{

	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_simple_neuron<FUNC>::save(fout);
			fout.write((char*)&bias,sizeof(bias));
		}
		catch (cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron_recurrent::save() - file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_simple_neuron<FUNC>::load(fin);
			fin.read((char*)&bias,sizeof(bias));
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron_recurrent::load() - file is bad for input.\n");
		}
	}

	double bias;
};
