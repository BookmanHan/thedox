#include "bayes_import.h"

class cls_bayes_base
{
public:
	virtual ~cls_bayes_base()
	{
		;
	}
};

class cls_bayes_numerical_rcond
{
private:
	double limit_pd;

public:
	cls_bayes_numerical_rcond()
	{
		limit_pd = const_bayes_limit_pd_init;
	}

	void set_limit_pd(double nl)
	{
		limit_pd = nl;
	}
};

class cls_bayes_predict_model_base : public cls_bayes_base
{
	
};

class cls_bayes_sampled_predict_model : virtual public cls_bayes_predict_model_base
{
public:
	virtual const vector<double> fw(const vector<double>& x) const = 0;
};

class cls_bayes_functional_predict_model : virtual public cls_bayes_predict_model_base
{
public:
	virtual const vector<double> fx(const vector<double> & x) const = 0;

	const vector<double> & operator()(const vector<double> &x) const
	{
		return fx(x);
	}
};

class cls_bayes_gaussian_predict_model : virtual public cls_bayes_predict_model_base
{
public:
	cls_bayes_gaussian_predict_model(unsigned size_x,unsigned size_q)
		:q(size_q),
		 g(size_x,size_q)
	{
		;
	}

	vector<double> q;
	cls_matrix<double> g;

	cls_bayes_numerical_rcond rc_limit;
};

class cls_bayes_addative_predict_model : virtual public cls_bayes_predict_model_base
{
public:
	cls_bayes_addative_predict_model(unsigned size_x,unsigned size_q)
		:q(size_q),
		 g(size_x,size_q)
	{
		;
	}

	virtual const vector<double> f(const vector<double>& x) const = 0;

	vector<double> q;
	cls_matrix<double> g;

	cls_bayes_numerical_rcond rc_limit;
};

class cls_bayes_linrz_predict_model : public cls_bayes_addative_predict_model
{
public:
	cls_bayes_linrz_predict_model (unsigned size_x,unsigned size_q)
		:cls_bayes_addative_predict_model(size_x,size_q);
		 fx(size_x,size_q)
	{
		;
	}

	cls_matrix fx;	
};

class cls_bayes_linear_predict_model : public cls_bayes_linrz_predict_model
{
public:
	cls_bayes_linear_predict_model (unsigned size_x,unsigned size_q)
		:cls_bayes_linrz_predict_model(size_x,size_q)
		 xp(size_x)
	{
		;
	}

	const vector<double> & f(const vector<double> & x) const
	{	
		xp.assign (prod());
		return xp;
	}
private:
	mutable vector<double> xp;
};

class cls_bayes_linear_invertable_predict_model : public cls_bayes_linear_predict_model

{
public:
	cls_bayes_linear_invertable_predict_model (unsigned x_size, unsigned q_size)
		:fx(x_size,q_size)
	{
		;
	}

	struct inverse_model {
		inverse_model (unsigned x_size);
		cls_matrix<double> Fx;
	} inv;
};

class cls_bayes_observe_model_base : public cls_bayes_base
{
	//
};

class cls_bayes_observe_function : public cls_bayes_base
{
public:
	virtual const vector<double>& h(const vector<double>& x) const = 0;
}

class cls_bayes_likelihood_observe_model : virtual public cls_bayes_observe_model_base
{
public:
	cls_bayes_likelihood_observe_model(unsigned z_size) : z(z_size)
	{}
	virtual double L(const vector<double>& x) const = 0;

	virtual void Lz (const vector<double>& zz)
	{
		z = zz;
	}
protected:
	vector<double> z;		
};

class cls_bayes_functional_observe_model : virtual public cls_bayes_observe_model_base, public cls_bayes_observe_function
{
public:
	cls_bayes_functional_observe_model()
	{}
	const vector<double>& operator()(const vector<double>& x) const
	{	
		return h(x);
	}
};

class cls_bayes_parametised_observe_model : virtual public cls_bayes_observe_model_base, public cls_bayes_observe_function
{
public:
	cls_bayes_parametised_observe_model()
	{
		;
	}
	virtual const vector<double>& h(const vector<double>& x) const = 0;
	virtual void normalise (vector<double>& z_denorm, const vector<double>& z_from) const
	{
		;
	}	

	cls_bayes_numerical_rcond rclimit;
};

class cls_bayes_uncorrelated_addative_observe_model : public cls_bayes_parametised_observe_model
{
public:
	cls_bayes_uncorrelated_addative_observe_model (unsigned z_size) :
		cls_bayes_parametised_observe_model(z_size), Zv(z_size)
	{
		;
	}

	vector<double> Zv;	
};

class cls_bayes_correlated_addative_observe_model : public cls_bayes_parametised_observe_model
{
public:
	cls_bayes_correlated_addative_observe_model (unsigned z_size) :
		cls_bayes_parametised_observe_model(z_size), Z(z_size,z_size)
	{
		;
	}

	cls_matrix<double> Z;
};

class cls_bayes_jacobian_observe_model : virtual public cls_bayes_observe_model_base
{
public:
	cls_matrix<double> Hx;		
protected:
	cls_bayes_jacobian_observe_model (unsigned x_size, unsigned z_size) :
		Hx(z_size, x_size)

	{
		;
	}
};

class cls_bayes_linrz_correlated_observe_model : public cls_bayes_correlated_addative_observe_model, public cls_bayes_jacobian_observe_model
{
public:
	cls_bayes_linrz_correlated_observe_model (unsigned x_size, unsigned z_size) :
		cls_bayes_correlated_addative_observe_model(z_size), cls_bayes_jacobian_observe_model(x_size, z_size)
	{
		;
	}
};

class cls_bayes_linrz_uncorrelated_observe_model : public cls_bayes_uncorrelated_addative_observe_model, public cls_bayes_jacobian_observe_model
{
public:
	cls_bayes_linrz_uncorrelated_observe_model (unsigned x_size, unsigned z_size) :
		cls_bayes_uncorrelated_addative_observe_model(z_size), cls_bayes_jacobian_observe_model(x_size, z_size)
	{
		;
	}
};

class cls_bayes_linear_correlated_observe_model : public cls_bayes_linrz_correlated_observe_model
{
public:
	cls_bayes_linear_correlated_observe_model (unsigned x_size, unsigned z_size) :
		cls_bayes_linrz_correlated_observe_model(x_size, z_size), hx(z_size)
	{
		;
	}
	
	const vector<double>& h(const vector<double>& x) const
	{	
		hx.assign (FM::prod(Hx,x));
		return hx;
	}

private:
	mutable vector<double> hx;
};

class cls_bayes_linear_uncorrelated_observe_model : public cls_bayes_linrz_uncorrelated_observe_model
{
public:
	cls_bayes_linear_uncorrelated_observe_model (unsigned x_size, unsigned z_size) :
		cls_bayes_linrz_uncorrelated_observe_model(x_size, z_size), hx(z_size)
	{
		;
	}

	const vector<double>& h(const vector<double>& x) const
	{	
		hx.assign (FM::prod(Hx,x));
		return hx;
	}
private:
	mutable vector<double> hx;
};

class cls_bayes_filter_base : public cls_bayes_base
{
	// Empty
};

class cls_bayes_likelihood_filter : virtual public cls_bayes_filter_base
{
public:
};

class cls_bayes_functional_filter : virtual public cls_bayes_filter_base
{
public:
	virtual void predict (cls_bayes_functional_predict_model & f) = 0;

};

class cls_bayes_state_filter : virtual public cls_bayes_filter_base
{
public:
	cls_bayes_state_filter (unsigned x_size)
		:x(x_size)
	{
		if (x_size < 1)
			throw cls_exception_io("Zero state filter constructed");
	}

	vector<double> x;		

	virtual void update () = 0;
};

class cls_bayes_kalman_state_filter : public cls_bayes_state_filter
{
public:
	cls_matrix<double> X;	

	cls_bayes_kalman_state_filter (unsigned x_size)
		:cls_bayes_state_filter(x_size),
		 X(x_size,x_size)
	{
		;
	}

	virtual void init () = 0;

	void init_kalman (const vector<double>& x, const cls_matrix<double>& X)
	{
		this->x  = x;
		this->X = X;

		init();
	}

	virtual void update () = 0;

	cls_bayes_numerical_rcond rclimit;
};

class cls_bayes_information_state_filter : virtual public cls_bayes_filter_base
{
public:
	cls_bayes_information_state_filter (unsigned x_size)
		:y(x_size)
		 Y(x_size,x_size)
	{
		;
	}

	vector<double> y;				
	cls_matrix<double> Y;		

	virtual void init_yY () = 0;

	void init_information (const vector<double>& y, const cls_matrix<double>& Y)
	{
		this->y = y;
		this->Y = Y;
	}

	virtual void update_yY () = 0;
};


class cls_bayes_linrz_filter : virtual public cls_bayes_filter_base
{ 
public:
	virtual double predict (cls_bayes_linrz_predict_model& f) = 0;

	virtual double observe (cls_bayes_linrz_uncorrelated_observe_model& h, const vector<double>& z) = 0;
	
	virtual double observe (cls_bayes_linrz_correlated_observe_model& h, const vector<double>& z) = 0;
};

class cls_bayes_linrz_kalman_filter : public cls_bayes_linrz_filter, virtual public cls_bayes_kalman_state_filter
{
protected:
	cls_bayes_linrz_kalman_filter() : cls_bayes_kalman_state_filter(0) // define a default constructor
	{
		;
	}
};

class cls_bayes_extended_kalman_filter : public cls_bayes_linrz_kalman_filter
{
protected:
	cls_bayes_extended_kalman_filter() : cls_bayes_kalman_state_filter(0) // define a default constructor
	{
		;
	}

public:
	virtual double observe (cls_bayes_linrz_correlated_observe_model& h, const vector<double>& z)
	{
		update();
		const vector<double>& zp = h.h(x);
		vector<double> s = z;
		h.normalise(s,zp);
		nolias(s) -= zp;

		return observe_innovation(h,s);
	}

	virtual double observe (cls_bayes_linrz_correlated_observe_model& h, const vector<double>& z)
	{
		update();
		const vector<double>& zp = h.h(x);
		vector<double> s = z;
		h.normalise(s,zp);
		nolias(s) -= zp;

		return observe_innovation(h,s);

	}

	virtual double observe_innovation (cls_bayes_linrz_correlated_observe_model& h, const vector<double>& s) = 0;
	virtual double observe_innovation (cls_bayes_linrz_uncorrelated_observe_model& h, const vector<double>& s) = 0;
};

class cls_bayes_col_proxy
{
public:
	cls_matrix<double> *cm;
	unsigned col;

	const cls_bayes_col_proxy& operator=(const cls_bayes_col_proxy&a)
	{
		this->col = a.col;
		return a;
	}

	static bool less(const cls_bayes_col_proxy&a,const cls_bayes_col_proxy &b)
	{
		
	}
};

class cls_bayes_sample_state_filter : virtual public cls_bayes_filter_base
{
public:
	cls_matrix<double>S;		

	cls_bayes_sample_state_filter(unsigned x_size,unsigned s_size)
		:S(x_size,s_size)
	{
		;
	}

	~cls_bayes_sample_state_filter()
	{
		;
	}

	virtual void init_S () = 0;

	void init_sample (const cls_matrix<double>& initS)
	{
		S = initS;
		init_S();
	}

	virtual double update_resample () = 0;

	unsigned unique_samples () const
	{

	}
};

class cls_bayes_sample_filter : public cls_bayes_likelihood_filter, public cls_bayes_functional_filter, virtual public cls_bayes_sample_state_filter
{
public:
	cls_bayes_sample_filter (unsigned x_size, unsigned s_size);

	virtual void predict (cls_bayes_functional_filter& f);

	virtual void predict (cls_bayes_sampled_predict_model& f) = 0;

	virtual void observe (cls_bayes_likelihood_observe_model& h, const vector<double>& z) = 0;

	virtual void observe_likelihood (const vector<double>& lw) = 0;
};