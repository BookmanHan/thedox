#include "lib_import.h"

class cls_linear_sensor
	:public cls_linear
{
protected:
	double d_factor;
	double d_re;

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

	cls_linear_sensor(int num,int num_per,double d_factor_study = 0.2,int num_output = 1,bool b_add = true)
		:cls_linear(num,num_per,num_output)
	{
		d_factor =d_factor_study;

		if (b_add)
		{
			for(int cnt=0;cnt<num;cnt++)
			{
				add_unit();
			}
		}
	}

	virtual double get_factor()
	{
		return d_factor;
	}

	virtual void set_factor(double f)
	{
		d_factor = f;
	}

	virtual void add_unit() 
	{
		int size = get_size();
		units.push_back(new cls_linear_unit_sensor(_size_per_unit+1));
	}

	virtual void train_example(cls_training_set & ts)
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

		d_re = re;
		return;
	}

	void train(cls_training_set& ts,int n_times)
	{
		while(n_times--)
		{
			train_example(ts);
			if (d_re == true)
				break;
		}
	}

	virtual int get_output(double *input)
	{
		return cls_linear::get_output(input);
	}

	virtual int get_output(vector<double>& input)
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