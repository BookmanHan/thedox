#include "lib_import.h"

class cls_svm_c_svc
	:public cls_svm_base
{
public:
	cls_svm_c_svc(enum_kt kt,int degree,double gamma,double coef0) 
	{
		if (kt != en_poly)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");
		
		param.svm_type = en_c_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.degree = degree;
		param.coef = coef0;
	}

	cls_svm_c_svc(enum_kt kt,double gamma)
	{
		if (kt != en_rbf)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_c_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
	}

	cls_svm_c_svc(enum_kt kt,double gamma,double coef0)
	{
		if (kt != en_sigmoid)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_c_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.coef = coef0;
	}

	cls_svm_c_svc(enum_kt kt)
	{
		param.svm_type = en_c_svc;
		param.kernel_type = kt;
	}
};

class cls_svm_nu_svc
	:public cls_svm_base
{
public:
	cls_svm_nu_svc(enum_kt kt,int degree,double gamma,double coef0) 
	{
		if (kt != en_poly)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.degree = degree;
		param.coef = coef0;
	}

	cls_svm_nu_svc(enum_kt kt,double gamma)
	{
		if (kt != en_rbf)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
	}

	cls_svm_nu_svc(enum_kt kt,double gamma,double coef0)
	{
		if (kt != en_sigmoid)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svc;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.coef = coef0;
	}

	cls_svm_nu_svc(enum_kt kt)
	{
		param.svm_type = en_nu_svc;
		param.kernel_type = kt;
	}
};

class cls_svm_one_class
	:public cls_svm_base
{
public:
	cls_svm_one_class(enum_kt kt,int degree,double gamma,double coef0) 
	{
		if (kt != en_poly)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_one_class;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.degree = degree;
		param.coef = coef0;
	}

	cls_svm_one_class(enum_kt kt,double gamma)
	{
		if (kt != en_rbf)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_one_class;
		param.kernel_type = kt;

		param.gamma = gamma;
	}

	cls_svm_one_class(enum_kt kt,double gamma,double coef0)
	{
		if (kt != en_sigmoid)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_one_class;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.coef = coef0;
	}

	cls_svm_one_class(enum_kt kt)
	{
		param.svm_type = en_one_class;
		param.kernel_type = kt;
	}
};

class cls_svm_epsilon_svr
	:public cls_svm_base
{
public:
	cls_svm_epsilon_svr(enum_kt kt,int degree,double gamma,double coef0) 
	{
		if (kt != en_poly)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_epsilon_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.degree = degree;
		param.coef = coef0;
	}

	cls_svm_epsilon_svr(enum_kt kt,double gamma)
	{
		if (kt != en_rbf)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_epsilon_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
	}

	cls_svm_epsilon_svr(enum_kt kt,double gamma,double coef0)
	{
		if (kt != en_sigmoid)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_epsilon_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.coef = coef0;
	}

	cls_svm_epsilon_svr(enum_kt kt)
	{
		param.svm_type = en_epsilon_svr;
		param.kernel_type = kt;
	}
};


class cls_svm_nu_svr
	:public cls_svm_base
{
public:
	cls_svm_nu_svr(enum_kt kt,int degree,double gamma,double coef0) 
	{
		if (kt != en_poly)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.degree = degree;
		param.coef = coef0;
	}

	cls_svm_nu_svr(enum_kt kt,double gamma)
	{
		if (kt != en_rbf)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
	}

	cls_svm_nu_svr(enum_kt kt,double gamma,double coef0)
	{
		if (kt != en_sigmoid)
			throw cls_exception_not_sense("cls_svm_c_svc::cls_svm_c_svc() - the param is erro.");

		param.svm_type = en_nu_svr;
		param.kernel_type = kt;

		param.gamma = gamma;
		param.coef = coef0;
	}

	cls_svm_nu_svr(enum_kt kt)
	{
		param.svm_type = en_nu_svr;
		param.kernel_type = kt;
	}
};