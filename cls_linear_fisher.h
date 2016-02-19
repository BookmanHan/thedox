#include "lib_import.h"

class cls_linear_fisher
	:public cls_linear
{
protected:
	int org_num;

public:
	virtual void save(ofstream& fout) const
	{
		cls_linear::save(fout);
		fout.write((char*)&org_num,sizeof(org_num));
	}

	virtual void load(ifstream& fin)
	{
		cls_linear::load(fin);
		fin.read((char*)&org_num,sizeof(org_num));
	}

	cls_linear_fisher(int num,int per,int num_ouput=1)
		:cls_linear(num*(num-1)/2,per,num_ouput),
		org_num(num)
	{
		for(int cnt=0;cnt<(num*(num-1)/2);cnt++)
		{
			add_unit();
		}
	}

	virtual void add_unit() 
	{
		int size = get_size_per_unit();
		units.push_back(new cls_linear_unit_fisher(size));
	}

	virtual void train_example( cls_training_set & ts ) 
	{
		int num = org_num;

		int cnt_num = 0;
		for(int i=0;i<num;i++)
		{
			for(int j=i+1;j<num;j++)
			{
				vector<vector<double>*> input1;
				vector<vector<double>*> input2;

				ts.initialize();
				for(int cnt=0;cnt<ts.get_size();cnt++)
				{
					get_next_pair_for_ts(ts,a,b);
					int num = ts.get_example_id(cnt);
					if (num == i)
					{
						input1.push_back(&a);
					}
					else if (num == j)
					{
						input2.push_back(&a);
					}
				}

				((cls_linear_unit_fisher*)units[cnt_num])->set_input(&input1,&input2);
				units[cnt_num]->update(0.0);

				cnt_num++;
			}
		}

		return;
	}
	
	virtual int get_output(double *input)
	{
		return cls_linear::get_output(input);
	}

	virtual void train(cls_training_set & ts)
	{
		train_example(ts);
	}

	virtual int get_output(vector<double>& input) 
	{
		int num = org_num;

		vector<bool> flag(num);
		fill(flag.begin(),flag.end(),true);

		int cnt=0;
		for(int i=0;i<num;i++)
		{
			for(int j=i+1;j<num;j++)
			{
				cnt++;

				if (flag[j] == false || flag[i] == false)
					continue;

				units[cnt-1]->set_input(input);
				double d = units[cnt-1]->get_output();

				if (units[cnt-1]->get_output() >= 0)
				{
					flag[j] = false;
				}
				else
				{
					flag[i] = false;
				}
			}
		}

		for(int cnt=0;cnt<num;cnt++)
		{
			if (flag[cnt])
				return cnt;
		}
	}
};