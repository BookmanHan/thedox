#include "lib_import.h"

struct a_less_hier
{
	bool operator()(const list<pair<int,vector<double> > >::iterator& src,
				 const list<pair<int,vector<double> > >::iterator& des) const 
	{
		//TODO:list的迭代器比较，速度会更快。
		if (src->first - des->first < 0)
			return true;
		else if (src->first - des->first > 0)
			return false;
		else if (src->second < des->second)
			return true;
		else
			return false;
	}

};

struct a_less_hier_pair
{
	bool operator()(const pair<double,pair<list<pair<int,vector<double> > >::iterator,list<pair<int,vector<double> > >::iterator> > & src,
		const pair<double,pair<list<pair<int,vector<double> > >::iterator,list<pair<int,vector<double> > >::iterator> > & des) const 
	{
		if (src.first - des.first > 0)
			return true;
		else
			return false;
	}

};

template<typename FUNC>
class cls_cluster_hierarchical
	:public cls_cluster
{
private:
	int n_pat;

public:
	cls_cluster_hierarchical(int num_pat)
		:n_pat(num_pat)
	{
		;
	}

	virtual vector<int> train_example( cls_training_set & ts ) 
	{
		FUNC func;

		list<pair<int,vector<double> > > centers;
		vector<int> re(ts.get_size());
		set<list<pair<int,vector<double> > >::iterator,a_less_hier> s_invalide;

		ts.initialize();
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,in,out);
			centers.push_back(make_pair(1,in));
		}

		//set<list<pair<int,vector<double> > >::iterator,a_less_hier> set_used;
		//list<pair<int,vector<double> > >::iterator itor;
		//for(itor=centers.begin();itor!=centers.end();itor++)
		//{
		//	set_used.insert(itor);
		//}

		priority_queue<pair<double,pair<list<pair<int,vector<double> > >::iterator,list<pair<int,vector<double> > >::iterator> > 
			,vector<pair<double,pair<list<pair<int,vector<double> > >::iterator,list<pair<int,vector<double> > >::iterator> > >,
			 a_less_hier_pair> pq;

		while(centers.size() > n_pat)
		{
			list<pair<int,vector<double> > >::iterator itor_outter,itor_inner;

			for(itor_outter=centers.begin();itor_outter!=centers.end();itor_outter++)
			{
				for(itor_inner=centers.begin();itor_inner!=centers.end();itor_inner++)
				{
					if (itor_inner == itor_outter)
						continue;

					pq.push(make_pair(func(itor_inner->second,itor_outter->second),
						make_pair(itor_inner,itor_outter)));
				}
			}

			list<pair<int,vector<double> > >::iterator itor_src,itor_des;

			while(true && !pq.empty())
			{
				itor_src = pq.top().second.first;
				itor_des = pq.top().second.second;
				
				pq.pop();

				if (s_invalide.find(itor_src) != s_invalide.end())
				{
					continue;
				}
				else if ( s_invalide.find(itor_des) != s_invalide.end())
				{
					continue;
				}
				else
				{
					break;
				}
			}

			if (pq.empty())
				break;


			int size = itor_src->second.size();
			vector<double> new_center(size);
			
			int num = itor_src->first + itor_des->first;
			vector<double>::iterator itor,i_src,i_des;
			for(itor=new_center.begin(),i_src = itor_src->second.begin(),i_des = itor_des->second.begin()
				;itor!=new_center.end();
				itor++,i_src++,i_des++)
			{
				*itor = (*i_src)*(itor_src->first) + (*i_des)*(itor_src->first);
				*itor /= num;
			}

			centers.push_back(make_pair(itor_src->first + itor_des->first,new_center));
			//set_used.erase(set_used.find(itor_src));
			//set_used.erase(set_used.find(itor_des));
			centers.erase(itor_src);
			centers.erase(itor_des);

			s_invalide.insert(itor_src);
			s_invalide.insert(itor_des);

			//set_used.insert(--centers.end());
		}

		ts.initialize();
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);

			double d_min_dist = inf;
			int cnt_min_dist = -1;

			int cnt = 0;
			list<pair<int,vector<double> > >::iterator itor;
			for(cnt=0,itor=centers.begin();
				itor!=centers.end();
				cnt++,itor++)
			{
				double d_cur_dist = func(itor->second,a);

				if (d_cur_dist < d_min_dist)
				{
					d_min_dist = d_cur_dist;
					cnt_min_dist = cnt;
				}
			}

			re[ts.get_count()-1] = cnt_min_dist;
		}

		return re;
	}
};