#include "lib_import.h"

class cls_svm_solver_nu 
	:public cls_svm_solver
{
private:
	solution_info *si;
	int select_working_set(int &out_i,int &out_j)
	{
		double Gmaxp = -inf;
		double Gmaxp2 = -inf;
		int Gmaxp_idx = -1;

		double Gmaxn = -inf;
		double Gmaxn2 = -inf;
		int Gmaxn_idx = -1;

		int Gmin_idx = -1;
		double obj_diff_min = inf;

		for(int t=0;t<active_size;t++)
			if(y[t]==+1)
			{
				if(!check_upper_bound(t))
					if(-g[t] >= Gmaxp)
					{
						Gmaxp = -g[t];
						Gmaxp_idx = t;
					}
			}
			else
			{
				if(!check_lower_bound(t))
					if(g[t] >= Gmaxn)
					{
						Gmaxn = g[t];
						Gmaxn_idx = t;
					}
			}

			int ip = Gmaxp_idx;
			int in = Gmaxn_idx;
			const double *Q_ip = NULL;
			const double *Q_in = NULL;
			if(ip != -1)
				Q_ip = q->get_q(ip,active_size);
			if(in != -1)
				Q_in = q->get_q(in,active_size);

			for(int j=0;j<active_size;j++)
			{
				if(y[j]==+1)
				{
					if (!check_lower_bound(j))	
					{
						double grad_diff=Gmaxp+g[j];
						if (g[j] >= Gmaxp2)
							Gmaxp2 = g[j];
						if (grad_diff > 0)
						{
							double obj_diff; 
							double quad_coef = Q_ip[ip]+qd[j]-2*Q_ip[j];
							if (quad_coef > 0)
								obj_diff = -(grad_diff*grad_diff)/quad_coef;
							else
								obj_diff = -(grad_diff*grad_diff)/tau;

							if (obj_diff <= obj_diff_min)
							{
								Gmin_idx=j;
								obj_diff_min = obj_diff;
							}
						}
					}
				}
				else
				{
					if (!check_upper_bound(j))
					{
						double grad_diff=Gmaxn-g[j];
						if (-g[j] >= Gmaxn2)
							Gmaxn2 = -g[j];
						if (grad_diff > 0)
						{
							double obj_diff; 
							double quad_coef = Q_in[in]+qd[j]-2*Q_in[j];
							if (quad_coef > 0)
								obj_diff = -(grad_diff*grad_diff)/quad_coef;
							else
								obj_diff = -(grad_diff*grad_diff)/tau;

							if (obj_diff <= obj_diff_min)
							{
								Gmin_idx=j;
								obj_diff_min = obj_diff;
							}
						}
					}
				}
			}

			if(max(Gmaxp+Gmaxp2,Gmaxn+Gmaxn2) < esp)
				return 1;

			if (y[Gmin_idx] == +1)
				out_i = Gmaxp_idx;
			else
				out_i = Gmaxn_idx;

			out_j = Gmin_idx;

			return 0;
	}

	double calc_rho()
	{
		int nr_free1 = 0,nr_free2 = 0;
		double ub1 = inf, ub2 = inf;
		double lb1 = -inf, lb2 = -inf;
		double sum_free1 = 0, sum_free2 = 0;

		for(int i=0;i<active_size;i++)
		{
			if(y[i]==+1)
			{
				if(check_upper_bound(i))
					lb1 = max(lb1,g[i]);
				else if(check_lower_bound(i))
					ub1 = min(ub1,g[i]);
				else
				{
					++nr_free1;
					sum_free1 += g[i];
				}
			}
			else
			{
				if(check_upper_bound(i))
					lb2 = max(lb2,g[i]);
				else if(check_lower_bound(i))
					ub2 = min(ub2,g[i]);
				else
				{
					++nr_free2;
					sum_free2 += g[i];
				}
			}
		}

		double r1,r2;
		if(nr_free1 > 0)
			r1 = sum_free1/nr_free1;
		else
			r1 = (ub1+lb1)/2;

		if(nr_free2 > 0)
			r2 = sum_free2/nr_free2;
		else
			r2 = (ub2+lb2)/2;

		si->r = (r1+r2)/2;
		return (r1-r2)/2;
	}

	bool be_shrunk(int i,double gmax1,double gmax2,double gmax3,double gmax4)
	{
		if (check_upper_bound(i))
		{
			if (y[i] == +1)
				return -g[i] > gmax1;
			else
				return -g[i] > gmax4;
		}
		else if (check_lower_bound(i))
		{
			if (y[i] == +1)
				return g[i] > gmax2;
			else 
				return g[i] > gmax3;
		}
		else
		{
			return false;
		}
	}

	void do_shrinking()
	{
		double gmax1 = - inf;
		double gmax2 = - inf;
		double gmax3 = - inf;
		double gmax4 = - inf;

		for(int i=0;i<active_size;i++)
		{
			if (!check_upper_bound(i))
			{
				if (y[i] == +1)
				{
					if (-g[i] > gmax1)
						gmax1 = -g[i];
				}
				else if (-g[i] > gmax4)
				{
					gmax4 = -g[i];
				}
			}

			if (!check_lower_bound(i))
			{
				if (y[i] == +1)
				{
					if (g[i] > gmax2)
						gmax2 = g[i];
					else if (g[i] > gmax3)
						gmax3 = g[i];
				}
			}
		}

		if (unshrink == false && max(gmax1 + gmax2,gmax3+gmax4) <= esp * 10)
		{
			unshrink = true;
			reconstruct_gradient();
			active_size = l;
		}

		for(int i=0;i<active_size;i++)
		{
			if (be_shrunk(i,gmax1,gmax2,gmax3,gmax4))
			{
				active_size--;
				while(active_size>1)
				{
					if (!be_shrunk(active_size,gmax1,gmax2,gmax3,gmax4))
					{
						swap_index(i,active_size);
						break;
					}

					active_size--;
				}
			}
		}
	}

public:
	cls_svm_solver_nu()
	{
		;
	}

	void solve(int l,const a_conject_kernel& q, const double *p,const char*y,double* alpha,
		double cp,double cn,double eps,solution_info*si,int shrinking)
	{
		this->si = si;
		cls_svm_solver::solve(l,q,p,y,alpha,cp,cn,esp,si,shrinking);
	}
};