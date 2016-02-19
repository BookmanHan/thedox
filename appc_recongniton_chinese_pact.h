#include "lib_import.h"

class appc_recongntion_chinese_compat
{
protected:
	vector<cls_ann_network_bilayer<a_sigmod>*> clsers;
	vector<vector<double> > mat_feature;
	vector<vector<int> > v_num;

public:
	appc_recongntion_chinese_compat(string str_recng_file)
	{
		clsers.resize(801);
		mat_feature.resize(20000);

		ifstream fin(str_recng_file.c_str(),ios::binary);

		vector<cls_ann_network_bilayer<a_sigmod>*>::iterator itor;
		for(itor = clsers.begin();itor != clsers.end();itor++)
		{
			*itor = new cls_ann_network_bilayer<a_sigmod>(36,20,25);
			(*itor)->load(fin);
		}

		vector<vector<double> >::iterator itor_f;
		for(itor_f=mat_feature.begin();itor_f!= mat_feature.end();itor_f++)
		{
			itor_f->resize(36);
			vector<double>::iterator itor;
			for(itor = itor_f->begin();itor != itor_f->end();itor++)
			{
				double dval;
				fin.read((char*)&dval,sizeof(double));
				*itor = dval;
			}
		}

		fin.close();
		v_num.resize(800);
		fin.open((str_recng_file + ".num").c_str());

		for(int cnt=0;cnt<800;cnt++)
		{
			v_num[cnt].resize(25);
			for(int icnt=0;icnt<25;icnt++)
			{
				fin>>v_num[cnt][icnt];
			}
		}

		fin.close();
	}

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
	}

	int decode(vector<double>& src)
	{
		int indx = 0;
		for(int cnt=1;cnt<25;cnt++)
		{
			if (src[cnt] > src[indx])
				indx = cnt;
		}

		return indx;
	}

	vector<double> encode_directive(int num)
	{
		vector<double> re(25);
		fill(re.begin(),re.end(),0.0);
		re[num] = 1.0;

		return re;
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

	wchar_t run(cls_image_ot<unsigned>& src)
	{
		vector<double> v_feature;
		get_feature(&src,v_feature);

		vector<pair<int,int> > v_re;
		vector<cls_ann_network_bilayer<a_sigmod> *>::iterator itor;

		//wstring str_total;

		int cnt_clser = 0;
		for(itor=clsers.begin()
			;itor!=clsers.end();
			itor++)
		{
			vector<double> d_re = (*itor)->get_output(v_feature);

			int cnt = decode(d_re);
			if (cnt < 25 && cnt >= 0)
			{
				v_re.push_back(make_pair(cnt_clser,cnt));
			}

			cnt_clser++;
		}

		//cout<<str_total<<endl;

		double d_min = inf;
		int indx_min = -1;
		vector<pair<int,int> >::iterator itor_last;
		for(itor_last=v_re.begin();itor_last!=v_re.end();itor_last++)
		{
			if (itor_last->first*25 + itor_last->second >= 20000)
			{
				continue;
			}

			double d = get_dist_for_feature(v_feature,mat_feature[itor_last->first*25 + itor_last->second]);
			//a_vector_output(cout,mat_feature[itor_last->first*25 + itor_last->second]);
			if (d < d_min)
			{
				indx_min = v_num[itor_last->first][itor_last->second];
				d_min = d;
			}
		}

		return indx_min;
	}
};
