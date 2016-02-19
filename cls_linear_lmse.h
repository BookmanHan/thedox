#include "lib_import.h"

class cls_linear_lmse
	:public cls_linear
{
public:
	virtual void save(ofstream& fout) const
	{
		cls_linear::save(fout);
	}
	
	virtual void load(ifstream& fin)
	{
		cls_linear::load(fin);
	}

	cls_linear_lmse(int num,int per,int num_output=1)
		:cls_linear(num,per,num_output)
	{
		for(int cnt=0;cnt<num;cnt++)
		{
			add_unit();
		}
	}

	virtual void train(cls_training_set & ts,int n_times)
	{
		while(n_times--)
		{
			train_example(ts);
		}
	}

	virtual void add_unit() 
	{
		int size = get_size_per_unit();
		units.push_back(new cls_linear_unit_lmse(size));
	}

	virtual int get_output(vector<double> & input) 
	{
		vector<double> d_out;

		int cnt=0;
		vector<cls_linear_unit*>::iterator itor;
		for(itor = units.begin();itor != units.end();itor++,cnt++)
		{
			(*itor)->set_input(input);
			d_out.push_back((*itor)->get_output());
		}

		int ind = 0;
		for(int cnt=0;cnt<d_out.size();cnt++)
		{
			if (d_out[ind] < d_out[cnt])
			{
				ind = cnt;
			}
		}

		return ind;
	}

	virtual int get_output(double *input)
	{
		return cls_linear::get_output(input);
	}

	virtual void train_example( cls_training_set & ts ) 
	{
		ts.initialize();
		while(! ts.epoch_over())
		{
			get_next_pair_for_ts(ts,in,out);
			int num = ts.get_example_id(ts.get_count()-1);

			vector<cls_linear_unit*>::iterator itor;
			for(itor = units.begin();itor != units.end();itor++)
			{
				(*itor)->set_input(in);
			}

			int cnt=0;
			for(itor = units.begin();itor != units.end();itor++,cnt++)
			{
				double rx;
				if (cnt == num)
					rx = 1.0;
				else
					rx = -1.0;

				(*itor)->update(rx);
			}
		}
			
		return;
	}
};