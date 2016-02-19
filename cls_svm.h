#include "lib_import.h"

class cls_svm
{
public:
	int l;
	cls_svm_param param;
	int nr_class;
	int num;
	cls_svm_node** sv;
	double** sv_coef;
	double *rho;
	double *probA;
	double *probB;

	int* n_labels;
	int* n_sv;

	int free_sv;

	virtual ~cls_svm()
	{
		if (free_sv && l>0)
		{
			delete[] sv[0];
		}

		for(int i=0;i<nr_class-1;i++)
		{
			delete [] sv_coef[i];
		}

		delete []sv;
		delete []sv_coef;
		delete []rho;
		delete []n_labels;
		delete []probA;
		delete []probB;
		delete []n_sv;
	}

	void train(const cls_svm_problem* prob,const cls_svm_param * param);
	
	enum_svm get_svm_type();

	int get_nr_class();

	void get_labels(int *label);

	double get_svr_probability();

	void predict_values(const cls_svm_node*x,double*dec_values);

	double predict(const cls_svm_node*x);

	double predict_probability(const cls_svm_node*x,double* prob_est);

	const char* check_param(const struct cls_svm_problem* prob,const struct cls_svm_param *param);

	int check_probability_model();
};

inline
void a_svm_cross_validation(const cls_svm_problem * prob,const cls_svm_param *param, int nr_fold,double *targer);