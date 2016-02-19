#include "lib_import.h"

class cls_nearest
	:public cls_classifier
{
protected:
	cls_training_set& ts;
	cls_conject_linear *func;
	cls_conject_linear_euclid default_func;

public:
	cls_nearest(cls_training_set& a_ts)
		:ts(a_ts),
		 cls_classifier(en_clser_neaest)
	{
		func = &default_func;
	}

	virtual void set_func(cls_conject_linear* a_func)
	{
		func = a_func;
	}

	virtual void save( ofstream& fout ) const
	{
		ts.save(fout);
	}

	virtual void train_example( cls_training_set & ts )
	{
		throw cls_exception_not_sense("cls_nearest::train_example() - there is nothing to do.\n");
	}

	virtual vector<double> output(const vector<double> &input )
	{
		ts.initialize();

		int max_indx;
		double max_value = -inf;
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			double d_val = (*func)(input,a);
			if(d_val < max_value)
			{
				max_value = d_val;
				max_indx = ts.get_example_id(ts.get_count()-1);
			}
		}

		return ts.get_output_for_id(max_indx);
	}

	virtual void train( cls_training_set & ts,int n_times )
	{
		train_example(ts);
	}

	virtual void load( ifstream& fin )
	{
		ts.load(fin);
	}

	virtual vector<double> get_output_data( const vector<double> & input )
	{
		return output(input);
	}

	virtual int get_output_index(const vector<double> & input )
	{
		throw cls_exception_not_sense("cls_nearest::get_output_index() - use data instread.\n");
	}

	virtual double get_output_double(const cls_training_set& ts )
	{
		throw cls_exception_not_sense("cls_nearest::get_output_double() - use data instread.\n");
	}
};

class cls_nearest_k
	:public cls_nearest
{
protected:
	int k;

public:
	cls_nearest_k(cls_training_set& ts,int a_k)
		:cls_nearest(ts),
		 k(a_k)
	{
		;
	}

	//low effetive.
	virtual vector<double> output( vector<double> &input )
	{
		ts.initialize();

		priority_queue<pair<double,int> > pq;

		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			double d_val = (*func)(input,a);
			pq.push(make_pair(d_val,ts.get_count()));
		}

		map<int,int> counter;

		for(int cnt=0;cnt<k;cnt++)
		{
			counter[pq.top().second]++;
			pq.pop();
		}

		map<int,int>::iterator itor_re = counter.begin();
		map<int,int>::iterator itor;
		for(itor=counter.begin();itor != counter.end();itor++)
		{
			if (itor_re->second < itor->second)
			{
				itor_re = itor;
			}
		}

		return ts.get_output_for_id(ts.get_example_id(itor->first));
	}
};
