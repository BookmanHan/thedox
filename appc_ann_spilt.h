#include "lib_import.h"

class appc_classifier_word_segment
{
private:
	int total_status;
	cls_training_set ts;
	cls_svm_c_svc svm;

	static bool cmp_wstr(const wstring & a,const wstring & b)
	{
		wstring::const_iterator i_a,i_b;
		for(i_a = a.begin(),i_b = b.begin();i_a < a.end() && i_b < b.end(); i_a++,i_b++)
		{
			if (*i_a < *i_b)
				return true;
			else if (*i_a > * i_b)
				return false;
		}

		if(i_b == b.end() && i_a != a.end())
			return true;
		else
			return false;
	}

protected:
	map<pair<wchar_t,int>,int> statuses;
	vector<wstring> words;

	void make_status_sub(int pre,int level,vector<wstring>::iterator beg,vector<wstring>::iterator end)
	{
		if (beg == words.end() || end == words.end())
			return;

		wchar_t w = (*beg)[level - 1];

		if (beg + 1>= end)
		{
			if (level == beg->length())
			{
				statuses[make_pair(w,pre)] = 0;
			}
			else
			{
				int p = pre;
				for(wstring::iterator i=beg->begin() + level - 1;i < beg->end() - 1;i++)
				{
					total_status++;
					statuses[make_pair(*i,p)] = total_status;

					p = total_status;
				}

				total_status++;
				statuses[make_pair(*beg->rbegin(),p)] = 0;
			}

			return;
		}

		++total_status;
		statuses[make_pair(w,pre)] = total_status;
		int p_s = total_status;

		vector<wstring>::iterator itor = beg;
		while(itor < end)
		{
			if (itor->length() >= level + 1)
			{
				wstring str_find = itor->substr(0,level + 1);

				make_status_sub
				(
					p_s,
					level + 1,
					itor,
					upper_bound(itor,end,str_find,cmp_wstr)
				);

				itor = upper_bound(beg,end,str_find,cmp_wstr);
			}
			else
			{
				int p = p_s;
				for(wstring::iterator i=itor->begin() + level;i < itor->end() - 1;i++)
				{
					total_status++;
					statuses[make_pair(*i,p)] = total_status;

					p = total_status;
				}

				total_status++;
				statuses[make_pair(*itor->rbegin(),p)] = 0;
				itor++;
			}
		}
	}

public:
	appc_classifier_word_segment(wstring str_filename)
		:ts(28,13),svm(en_poly,4,0.5,0.5)
	{
		wifstream fin(str_filename.c_str());
		fin.imbue(locale("chs"));

		while(!fin.eof())
		{
			wstring str_cur;
			getline(fin,str_cur);

			int pos_word = str_cur.find('\t');

			if (pos_word != 1)
				words.push_back(str_cur.substr(0,pos_word));
		}

		fin.close();
	}

	void save_net(string str)
	{
		ofstream fout(str,ios::binary | ios::binary);
		svm.save(fout);
		fout.close();
	}

	void load_net(string str)
	{
		ifstream fin(str,ios::binary  | ios::binary );
		svm.load(fin);
		fin.close();
	}

	void make_status()
	{
		sort(words.begin(),words.end(),cmp_wstr);
		total_status = 2;

		vector<wstring>::iterator itorl = lower_bound(words.begin(),words.end(),L"°é",cmp_wstr);
		vector<wstring>::iterator itoru = upper_bound(words.begin(),words.end(),L"°é",cmp_wstr);
		
		vector<wstring>::iterator itor = words.begin();
		while(itor != words.end())
		{
			wstring str_find;
			str_find = str_find + (*itor)[0];

			make_status_sub
			(
				1,
				1,
				itor,
				upper_bound(itor,words.end(),str_find,cmp_wstr)
			);

			itor = upper_bound(words.begin(),words.end(),str_find,cmp_wstr);
		}

		make_training_set();
	}

	static void init_feature(vector<double>& v_in,vector<double>& v_out)
	{
		v_in.resize(28);
		v_out.resize(13);
	}

	static void build_feature_in(pair<wchar_t,int> src,vector<double>& v_re)
	{
		int w = src.first - 19968;
		for(int cnt=0;cnt<15;cnt++)
		{
			v_re[cnt] = w % 2;
			w /= 2;
		}

		w = src.second;
		for(int cnt=0;cnt<13;cnt++)
		{
			v_re[cnt + 15] = w % 2;
			w /= 2;
		}
	}

	static void build_feature_out(int src,vector<double>& v_re)
	{
		int w = src;
		for(int cnt=0;cnt<13;cnt++)
		{
			v_re[cnt] = w % 2;
			w /= 2;
		}
	}

	static int restore_feature_out(vector<double>& v_re)
	{
		int w=0;
		for(int cnt=12;cnt>=0;cnt--)
		{
			w *= 2;
			w += v_re[cnt] > 0.5?1:0;
		}

		return w;
	}


	void make_training_set()
	{
		vector<double> d_in;
		vector<double> d_out;
		init_feature(d_in,d_out);

		map<pair<wchar_t,int>,int>::iterator itor;
		for(itor=statuses.begin();itor != statuses.end();itor++)
		{
			build_feature_in(itor->first,d_in);
			build_feature_out(itor->second,d_out);

			ts.add_io_pair_with_map(d_in,d_out);
		}
	}

	void train_network()
	{
		svm.train(ts);
	}

	vector<wstring::iterator>& word_segment(wstring& str,vector<wstring::iterator>& vs)
	{
		vector<double> d_in,dout;
		init_feature(d_in,dout);

		int status = 1;
		for(wstring::iterator itor=str.begin();itor != str.end();itor++)
		{
			build_feature_in(make_pair(*itor,status),d_in);
			int re = restore_feature_out(ts.get_output_for_id(svm.get_output(d_in)));

			cout<<re<<endl;

			if (re == 0)
			{
				vs.push_back(itor);
				status = 1;
			}
			else
			{
				status = re;
			}
		}

		vs.push_back(str.end());
		return vs;
	}

	vector<wstring::iterator>& word_segment_commmon(wstring& str,vector<wstring::iterator>& vs)
	{
		int status = 1;
		for(wstring::iterator itor=str.begin();itor != str.end();itor++)
		{
			map<pair<wchar_t,int>,int>::iterator i_re = statuses.find(make_pair(*itor,status));

			if (i_re == statuses.end())
			{
				vs.push_back(itor);

				cout<<0<<endl;
				status = 1;
			}
			else if (i_re->second == 0)
			{
				vs.push_back(itor);

				cout<<i_re->second<<endl;
				status = 1;
			}
			else
			{
				cout<<i_re->second<<endl;
				status = i_re->second;
			}
		}
		
		vs.push_back(str.end());
		return vs;
	}
};