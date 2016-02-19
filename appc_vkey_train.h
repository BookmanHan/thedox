#include "lib_import.h"

class appc_vkey_train
{
protected:
	vector<cls_ann_network_bilayer<a_sigmod>* > clser;
	vector<cls_training_set*> ts;
	vector<vector<double> > features;

public:
	appc_vkey_train()
		:ts(4),
		 clser(4)
	{
		vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor;
		for(itor=clser.begin();itor!=clser.end();itor++)
		{
			(*itor) = new cls_ann_network_bilayer<a_sigmod>(32,23,16);
			(*itor)->connect_all();
		}

		vector<cls_training_set*>::iterator itor_ts;
		for(itor_ts=ts.begin();itor_ts!=ts.end();itor_ts++)
		{
			(*itor_ts) = new cls_training_set(32,16);
		}
	}

	~appc_vkey_train()
	{
		vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor;
		for(itor=clser.begin();itor!=clser.end();itor++)
		{
			delete (*itor);
		}

		vector<cls_training_set*>::iterator itor_ts;
		for(itor_ts=ts.begin();itor_ts!=ts.end();itor_ts++)
		{
			delete (*itor_ts);
		}
	}

	void get_feature(cls_image_ot<unsigned>& img,vector<double>& d_in)
	{
		img = img.trans_bi_stretch(img.tools_get_real_rect().dilate(1),cls_rect(0,0,64,32));
		img.filter_threshod(127);
		img.feature_grids_percent(d_in,8,8);
	}

	void make_training_set(const string& str_base,int per_num)
	{
		vector<cls_training_set*>::iterator itor_ts;
		for(itor_ts=ts.begin();itor_ts!=ts.end();itor_ts++)
		{
			delete (*itor_ts);
			(*itor_ts) = new cls_training_set(32,16);
		}

		features.clear();

		for(int cnt=0;cnt<62;cnt++)
		{
			vector<double> d_out(16);
			d_out[cnt%16] = 1;

			for(int per=1;per<=per_num;per++)
			{
				string str_fn = str_base + cnt + "_" + per + ".bmp";
				cls_image_ot<unsigned> img;
				a_load(img,str_fn);

				vector<double> d_in;
				get_feature(img,d_in);

				(ts[cnt/16])->add_io_pair(d_in,d_out);

				features.push_back(d_in);
			}
		}
	}

	void train()
	{
		cout<<"training loaded..."<<endl;

		int n_times = 100;
		while(n_times--)
		{
			cout<<n_times<<" finished..."<<endl;

			vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor_clser;
			vector<cls_training_set*>::iterator itor_ts;

			for(itor_clser=clser.begin(),itor_ts=ts.begin();
				itor_clser!=clser.end();
				itor_clser++,itor_ts++)
			{
				(*itor_clser)->train_example(*(*itor_ts));
			}
		}

		cout<<"trainning ends..."<<endl;
	}
 
	void save(string str)
	{
		ofstream fout(str.c_str(),ios::binary);
		
		vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor;
		for(itor=clser.begin();itor!=clser.end();itor++)
		{
			(*itor)->save(fout);
		}

		fout.close();
	}

	void load(string str)
	{
		ifstream fout(str.c_str(),ios::binary);
		
		vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor;
		for(itor=clser.begin();itor!=clser.end();itor++)
		{
			(*itor)->load(fout);
		}

		fout.close();
	}

	double get_dist_for_feature(vector<double>&src, vector<double>&des)
	{
		double d_re = 0.0;
		double d_src = 0.0;
		double d_des = 0.0;

		vector<double>::iterator itor_src,itor_des;
		for(itor_src = src.begin(),itor_des=des.begin();
			itor_src!= src.end() && itor_des != des.end();
			itor_des++,itor_src++)
		{
			d_re += (*itor_des - *itor_src) * (*itor_des - *itor_src);
			d_des += (*itor_des)*(*itor_des);
			d_src += (*itor_src)*(*itor_src);
		}

		return d_re*d_re/d_src/d_des;
	}

	string recongnition(cls_image_ot<unsigned> &img)
	{
		img.filter_threshod(193);

		for(int i=1;i<img.get_width()-1;i++)
		{
			for(int j=1;j<img.get_height()-1;j++)
			{
				int sum = 
					img.get_binary(i-1,j-1) + img.get_binary(i-1,j) + img.get_binary(i-1,j+1)
					+
					img.get_binary(i,j-1) + img.get_binary(i,j) + img.get_binary(i,j+1)
					+
					img.get_binary(i+1,j-1) + img.get_binary(i+1,j) + img.get_binary(i+1,j+1);

				if (sum <= 1)
				{
					img.set_gray(i,j,255);
				}
			}
		}

		vector<cls_rect> rect;
		vector<int> num;
		img.tools_except_border(255);
		img.tools_mark_connection_region(rect,num);

		int size = rect.size();
		for(int i=0;i<size;i++)
		{
			if (num[i] <= 5)
				continue;

			for(int j=0;j<size;j++)
			{
				if (num[j] <= 5)
					continue;

				if (labs(rect[i].right - rect[j].left) <= 1 && labs(rect[i].top - rect[j].top) <= 2)
				{
					rect.push_back(cls_rect(min(rect[i].top,rect[j].top),
						rect[i].left,max(rect[i].bottom,rect[j].bottom),
						rect[j].right));
					num.push_back(num[i] + num[j]);
				}
			}
		}

		pair<int,char> c_out[4];
		for(int cnt_d=0;cnt_d<4;cnt_d++)
		{
			int cnt_arr = max_element(num.begin(),num.end()) - num.begin();
			num[cnt_arr] = -1;

			rect[cnt_arr].top -= 4;
			rect[cnt_arr].left -= 2;
			rect[cnt_arr].right += 2;
			rect[cnt_arr].bottom += 2;

			c_out[cnt_d].first = rect[cnt_arr].left;

			cls_image_ot<unsigned> img_done = img.tools_part(rect[cnt_arr]);

			int out = recongnition_single_char(img_done);
			if (out < 10)
			{
				c_out[cnt_d].second = char('0' + out);
			}
			else if (out < 36)
			{
				c_out[cnt_d].second = char('a' + out - 10);
			}
			else
			{
				c_out[cnt_d].second = char('A' + out - 36);
			}
		}

		sort(c_out,c_out+4);

		string str_re;
		for(int cnt=0;cnt<4;cnt++)
		{
			str_re += c_out[cnt].second;
		}

		return str_re;
	}

	int recongnition_single_char(cls_image_ot<unsigned> &img)
	{
		int ar_weighted[] = {28,30,31,32,33,54,56,57,58,59,61};

		vector<double> d_in;
		get_feature(img,d_in);

		vector<int> v_in;
		vector<cls_ann_network_bilayer<a_sigmod>* >::iterator itor;
		for(itor=clser.begin();itor!=clser.end();itor++)
		{
			int num_clser = itor - clser.begin();

			vector<double> d_out = (*itor)->get_output_data(d_in);
			v_in.push_back(num_clser*16 +(max_element(d_out.begin(),d_out.end()) - d_out.begin()));
			d_out[v_in.back() - num_clser*16] = -1;
			v_in.push_back(num_clser*16 +(max_element(d_out.begin(),d_out.end()) - d_out.begin()));
			if (d_out[v_in.back() - num_clser*16] < 0.5)
			{
				d_out.pop_back();
			}
		}

		double d_min = inf;
		int out = -1;

		for(int cnt=0;cnt<v_in.size();cnt++)
		{
			int num = v_in[cnt];

			if (num >= 62)
				continue;

			double ar_min[5];
			for(int i=0;i<5;i++)
			{
				ar_min[i] = get_dist_for_feature(d_in,features[num*5+i]);
			}

			double d_cur = *min_element(ar_min,ar_min+5);

			if (find(ar_weighted,ar_weighted+sizeof(ar_weighted),num) != ar_weighted + sizeof(ar_weighted))
			{
				d_cur *= 1.5;
			}

			if (d_cur < d_min)
			{
				d_min = d_cur;
				out = num;
			}
		}

		return out;
	}
};