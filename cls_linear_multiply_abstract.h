#include "lib_import.h"

class cls_linear_mutiply_abstract
	:public cls_linear_mutiply
{
public:
	cls_linear_mutiply_abstract(int num,int per)
		:cls_linear_mutiply(num,per)
	{
		;
	}

	virtual int get_output( vector<double> & input ) 
	{
		vector<double> v_b(get_num(),0);
		fill(v_b.begin(),v_b.end(),0.0);

		int cnt=0;
		for(int i=0;i<num;i++)
		{
			for(int j=0;j<num;j++)
			{	
				if (i == j)
				{
					continue;
				}

				if (linears[cnt]->get_output(input) == 1)
				{
					v_b[j]+= 1.0;
					v_b[i]-= 0.5;
				}
				else
				{
					v_b[i]+= 1.0;
					v_b[i]-= 0.5;
 				}

				cnt++;
			}
		}

		int indx = 0;
		for(int cnt=0;cnt<get_num();cnt++)
		{
			if (v_b[cnt] >= v_b[indx])
			{
				indx = cnt;
			}
		}

		return indx;
	}

	virtual void train_example(cls_training_set & ts)
	{
		train(ts,1);
		throw cls_exception_low_effective("cls_linear_mutiply_abstract::train_example() - low effetive. please use train().");
	}

	virtual void train( cls_training_set & ts,int n_times ) 
	{
		int cnt =0;
		int num = get_num();
		
		for(int i=0;i<num;i++)
		{
			for(int j=0;j<num;j++)
			{
				if (i == j)
					continue;

				ts.initialize();

				cls_training_set ts_tmp(ts.get_input_size(),ts.get_output_size());
				while(!ts.epoch_over())
				{
					get_next_pair_for_ts(ts,a,b);
					int cn = ts.get_example_id(ts.get_count()-1);
					if (cn == i)
					{
						ts_tmp.add_io_pair(a,0);
					}
					else if (cn == j)
					{
						ts_tmp.add_io_pair(a,1);
					}
				}

				int a_times = n_times;
				while(a_times--)
				{
					linears[cnt]->train_example(ts_tmp);
				}

				cnt++;
			}
		}
	}

	virtual int get_output(double * input)
	{
		return cls_linear_mutiply::get_output(input);
	}
};