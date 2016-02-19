#include "lib_import.h"

template<typename FUNC>
class cls_cluster_threshod
	:public cls_cluster
{
protected:
	double threshod;

public:
	cls_cluster_threshod(double d_threshod)
		:threshod(d_threshod),

	{
		;
	}

	virtual vector<int> train_example( cls_training_set & ts ) 
	{
		vector<pair<int,vector<double> > > centers;

		vector<int> re(ts.get_size());

		FUNC func;

		ts.initialize();
		get_next_pair_for_ts(ts,in,out);
		centers.push_back(make_pair(1,in));

		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,in,out);

			double d_min_dis = inf;
			vector<pair<int,vector<double> > >::iterator itor,itor_beg;
			for(itor=centers.begin();itor != centers.end();itor++)
			{
				double d_cur_dis = func(in,itor->second);
				if ( d_cur_dis < d_min_dis )
				{
					d_min_dis = d_cur_dis;
					itor_beg = itor;
				}
			}

			if (d_min_dis > threshod)
			{
				centers.push_back(make_pair(1,in));
				re.push_back(centers.size()-1);
			}
			else
			{
				vector<double>::iterator itor,itor_in;
				for(itor=itor_beg->second.begin(),itor_in=in.begin();
					itor!=itor_beg->second.end();
					itor++,itor_in++)
				{
					*itor = (*itor) * (itor_beg->first) + *itor_in;
					*itor /= itor_beg->first;
				}

				itor_beg->first++;
				re.push_back(itor_beg - centers.begin());
			}
		}

		return re;
	}
};