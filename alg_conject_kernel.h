#include "lib_import.h"

class a_conject_kernel_base
	:public cls_object
{
public:
	virtual double *get_q(int colum,int  len) const = 0;
	virtual double *get_qd() const = 0;
	virtual void swap_index(int i,int j) const =0;
	virtual ~a_conject_kernel_base()
	{
		;
	}
};

class a_conject_kernel
	:public a_conject_kernel_base
{
public:
	a_conject_kernel(int l,cls_svm_node* const * x_, const cls_svm_param& param)
		:kernal_type(param.kernel_type),
		 degree(param.degree),
		 gamma(param.gamma),
		 coef0(param.coef)
	{
		switch(kernal_type)
		{
		case en_linear:
			kernal_function = &a_conject_kernel::kernel_linear;
			break;
		case en_poly:
			kernal_function = &a_conject_kernel::kernel_poly;
			break;
		case en_rbf:
			kernal_function = &a_conject_kernel::kernel_rbf;
			break;
		case en_sigmoid:
			kernal_function = &a_conject_kernel::kernel_sigmoid;
			break;
		case en_precomputed:
			kernal_function = &a_conject_kernel::kernel_precomputed;
			break;
		default:
			throw cls_exception_not_implemented("a_conject_kernel.a_conject_kernel() - the kernel is not implemented.\n");
		}

		clone(x,x_,l);

		if (kernal_type == en_rbf)
		{
			x_square = new double[l];
			for(int cnt=0;cnt<l;cnt++)
			{
				x_square[cnt] = dot(x[cnt],x[cnt]);
			}
		}
		else
		{
			x_square = null;
		}
	}

	virtual ~a_conject_kernel()
	{
		delete [] x;
		delete [] x_square;
	}

	static double k_function(const cls_svm_node *x,const cls_svm_node *y,const cls_svm_param & param)
	{
		switch(param.kernel_type)
		{
		case en_linear:
			return dot(x,y);
			break;
		case en_poly:
			return pow(param.gamma*dot(x,y) + param.coef,param.degree);
			break;
		case en_rbf:
			{
				double sum = 0;
				while(x->index != -1 && y->index != -1)
				{
					if (x->index == y->index)
					{
						double d = x->value - y->value;
						sum += d*d;
						++x;
						++y;
					}
					else
					{
						if (x->index > y->index)
						{
							sum += y->value * y->value;
							++y;
						}
						else
						{
							sum += x->value * x->value;
							++x;
						}
					}
				}

				while(x->index != -1)
				{
					sum += x->value * x->value;
					++x;
				}

				while(y->index != -1)
				{
					sum += y->value * y->value;
					++y;
				}

				return exp(-param.gamma*sum);
			}

		case en_sigmoid:
			return tanh(param.gamma*dot(x,y)+param.coef);
		case en_precomputed:
			return x[(int)(y->value)].value;
		default:
			throw cls_exception_not_implemented("a_conject_kernel.k_function() - the kernel is not implemented.\n");
		}
	}

	virtual void swap_index( int i,int j ) const
	{
		swap(x[i],x[j]);
		if (x_square) 
			swap(x_square[i],x_square[j]);
	}

	virtual double * get_qd() const = 0;
	virtual double * get_q( int colum,int len ) const =0;

protected:
	double (a_conject_kernel::*kernal_function)(int i,int j) const;

private:
	const cls_svm_node **x;
	double *x_square;
	const enum enum_kt kernal_type;
	const int degree;
	const double gamma;
	const double coef0;

	static double dot(const cls_svm_node*px,const cls_svm_node *py)
	{
		double sum=0;
		while(px->index != -1 && py->index != -1)
		{
			if (px->index == py->index)
			{
				sum += px->value * py->value;
				++px;
				++py;
			}
			else
			{
				if (px->index > py->index)
				{
					++py;
				}
				else
				{
					++px;
				}
			}
		}

		return sum;
	}

	double kernel_linear(int i,int j) const
	{
		return dot(x[i],x[j]);
	}

	double kernel_poly(int i, int j) const
	{
		return pow(gamma*dot(x[i],x[j])+coef0,degree);
	}

	double kernel_rbf(int i, int j) const
	{
		return exp(-gamma*(x_square[i]+x_square[j]-2*dot(x[i],x[j])));
	}

	double kernel_sigmoid(int i, int j) const
	{
		return tanh(gamma*dot(x[i],x[j])+coef0);
	}

	double kernel_precomputed(int i, int j) const
	{
		return x[i][(int)(x[j][0].value)].value;
	}
};