#include "ann_import.h"

template<typename FUNC>
class cls_ann_network_bilayer
	: public cls_ann_network_multilayer<FUNC>
{
public:
	cls_ann_network_bilayer()
		:cls_ann_network_multilayer<FUNC>(0)
	{
		;
	}

	cls_ann_network_bilayer(unsigned inputs,unsigned hidden,unsigned outputs)
		:cls_ann_network_multilayer<FUNC>(inputs)
	{
		cls_ann_network_multilayer<FUNC>::add_layer(hidden);
		cls_ann_network_multilayer<FUNC>::add_layer(outputs);
		connect_all();
	}

	virtual void connect_bi_in(int input,int hidden)
	{
		this->connect(0,input,hidden);
	}

	virtual void connect_bi_in(int input,int hidden,double weight)
	{
		this->connect(0,input,hidden,weight);
	}

	virtual void connect_bi_out(int hidden,int output)
	{
		this->connect(1,hidden,output);
	}

	virtual void connect_bi_out(int hidden,int output,double weight)
	{
		this->connect(1,hidden,output,weight);
	}

	virtual void connect_all()
	{
		this->connect_layer(0);
		this->connect_layer(1);
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_network_multilayer<FUNC>::save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_bilayer::save() - the file is bad for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_network_multilayer<FUNC>::load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_network_bilayer::load() - the file is bad for input.\n");
		}
	}
};
