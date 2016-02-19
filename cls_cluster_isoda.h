#include "lib_import.h"

template < typename FUNC>
class cls_cluster_isoda
	:public cls_cluster
{
protected:
	int pat;
	int pat_init;
	int least_num_for_pat;
	double sigma_upper_limit;
	double least_dist_between_two;
	int merge_max_each_time;
	int n_times;
	double d_new_rate;

public:
	cls_cluster_isoda(int a_pat,int a_init_pat,
					  int a_least_num_for_pat,
					  double a_sigma_upper_limit,
					  double a_least_dist_between_two,
					  int a_merge,
					  double a_new_rate,
					  int a_times)
					  :
					  pat(a_pat),
					  pat_init(a_init_pat),
					  least_num_for_pat(a_least_num_for_pat),
					  sigma_upper_limit(a_sigma_upper_limit),
					  least_dist_between_two(a_least_dist_between_two),
					  merge_max_each_time(a_merge),
					  n_times(a_times),
					  d_new_rate(a_new_rate)
					  {
						  ;
					  }

	virtual vector<int> train_example( cls_training_set & ts ) 
	{
		FUNC func;

		vector<pair<int,vector<double> > > centers;
		vector<int> re(ts.get_size());

		ts.initialize();
		for(int cnt=0;cnt<pat_init;cnt++)
		{
			if (ts.epoch_over())
				break;

			get_next_pair_for_ts(ts,a,b);
			centers.push_back(make_pair(0,a));
		}

		while(n_times--)
		{
			bool b_out = true;
			while(b_out)
			{
				vector<pair<int,vector<double> > >::iterator itor_init;
				for(itor_init=centers.begin();itor_init!=centers.end();itor_init++)
				{
					itor_init->first = 0;
				}

				b_out = false;

				ts.initialize();
				while(!ts.epoch_over())
				{
					get_next_pair_for_ts(ts,a,b);

					double d_min_dist = inf;
					int cnt_min = -1;

					vector<pair<int,vector<double> > >::iterator itor,itor_min;

					int cnt=0;
					for(cnt=0,itor=centers.begin();
						itor!=centers.end();
						cnt++,itor++)
					{
						double d_cur_dist = func(a,itor->second);

						if (d_cur_dist < d_min_dist)
						{
							d_min_dist = d_cur_dist;
							itor_min = itor;
							cnt_min = cnt;
						}
					}

					re[ts.get_count()-1] = cnt_min;

					itor_min->first++;
					vector<double>::iterator itor_in,itor_here;

					for(itor_here=itor_min->second.begin(),itor_in=a.begin();
						itor_here != itor_min->second.end();
						itor_here++,itor_in++)
					{
						*itor_here = (*itor_here)*(itor_min->first-1) + *itor_in;
						*itor_here /=itor_min->first;
					}
				}

				vector<pair<int,vector<double> > >::iterator itor_n,itor_tmp;
				for(int cnt=0;cnt<centers.size();cnt++)
				{
					if (centers[cnt].first < least_num_for_pat)
					{
						centers.erase(centers.begin() + cnt);
						cnt--;
						b_out = true;
					}
				}
			}

			vector<double> d_ave_dist(centers.size());
			fill(d_ave_dist.begin(),d_ave_dist.end(),0.0);

			double d_total_dist = 0.0;

			ts.initialize();
			while(!ts.epoch_over())
			{
				get_next_pair_for_ts(ts,a,b);

				int cls_no = re[ts.get_count()-1];
				d_ave_dist[cls_no] += func(a,centers[cls_no].second);
			}

			vector<pair<int,vector<double> > >::iterator itor_c;
			vector<double>::iterator itor_dist;
			for(itor_c=centers.begin(),itor_dist=d_ave_dist.begin()
				;itor_c!=centers.end();
				itor_dist++,itor_c++)
			{
				d_total_dist += *itor_dist;
				*itor_dist /= itor_c->first;
			}

			d_total_dist /= ts.get_size();

			double d_between = 0;
			int size = centers.size(); 
			if (size <= pat/2 || (size > pat/2 && size < 2*pat && n_times%2 == 1))
			{
				vector<double> v_sigmod(centers.size());
				ts.initialize();
				while(!ts.epoch_over())
				{
					get_next_pair_for_ts(ts,a,b);

					int cls_no = re[ts.get_count()-1];

					double d_tmp = func(a,centers[cls_no].second);
					v_sigmod[cls_no] += d_tmp * d_tmp;
				}

				vector<pair<int,vector<double> > >::iterator itor_ce;
				vector<double>::iterator itor_dist;
				for(itor_ce=centers.begin(),itor_dist=v_sigmod.begin()
					;itor_ce!=centers.end();
					itor_dist++,itor_ce++)
				{
					*itor_dist /= itor_ce->first*itor_ce->first;
					*itor_dist = sqrt(*itor_dist);
				}

				vector<double>::iterator d_sen = max_element(v_sigmod.begin(),v_sigmod.end());
				int num = d_sen - v_sigmod.begin();
				int num_feautre = centers[num].second.size();

				if (d_ave_dist[num] >= 0.95*d_total_dist &&
					centers[num].first > 2*(least_num_for_pat + 1)
					)
				{
					vector<double>::iterator itor;
					vector<double> v_pos;
					vector<double> v_neg;

					for(itor=centers[num].second.begin();
						itor!=centers[num].second.end();
						itor++)
					{
						v_pos.push_back(*itor + v_sigmod[num]*d_new_rate);
						v_neg.push_back(*itor - v_sigmod[num]*d_new_rate);
					}

					centers.push_back(make_pair(1,v_pos));
					centers.push_back(make_pair(1,v_neg));
					centers.erase(centers.begin() + num);
				}

				continue;
			}
			else if (size >= pat*2  || (size > pat/2 && size < 2*pat && n_times%2 == 0))
			{
				priority_queue<pair<double,pair<int,int> >,vector<pair<double,pair<int,int> > >,less<pair<double,pair<int,int> > > > pq;

				int size_cen=centers.size();
				for(int i=0;i<size_cen;i++)
				{
					for(int j=0;j<size_cen;j++)
					{
						pq.push(make_pair(func(centers[i].second,centers[j].second),
							make_pair(i,j)));
					}
				}

				int size_bs = centers.size();
				vector<bool> bs(centers.size());
				fill(bs.begin(),bs.end(),false);

				for(int cnt=0;cnt<merge_max_each_time;cnt++)
				{
					int src = pq.top().second.first;
					int des = pq.top().second.second;

					pq.pop();

					if (bs[src]== true || bs[des] == true || src == des)
						continue;

					bs[src] = true;
					bs[des] = true;

					vector<double> v_new;
					vector<double>::iterator itor_src,itor_des;
					int size_src = centers[src].first;
					int size_des = centers[des].first;

					for(itor_src=centers[src].second.begin(),itor_des = centers[des].second.begin();
						itor_src != centers[src].second.end();
						itor_src++,itor_des++)
					{
						v_new.push_back((size_src*(*itor_src) + size_des*(*itor_des))/(size_src+size_des));
					}

					centers.push_back(make_pair(size_src+size_des,v_new));
				}

				int num_remove = 0;
				int here = centers.size() - 1;
				int pos = size_bs - 1;
				while(pos >= 0)
				{
					while(pos >= 0 && bs[pos] == false)
						--pos;

					if (pos < 0 || here < 0)
						break;

					num_remove++;
					swap(centers[here],centers[pos]);
					--here;
					--pos;
				}

				centers.erase(centers.begin() + (centers.size() - num_remove),centers.end());
			}
		}

		return re;
	}
};