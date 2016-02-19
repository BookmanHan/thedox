#include "lib_import.h"

template<typename T>
class cls_circle
	:public virtual cls_serialized_object
{
public:
	cls_point<T> pt_center;
	T d_r;

public:
	cls_circle()
	{
		d_r = 0;
	}

	cls_circle(T ax,T ab,T ar):pt_center(ax,ab)
	{
		d_r = ar;
	}

	cls_circle(cls_point<T> pt,T ar):pt_center(pt)
	{
		d_r = ar;
	}

	virtual void save( ofstream& fout ) const
	{
		throw cls_exception_io("The method or operation is not implemented.");
	}

	virtual void load( ifstream& fin )
	{
		throw cls_exception_io("The method or operation is not implemented.");
	}
};
