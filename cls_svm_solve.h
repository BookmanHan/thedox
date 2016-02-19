#include "lib_import.h"

class cls_svm_kernel_svc_q 
	:public a_conject_kernel
{
private:
	char *y;
	cls_svm_cache *cache;
	double *qd;

public:
	cls_svm_kernel_svc_q(const cls_svm_problem& prob,const cls_svm_param& param,const char* y_)
		:a_conject_kernel(prob.l,prob.x,param)
	{
		clone(y,y_,prob.l);
		cache = new cls_svm_cache(prob.l,param.cache_size*(1<<20));
		qd = new double[prob.l];

		for(int i=0;i<prob.l;i++)
		{
			qd[i] = (double)(this->*kernal_function)(i,i);
		}
	}

	double *get_q( int i,int len) const
	{
		double *data = null;
		int start;

		if ((start = cache->get_data(i,&data,len)) < len)
		{
			for(int j=start;j<len;j++)
			{
				data[j] = (double)(y[i]*y[j]*(this->*kernal_function)(i,j));
			}
		}

		return data;
	}

	double *get_qd() const
	{
		return qd;
	}

	void swap_index(int i,int j) const
	{
		cache->swap_index(i,j);
		a_conject_kernel::swap_index(i,j);
		swap(y[i],y[j]);
		swap(qd[i],qd[j]);
	}

	virtual ~cls_svm_kernel_svc_q()
	{
		delete[] y;
		delete cache;
		delete[] qd;
	}
};


class cls_svm_kernel_one_class_q 
	:public a_conject_kernel
{
private:
	cls_svm_cache *cache;
	double *qd;

public:
	cls_svm_kernel_one_class_q(const cls_svm_problem& prob,const cls_svm_param& param)
		:a_conject_kernel(prob.l,prob.x,param)
	{
		cache = new cls_svm_cache(prob.l,param.cache_size*(1<<20));
		qd = new double[prob.l];

		for(int i=0;i<prob.l;i++)
		{
			qd[i] = (double)(this->*kernal_function)(i,i);
		}
	}

	double *get_q( int i,int len) const
	{
		double *data = null;
		int start;

		if ((start = cache->get_data(i,&data,len)) < len)
		{
			for(int j=start;j<len;j++)
			{
				data[j] = (double)((this->*kernal_function)(i,j));
			}
		}

		return data;
	}

	double *get_qd() const
	{
		return qd;
	}

	void swap_index(int i,int j) const
	{
		cache->swap_index(i,j);
		a_conject_kernel::swap_index(i,j);
		swap(qd[i],qd[j]);
	}

	virtual ~cls_svm_kernel_one_class_q()
	{
		delete cache;
		delete[] qd;
	}
};

class cls_svm_kernel_svr_q: 
	public a_conject_kernel
{ 
public:
	cls_svm_kernel_svr_q(const cls_svm_problem& prob, const cls_svm_param& param)
		:a_conject_kernel(prob.l, prob.x, param)
	{
		l = prob.l;
		cache = new cls_svm_cache(l,(long int)(param.cache_size*(1<<20)));
		QD = new double[2*l];
		sign = new char[2*l];
		index = new int[2*l];
		for(int k=0;k<l;k++)
		{
			sign[k] = 1;
			sign[k+l] = -1;
			index[k] = k;
			index[k+l] = k;
			QD[k]= (double)(this->*kernal_function)(k,k);
			QD[k+l]=QD[k];
		}
		buffer[0] = new double[2*l];
		buffer[1] = new double[2*l];
		next_buffer = 0;
	}

	void swap_index(int i, int j) const
	{
		swap(sign[i],sign[j]);
		swap(index[i],index[j]);
		swap(QD[i],QD[j]);
	}

	double *get_q(int i, int len) const
	{
		double *data;
		int j, real_i = index[i];
		if(cache->get_data(real_i,&data,l) < l)
		{
			for(j=0;j<l;j++)
				data[j] = (double)(this->*kernal_function)(real_i,j);
		}

		// reorder and copy
		double *buf = buffer[next_buffer];
		next_buffer = 1 - next_buffer;
		char si = sign[i];
		for(int j=0;j<len;j++)
			buf[j] = (double) si * (double) sign[j] * data[index[j]];
		return buf;
	}

	double *get_qd() const
	{
		return QD;
	}

	~cls_svm_kernel_svr_q()
	{
		delete cache;
		delete[] sign;
		delete[] index;
		delete[] buffer[0];
		delete[] buffer[1];
		delete[] QD;
	}
private:
	int l;
	cls_svm_cache *cache;
	char *sign;
	int *index;
	mutable int next_buffer;
	double *buffer[2];
	double *QD;
};

inline
void a_solve_c_svc(const cls_svm_problem *prob,const cls_svm_param* param, double *alpha,
						 cls_svm_solver::solution_info* si, double cp,double cn)
{
	int l = prob->l;
	double *minus_ones = new double[l];
	char* y = new char[l];

	for(int i=0;i<l;i++)
	{
		alpha[i] = 0;
		minus_ones[i] = -1;
		if (prob->y[i] > 0)
			y[i] = +1;
		else
			y[i] = -1;
	}

	cls_svm_solver solver;
	solver.solve(l,cls_svm_kernel_svc_q(*prob,*param,y),minus_ones,y,alpha,cp,cn,param->eps,si,param->shrinking);

	double sum_alpha = 0;
	for(int i=0;i<l;i++)
		sum_alpha += alpha[i];

	for(int i=0;i<l;i++)
	{
		alpha[i] *= y[i];
	}

	delete [] minus_ones;
	delete [] y;
}

inline
void a_solve_nu_svc(const cls_svm_problem * prob,const cls_svm_param *param,double *alpha,cls_svm_solver::solution_info* si)
{
	int i;
	int l = prob->l;
	double nu = param->nu;

	char *y = new char[l];

	for(i=0;i<l;i++)
		if(prob->y[i]>0)
			y[i] = +1;
		else
			y[i] = -1;

	double sum_pos = nu*l/2;
	double sum_neg = nu*l/2;

	for(i=0;i<l;i++)
		if(y[i] == +1)
		{
			alpha[i] = min(1.0,sum_pos);
			sum_pos -= alpha[i];
		}
		else
		{
			alpha[i] = min(1.0,sum_neg);
			sum_neg -= alpha[i];
		}

		double *zeros = new double[l];

		for(i=0;i<l;i++)
			zeros[i] = 0;

		cls_svm_solver_nu s;
		s.solve(l,cls_svm_kernel_svc_q(*prob,*param,y), zeros, y,
			alpha, 1.0, 1.0, param->eps, si,  param->shrinking);
		
		double r = si->r;

		for(i=0;i<l;i++)
			alpha[i] *= y[i]/r;

		si->rho /= r;
		si->obj /= (r*r);
		si->upper_bound_p = 1/r;
		si->upper_bound_n = 1/r;

		delete[] y;
		delete[] zeros;
}

inline
void a_solve_one_class(
						 const cls_svm_problem *prob, const cls_svm_param *param,
						 double *alpha,cls_svm_solver::solution_info* si)
{
	int l = prob->l;
	double *zeros = new double[l];
	char *ones = new char[l];
	int i;

	int n = (int)(param->nu*prob->l);	// # of alpha's at upper bound

	for(i=0;i<n;i++)
		alpha[i] = 1;
	if(n<prob->l)
		alpha[n] = param->nu * prob->l - n;
	for(i=n+1;i<l;i++)
		alpha[i] = 0;

	for(i=0;i<l;i++)
	{
		zeros[i] = 0;
		ones[i] = 1;
	}

	cls_svm_solver s;
	s.solve(l, cls_svm_kernel_one_class_q(*prob,*param), zeros, ones,
		alpha, 1.0, 1.0, param->eps, si, param->shrinking);

	delete[] zeros;
	delete[] ones;
}

inline
void a_solve_epsilon_svr(
						   const cls_svm_problem *prob, const cls_svm_param *param,
						   double *alpha, cls_svm_solver::solution_info* si)
{
	int l = prob->l;
	double *alpha2 = new double[2*l];
	double *linear_term = new double[2*l];
	char *y = new char[2*l];
	int i;

	for(i=0;i<l;i++)
	{
		alpha2[i] = 0;
		linear_term[i] = param->p - prob->y[i];
		y[i] = 1;

		alpha2[i+l] = 0;
		linear_term[i+l] = param->p + prob->y[i];
		y[i+l] = -1;
	}

	cls_svm_solver s;
	s.solve(2*l, cls_svm_kernel_svr_q(*prob,*param), linear_term, y,
		alpha2, param->c, param->c, param->eps, si, param->shrinking);

	double sum_alpha = 0;
	for(i=0;i<l;i++)
	{
		alpha[i] = alpha2[i] - alpha2[i+l];
		sum_alpha += fabs(alpha[i]);
	}

	delete[] alpha2;
	delete[] linear_term;
	delete[] y;
}

inline
void a_solve_nu_svr(
					  const cls_svm_problem *prob, const cls_svm_param *param,
					  double *alpha, cls_svm_solver::solution_info* si)
{
	int l = prob->l;
	double C = param->c;
	double *alpha2 = new double[2*l];
	double *linear_term = new double[2*l];
	char *y = new char[2*l];
	int i;

	double sum = C * param->nu * l / 2;
	for(i=0;i<l;i++)
	{
		alpha2[i] = alpha2[i+l] = min(sum,C);
		sum -= alpha2[i];

		linear_term[i] = - prob->y[i];
		y[i] = 1;

		linear_term[i+l] = prob->y[i];
		y[i+l] = -1;
	}

	cls_svm_solver_nu s;
	s.solve(2*l, cls_svm_kernel_svr_q(*prob,*param), linear_term, y,
		alpha2, C, C, param->eps, si, param->shrinking);

	for(i=0;i<l;i++)
		alpha[i] = alpha2[i] - alpha2[i+l];

	delete[] alpha2;
	delete[] linear_term;
	delete[] y;
}