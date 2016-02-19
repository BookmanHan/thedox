#include "lib_import.h"

class cls_svm_solver
{
public:
	cls_svm_solver()
	{

	}

	~cls_svm_solver()
	{

	}

	struct solution_info
	{
		double obj;
		double rho;
		double upper_bound_p;
		double upper_bound_n;
		double r;
	};

	void solve(int l,const a_conject_kernel_base& q,const double *p_,const char* y_,double *alpha_,
		double cp,double cn,double eps,solution_info*si,int shrinking)
	{
		this->l = l;
		this->q = &q;
		qd = this->q->get_qd();
		clone(p,p_,l);
		clone(y,y_,l);
		clone(alpha,alpha_,l);
		this->cp = cp;
		this->cn = cn;
		this->esp = eps;

		unshrink = false;

		{
			alpha_status = new char[l];
			for(int i=0;i<l;i++)
				update_alpha_status(i);
		}

		{
			active_set = new int[l];
			for(int i=0;i<l;i++)
				active_set[i] = i;

			active_size = l;
		}

		{
			g = new double[l];
			g_bar = new double[l];

			for(int cnt=0;cnt<l;cnt++)
			{
				g[cnt] = p[cnt];
				g_bar[cnt] = 0;
			}

			for(int i=0;i<l;i++)
			{
				if (!check_lower_bound(i))
				{
					const double *q_i = this->q->get_q(i,l);
					double alpha_i = alpha[i];
					
					for(int j=0;j<l;j++)
						g[j] += alpha_i * q_i[j];

					if (check_upper_bound(i))
					{
						for(int j=0;j<l;j++)
							g_bar[j] += get_c(i) * q_i[j];
					}
				}
			}
		}

		int iter = 0;
		int counter = min(l,1000) + 1;

		while(true)
		{
			if (--counter == 0)
			{
				counter = min(l,1000);
				if (shrinking)
					do_shrinking();
			}

			int i = 0,j = 0;
			if (select_working_set(i,j) != 0)
			{
				reconstruct_gradient();

				active_size = l;
				if (select_working_set(i,j) != 0)
					break;
				else
					counter = 1;
			}

			++iter;

			const double *q_i = q.get_q(i,active_size);
			const double *q_j = q.get_q(j,active_size);

			double c_i = get_c(i);
			double c_j = get_c(j);

			double old_alpha_i = alpha[i];
			double old_alpha_j = alpha[j];

			if (y[i] != y[j])
			{
				double quad_conf = q_i[i] + q_j[j] + 2*q_i[j];
				if(quad_conf <= 0)
					quad_conf = tau;

				double delta = (-g[i] - g[j]) / quad_conf;
				double diff = alpha[i] - alpha[j];

				alpha[i] += delta;
				alpha[j] += delta;

				if (diff >0)
				{
					if (alpha[j] < 0)
					{
						alpha[j] = 0;
						alpha[i] = diff;
					}
				}
				else
				{
					if (alpha[i] < 0)
					{
						alpha[i] = 0;
						alpha[j] = -diff;
					}
				}

				if (diff > c_i - c_j)
				{
					if (alpha[i] > c_i)
					{
						alpha[i] = c_i;
						alpha[j] = c_i - diff;
					}
				}
				else
				{
					if (alpha[j] > c_j)
					{
						alpha[j] = c_j;
						alpha[i] = c_j + diff;
					}
				}
			}
			else
			{
				double quad_coef = q_i[i] + q_j[j] - 2*q_i[j];
				if (quad_coef <= 0)
					quad_coef = tau;

				double delta = (g[i] - g[j])/quad_coef;
				double sum = alpha[i] + alpha[j];

				alpha[i] -= delta;
				alpha[j] += delta;

				if (sum > c_i)
				{
					if (alpha[i] > c_i)
					{
						alpha[i] = c_i;
						alpha[j] = sum - c_i;
					}
				}
				else
				{
					if (alpha[j] < 0)
					{
						alpha[j] = 0;
						alpha[i] = sum;
					}
				}

				if (sum > c_j)
				{
					if (alpha[j] > c_j)
					{
						alpha[j] = c_j;
						alpha[i] = sum - c_j;
					}
				}
				else
				{
					if (alpha[i] < 0)
					{
						alpha[i] = 0;
						alpha[j] = sum;
					}
				}
			}

			double delta_alpha_i = alpha[i] - old_alpha_i;
			double delta_alpha_j = alpha[j] - old_alpha_j;

			for(int k=0;k<active_size;k++)
			{
				g[k] += q_i[k] * delta_alpha_i + q_j[k]*delta_alpha_j;
			}

			{
				bool ui = check_upper_bound(i);
				bool uj = check_upper_bound(j);

				update_alpha_status(i);
				update_alpha_status(j);
				
				int k;
				if (ui != check_upper_bound(i))
				{
					q_i = q.get_q(i,l);
					if (ui)
					{
						for(int k=0;k<l;k++)
						{
							g_bar[k] -= c_i * q_i[k];
						}
					}
					else
					{
						for(int k=0;k<l;k++)
						{
							g_bar[k] += c_i * q_i[k];
						}
					}
				}

				if (uj != check_upper_bound(j))
				{
					q_j = q.get_q(j,l);

					if (uj)
					{
						for(int k=0;k<l;k++)
						{
							g_bar[k] -= c_j * q_j[k];
						}
					}
					else
					{
						for(int k=0;k<l;k++)
						{
							g_bar[k] += c_j * q_j[k];
						}
					}
				}
			}
		}

		si->rho = calc_rho();

		{
			double v=0;
			for(int i=0;i<l;i++)
			{
				v+= alpha[i] * (g[i] + p[i]);
			}

			si->obj = v/2;
		}

		{
			for(int i=0;i<l;i++)
			{
				alpha_[active_set[i]] = alpha[i];
			}
		}

		si->upper_bound_n = cn;
		si->upper_bound_p = cp;

		delete [] p;
		delete [] y;
		delete [] alpha;
		delete [] alpha_status;
		delete [] active_set;
		delete [] g;
		delete [] g_bar;
	}

protected:
	int active_size;
	char* y;
	double *g;
	enum {en_lowwer_bound,en_upper_bound,en_free};
	char *alpha_status;
	double *alpha;
	const a_conject_kernel_base *q;
	const double *qd;
	double esp;
	double cp,cn;
	double *p;
	int *active_set;
	double *g_bar;

	int l;
	bool unshrink;

	double get_c(int i)
	{
		return y[i] > 0? cp:cn;
	}

	void update_alpha_status(int i)
	{
		if (alpha[i] >= get_c(i))
		{
			alpha_status[i] = en_upper_bound;
		}
		else if (alpha[i] <= 0)
		{
			alpha_status[i] = en_lowwer_bound;
		}
		else
		{
			alpha_status[i] = en_free;
		}
	}

	bool check_upper_bound(int i)
	{
		return alpha_status[i] == en_upper_bound;
	}

	bool check_lower_bound(int i)
	{
		return alpha_status[i] == en_lowwer_bound;
	}

	bool check_free(int i)
	{
		return alpha_status[i] == en_free;
	}

	void swap_index(int i,int j)
	{
		q->swap_index(i,j);
		swap(y[i],y[j]);
		swap(g[i],g[j]);
		swap(alpha_status[i],alpha_status[j]);
		swap(alpha[i],alpha[j]);
		swap(p[i],p[j]);
		swap(active_set[i],active_set[j]);
		swap(g_bar[i],g_bar[j]);
	}

	void reconstruct_gradient()
	{
		if (active_size == l)
			return;

		int i,j;
		int nr_free = 0;

		for(j=active_size;j<l;j++)
		{
			g[j] = g_bar[j] + p[j];
		}

		for(j=0;j<active_size;j++)
		{
			if (check_free(j))
				nr_free++;
		}

		//if (2*nr_free < active_size)
		//	throw cls_exception_low_effective("cls_svm_solver::reconstruct_gradient() - using -h 0 may be faster");

		if (nr_free*l > 2*active_size*(l-active_size))
		{
			for(int i=active_size;i<l;i++)
			{
				const double * q_i = q->get_q(i,active_size);
				for(j=0;j<active_size;j++)
				{
					if (check_free(j))
					{
						g[i] += alpha[j] * q_i[j];
					}
				}
			}
		}
		else
		{
			for(i=0;i<active_size;i++)
			{
				if (check_free(i))
				{
					const double * q_i = q->get_q(i,l);
					double alpha_i = alpha[i];

					for(j=active_size;j<l;j++)
					{
						g[j] += alpha_i * q_i[j];
					}
				}
			}
		}
	}

	virtual int select_working_set(int &out_i,int &out_j)
	{
		double gmax = -inf;
		double gmax2 = -inf;

		int gmax_idx = -1;
		int gmin_idx = -1;

		double obj_diff_min = inf;

		for(int t=0;t<active_size;t++)
		{
			if (y[t] == +1)
			{
				if (!check_upper_bound(t))
				{
					if (-g[t] >= gmax)
					{
						gmax = -g[t];
						gmax_idx = t;
					}
				}
			}
			else
			{
				if (!check_lower_bound(t))
				{
					if (g[t] >= gmax)
					{
						gmax = g[t];
						gmax_idx = t;
					}
				}
			}
		}

		int i = gmax_idx;
		const double *q_i = null;

		if (i != -1)
			q_i = q->get_q(i,active_size);

		for(int j=0;j<active_size;j++)
		{
			if (y[j] == +1)
			{
				if (!check_lower_bound(j))
				{
					double grad_diff = gmax + g[j];
					if (g[j] >= gmax2)
						gmax2 = g[j];

					if (grad_diff > 0)
					{
						double obj_diff;
						double quad_coef = q_i[i] + qd[j] - 2.0*y[i]*q_i[j];

						if (quad_coef > 0)
							obj_diff = - (grad_diff*grad_diff) / quad_coef;
						else
							obj_diff = - (grad_diff * grad_diff) / tau;

						if (obj_diff <= obj_diff_min)
						{
							gmin_idx = j;
							obj_diff_min = obj_diff;
						}
					}
				}
			}
			else
			{
				if (!check_upper_bound(j))
				{
					double grad_diff = gmax - g[j];

					if (-g[j] >= gmax2)
						gmax2 = -g[j];

					if (grad_diff > 0)
					{
						double obj_diff;
						double quad_coef = q_i[i] + qd[j] + 2.0 * y[i] * q_i[j];

						if (quad_coef > 0)
						{
							obj_diff = - (grad_diff*grad_diff)/quad_coef;
						}
						else
						{
							obj_diff = - (grad_diff*grad_diff)/tau;
						}

						if (obj_diff <= obj_diff_min)
						{
							gmin_idx = j;
							obj_diff_min = obj_diff;
						}
					}
				}
			}
		}

		if (gmax + gmax2 < esp)
			return 1;

		out_i = gmax_idx;
		out_j = gmin_idx;

		return 0;
	}

	virtual double calc_rho()
	{
		double r;
		int nr_free = 0;
		double ub= inf,lb=-inf,sum_free =0;

		for(int i=0;i<active_size;i++)
		{
			double yg = y[i]*g[i];

			if (check_upper_bound(i))
			{
				if (y[i] == -1)
					ub = min(ub,yg);
				else
					lb = max(lb,yg);
			}
			else if (check_lower_bound(i))
			{
				if (y[i] == +1)
					ub = min(ub,yg);
				else
					lb = max(lb,yg);
			}
			else
			{
				++nr_free;
				sum_free += yg;
			}
		}

		if (nr_free > 0)
			r = sum_free / nr_free;
		else
			r = (ub + lb)/2;

		return r;
	}

	virtual void do_shrinking()
	{
		double gmax1 = -inf;
		double gmax2 = -inf;

		for(int i=0;i<active_size;i++)
		{
			if (y[i] == +1)
			{
				if (!check_upper_bound(i))
				{
					if (-g[i] >= gmax1)
						gmax1 = -g[i];
				}

				if (!check_lower_bound(i))
				{
					if (g[i] >= gmax2)
						gmax2 = g[i];
				}
			}
			else
			{
				if (!check_upper_bound(i))
				{
					if (-g[i] >= gmax2)
					{
						gmax2 = -g[i];
					}
				}
				
				if (!check_lower_bound(i))
				{
					if (g[i] >= gmax1)
						gmax1 = g[i];
				}
			}
		}

		if (unshrink == false && gmax1 + gmax2 <= esp * 10)
		{
			unshrink = true;
			reconstruct_gradient();
			active_size = l;
		}

		for(int i=0;i<active_size;i++)
		{
			if (be_shrunk(i,gmax1,gmax2))
			{
				active_size--;
				while(active_size>i)
				{
					if (!be_shrunk(active_size,gmax1,gmax2))
					{
						swap_index(i,active_size);
						break;
					}
					active_size--;
				}
			}
		}
	}

private:
	bool be_shrunk(int i,double g_max1,double g_max2)
	{
		if (check_upper_bound(i))
		{
			if (y[i] == +1)
			{
				return -g[i] > g_max1;
			}
			else
			{
				return -g[i] > g_max2;
			}
		}
		else if (check_lower_bound(i))
		{
			if (y[i] == +1)
				return g[i] > g_max2;
			else
				return g[i] > g_max1;
		}
		else
		{
			return false;
		}
	}
};

