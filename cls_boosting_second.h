#include "lib_import.h"

class cls_boosting_adaptive_second
	:public cls_boosting
{
public:
	cls_boosting_adaptive_second()
		:cls_boosting()
	{
		;
	}

	virtual void train_example( cls_training_set & ts_in )
	{
		if (ts_in.get_output_classes_count() != 2)
			throw cls_exception_not_sense("cls_boosting_first::train_example() - the clser only can train two classes.");

		cls_training_set ts(ts_in.get_input_size(),ts_in.get_output_size());
		trans_training_set(ts_in,ts);

		vector<double> weights;
		weights.resize(ts.get_size());

		int size = weights.size();
		fill(weights.begin(),weights.end(),1.0/size);

		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor = weakers.begin();itor != weakers.end();itor++)
		{
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
				itor->first->train_example(ts);
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

			for(int cnt=0;cnt<size;cnt++)
			{
				if (bs[cnt] == false)
				{
					weights[cnt] = weights[cnt] * (itor->second + 0.001) / (1 - itor->second + 0.001);
				}
				else
				{
					weights[cnt] = weights[cnt] *(1 - itor->second + 0.001)/(itor->second + 0.001);
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

	virtual int get_output_index(const vector<double> & input )
	{
		double d_sign = 0.0;

		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor = weakers.begin();itor != weakers.end();itor++)
		{
			try
			{
				int idx = itor->first->get_output_index(input);
				d_sign += ((idx==1)?-1:1) * log((1 - itor->second + 0.001)/(itor->second + 0.001));
			}
			catch (cls_exception &exc)
			{
				try
				{
					int idx = get_index_for_output(itor->first->get_output_data(input));
					d_sign += ((idx==1)?-1:1) * log((1 - itor->second + 0.001)/(itor->second + 0.001));
				}
				catch (cls_exception &exc)
				{
					cout<<exc.display()<<endl;
				}
			}
		}

		if (d_sign >= 0)
			return 0;
		else
			return 1;
	}
};
