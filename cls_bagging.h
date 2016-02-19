#include "lib_import.h"

class cls_bagging
	:public cls_boosting
{
protected:
	double prob;

	void trans_training_set(cls_training_set& ts,cls_training_set &ts_re)
	{
		double d_pro = prob;

		mem_num_output.resize(2);
		bool b_flag[2];
		b_flag[0] = b_flag[1] = true;

		ts.initialize();
		while(!ts.epoch_over())
		{
			if (a_unfirom_random(0,1) >= d_pro)
			{
				continue;
			}

			get_next_pair_for_ts(ts,a,b);
			int num = ts.get_example_id(ts.get_count()-1);

			vector<double> d_out;
			d_out.push_back(num);

			if (b_flag[num])
			{
				mem_num_output[num] = b;
				b_flag[num] = false;
			}

			ts_re.add_io_pair_with_map(a,d_out,num);
		}
	}

public:

	cls_bagging(double a_prob = 0.5)
		:cls_boosting(),
		 prob(a_prob)
	{
		;
	}

	virtual void train_example( cls_training_set & ts )
	{
		if (ts.get_output_classes_count() != 2)
			throw cls_exception_not_sense("cls_boosting_first::train_example() - the clser only can train two classes.");

		vector<cls_training_set> v_ts(size_weaker);

		vector<cls_training_set>::iterator itor_v;
		for(itor_v=v_ts.begin();itor_v != v_ts.end();itor_v++)
		{
			trans_training_set(ts,*itor_v);
		}

		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor_v=v_ts.begin(),itor = weakers.begin();itor != weakers.end();itor_v++,itor++)
		{
			vector<double> weights(itor_v->get_size());
			int size=itor_v->get_size();

			if (size == 0)
				throw cls_exception_un_stable("cls_bagging::train_example() - the system is unstable.\n");

			fill(weights.begin(),weights.end(),1.0/size);

			vector<bool> bs(size);
			fill(bs.begin(),bs.end(),false);

			int n_times = 0;

			switch(itor->first->get_classifier_type())
			{
			case en_clser_ann:
				n_times = 50;
				break;
			case en_clser_bagging:
				n_times = 10;
				break;
			case en_clser_bayes:
				n_times = 1;
				break;
			case en_clser_boosting:
				n_times = 10;
				break;
			case en_clser_hmm:
				n_times = 1;
				break;
			case en_clser_linear:
				n_times = 50;
				break;
			case en_clser_neaest:
				n_times = 1;
				break;
			case en_clser_svm:
				n_times = 1;
				break;
			default:
				throw cls_exception_not_found("cls_boosting_first::train_example() - the classifier can't be found,\n");
			}

			while(n_times--)
			{
				itor->first->train_example(*itor_v);
			}

			itor->second = 0.0;
			ts.initialize();
			while(!ts.epoch_over())
			{
				get_next_pair_for_ts(ts,a,b);

				try
				{
					if (itor->first->get_output_data(a) != b)
					{
						bs[ts.get_count()-1] = true;
						itor->second += weights[ts.get_count()-1];
					}
				}
				catch (cls_exception &exc)
				{
					try
					{
						if (itor->first->get_output_index(a) != ts.get_example_id(ts.get_count()-1))
						{
							bs[ts.get_count()-1] = true;
							itor->second += weights[ts.get_count()-1];
						}
					}
					catch (cls_exception &exc)
					{
						throw ;
					}
				}
			}

			itor->second /= size;
			itor->second = log((1-itor->second)*itor->second)/2;

			for(int cnt=0;cnt<size;cnt++)
			{
				if (bs[cnt] == false)
				{
					weights[cnt] = weights[cnt] * exp(itor->second);
				}
				else
				{
					weights[cnt] = weights[cnt] * exp(-itor->second);
				}
			}

			vector<double>::iterator itor;
			double d_sum = 0.001;
			for(itor=weights.begin();itor !=weights.end();itor++)
			{
				d_sum += (*itor);
			}

			for(itor=weights.begin();itor != weights.end();itor++)
			{
				(*itor) /= d_sum;
			}
		}
	}

	virtual int get_output_index(const vector<double>& input)
	{
		double d_sign = 0.0;

		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor = weakers.begin();itor != weakers.end();itor++)
		{
			try
			{
				int idx = itor->first->get_output_index(input);
				d_sign += ((idx==1)?-1:1) * itor->second;
			}
			catch (cls_exception &exc)
			{
				try
				{
					int idx = get_index_for_output(itor->first->get_output_data(input));
					d_sign += ((idx==1)?-1:1) * itor->second;
				}
				catch (cls_exception &exc)
				{
					cout<<exc.display()<<endl;
				}
			}
		}

		if (d_sign >= 0)
			return 1;
		else
			return 0;
	}
};
