#pragma once

#include "lib_import.h"
#define default_control	get_default_control()

const static int const_max_layer_size = 200;
const static double const_default_momentum = 0.3;
const static double const_default_learning_rate = 0.2;
const static double const_display_threshold = 0.25;
static const int const_input_neurons_label_offset = 200;

class cls_ann_public_values;
class cls_ann_value;

class cls_ann_value
{
public:
	enum {en_visible = 1, en_default = en_visible};
	unsigned flags;

	typedef double val_t;

	void set(val_t to)
	{
		_last = _current;
		_current = to;
		_last_time = _time;
		_time.current();
	}

	val_t get() const
	{
		return _current;
	}

	val_t get_last() const
	{
		return _last;
	}

	cls_ann_value & operator= (val_t val)
	{
		set(val);
		return *this;
	}

	val_t operator()() const
	{
		return get();
	}

	operator val_t() const 
	{
		return get();
	}

	bool check_visible() const
	{
		return flags & en_visible;
	}

	string _name;

	cls_ann_value(const string &name)
		:flags(cls_ann_value::en_default),
		 _current(0),
		 _last(0),
		 _last_time_displayed(cls_ann_time() - cls_ann_time()),
		 _name(name)
	{
		_current = 0;
	}

#ifndef __USING_GCC__
	class cls_ann_time
	{
	protected:
		cls_ann_time(const time_val & art)
			:real_time(art)
		{
			;
		}

	public:
		time_val real_time;
		time_val _realCalendar;

		cls_ann_time()
		{
			current();
		}

		double diff_real(const cls_ann_time& t)
		{
			cls_ann_time tmp = (*this) - t;
			return tmp.to_sec();
		}

		cls_ann_time operator-(const cls_ann_time&t) const
		{
			time_val diff;
			time_val b1 = _realCalendar;
			time_val b2 = t._realCalendar;
			a_timeval_subtract(&diff,&b1,&b2);

			return cls_ann_time(diff);
		}

		cls_ann_time& operator-=(const cls_ann_time&t)
		{
			(*this) = (*this)-t;
			return *this;
		}

		double to_sec() const
		{

		}

		void current()
		{
			gettimeofday(&_realCalendar);
		}

		cls_ann_time & operator=(const cls_ann_time& t)
		{
			this->_realCalendar = t._realCalendar;
			this->real_time = t.real_time;

			return *this;
		}
	};
#else
	class cls_ann_time
	{
	protected:
		cls_ann_time(const timeval & art)
			:real_time(art)
		{
			;
		}

	public:
		timeval real_time;
		timeval _realCalendar;

		cls_ann_time()
		{
			current();
		}

		double diff_real(const cls_ann_time& t)
		{
			cls_ann_time tmp = (*this) - t;
			return tmp.to_sec();
		}

		cls_ann_time operator-(const cls_ann_time&t) const
		{
#ifndef __USING_GCC__
			time_val diff;
			time_val b1 = _realCalendar;
			time_val b2 = t._realCalendar;
#else
			timeval diff;
			timeval b1 = _realCalendar;
			timeval b2 = t._realCalendar;
#endif
			a_timeval_subtract(&diff,&b1,&b2);

			return cls_ann_time(diff);
		}

		cls_ann_time& operator-=(const cls_ann_time&t)
		{
			(*this) = (*this)-t;
			return *this;
		}

		double to_sec() const
		{

		}

		void current()
		{
			gettimeofday(&_realCalendar,null);
		}

		cls_ann_time & operator=(const cls_ann_time& t)
		{
			this->_realCalendar = t._realCalendar;
			this->real_time = t.real_time;

			return *this;
		}
	};
#endif
	bool enough_time_passed()
	{
		cls_ann_time now;
		if (now.diff_real(_last_time_displayed) >= const_display_threshold)
		{
			_last_time_displayed = now;
			return true;
		}
		else
		{
			return false;
		}
	}

	double time_from_last_change() const
	{
		cls_ann_time current;
		current -= _time;
		return current.to_sec();
	}

private:
	double _current;
	double _last;
	cls_ann_time _time,_last_time;
	cls_ann_time _last_time_displayed;
};

class cls_ann_public_real;
struct cls_ann_update_listener;
class cls_ann_public_values
{
	friend class cls_ann_public_real;

public:
	cls_ann_public_real  init(const string &name,double initd = 0);
	cls_ann_public_real  get(const string &name);
	cls_ann_public_real  operator[] (const string& name);
	void inc(const string &name);

	cls_ann_public_values()
	{

	}

	~cls_ann_public_values()
	{

	}

	void change(const string &name,double new_value)
	{
		change(name,new_value);
	}

	void set_flags(const string &name,unsigned flags)
	{
		cls_ann_value &v = exist(name);
		v.flags = flags;

		update(v);
	}

	void add_listener(cls_ann_update_listener *l)
	{
		_listeners.push_front(l);
	}

	void remove_listener(cls_ann_update_listener *l)
	{
		_listeners.remove(l);
	}

	void add_listener(cls_ann_update_listener &l)
	{
		_listeners.push_front(&l);
	}

	void remove_listener(cls_ann_update_listener &l)
	{
		_listeners.remove(&l);
	}

	void trigger_all()
	{
		for(Values::const_iterator i=_values.begin();i != _values.end();i++)
		{
			update(i->second);
		}
	}

	void update(const cls_ann_value& v) const;

private:
	void hook_value(const cls_ann_value &v)
	{

	}

	void un_hook_value(const cls_ann_value *v)
	{

	}

	void change(cls_ann_value &v,double new_value)
	{
		if (v.get() != new_value)
		{
			v.set(new_value);
			update(v);
		}
		else
		{
			;
		}
	}
	cls_ann_value & exist(const string &name)
	{
		Values::iterator i = _values.find(string(name));
		if (i == _values.end())
		{
			_values.insert(Values::value_type(name,cls_ann_value(name)));
			return _values.find(name)->second;
		}
		else
		{
			return (*i).second;
		}
	}

	typedef map<string,cls_ann_value> Values;
	Values _values;
	typedef list<cls_ann_update_listener *> Listeners;
	Listeners _listeners;

};

class cls_ann_public_real
{
public:
	friend class cls_ann_public_values;

	cls_ann_public_real(const cls_ann_public_real &p)
		:_val(p._val),
		 _control(p._control)
	{
		_control->hook_value(_val);
	}

	cls_ann_public_real& operator=(const cls_ann_public_real & p)
	{
		_val = p._val;
		_control = p._control;
		_control->hook_value(_val);
	}

	operator double() const
	{
		return get();
	}

	int to_int() const
	{
		return (int)get();
	}

	double to_double() const
	{
		return (double)get();
	}

	double operator=(double r) 
	{ 
		set(r); 
		return r; 
	}

	double operator++ ()	
	{	
		set(get() + 1.); 
		return get();	
	}

	double operator++(int)
	{ 
		double r = get();
		set(r + 1); return r; 
	}

	double operator-(double r) const
	{ 
		return get() - r; 
	}

	double operator+(double r) const 
	{ 
		return get() + r; 
	}

	double operator*(double r) const 
	{ 
		return get() * r; 
	}

	double operator*=(double r) 
	{ 
		set(get() * r); return get(); 
	}

	double operator+=(double r) 
	{ 
		set(get() + r); return get(); 
	}

	double operator-=(double r)
	{ 
		set(get() - r); return get(); 
	}

	double time_from_last_change()	const 
	{ 
		return _val.time_from_last_change(); 
	}

	cls_ann_value &_val;
	cls_ann_public_values *_control;

	~cls_ann_public_real()
	{
		_control->un_hook_value(&_val);
	}

private:
	void set(double r)
	{
		_control->change(_val,r);
	}

	double get() const
	{
		return _val.get();
	}

	cls_ann_public_real(cls_ann_value &v,cls_ann_public_values &c) 
		:_val(v),
		_control(&c)
	{
		_control->hook_value(_val);
	}
};

inline
cls_ann_public_values & get_default_control()
{
	static cls_ann_public_values ctrl;
	return ctrl;
}

struct cls_ann_update_listener
{
public:
	cls_ann_update_listener(cls_ann_public_values & ctrl = default_control)
		:control(ctrl)
	{

	}

	virtual void value_changed(const cls_ann_value &val) = 0;

	cls_ann_public_values &get_public_values() const
	{
		return default_control;
	}

	virtual ~cls_ann_update_listener()
	{

	}

protected:
	cls_ann_public_values &control;
};

inline
cls_ann_public_real  cls_ann_public_values::get(const string &name)
{
	cls_ann_value &v = exist(name);
	return cls_ann_public_real(v,*this);
}

inline
void cls_ann_public_values::inc(const string &name)
{
	cls_ann_public_real v = (*this)[name];
	change(name,++(v));
}

inline
cls_ann_public_real cls_ann_public_values::operator[] (const string& name)
{
	Values::iterator i = _values.find(name);
	if (i == _values.end())
		throw cls_exception_io("not found.");

	return cls_ann_public_real(i->second,*this);
}

inline
cls_ann_public_real  cls_ann_public_values::init(const string &name,double initd)
{
	cls_ann_public_real r = get(name);
	r = initd;
	return r;
}

inline
void cls_ann_public_values::update(const cls_ann_value& v) const
{
	for(Listeners::const_iterator i=_listeners.begin();i != _listeners.end();i++)
	{
		(*i)->value_changed(v);
	}
}

const static cls_ann_public_real const_creal_default_learning_rate = default_control.init("defautl_learning_reate",const_default_learning_rate);
const static cls_ann_public_real const_creal_display_threshold = default_control.init("defautl_momentum",const_default_momentum);
