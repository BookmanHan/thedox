#include "lib_import.h"

class appc_recongntion_chinese
{
protected:
	const static int n_start = 19968;
	const static int n_end = 40864;

protected:
	vector<cls_training_set*> v_ts;
	vector<cls_ann_network_bilayer<a_sigmod>*> v_clser;
	vector<cls_image_ot<unsigned>*> v_img;
	vector<cls_image_ot<unsigned>*> v_img_real;
	vector<vector<double> > mat_feature;

	vector<vector<int> > v_num;
	map<int,int> map_code_order;

	int num_res;
	int word_per_pic;
	int num_words_unit;

public:
	appc_recongntion_chinese(string a_path,int num_pic_res,int word_per_pic)
	{
		this->num_res = num_pic_res;
		this->word_per_pic = word_per_pic;

		num_words_unit = num_pic_res*100/word_per_pic + 1;

		v_ts.resize(num_words_unit);
		v_clser.resize(num_words_unit);
		for(int cnt=0;cnt<num_words_unit;cnt++)
		{
			v_ts[cnt] = new cls_training_set(36,word_per_pic);
			v_clser[cnt] = new cls_ann_network_bilayer<a_sigmod>(36,36,word_per_pic);
		}

		v_img.clear();
		v_img_real.clear();

		load_res_to_training_set(a_path);
	}

	void save_net(string str_filename)
	{
		ofstream fout(str_filename.c_str(),ios::binary);

		vector<cls_ann_network_bilayer<a_sigmod>*>::iterator itor;
		for(itor=v_clser.begin();itor < v_clser.end();itor++)
		{
			(*itor)->save(fout);
		}

		int size = mat_feature.size();
		for(int cnt=0;cnt<size;cnt++)
		{
			int isize = mat_feature[cnt].size();
			for(int i=0;i<isize;i++)
			{
				fout.write((char*)&mat_feature[cnt][i],sizeof(double));
			}
		}

		fout.close();
	}

	void load_net(string str_filename)
	{
		ifstream fin(str_filename.c_str(),ios::binary);

		vector<cls_ann_network_bilayer<a_sigmod>*>::iterator itor;
		for(itor=v_clser.begin();itor < v_clser.end();itor++)
		{
			(*itor)->load(fin);
		}

		fin.close();
	}

	void train(int n_times)
	{
		int nums = 0;
		while(n_times--)
		{
			cout<<"loop times:"<<++nums<<endl;

			vector<cls_ann_network_bilayer<a_sigmod>*>::iterator itor;
			vector<cls_training_set*>::iterator itor_ts;
			for(itor = v_clser.begin(),itor_ts = v_ts.begin();
				itor != v_clser.end() && itor_ts != v_ts.end();
				itor++,itor_ts++)
			{
				(*itor)->train_example(*(*itor_ts));
			}

			if (nums%20 == 0)
			{
				double d_err = train_error();
				cout<<"\t error_rate:"<<d_err<<endl;
				if (d_err < 0.01)
					break;
			}
		}
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

	wchar_t recognition(cls_image_ot<unsigned>& src)
	{
		vector<double> v_feature;
		get_feature(&src,v_feature);
		vector<int> v_re;

		vector<vector<int> >::iterator itor_vd;
		vector<cls_ann_network_bilayer<a_sigmod> *>::iterator itor;
		for(itor=v_clser.begin(),itor_vd = v_num.begin()
			;itor!=v_clser.end();
			itor++,itor_vd++)
		{
			vector<double> d_re = (*itor)->get_output(v_feature);

			int cnt = decode(d_re);
			if (cnt < word_per_pic && cnt >= 0)
			{
				v_re.push_back(itor_vd->at(cnt));
			}
		}

		double d_min = inf;
		int indx_min = -1;
		vector<int>::iterator itor_last;
		for(itor_last=v_re.begin();itor_last!=v_re.end();itor_last++)
		{
			//cout<<a_trans_wchar_to_wstr(*itor_last);
			vector<double> d_cur;
			get_feature(v_img_real[map_code_order[*itor_last]],d_cur);

			double d = get_dist_for_feature(v_feature,d_cur);
			if (d < d_min)
			{
				indx_min = *itor_last;
				d_min = d;
			}
		}

		//cout<<endl;
		return indx_min;
	}

	void inner_try()
	{
		int size = v_img_real.size();
		for(int cnt=0;cnt<size;cnt++)
		{
			//cout<<"cnt-"<<cnt%word_per_pic<<":";
			try
			{
				vector<double> v_feature;
				cls_image_ot<unsigned> img_tmp = *v_img[cnt];
				img_tmp = img_tmp.tools_part(img_tmp.tools_get_real_rect());

				get_feature(&img_tmp,v_feature);

				vector<double> d_re = v_clser[cnt/word_per_pic]->get_output(v_feature);
				//cout<<decode(d_re);
			}
			catch (cls_exception &exc)
			{
				;
			}

			//cout<<endl;
		}
	}

	double train_error()
	{
		int size = v_img_real.size();

		int num_corrent = 0;

		for(int cnt=0;cnt<num_words_unit;cnt++)
		{
			for(int i=0;i<word_per_pic;i++)
			{
				if (cnt*word_per_pic + i >= size)
				{
					return 1.0 - num_corrent*1.0/size;
				}

				try
				{
					vector<double> d_re = v_clser[cnt]->get_output(mat_feature[cnt*word_per_pic + i]);
					if (decode(d_re) == i)
						num_corrent++;
				}
				catch (cls_exception &exc)
				{
					;
				}
			}
		}

		return 1.0 - num_corrent*1.0/size;
	}

protected:

	void load_res_to_training_set(string str_path)
	{
		for(int cnt=1;cnt<=num_res;cnt++)
		{
			cls_image_ot<unsigned> img_all;
			a_load(img_all,str_path + cnt + ".bmp");

			for(int j=0;j<10;j++)
			{
				for(int i=0;i<10;i++)
				{
					cls_image_ot<unsigned>* img_new = new cls_image_ot<unsigned>();
					*img_new = img_all.tools_part(cls_rect(j*64,i*64,j*64+64,i*64+64));
					img_new->filter_threshod(200);

					v_img.push_back(img_new);

					cls_image_ot<unsigned>* img_real = new cls_image_ot<unsigned>();
					*img_real = *img_new;
					*img_real = img_real->tools_part(img_real->tools_get_real_rect());
					img_real->filter_threshod(200);

					v_img_real.push_back(img_real);
				}
			}
		}

		int size = v_img.size();

		ifstream fin((str_path + "num.txt").c_str());

		v_num.resize(num_words_unit);
		for(int cnt=0;cnt<num_words_unit;cnt++)
		{
			v_num[cnt].resize(word_per_pic);
			for(int i=0;i<word_per_pic;i++)
			{
				if (cnt*word_per_pic + i >= size)
				{
					fin.close();
					return;
				}

				vector<double> src;
				vector<double> des;
				int num;
				fin>>num;

				v_num[cnt][i] = num;

				map_code_order[num] = cnt*word_per_pic + i;

				des = encode(i);

				get_feature(v_img[cnt*word_per_pic + i],src);
				v_ts[cnt]->add_io_pair_with_map(src,des);

				vector<double> v_feature;
				get_feature(v_img_real[cnt*word_per_pic + i],v_feature);
				mat_feature.push_back(v_feature);
			}
		}

		fin.close();
	}

protected:
	void get_feature(cls_image_ot<unsigned>* re,vector<double>& src)
	{
		if (re->get_width() <=4 || re->get_height() <=4)
			throw cls_exception_not_found(string(__FUNCTION__) + "() - image is too small.\n");

		re->trans_bi_stretch(cls_rect(0,0,64,64));
		re->filter_threshod(200);
		re->feature_grids_percent(src,re->get_width()/6.0,re->get_height()/6.0);

		double max_val = *max_element(src.begin(),src.end());

		vector<double>::iterator itor;
		for(itor=src.begin();itor != src.end();itor++)
		{
			*itor /= max_val;
		}

		//vector<double>::iterator itor;
		//for(itor=src.begin();itor != src.end();itor++)
		//{
		//	if (*itor > 0.5)
		//		*itor = 1.0;
		//	else
		//		*itor = 0.0;
		//}
	}

	int decode(vector<double>& v_in)
	{
		return decode_directive(v_in);

		int re = 0;
		//for(int cnt=0;cnt<7;cnt++)
		//{
		//	if (v_in[cnt] > 0.5)
		//	{
		//		v_in[cnt] = 1.0;
		//	}
		//	else
		//	{
		//		v_in[cnt] = 0.0;
		//	}
		//}

		//int err_vote[7];
		//memset(err_vote,0,sizeof(int)*7);

		//for(int cnt=0;cnt<7;cnt++)
		//{
		//	unsigned b_1 = v_in[cnt] > 0.5 ? 1 : 0;
		//	unsigned b_2 = v_in[(cnt+1)%7] > 0.5 ? 1 : 0;

		//	unsigned b_should = b_1 ^ b_2;
		//	unsigned b_real = 0;

		//	if (v_in[cnt] > 0.5)
		//	{
		//		b_real = 1;
		//	}
		//	else
		//	{
		//		b_real = 0;
		//	}

		//	if (b_should != b_real)
		//	{
		//		err_vote[cnt]++;
		//		err_vote[cnt+1]++;
		//	}
		//}

		for(int cnt=0;cnt<7;cnt++)
		{
			/*if (err_vote[cnt] == 2)
				v_in[cnt] = 1-v_in[cnt];*/

			if (v_in[cnt] > 0.5)
			{
				re |= (1<<cnt);
			}
		}

		return re;
	}

	int decode_directive(vector<double>& src)
	{
		int indx = 0;
		for(int cnt=1;cnt<word_per_pic;cnt++)
		{
			if (src[cnt] > src[indx])
				indx = cnt;
		}

		return indx;
	}

	vector<double> encode_directive(int num)
	{
		vector<double> re(word_per_pic);
		fill(re.begin(),re.end(),0.0);
		re[num] = 1.0;

		return re;
	}

	vector<double> encode(int num)
	{
		return encode_directive(num);

		vector<double> re;

		for(int cnt=0;cnt<7;cnt++)
		{
			if ((num & (1<<cnt)) != 0)
			{
				re.push_back(1.0);
			}
			else
			{
				re.push_back(0.0);
			}
		}

		//for(int cnt=0;cnt<7;cnt++)
		//{
		//	unsigned b_1 = (num & (1<<cnt)) != 0?1:0;
		//	unsigned b_2 = (num & (1<<((cnt+1)%7))) != 0?1:0;

		//	re.push_back(b_1 ^ b_2);
		//}

		return re;
	}

public:
	void valid_order()
	{
		for(int cnt=0;cnt<100;cnt++)
		{
			a_save(*v_img_real[cnt],string("D:\\trys\\recog\\text\\") + cnt + ".bmp");
			//cout<<cnt<<" - "<<a_trans_wchar_to_wstr(v_num[cnt/word_per_pic][cnt%word_per_pic])<<endl;
		}
	}

	~appc_recongntion_chinese()
	{
		for(int cnt=0;cnt<num_words_unit;cnt++)
		{
			delete v_clser[cnt];
			delete v_ts[cnt];
		}

		vector<cls_image_ot<unsigned>*>::iterator itor;
		vector<cls_image_ot<unsigned>*>::iterator itor_real;
		for(itor=v_img.begin(),itor_real = v_img_real.begin();
			itor != v_img.end();
			itor++,itor_real++)
		{
			delete (*itor);
			delete (*itor_real);
		}
	}
};
