#include "lib_import.h"

template<typename FUNC>
class cls_cluster_k_means
	:cls_cluster
{
protected:
	vector<vector<double> > center;
	int n_pat;
	int n_feauture;
	int n_times;

public:
	cls_cluster_k_means(int num_pattern,int num_feature,int max_times = 1000)
		:n_pat(num_pattern),
		 n_feauture(num_feature),
		 center(num_pattern),
		 n_times(max_times)
	{
		vector<vector<double> >::iterator itor;
		for(itor=center.begin();itor!=center.end();itor++)
		{
			itor->resize(n_feauture);
		}
	}

	virtual vector<int> train_example(cls_training_set & ts ) 
	{
		vector<int> re(ts.get_size());

		FUNC func;

		if (ts.get_size() <= n_pat)
		{
			throw cls_exception_not_sense(string(__FUNCTION__) + "() - the dataset is too too too small.\n");
		}

		int cnt = 0;
		ts.initialize();
		while(!ts.epoch_over())
		{
			if (cnt >= n_pat)
				break;

			get_next_pair_for_ts(ts,a,b);
			center[cnt++] = a;
		}

		int num_times = n_times;
		bool b_change = true;
		while(b_change && num_times--)
		{
			b_change = false;

			ts.initialize();
			while(!ts.epoch_over())
			{
				get_next_pair_for_ts(ts,a,b);

				vector<double> d_re(n_pat);
				vector<vector<double> >::iterator itor;
				vector<double>::iterator itor_re;

				for(itor_re=d_re.begin(),itor=center.begin();
					itor!=center.end();
					itor++,itor_re++)
				{
					*itor_re = func(a,*itor);
				}

				int indx = 0;
				for(int cnt=1;cnt<n_pat;cnt++)
				{
					if (d_re[cnt] < d_re[indx])
					{
						indx = cnt;
					}
				}

				if (re[ts.get_count() - 1] != indx)
				{
					re[ts.get_count()-1] = indx;
					b_change = true;
				}
			}

			for(int cnt=0;cnt<n_pat;cnt++)
			{
				fill(center[cnt].begin(),center[cnt].end(),0.0);
			}

			vector<int> num(n_pat);
			fill(num.begin(),num.end(),0);

			const vector<vector<double> > & inputs = ts.get_inputs();
			vector<int>::iterator itor;
			int pos=0;
			for(pos=0,itor=re.begin();
				itor!=re.end();
				pos++,itor++)
			{
				num[*itor]++;

				for(int cnt=0;cnt<n_feauture;cnt++)
				{
					center[*itor][cnt] += inputs[pos][cnt];
				}
			}

			int cnt=0;
			vector<vector<double> >::iterator itor_v;
			for(cnt=0,itor_v=center.begin();
				itor_v!=center.end();
				cnt++,itor_v++)
			{
				if (num[cnt] == 0)
					continue;

				for(int pos=0;pos<n_feauture;pos++)
				{
					itor_v->operator [](pos) /= 1.0 * num[cnt];
				}
			}
		}

		return re;
	}
};