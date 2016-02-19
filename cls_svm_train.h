#include "lib_import.h"

struct decision_function
{
	double *alpha;
	double rho;
};

inline
decision_function a_svm_train_one
	(const cls_svm_problem* prob,const cls_svm_param *param,
	double cp,double cn)
{
	double *alpha = new double[prob->l];
	cls_svm_solver::solution_info si;
	switch(param->svm_type)
	{
	case en_c_svc:
		a_solve_c_svc(prob,param,alpha,&si,cp,cn);
		break;
	case en_nu_svc:
		a_solve_nu_svc(prob,param,alpha,&si);
		break;
	case en_one_class:
		a_solve_one_class(prob,param,alpha,&si);
		break;
	case en_epsilon_svr:
		a_solve_epsilon_svr(prob,param,alpha,&si);
		break;
	case en_nu_svr:
		a_solve_nu_svr(prob,param,alpha,&si);
		break;
	}

	decision_function f;
	f.alpha = alpha;
	f.rho = si.rho;

	return f;
}

inline
double sigmoid_predict(double decision_value, double A, double B)
{
	double fApB = decision_value*A+B;
	if (fApB >= 0)
		return exp(-fApB)/(1.0+exp(-fApB));
	else
		return 1.0/(1+exp(fApB)) ;
}

inline
void sigmoid_train(
				   int l, const double *dec_values, const double *labels, 
				   double& A, double& B)
{
	double prior1=0, prior0 = 0;
	int i;

	for (i=0;i<l;i++)
	{
		if (labels[i] > 0) prior1+=1;
		else prior0+=1;
	}

	int max_iter=100; 
	double min_step=1e-10;	
	double sigma=1e-12;	
	double eps=1e-5;
	double hiTarget=(prior1+1.0)/(prior1+2.0);
	double loTarget=1/(prior0+2.0);
	double *t= new double[l];

	double fApB,p,q,h11,h22,h21,g1,g2,det,dA,dB,gd,stepsize;
	double newA,newB,newf,d1,d2;
	int iter; 

	A=0.0; B=log((prior0+1.0)/(prior1+1.0));
	double fval = 0.0;

	for (i=0;i<l;i++)
	{
		if (labels[i]>0) t[i]=hiTarget;
		else t[i]=loTarget;
		fApB = dec_values[i]*A+B;
		if (fApB>=0)
			fval += t[i]*fApB + log(1+exp(-fApB));
		else
			fval += (t[i] - 1)*fApB +log(1+exp(fApB));
	}
	for (iter=0;iter<max_iter;iter++)
	{
		
		h11=sigma; 
		h22=sigma;
		h21=0.0;g1=0.0;g2=0.0;
		for (i=0;i<l;i++)
		{
			fApB = dec_values[i]*A+B;
			if (fApB >= 0)
			{
				p=exp(-fApB)/(1.0+exp(-fApB));
				q=1.0/(1.0+exp(-fApB));
			}
			else
			{
				p=1.0/(1.0+exp(fApB));
				q=exp(fApB)/(1.0+exp(fApB));
			}
			d2=p*q;
			h11+=dec_values[i]*dec_values[i]*d2;
			h22+=d2;
			h21+=dec_values[i]*d2;
			d1=t[i]-p;
			g1+=dec_values[i]*d1;
			g2+=d1;
		}

		if (fabs(g1)<eps && fabs(g2)<eps)
			break;

		det=h11*h22-h21*h21;
		dA=-(h22*g1 - h21 * g2) / det;
		dB=-(-h21*g1+ h11 * g2) / det;
		gd=g1*dA+g2*dB;


		stepsize = 1; 
		while (stepsize >= min_step)
		{
			newA = A + stepsize * dA;
			newB = B + stepsize * dB;

			newf = 0.0;
			for (i=0;i<l;i++)
			{
				fApB = dec_values[i]*newA+newB;
				if (fApB >= 0)
					newf += t[i]*fApB + log(1+exp(-fApB));
				else
					newf += (t[i] - 1)*fApB +log(1+exp(fApB));
			}
			
			if (newf<fval+0.0001*stepsize*gd)
			{
				A=newA;B=newB;fval=newf;
				break;
			}
			else
				stepsize = stepsize / 2.0;
		}

		if (stepsize < min_step)
		{
			break;
		}
	}

	delete[] t;
}

