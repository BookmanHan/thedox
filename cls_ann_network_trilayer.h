#include "ann_import.h"

template<typename FUNC>
class cls_ann_network_trilayer
	: public cls_ann_network_multilayer<FUNC>
{
public:
	cls_ann_network_trilayer()
		:cls_ann_network_multilayer<FUNC>(0)
	{
		;
	}

	cls_ann_network_trilayer(unsigned inputs,unsigned hidden,unsigned hidden1,unsigned outputs)
		:cls_ann_network_multilayer<FUNC>(inputs)
	{
		cls_ann_network_multilayer<FUNC>::add_layer(hidden);
		cls_ann_network_multilayer<FUNC>::add_layer(hidden1);
		cls_ann_network_multilayer<FUNC>::add_layer(outputs);
	}

	virtual void connect_all()
	{
		this->connect_layer(0);
		this->connect_layer(1);
		this->connect_layer(2);
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
