#include "ann_import.h"

template<typename FUNC> class cls_ann_neuron;

template<typename FUNC> class cls_ann_link;

template<typename FUNC>
class cls_ann_neuron
	:public cls_serialized_object
{
public:
	cls_ann_neuron(const cls_ann_neuron & a_neuron)
	{
		throw cls_exception_not_implemented("cls_ann_neuron::copy_constructor() - not implemented.\n");
	}

public:
	int label;
	vector<cls_ann_link<FUNC>*> input_links;
	double output_cache;
	double error_cache;
	double activation_cache;

	bool output_cache_valid;
	bool error_cache_valid;

public:
	vector<cls_ann_link<FUNC>*> output_links;

protected:
	FUNC func;

public:

	virtual void invalidate_output_cache()
	{
		if (output_cache_valid)
		{
			output_cache_valid = false;
#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif
			for(itor = output_links.begin();itor != output_links.end();itor++)
			{
				assert((*itor)->get_destination());
				(*itor)->get_destination()->invalidate_output_cache();
			}
		}
	}

	virtual void invalidate_error_cache()
	{
		if (error_cache_valid)
		{
			error_cache_valid = false;

#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif

			for(itor = input_links.begin();itor != input_links.end();itor++)
			{
				assert((*itor)->get_source());
				(*itor)->get_source()->invalidate_error_cache();
			}
		}
	}

	virtual void re_cache_error() = 0 ;
	virtual void re_cache_output() = 0 ;

	cls_ann_neuron(int a_label)
	{
		label = a_label;
		input_links.clear();
		output_links.clear();
		error_cache = activation_cache = output_cache = 0;
		output_cache_valid = error_cache_valid = true;
	}

	double get_activation()
	{
		re_cache_output();
		return activation_cache;
	}

	double get_error()
	{
		re_cache_error();
		return error_cache;
	}

	double get_output()
	{
		re_cache_output();
		return output_cache;
	}

	virtual ~cls_ann_neuron()
	{
		for(int cnt=0;cnt<input_links.size();cnt++)
		{
			delete input_links[cnt];
		}

		for(int cnt=0;cnt<output_links.size();cnt++)
		{
			delete output_links[cnt];
		}
	}

	virtual void disconnect(cls_ann_neuron * from)
	{
		cls_ann_link<FUNC> * link = new cls_ann_link<FUNC>(from,this,0.0);

#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif


		for(itor = input_links.begin();itor != input_links.end(); itor++)
		{
			if ((*itor)->check_equal_to(link))
			{
				delete (*itor);
				break;
			}
		}

		invalidate_output_cache();
		delete link;
	}

	virtual int get_label() const
	{
		return label;
	}

	virtual unsigned get_input_count() const
	{
		return input_links.size();
	}

	virtual unsigned get_output_count() const
	{
		return output_links.size();
	}

	virtual int get_inputs(vector<int> &labels,vector<double> &weights)
	{
		labels.clear();
		weights.clear();

#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif

		itor = input_links.begin();
		for(itor = input_links.begin();itor != input_links.end();itor++)
		{
			labels.push_back((*itor)->get_source()->get_label());
			weights.push_back((*itor)->get_weight());
		}

		return get_input_count();
	}

	virtual void get_weights(vector<double> & out) const
	{
		out.resize(input_links.size());

#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::const_iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::const_iterator itor;
#endif

		int i=0;
		for(itor=this->input_links.begin();itor != this->input_links.end();itor++)
		{
			assert((*itor));
			out[i] = (*itor)->get_weight();
			i++;
		}
	}

	virtual double get_weight(cls_ann_neuron * from)
	{
#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif

		for(itor=input_links.begin();itor != input_links.end();itor++)
		{
			assert((*itor));
			if ((*itor)->get_source() == from)
			{
				return (*itor)->get_weight();
			}
		}

		return 0.0;
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_neuron::save() - file is bad for output.\n");

		fout.write((char*)&label,sizeof(label));
		fout.write((char*)&activation_cache,sizeof(activation_cache));
		fout.write((char*)&error_cache,sizeof(error_cache));
		fout.write((char*)&error_cache_valid,sizeof(error_cache_valid));
		fout.write((char*)&output_cache,sizeof(output_cache));
		fout.write((char*)&output_cache_valid,sizeof(output_cache_valid));

		try
		{
			int num = output_links.size();
			fout.write((char*)&num,sizeof(int));

#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::const_iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::const_iterator itor;
#endif

			for(itor = output_links.begin(); itor != output_links.end(); itor++)
			{
				(*itor)->save(fout);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron::save() - file is bad for output.\n");
		}
	}

	virtual void connect_load(cls_ann_neuron *to,int cnt)
	{
		this->output_links[cnt]->from = this;
		to->input_links.push_back(this->output_links[cnt]);
		to->input_links.back()->to = to;
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_ann_neuron::load() - file is bad for input.\n");

		fin.read((char*)&label,sizeof(label));
		fin.read((char*)&activation_cache,sizeof(activation_cache));
		fin.read((char*)&error_cache,sizeof(error_cache));
		fin.read((char*)&error_cache_valid,sizeof(error_cache_valid));
		fin.read((char*)&output_cache,sizeof(output_cache));
		fin.read((char*)&output_cache_valid,sizeof(output_cache_valid));

		for(int cnt=0;cnt<input_links.size();cnt++)
		{
			delete input_links[cnt];
		}

		for(int cnt=0;cnt<output_links.size();cnt++)
		{
			delete output_links[cnt];
		}

		output_links.clear();
		input_links.clear();

		try
		{
			int num;
			fin.read((char*)&num,sizeof(int));
			for(int cnt=0;cnt<num;cnt++)
			{
				cls_ann_link<FUNC> *new_link = new cls_ann_link<FUNC>(this,null);
				new_link->load(fin);
				output_links.push_back(new_link);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_neuron::load() - file is bad for input.\n");
		}
	}
};
