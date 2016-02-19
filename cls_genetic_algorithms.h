#include "lib_import.h"

class cls_ganetic_algorithms
	:public cls_ci
{
protected:
	vector<cls_ganetic_algorithms_item*> genes;
	int n_times;
	int gene_size;

public:
	virtual int get_gene_size()
	{
		return gene_size;
	}

	cls_ganetic_algorithms(int init_group_size,int a_gene_size)
		:gene_size(a_gene_size)
	{
		n_times = 0;
	}

	virtual void run_once()
	{
		choose();
		cross();
		var();

		n_times++;
	}

	virtual void add_item() = 0;

	virtual void cross()
	{
		int size = get_genes_num();

		vector<cls_ganetic_algorithms_item*>::iterator itor;
		for(int cnt=0;cnt<size;cnt++)
		{
			if (a_unfirom_random(0,1) <genes[cnt]->get_cross_prob(n_times))
			{
				int pos = 0;

				pos = rand()%size;
				genes.push_back(genes[cnt]->cross(*genes[pos]));

				pos = rand()%size;
				genes.push_back(genes[cnt]->cross(*genes[pos]));
			}
		}
	}

	virtual void var()
	{
		int size = get_genes_num();

		vector<cls_ganetic_algorithms_item*>::iterator itor;
		for(itor = genes.begin();itor != genes.end();itor++)
		{
			if (a_unfirom_random(0,1) < (*itor)->get_var_prob(n_times))
			{
				int pos = rand()%size;
				(*itor)->var(*genes[pos]);
			}
		}
	}

	virtual void choose()
	{
		vector<pair<double,int> > rank;

		int size_post = 0;

		int cnt = 0;
		vector<cls_ganetic_algorithms_item*>::iterator itor;
		for(cnt=0,itor = genes.begin();itor != genes.end();itor++,cnt++)
		{
			rank.push_back(make_pair((*itor)->get_select_value(),cnt));
		}

		sort(rank.begin(),rank.end());

		vector<bool> b_select(rank.size());
		fill(b_select.begin(),b_select.end(),0);

		double size = get_genes_num();

		vector<pair<double,int> >::iterator itor_v;
		cnt = 0;
		for(itor_v = rank.begin();itor_v != rank.end();itor_v ++,cnt++)
		{
			b_select[itor_v->second] = genes[itor_v->second]->select(rank,itor_v->first,double(cnt)/size,n_times);

			if (b_select[itor_v->second] == true)
				size_post++;
		}

		int i=0;
		int j=size-1;
		while(i < j)
		{
			while(j >=0 && b_select[j] == false)
				j--;

			while(i < size && b_select[i] == true)
				i++;

			if (j < 0)
				break;
			else if (i >= size)
				break;

			swap(genes[i],genes[j]);
			j--;
			i++;
		}

		genes.resize(size_post);
	}

	virtual int get_genes_num()
	{
		return genes.size();
	}

	virtual ~cls_ganetic_algorithms()
	{
		vector<cls_ganetic_algorithms_item*>::iterator itor;
		for(itor = genes.begin();itor != genes.end();itor++)
		{
			delete (*itor);
		}
	}

	virtual vector<cls_ganetic_algorithms_item*>& get_output()
	{
		return genes;
	}

	virtual vector<double>& get_output_data()
	{
		throw cls_exception_not_implemented(string(__FUNCTION__) + "() - this function is of no sense.\n");
	}
};


template<typename T>
class cls_ganetic_algorithms_frame
	:public cls_ganetic_algorithms
{
public:
	cls_ganetic_algorithms_frame(int init_size,int gens_size)
		:cls_ganetic_algorithms(init_size,gens_size)
	{
		for(int cnt=0;cnt<init_size;cnt++)
		{
			add_item();
		}
	}

	virtual void add_item()
	{
		int size = get_gene_size();
		genes.push_back(new T);
		((T*)genes.back())->set_gens_init(size);
	}
};
