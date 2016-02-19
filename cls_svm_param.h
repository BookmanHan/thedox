#include "lib_import.h"

class cls_svm_node
{
public:
	int index;
	double value;
};

class cls_svm_problem
{
public:
	int l;
	double* y;
	cls_svm_node** x;
};

enum enum_kt
{en_linear,en_poly,en_rbf,en_sigmoid,en_precomputed};

enum enum_svm
{en_c_svc,en_nu_svc,en_one_class,en_epsilon_svr,en_nu_svr};

class cls_svm_param
{
public:
	enum_svm svm_type;
	int degree;
	double gamma;
	double coef;
	double cache_size;
	double eps;
	double c;
	int nr_weight;
	int* weight_label;
	double* weight;
	double nu;
	double p;
	int shrinking;
	int probability;
	enum_kt kernel_type;
};

