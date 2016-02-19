#include "lib_import.h"

class appc_recog_chinese_pic
{
private:
	const static int n_start = 19968;
	const static int n_end = 40864;

private:
	string str_path;
	vector<cls_image_ot<unsigned int> > v_img;
	vector<int> v_num;
	map<int,int> map_num;
	vector<string> v_str;
	vector<cls_svm_c_svc*> clser;
	vector<cls_training_set*> v_ts;

public:
	int num_train_pic;
	int words_per_pic;

public:
	appc_recog_chinese_pic(string arg_path)
	{
		num_train_pic = 1;
		words_per_pic = 100;

		str_path = arg_path;

		r_bpnn.resize(num_train_pic);
		for(int cnt=0;cnt<num_train_pic;cnt++)
		{
			clser[cnt] = new cls_svm_c_svc(en_sigmoid,0.1,0.1);
		}

		v_img.clear();
		v_str.clear();
		v_num.clear();
	}

	void pre_do()
	{
		for(int cnt=1;cnt<=num_train_pic;cnt++)
		{
			cls_image_ot<unsigned int> img_tmp_all;
			a_load(img_tmp_all,str_path + cnt + ".bmp");

			for(int j=0;j<10;j++)
			{
				for(int i=0;i<10;i++)
				{
					cls_image_ot<unsigned> img_that = img_tmp_all.tools_part(cls_rect(j*64,i*64,j*64+64,i*64+64));
					img_that = img_that.trans_bi_stretch(img_that.tools_get_real_rect(),cls_rect(0,0,64,64));
					img_that.filter_threshod(200);
					v_img.push_back(img_that);
				}
			}
		}

		ifstream fin((str_path + "num.txt").c_str());
		while(!fin.eof())
		{
			int n_tmp;
			fin> >n_tmp;

			map_num.insert(make_pair(n_tmp,v_num.size()));
			v_num.push_back(n_tmp);
		}
		fin.close();
	}

	vector<double>& encode_for_bpnn(int num,vector<double>&re)
	{
		for(int cnt=0;cnt<8;cnt++)
		{
			if ((num & (1<<cnt)) != 0)
				re.push_back(1.0);
			else
				re.push_back(0);
		}

		return re;
	}

	int decode_for_bpnn(vector<double>&re)
	{
		int res = 0;
		for(int cnt=0;cnt<8;cnt++)
		{
			if (re[cnt] > 0.5)
			{
				res |= (1<<cnt);
			}
		}

		return res;
	}

	/*
	vector<double>& encode_for_bpnn(int num,vector<double>&re)
	{
		re.resize(100);
		fill(re.begin(),re.end(),0);
		re[num] = 1;

		return re;
	}

	int decode_for_bpnn(vector<double>&re)
	{
		int n_min = 0;
		for(int cnt=0;cnt<100;cnt++)
		{
			if (re[n_min] > re[cnt])
			{
				n_min = cnt;
			}
		}

		return n_min;
	}
	*/

	static void get_feature_for_imgae(cls_image_ot<unsigned>& re,vector<double>& src)
	{
		re.feature_cross_full(src,re.get_width()/8,re.get_height()/8);
		re.feature_grids_percent(src,4,4);
	}

	static double get_dist(vector<double>& src,vector<double>&des)
	{
		double d_sum = 0;
		for(int cnt=0;cnt<min(src.size(),des.size());cnt++)
		{
			d_sum += pow(src[cnt] - des[cnt],2);
		}

		return d_sum;
	}

	void train(int n_times)
	{
		int cnt_num = -1;
		for(int cnt=0;cnt<v_img.size();cnt++)
		{
			if (cnt%words_per_pic == 0)
			{
				v_ts.push_back(new cls_training_set(32,1));
				cnt_num++;
			}

			vector<double> v_src;
			vector<double> v_des;
			get_feature_for_imgae(v_img[cnt],v_src);
			v_ts[cnt_num]->add_io_pair_with_map(v_src,v_des);
		}

		for(int cnt=0;cnt<num_train_pic;cnt++)
		{
			clser[cnt]->train(v_ts[cnt]);
		}
	}

	int simulate(cls_image_ot<unsigned> & img_re)
	{
		vector<int> v_re;
		vector<double> data_src;

		get_feature_for_imgae(img_re,data_src);

		wstring wcs_output;
		for(int cnt=0;cnt<num_train_pic;cnt++)
		{
			vector<double> data_des;
			data_des =
			int re = decode_for_bpnn(data_des);
			v_re.push_back(v_num[words_per_pic*cnt + re]);

			cout<<re<<endl;
		}


		int min_cnt = -1;
		for(int cnt=0;cnt<v_re.size();cnt++)
		{
			int n_bound = map_num[v_re[cnt]];
			if (n_bound >= 0 && n_bound < v_img.size())
			{
				min_cnt = cnt;
				break;
			}
		}

		if (min_cnt == -1)
			return -1;

		for(int cnt=0;cnt<v_re.size();cnt++)
		{
			vector<double> data_des;
			vector<double> data_min;

			if (map_num[v_re[cnt]] >= 0 && map_num[v_re[cnt]] < v_img.size())
			{
				get_feature_for_imgae(v_img[map_num[v_re[cnt]]],data_des);
				get_feature_for_imgae(v_img[map_num[v_re[min_cnt]]],data_min);

				if (get_dist(data_src,data_des)
					<
					get_dist(data_src,data_min))
				{
					min_cnt = cnt;
				}
			}
		}

		return v_re[min_cnt];
	}

	wstring& recognition(list<cls_image_ot<unsigned int> > &lst_img,wstring & strd)
	{
		strd.clear();

		/*
		lst_img.clear();
		srand(time(null));
		for(int cnt=0;cnt<20;cnt++)
		{
			int n_word = rand()%v_img.size();
			cout<<a_trans_wchar_to_wstr(v_num[n_word]);
			lst_img.push_back(v_img[n_word]);
		}
		cout<<endl;
		*/


		lst_img.clear();
		srand(time(null));
		for(int cnt=0;cnt<7;cnt++)
		{
			lst_img.push_back(v_img[cnt]);
		}
		cout<<endl;


		/*
		for(int cnt=10;cnt<20;cnt++)
		{
			vector<double> v_d;
			get_feature_for_imgae(v_img[cnt],v_d);
			a_vector_output(cout,v_d);
			cout<<endl;
		}
		*/

		int cnt= 0;
		list<cls_image_ot<unsigned int> >::iterator itor = lst_img.begin();
		while(itor != lst_img.end())
		{
			if (itor->get_width() * itor->get_height() <= 32)
			{
				itor++;
				continue;
			}

			vector<double> v_d;
			get_feature_for_imgae(*itor,v_d);
			a_vector_output(cout,v_d);
			cout<<endl;

			//a_save(*itor,string("D:\\trys\\recog\\text\\") + cnt + "_org.bmp");

			*itor= itor->trans_bi_stretch(itor->tools_get_real_rect(),cls_rect(0,0,64,64));
			itor->filter_threshod(200);

			//a_save(*itor,string("D:\\trys\\recog\\text\\") + cnt + "_changed.bmp");

			v_d.clear();
			get_feature_for_imgae(*itor,v_d);
			a_vector_output(cout,v_d);
			cout<<endl;

			wchar_t wc = simulate(*itor);

			if (wc != -1)
				strd += a_trans_wchar_to_wstr(wc);

			itor++;
			cnt++;
		}

		return strd;
	}
};
