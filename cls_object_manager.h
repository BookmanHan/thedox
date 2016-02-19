#pragma once
#include "lib_import.h"
#define ____default_manager ____get_default_manager()

class cls_object_manager
	:public cls_object
{
private:
	cls_cache * cache;
	set<void*> info;

public:
	cls_object_manager()
	{
		cache = new cls_cache_best_fit();
	}

	template<typename T>
	T* make_object()
	{
		void* addr_new = cache->get_space(sizeof(T));
		T* addr_re = new (addr_new) T ();
		
		info.insert(addr_new);

		return addr_re;
	}

	template<typename T>
	void return_object(T* addr)
	{
		info.erase(info.find((void*)addr));
		cache->return_space(addr);
	}

	virtual ~cls_object_manager()
	{
		delete cache;
	}
};

inline
cls_object_manager & ____get_default_manager()
{
	static cls_object_manager manager;

	return manager;
}

template<typename T,typename F=cls_object>
class cls_auto_object
	:public cls_object
{
protected:
	T* value;
	unsigned short times;
	cls_object_manager & man;

public:
	cls_auto_object(cls_object_manager& aman = ____default_manager)
		:man(aman),
		times(1)
	{
		value = man.make_object<T>();
	}

	operator T&()
	{
		return *value;
	}

	operator F*()
	{
		return (F*)(value);
	}

	cls_auto_object& operator = (const cls_auto_object& that) 
	{
		this->times--;
		if (this->times == 0)
			man.return_object(value);

		this->value = that.value;
		that->times++;
	}

	virtual ~cls_auto_object()
	{
		this->times--;
		if (this->times == 0)
			man.return_object(value);
	}
};
