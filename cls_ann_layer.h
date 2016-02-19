#include "ann_import.h"

template<typename FUNC>
class cls_ann_layer
	: public cls_serialized_object
{
protected:
	int label;
	unsigned _size;
	vector<cls_ann_neuron<FUNC>*> neurons;

public:
	cls_ann_layer(int label)
	{
		_size = 0;
		this->label = label;
	}

	cls_ann_layer(int label,unsigned a_size)
	{
		_size = a_size;
		this->label = label;
	}

	virtual ~cls_ann_layer()
	{
	    #ifdef __USING_GCC__
		typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#endif

		for(itor=neurons.begin();itor !=neurons.end();itor++)
		{
			assert(*itor);
			delete (*itor);
		}
	}

	virtual int get_label() const
	{
		return label;
	}

	virtual unsigned get_size() const
	{
		return neurons.size();
	}

	unsigned size() const
	{
		return get_size();
	}

	virtual void add_neuron(cls_ann_neuron<FUNC>  * nrn)
	{
		neurons.push_back(nrn);
		_size++;
	}

	virtual cls_ann_neuron<FUNC> & get_neuron(unsigned i)
	{
		if (i <0 || i >=get_size())
		{
			throw cls_exception_not_identical("cls_ann_layer::get_neuron() - index out of srcipt.");
		}

		return *(neurons[i]);
	}

	virtual const cls_ann_neuron<FUNC> & get_neuron(unsigned i) const
	{
		if (i <0 || i >=get_size())
		{
			throw cls_exception_not_identical("cls_ann_layer::get_neuron() - index out of srcipt.");
		}

		return *(neurons[i]);
	}

	cls_ann_neuron<FUNC> & operator[] (unsigned i)
	{
		assert(i <0 || i >=get_size());

		return get_neuron(i);
	}

	const cls_ann_neuron<FUNC> & operator[] (unsigned i) const
	{
		assert(i <0 || i >=get_size());

		return get_neuron(i);
	}

	virtual vector<double> get_activation()
	{
		vector<double> answer;

		#ifdef __USING_GCC__
		typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#endif

		for(itor=this->neurons.begin();itor != this->neurons.end();itor++)
		{
			assert(*itor);
			answer.push_back((*itor)->get_activation());
		}

		return answer;
	}

	virtual vector<double> get_output()
	{
		vector<double> answer;

		#ifdef __USING_GCC__
		typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#else
		vector<cls_ann_neuron<FUNC>*>::iterator itor;
		#endif

		for(itor=neurons.begin();itor != neurons.end();itor++)
		{
			assert(*itor);
			answer.push_back((*itor)->get_output());
		}

		return answer;
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_layer::save() - the file is bad for output.\n");

		fout.write((char*)&label,sizeof(label));
		fout.write((char*)&_size,sizeof(_size));

		try
		{
		    #ifdef __USING_GCC__
            typename vector<cls_ann_neuron<FUNC>*>::const_iterator itor;
            #else
            vector<cls_ann_neuron<FUNC>*>::const_iterator itor;
            #endif

			for(itor = neurons.begin();itor != neurons.end(); itor++)
			{
				(*itor)->save(fout);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io("cls_ann_layer::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_ann_layer::load() - the file is bad for input.\n");

		fin.read((char*)&label,sizeof(label));
		fin.read((char*)&_size,sizeof(_size));

		try
		{
		    #ifdef __USING_GCC__
			typename vector<cls_ann_neuron<FUNC>*>::iterator itor;
			#else
			vector<cls_ann_neuron<FUNC>*>::iterator itor;
			#endif

			for(itor = neurons.begin();itor != neurons.end(); itor++)
			{
				(*itor)->load(fin);
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io("cls_ann_layer::load() - the file is bad for input.\n");
		}
	}
};

template <typename T,typename FUNC>
class TLayer:public cls_ann_layer<FUNC>
{
public:
	TLayer(int label)
		:cls_ann_layer<FUNC>(label)
	{

	}

	T& operator[] (unsigned i)
	{
		assert(i<0 || i>=this->get_size());

		return (T&)this->get_neuron(i);
	}

	const T & operator[] (unsigned i) const
	{
		assert(i<0 || i>=this->get_size());

		return (const T&) this->get_neuron(i);
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_layer<FUNC>::save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw exc;
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_layer<FUNC>::load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw exc;
		}
	}
};
