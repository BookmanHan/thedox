#include "lib_import.h"

void test_genetic_algorthim_frame()
{
	cls_ganetic_algorithms_frame<cls_ganetic_algorithms_item_abstract<double> > cier(100,2);

	int n_time = 100;
	while(n_time--)
	{
		cout<<n_time<<endl;
		cier.run_once();
	}

	vector<cls_ganetic_algorithms_item*>& re = cier.get_output();
	vector<cls_ganetic_algorithms_item*>::iterator itor;
	for(itor=re.begin();itor != re.end();itor++)
	{
		a_vector_output(cout,((cls_ganetic_algorithms_item_abstract<double>*)(*itor))->get_gens());
		cout<<endl;
	}
}

void test_genetic_group_stable()
{
	cls_ganetic_algorithms_frame<cls_ganetic_algorithms_item_stable_group_func_peak<double,cls_ci_function_peak_guass<double> > > cier(1000,9);

	int n_time = 100;
	while(n_time--)
	{
		cout<<n_time<<":"<<cier.get_genes_num()<<endl;
		cier.run_once();
	}

	vector<cls_ganetic_algorithms_item*>& re = cier.get_output();
	vector<cls_ganetic_algorithms_item*>::iterator itor;
	for(itor=re.begin();itor != re.end();itor++)
	{
		a_vector_output(cout,((cls_ganetic_algorithms_item_stable_group_func_peak<double,cls_ci_function_peak_guass<double> >*)(*itor))->get_gens());
		cout<<endl;
	}
}
