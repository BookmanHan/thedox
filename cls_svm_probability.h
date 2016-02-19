#include "lib_import.h"

inline
void multiclass_probability(int k, double **r, double *p)
{
	int t,j;
	int iter = 0, max_iter=max(100,k);
	double **Q= new double*[k];
	double *Qp= new double[k];
	double pQp, eps=0.005/k;

	for (t=0;t<k;t++)
	{
		p[t]=1.0/k;  // Valid if k = 1
		Q[t]=new double[k];
		Q[t][t]=0;
		for (j=0;j<t;j++)
		{
			Q[t][t]+=r[j][t]*r[j][t];
			Q[t][j]=Q[j][t];
		}
		for (j=t+1;j<k;j++)
		{
			Q[t][t]+=r[j][t]*r[j][t];
			Q[t][j]=-r[j][t]*r[t][j];
		}
	}
	for (iter=0;iter<max_iter;iter++)
	{
		pQp=0;
		for (t=0;t<k;t++)
		{
			Qp[t]=0;
			for (j=0;j<k;j++)
				Qp[t]+=Q[t][j]*p[j];
			pQp+=p[t]*Qp[t];
		}
		double max_error=0;
		for (t=0;t<k;t++)
		{
			double error=fabs(Qp[t]-pQp);
			if (error>max_error)
				max_error=error;
		}
		if (max_error<eps) break;

		for (t=0;t<k;t++)
		{
			double diff=(-Qp[t]+pQp)/Q[t][t];
			p[t]+=diff;
			pQp=(pQp+diff*(diff*Q[t][t]+2*Qp[t]))/(1+diff)/(1+diff);
			for (j=0;j<k;j++)
			{
				Qp[j]=(Qp[j]+diff*Q[t][j])/(1+diff);
				p[j]/=(1+diff);
			}
		}
	}
	
	for(t=0;t<k;t++) delete (Q[t]);
	delete []Q;
	delete []Qp;
}

inline
void svm_binary_svc_probability(
								const cls_svm_problem *prob, const cls_svm_param *param,
								double Cp, double Cn, double& probA, double& probB)
{
	int i;
	int nr_fold = 5;
	int *perm = new int[prob->l];
	double *dec_values = new double[prob->l];

	// random shuffle
	for(i=0;i<prob->l;i++) perm[i]=i;
	for(i=0;i<prob->l;i++)
	{
		int j = i+rand()%(prob->l-i);
		swap(perm[i],perm[j]);
	}
	for(i=0;i<nr_fold;i++)
	{
		int begin = i*prob->l/nr_fold;
		int end = (i+1)*prob->l/nr_fold;
		int j,k;
		cls_svm_problem subprob;

		subprob.l = prob->l-(end-begin);
		subprob.x = new cls_svm_node*[subprob.l];
		subprob.y = new double[subprob.l];

		k=0;
		for(j=0;j<begin;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}
		for(j=end;j<prob->l;j++)
		{
			subprob.x[k] = prob->x[perm[j]];
			subprob.y[k] = prob->y[perm[j]];
			++k;
		}
		int p_count=0,n_count=0;
		for(j=0;j<k;j++)
			if(subprob.y[j]>0)
				p_count++;
			else
				n_count++;

		if(p_count==0 && n_count==0)
			for(j=begin;j<end;j++)
				dec_values[perm[j]] = 0;
		else if(p_count > 0 && n_count == 0)
			for(j=begin;j<end;j++)
				dec_values[perm[j]] = 1;
		else if(p_count == 0 && n_count > 0)
			for(j=begin;j<end;j++)
				dec_values[perm[j]] = -1;
		else
		{
			cls_svm_param subparam = *param;
			subparam.probability=0;
			subparam.c=1.0;
			subparam.nr_weight=2;
			subparam.weight_label = new int[2];
			subparam.weight = new double[2];
			subparam.weight_label[0]=+1;
			subparam.weight_label[1]=-1;
			subparam.weight[0]=Cp;
			subparam.weight[1]=Cn;
			cls_svm submodel;
			submodel.train(&subprob,&subparam);
			for(j=begin;j<end;j++)
			{
				submodel.predict_values(prob->x[perm[j]],&(dec_values[perm[j]]));
	
				// ensure +1 -1 order; reason not using CV subroutine
				dec_values[perm[j]] *= submodel.n_labels[0];
			}
		}

		delete[] subprob.x;
		delete[] subprob.y;
	}		
	sigmoid_train(prob->l,dec_values,prob->y,probA,probB);
	delete[] dec_values;
	delete[] perm;
}

// Return parameter of a Laplace distribution 
inline
double svm_svr_probability(const cls_svm_problem *prob, const cls_svm_param *param)
{
	int i;
	int nr_fold = 5;
	double *ymv = new double[prob->l];
	double mae = 0;

	cls_svm_param newparam = *param;
	newparam.probability = 0;
	a_svm_cross_validation(prob,&newparam,nr_fold,ymv);
	for(i=0;i<prob->l;i++)
	{
		ymv[i]=prob->y[i]-ymv[i];
		mae += fabs(ymv[i]);
	}		
	mae /= prob->l;
	double std=sqrt(2*mae*mae);
	int count=0;
	mae=0;
	for(i=0;i<prob->l;i++)
		if (fabs(ymv[i]) > 5*std) 
			count=count+1;
		else 
			mae+=fabs(ymv[i]);

	mae /= (prob->l-count);
	
	delete[] ymv;
	return mae;
}

inline
void svm_group_classes(const cls_svm_problem *prob, int *nr_class_ret, int **label_ret, int **start_ret, int **count_ret, int *perm)
{
	int l = prob->l;
	int max_nr_class = 4096;
	int nr_class = 0;
	int *label = new int[max_nr_class];
	int *count = new int[max_nr_class];
	int *data_label = new int[l];	
	int i;

	for(i=0;i<l;i++)
	{
		int this_label = (int)prob->y[i];
		int j;
		for(j=0;j<nr_class;j++)
		{
			if(this_label == label[j])
			{
				++count[j];
				break;
			}
		}

		data_label[i] = j;

		if(j == nr_class)
		{
			if(nr_class == max_nr_class)
			{
				max_nr_class *= 2;
				int *tmp;

				tmp = new int[max_nr_class];
				memcpy(tmp,label,max_nr_class*sizeof(int));
				delete[] label;
				label = tmp;

				tmp = new int[max_nr_class];
				memcpy(tmp,count,max_nr_class*sizeof(int));
				delete[] count;
				count = tmp;
			}

			label[nr_class] = this_label;
			count[nr_class] = 1;
			++nr_class;
		}
	}

	int *start = new int[nr_class];
	start[0] = 0;
	for(i=1;i<nr_class;i++)
		start[i] = start[i-1]+count[i-1];

	for(i=0;i<l;i++)
	{
		perm[start[data_label[i]]] = i;
		++start[data_label[i]];
	}

	start[0] = 0;
	for(i=1;i<nr_class;i++)
		start[i] = start[i-1]+count[i-1];

	*nr_class_ret = nr_class;
	*label_ret = label;
	*start_ret = start;
	*count_ret = count;
	
	delete[] data_label;
}
