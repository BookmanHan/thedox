#include "lib_import.h"

//基因的初始化工作需要自己来完成。

class cls_ganetic_algorithms_item
{
public:
	virtual double get_select_value() = 0;
	virtual bool select(vector<pair<double,int> >& val_suit,double d_suit,double d_percent,int n_times) = 0;
	virtual double get_cross_prob(int n_times) = 0;
	virtual cls_ganetic_algorithms_item* cross(cls_ganetic_algorithms_item& that) = 0;
	virtual double get_var_prob(int n_times) = 0;
	virtual void var(cls_ganetic_algorithms_item& that) = 0;
};

template<typename T>
class cls_ganetic_algorithms_item_abstract
	:public cls_ganetic_algorithms_item
{
protected:
	vector<T> gens;

public:

	virtual vector<T>& get_gens()
	{
		return gens;
	}

	virtual int get_gene_size()
	{
		return gens.size();
	}

	cls_ganetic_algorithms_item_abstract()
	{
		;
	}

	void set_gens_init(int gens_size)
	{
		gens.resize(gens_size);

        #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor = gens.begin();itor != gens.end();itor++)
		{
			(*itor) = a_unfirom_random(0,1);
		}
	}

	cls_ganetic_algorithms_item_abstract(int gene_size)
	{
		set_gens_init(gene_size);
	}

	virtual bool select( vector<pair<double,int> >& val_suit,double d_suit,double d_percent,int n_times )
	{
		if (d_percent > 0.475)
			return true;
		else
			return false;
	}

	virtual double get_var_prob( int n_times )
	{
		return (1/240.0 + 0.11375/pow(2.0,1.0*n_times));
	}

	virtual double get_cross_prob( int n_times )
	{
		return 0.5;
	}

	virtual void var( cls_ganetic_algorithms_item& that )
	{
	    #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor=gens.begin();itor != gens.end();itor++)
		{
			*itor = a_unfirom_random(0,2) * (*itor);
		}
	}

	virtual cls_ganetic_algorithms_item* cross( cls_ganetic_algorithms_item & athat )
	{
		cls_ganetic_algorithms_item_abstract<T>& that = (cls_ganetic_algorithms_item_abstract<T>&) athat;
		cls_ganetic_algorithms_item_abstract<T> *item = new cls_ganetic_algorithms_item_abstract<T>(get_gene_size());

        #ifdef __USING_GCC__
		typename vector<T>::iterator itor,itor_this,itor_that;
		#else
		vector<T>::iterator itor,itor_this,itor_that;
		#endif

		for(itor= item->gens.begin(),itor_this = item->gens.begin(),itor_that= that.gens.begin();
			itor != item->gens.end();
			itor++,itor_that++,itor_this++)
		{
			if (a_unfirom_random(0,1) > 0.5)
			{
				*itor = *itor_that;
			}
			else
			{
				*itor = *itor_this;
			}
		}

		return item;
	}

	virtual double get_select_value()
	{
		double d_re = 0.0;

        #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor=gens.begin();itor!=gens.end();itor++)
		{
			d_re += -(*itor-1)*(*itor-1) + 1;
		}

		return d_re;
	}
};

template<typename T>
class cls_ganetic_algorithms_item_stable_group
	:public cls_ganetic_algorithms_item
{
protected:
	vector<T> gens;

public:

	virtual vector<T>& get_gens()
	{
		return gens;
	}

	virtual int get_gene_size()
	{
		return gens.size();
	}

	cls_ganetic_algorithms_item_stable_group()
	{
		;
	}

	void set_gens_init(int gens_size)
	{
		gens.resize(gens_size);

        #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor = gens.begin();itor != gens.end();itor++)
		{
			(*itor) = a_unfirom_random(0,1);
		}
	}

	cls_ganetic_algorithms_item_stable_group(int gene_size)
	{
		set_gens_init(gene_size);
	}

	virtual bool select( vector<pair<double,int> >& val_suit,double d_suit,double d_percent,int n_times )
	{
		static int size_group = 0;

		if (size_group == 0)
		{
			if (d_percent > 0.5)
				return true;
			else
				return false;
		}
		else if (size_group < val_suit.size())
		{
			if (d_percent > 0.6)
				return true;
			else
				return false;
		}
		else
		{
			if (d_percent > 0.3)
				return true;
			else
				return false;
		}

		size_group = val_suit.size();
	}

	virtual double get_var_prob( int n_times )
	{
		return (1/240.0 + 0.11375/pow(2.0,1.0*n_times));
	}

	virtual double get_cross_prob( int n_times )
	{
		return 0.5;
	}

	virtual void var( cls_ganetic_algorithms_item& that )
	{
	    #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor=gens.begin();itor != gens.end();itor++)
		{
			*itor = a_unfirom_random(-2,2) * (*itor);
		}
	}

	virtual cls_ganetic_algorithms_item* cross( cls_ganetic_algorithms_item& athat )
	{
		cls_ganetic_algorithms_item_stable_group& that = (cls_ganetic_algorithms_item_stable_group&) athat;
		cls_ganetic_algorithms_item_stable_group *item = new cls_ganetic_algorithms_item_stable_group(get_gene_size());

	    #ifdef __USING_GCC__
		typename vector<T>::iterator  itor,itor_this,itor_that;
		#else
		vector<T>::iterator  itor,itor_this,itor_that;
		#endif

		for(itor= item->gens.begin(),itor_this = item->gens.begin(),itor_that= that.gens.begin();
			itor != item->gens.end();
			itor++,itor_that++,itor_this++)
		{
			if (a_unfirom_random(0,1) > 0.5)
			{
				*itor = *itor_that;
			}
			else
			{
				*itor = *itor_this;
			}
		}

		return item;
	}

	virtual double get_select_value()
	{
		double d_re = 0.0;

        #ifdef __USING_GCC__
		typename vector<T>::iterator itor;
		#else
		vector<T>::iterator itor;
		#endif

		for(itor=this->gens.begin();itor!=this->gens.end();itor++)
		{
			d_re += -(*itor-1)*(*itor-1) + 1;
		}

		return d_re;
	}
};

template<typename T,typename FUNC>
class cls_ganetic_algorithms_item_stable_group_func_peak
	:public cls_ganetic_algorithms_item_stable_group<T>
{

protected:
	FUNC func;

public:
	cls_ganetic_algorithms_item_stable_group_func_peak()
	{
		;
	}

	virtual double get_select_value()
	{
		return func(this->gens);
	}
};
