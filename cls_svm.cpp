#include "lib_import.h"

void cls_svm::train(const cls_svm_problem* prob,const cls_svm_param * param)
{
	this->param = *param;
	this->free_sv = 0;

	if (param->svm_type == en_one_class ||
		param->svm_type == en_epsilon_svr ||
		param->svm_type == en_nu_svr)
	{
		nr_class = 2;
		n_labels = null;
		n_sv = null;
		probA = null;
		probB = null;
		sv_coef = new double*[1];

		if (param->probability && 
			(param->svm_type == en_epsilon_svr ||
			param->svm_type == en_nu_svr)
			)
		{
			probA = new double[1];
			probA[0] = svm_svr_probability(prob,param);
		}

		decision_function f = a_svm_train_one(prob,param,0,0);
		rho = new double[1];
		rho[0] = f.rho;

		int n_sv = 0;
		for(int i=0;i<prob->l;i++)
		{
			if(fabs(f.alpha[i]) >0)
				++ n_sv;
		}

		this->l = n_sv;
		this->sv = new cls_svm_node*[n_sv];
		this->sv_coef[0] = new double[n_sv];

		int j=0;
		for(int i=0;i<prob->l;i++)
		{
			if(fabs(f.alpha[i]) > 0)
			{
				this->sv[j] = prob->x[i];
				this->sv_coef[0][j] = f.alpha[i];
				++j;
			}
		}

		delete [] f.alpha;
	}
	else
	{
		int l = prob->l;
		int *label = null;
		int *start = null;
		int *count = null;

		int *perm = new int[l];

		svm_group_classes(prob,&nr_class,&label,&start,&count,perm);

		cls_svm_node **x = new cls_svm_node*[l];
		
		for(int i=0;i<l;i++)
		{
			x[i] = prob->x[perm[i]];
		}

		double *weighted_c = new double[nr_class];
		for(int cnt=0;cnt<nr_class;cnt++)
			weighted_c[cnt] = param->c;

		for(int i=0;i<param->nr_weight;i++)
		{
			int j;
			for(j=0;j<nr_class;j++)
			{
				if (param->weight_label[i] == label[j])
					break;
			}

			if (j != nr_class)
				weighted_c[j] *= param->weight[i];
		}

		bool *non_zero = new bool[l];
		for(int i=0;i<l;i++)
		{
			non_zero[i] = false;
		}

		decision_function *f = new decision_function[nr_class*(nr_class-1)/2];

		double *probA = null,*probB = null;
		if (param->probability)
		{
			probA = new double[nr_class*(nr_class-1)/2];
			probB = new double[nr_class*(nr_class-1)/2];
		}

		int p = 0;
		for(int i=0;i<nr_class;i++)
		{
			for(int j=i+1;j<nr_class;j++)
			{
				cls_svm_problem sub_prob;
				int si=start[i];
				int sj=start[j];
				int ci=count[i];
				int cj=count[j];

				sub_prob.l = ci+cj;
				sub_prob.x = new cls_svm_node*[sub_prob.l];
				sub_prob.y = new double[sub_prob.l];

				int k;
				for(k=0;k<ci;k++)
				{
					sub_prob.x[k] = x[si+k];
					sub_prob.y[k] = +1;
				}
				for(k=0;k<cj;k++)
				{
					sub_prob.x[ci+k] = x[sj+k];
					sub_prob.y[ci+k] = -1;
				}

				if (param->probability)
					svm_binary_svc_probability(&sub_prob,param,weighted_c[i],weighted_c[j],probA[p],probB[p]);

				f[p] = a_svm_train_one(&sub_prob,param,weighted_c[i],weighted_c[j]);

				for(k=0;k<ci;k++)
				{
					if (!non_zero[si + k] && fabs(f[p].alpha[k]) > 0)
						non_zero[si+k] = true;
				}

				for(k=0;k<cj;k++)
				{
					if (!non_zero[sj+k] && fabs(f[p].alpha[ci+k]) > 0)
						non_zero[sj+k] = true;
				}

				delete[] sub_prob.x;
				delete[] sub_prob.y;
				++p;
			}
		}

		this->n_labels = new int[nr_class];
		for(int i=0;i<nr_class;i++)
		{
			this->n_labels[i] = label[i];
		}

		this->rho = new double[nr_class*(nr_class-1)/2];

		for(int i=0;i<nr_class*(nr_class-1)/2;i++)
		{
			rho[i] = f[i].rho;
		}

		if (param->probability)
		{
			this->probA = new double[nr_class*(nr_class-1)/2];
			this->probB = new double[nr_class*(nr_class-1)/2];

			for(int i=0;i<nr_class*(nr_class-1)/2;i++)
			{
				this->probA[i] = probA[i];
				this->probB[i] = probB[i];
			}
		}
		else
		{
			this->probA = null;
			this->probB = null;
		}

		int total_sv = 0;
		int *nz_count = new int[nr_class];
		this->n_sv = new int[nr_class];
		for(int i=0;i<nr_class;i++)
		{
			int n_sv = 0;
			for(int j=0;j<count[i];j++)
			{
				if (non_zero[start[i]+j])
				{
					++n_sv;
					++total_sv;
				}
			}

			this->n_sv[i] = n_sv;
			nz_count[i] = n_sv;
		}

		this->l = total_sv;
		this->sv = new cls_svm_node*[total_sv];
		p = 0;
		for(int i=0;i<l;i++)
		{
			if (non_zero[i])
				this->sv[p++] = x[i];
		}

		int * nz_start = new int[nr_class];
		nz_start[0] = 0;
		for(int i=1;i<nr_class;i++)
			nz_start[i] = nz_start[i-1] + nz_count[i-1];

		this->sv_coef = new double*[nr_class-1];
		for(int i=0;i<nr_class-1;i++)
			this->sv_coef[i] = new double[total_sv];

		p = 0;
		for(int i=0;i<nr_class;i++)
		{
			for(int j=i+1;j<nr_class;j++)
			{
				int si = start[i];
				int sj = start[j];
				int ci = count[i];
				int cj = count[j];

				int q = nz_start[i];
				int k;
				for(k=0;k<ci;k++)
				{
					if (non_zero[si+k])
						this->sv_coef[j-1][q++] = f[p].alpha[k];
				}

				q = nz_start[j];
				for(k=0;k<cj;k++)
				{
					if(non_zero[sj+k])
						this->sv_coef[i][q++] = f[p].alpha[ci+k];
				}

				++p;
			}
		}

		delete[] label;
		delete[] probA;
		delete[] probB;
		delete[] count;
		delete[] perm;
		delete[] start;
		delete[] x;
		delete[] weighted_c;
		delete[] non_zero;
		for(int i=0;i<nr_class*(nr_class-1)/2;i++)
		{
			delete [] f[i].alpha;
		}

		delete[] f;
		delete[] nz_count;
		delete[] nz_start;
	}
}

enum_svm cls_svm::get_svm_type()
{
	return param.svm_type;
}

int cls_svm::get_nr_class()
{
	return nr_class;
}

void cls_svm::get_labels(int *label)
{
	if (n_labels != null)
	{
		for(int i=0;i<nr_class;i++)
			label[i] = n_labels[i];
	}
}

double cls_svm::get_svr_probability()
{
	if ((param.svm_type == en_epsilon_svr ||
		param.svm_type == en_nu_svr) && probA != null)
	{
		return probA[0];
	}
	else
	{
		return 0;
	}
}

void cls_svm::predict_values(const cls_svm_node*x,double*dec_values)
{
	if(param.svm_type == en_one_class ||
		param.svm_type == en_epsilon_svr ||
		param.svm_type == en_nu_svr)
	{
		double *sv_coef = this->sv_coef[0];
		double sum = 0;
		for(int i=0;i<l;i++)
			sum += sv_coef[i] * a_conject_kernel::k_function(x,this->sv[i],this->param);
		sum -= rho[0];
		*dec_values = sum;
	}
	else
	{
		int i;
		double *kvalue = new double[l];
		for(i=0;i<l;i++)
			kvalue[i] = a_conject_kernel::k_function(x,this->sv[i],this->param);

		int *start = new int[nr_class];
		start[0] = 0;
		for(i=1;i<nr_class;i++)
			start[i] = start[i-1]+ this->n_sv[i-1];

		int p=0;
		for(i=0;i<nr_class;i++)
			for(int j=i+1;j<nr_class;j++)
			{
				double sum = 0;
				int si = start[i];
				int sj = start[j];
				int ci = this->n_sv[i];
				int cj = this->n_sv[j];

				int k;
				double *coef1 = sv_coef[j-1];
				double *coef2 = sv_coef[i];
				for(k=0;k<ci;k++)
					sum += coef1[si+k] * kvalue[si+k];
				for(k=0;k<cj;k++)
					sum += coef2[sj+k] * kvalue[sj+k];
				sum -= this->rho[p];
				dec_values[p] = sum;
				p++;
			}

			delete[] kvalue;
			delete[] start;
	}
}

double cls_svm::predict(const cls_svm_node*x)
{
	if(param.svm_type == en_one_class ||
		param.svm_type == en_epsilon_svr ||
		param.svm_type == en_nu_svr)
	{
		double res;
		predict_values(x,&res);

		if(param.svm_type == en_one_class)
			return (res>0)?1:-1;
		else
			return res;
	}
	else
	{
		int i;
		double *dec_values = new double[nr_class*(nr_class-1)/2];

		predict_values(x,dec_values);

		int *vote = new int[nr_class];
		for(i=0;i<nr_class;i++)
			vote[i] = 0;

		int pos=0;
		for(i=0;i<nr_class;i++)
		{
			for(int j=i+1;j<nr_class;j++)
			{
				if(dec_values[pos++] > 0)
					++vote[i];
				else
					++vote[j];
			}
		}

		int vote_max_idx = 0;
		for(i=1;i<nr_class;i++)
			if(vote[i] > vote[vote_max_idx])
				vote_max_idx = i;

		delete[] vote;
		delete[] dec_values;

		return n_labels[vote_max_idx];
	}
}

double cls_svm::predict_probability(const cls_svm_node*x,double* prob_est)
{
	if ((-param.svm_type == en_c_svc || 
		param.svm_type == en_nu_svc) &&
		probA!=NULL && probB!=NULL)
	{
		int i;
		double *dec_values = new double[nr_class*(nr_class-1)/2];
		
		predict_values(x, dec_values);

		double min_prob=1e-7;
		double **pairwise_prob = new double*[nr_class];
		for(i=0;i<nr_class;i++)
			pairwise_prob[i]=new double[nr_class];
		
		int k=0;
		for(i=0;i<nr_class;i++)
			for(int j=i+1;j<nr_class;j++)
			{
				pairwise_prob[i][j]=min(max(sigmoid_predict(dec_values[k],probA[k],probB[k]),min_prob),1-min_prob);
				pairwise_prob[j][i]=1-pairwise_prob[i][j];
				k++;
			}
			multiclass_probability(nr_class,pairwise_prob,prob_est);

			int prob_max_idx = 0;
			for(i=1;i<nr_class;i++)
				if(prob_est[i] > prob_est[prob_max_idx])
					prob_max_idx = i;
			for(i=0;i<nr_class;i++)
				free(pairwise_prob[i]);
			free(dec_values);
			free(pairwise_prob);	     
			return n_labels[prob_max_idx];
	}
	else 
	{
		return predict(x);
	}
}

inline
void a_svm_cross_validation(const cls_svm_problem * prob,const cls_svm_param *param, int nr_fold,double *targer)
{
	int i;
	int *fold_start = new int[nr_fold+1];
	int l = prob->l;
	int *perm = new int[l];
	int nr_class;

	if((param->svm_type == en_c_svc ||
		param->svm_type == en_nu_svc) && nr_fold < l)
	{
		int *start = NULL;
		int *label = NULL;
		int *count = NULL;
		svm_group_classes(prob,&nr_class,&label,&start,&count,perm);

		int *fold_count = new int[nr_fold];
		int c;
		int *index = new int[l]; 
		for(i=0;i<l;i++)
			index[i]=perm[i];

		for (c=0; c<nr_class; c++) 
			for(i=0;i<count[c];i++)
			{
				int j = i+rand()%(count[c]-i);
				swap(index[start[c]+j],index[start[c]+i]);
			}

		for(i=0;i<nr_fold;i++)
		{
			fold_count[i] = 0;
			for (c=0; c<nr_class;c++)
				fold_count[i]+=(i+1)*count[c]/nr_fold-i*count[c]/nr_fold;
		}
		
		fold_start[0]=0;
		
		for (i=1;i<=nr_fold;i++)	
			fold_start[i] = fold_start[i-1]+fold_count[i-1];

		for (c=0; c<nr_class;c++)
		{
			for(i=0;i<nr_fold;i++)
			{
				int begin = start[c]+i*count[c]/nr_fold;
				int end = start[c]+(i+1)*count[c]/nr_fold;
				for(int j=begin;j<end;j++)
				{
					perm[fold_start[i]] = index[j];
					fold_start[i]++;
				}
			}
		}

		fold_start[0]=0;

		for (i=1;i<=nr_fold;i++)
			fold_start[i] = fold_start[i-1]+fold_count[i-1];

		delete []start;
		delete []label;
		delete []count;
		delete []index;
		delete []fold_count;
	}
	else
	{
		for(i=0;i<l;i++) 
			perm[i]=i;

		for(i=0;i<l;i++)
		{
			int j = i+rand()%(l-i);
			swap(perm[i],perm[j]);
		}

		for(i=0;i<=nr_fold;i++)
			fold_start[i]=i*l/nr_fold;
	}

	for(i=0;i<nr_fold;i++)
	{
		int begin = fold_start[i];
		int end = fold_start[i+1];
		int j,k;
		cls_svm_problem subprob;

		subprob.l = l-(end-begin);
		subprob.x = new cls_svm_node*[subprob.l];
		subprob.y = new double[subprob.l];

		k=0;
		for(j=0;j<begin;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}

		for(j=end;j<l;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}

		cls_svm submodel;

		submodel.train(&subprob,param);

		if(param->probability && 
			(param->svm_type == en_c_svc || param->svm_type == en_nu_svc))
		{
			double *prob_estimates= new double[submodel.get_nr_class()];
			for(j=begin;j<end;j++)
				targer[perm[j]] = submodel.predict_probability(prob->x[perm[j]],prob_estimates);
			
			delete [] prob_estimates;			
		}
		else
		{
			for(j=begin;j<end;j++)
				targer[perm[j]] = submodel.predict(prob->x[perm[j]]);
		}

		delete [] subprob.x;
		delete [] subprob.y;
	}		

	delete [] fold_start;
	delete [] perm;
}