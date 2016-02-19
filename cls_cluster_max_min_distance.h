#include "lib_import.h"

template<typename FUNC>
class cls_cluster_max_min_distance
	:public cls_cluster
{
protected:
	double rate;
	bool fixed_center;

public:
	cls_cluster_max_min_distance(double d_rate,bool b_fixed_center = false)
		:rate(d_rate),
		 fixed_center(b_fixed_center)
	{
		;
	}

	virtual vector<int> train_example( cls_training_set & ts ) 
	{
		vector<pair<int,vector<double> > > centers;
		vector<int> re(ts.get_size());
		vector<bool> bs(ts.get_size());

		fill(bs.begin(),bs.end(),false);

		FUNC func;

		ts.initialize();
		get_next_pair_for_ts(ts,in,out);
		centers.push_back(make_pair(1,in));
		re[1] = 0;
		bs[1] = true;

		double d_max_dist_f_s = 0;

		vector<double>* real_second;
		int second_cnt;

		ts.initialize();
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,in,out);

			double d_cur_dist = func(in,centers.back().second);
			if (d_cur_dist > d_max_dist_f_s)
			{
				d_max_dist_f_s = d_cur_dist;
				second_cnt = ts.get_count()-1;
				real_second = &in;
			}
		}

		bs[second_cnt] = true;
		re[second_cnt] = 1;
		centers.push_back(make_pair(1,*real_second));

		bool b_change = true;
		while(b_change)
		{
			b_change = false;

			double d_max_dist = 0.0;
			int cnt = -1;
			vector<double>* max_in;

			ts.initialize();
			while(!ts.epoch_over())
			{
				get_next_pair_for_ts(ts,in,out);

				if (bs[ts.get_count() - 1] == true)
				{
					continue;
				}

				double d_min_dist = inf;
				
				vector<pair<int,vector<double> > >::iterator itor,itor_reduce;
				for(itor=centers.begin();itor!=centers.end();itor++)
				{
					double d_cur_dist = func(in,itor->second);
					
					if (d_cur_dist < d_min_dist)
					{
						d_min_dist = d_cur_dist;
						itor_reduce = itor;
					}
				}

				if (d_min_dist > d_max_dist)
				{
					d_max_dist = d_min_dist;
					cnt = ts.get_count()-1;
					max_in = &in;
				}
			}

			if (d_max_dist > d_max_dist_f_s * rate)
			{
				centers.push_back(make_pair(1,*max_in));
				re[cnt] = centers.size()-1;
				b_change = true;
				bs[cnt] = true;
			}
		}

		ts.initialize();
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,in,out);

			if (bs[ts.get_count()-1] == true)
			{
				continue;
			}

			double d_min_dist = inf;
			vector<pair<int,vector<double> > >::iterator itor,itor_pos;
			for(itor=centers.begin();itor!=centers.end();itor++)
			{
				double d_cur_dist = func(itor->second,in);
				if (d_cur_dist < d_min_dist)
				{
					d_min_dist = d_cur_dist;
					itor_pos = itor;
				}
			}

			re[ts.get_count()-1] = itor_pos - centers.begin();

			if (fixed_center == false)
			{
				vector<double>::iterator itor,itor_in;
				for(itor=itor_pos->second.begin(),itor_in = in.begin();
					itor!=itor_pos->second.end();
					itor++,itor_in++)
				{
					*itor = itor_pos->first * (*itor) + *itor_in;
				}
			}
		}

		return re;
	}
};