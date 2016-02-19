#include "lib_import.h"

class a_ann_conjector
	:public cls_object
{
public:
	virtual  double  operator()(const  double  x) const = 0;
	virtual  double  operator[](const  double  x) const = 0;
};

class a_identity
	:public a_ann_conjector
{
public:
	 double  operator()(const  double  x)const
	{
		return x;
	}

	 double  operator[](const  double  x)const
	{
		return 1.0;
	}
};

class a_sigmod
	:public a_ann_conjector
{
public:
	 double  operator()(const  double  x)const
	{
		if (x < -100)
			return 0;
		else if (x > 100)
			return 1;

		return 1.0/(1.0 + exp(-x));
	}

	 double  operator[](const  double  x)const
	{
		double tmp = this->operator ()(x);
		return tmp*(1-tmp);
	}
};

class a_gaussian
	:public a_ann_conjector
{
public:
	 double  operator()(const  double  x)const
	{
		return exp(-1*x*x);
	}

	 double  operator[](const  double  x)const
	{
		return -2 * x * this->operator ()(x);
	}
};

class a_signum
	:public a_ann_conjector
{
public:
	 double  operator()(const  double  x)const
	{
		if (x < 0.0)
			return -1.0;
		else
			return 1.0;
	}

	virtual  double  operator []( const  double  x ) const
	{
		throw cls_exception_not_implemented("The method or operation is not implemented.");
	}
};

class a_tansig
	:public a_ann_conjector
{
public:
	 double  operator()(const  double  x)const
	{
		return 2.0/(1.0 + exp(-2.0*x)) - 1.0;
	}

	 double  operator[](const  double  x)const
	{
		return 1 - this->operator ()(x) * this->operator ()(x);
	}
};

class a_psigmoid
	:public a_ann_conjector
{
	double operator()(const double x,const double l) const
	{
		double xl = x*l;

		return 1.0/(1.0 + exp(-xl));
	}

	double operator[](const pair<double,double>& xl) const
	{
		double d = xl.first * xl.second;
		double f = this->operator ()(xl.first,xl.second);

		return xl.second * f * (1 - f);
	}
};
