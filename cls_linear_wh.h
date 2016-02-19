#include "lib_import.h"

class cls_linear_wh
	:public cls_linear
{
protected:
	double d_factor;

public:
	virtual void save(ofstream& fout) const
	{
		cls_linear::save(fout);

		fout.write((char*)&d_factor,sizeof(d_factor));
	}

	virtual void load(ifstream& fin)
	{
		cls_linear::load(fin);

		fin.read((char*)&d_factor,sizeof(d_factor));
	}

	cls_linear_wh(int num,int per,int factor = 0.1)
		:cls_linear(num,per)
	{
		d_factor = factor;
		for(int cnt=0;cnt<num;cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit() 
	{
		int size = get_size_per_unit();
		units.push_back(new cls_linear_unit_wh(size));
	}

	virtual void train(cls_training_set & ts ,int n_times)
	{
		while(n_times--)
		{
			train_example(ts);
		}
	}

	virtual void train_example( cls_training_set & ts ) 
	{
		ts.initialize();

		bool re = true;
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,ts_in,ts_out);
			int n_mem = ts.get_example_id(ts.get_count()-1);

			int cnt=0;
			vector<cls_linear_unit*>::iterator itor;
			for(itor = units.begin();itor != units.end();itor++)
			{
				assert(*itor);
				(*itor)->set_input(ts_in);

				if (cnt == n_mem)
					(*itor)->set_output(1);
				else
					(*itor)->set_output(-1);

				re &=(*itor)->update(d_factor);

				cnt++;
			}
		}

		return;
	}

	virtual int get_output(double* input )
	{
		vector<double> re;
		for(int cnt=0;cnt<get_size_per_unit();cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output(re);
	}

	virtual int get_output(vector<double>& input ) 
	{
		vector<double> d_o;
		vector<cls_linear_unit*>::iterator itor;
		for(itor = units.begin();itor != units.end();itor++)
		{
			assert(*itor);
			(*itor)->set_input(input);

			d_o.push_back((*itor)->get_output());
		}

		int indx = 0;
		for(int cnt=0;cnt<d_o.size();cnt++)
		{
			if (d_o[cnt] > d_o[indx])
				indx = cnt;
		}

		return indx;
	}
};