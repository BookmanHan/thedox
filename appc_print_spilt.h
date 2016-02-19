#include "lib_import.h"

class appc_print_spilt
{
private:
	string str_result_path;
	string str_filename;

public:
	double d_spilt_percent_v;
	double d_spilt_percent_h;
	int n_iter_v;
	int n_iter_h;
	int n_iter_re_h;
	double d_least_width;
	double d_most_width;
	int n_thd;
	int least_size;

public:
	appc_print_spilt(string arg_filename)
	{
		str_result_path = "";
		str_filename = arg_filename;

		d_spilt_percent_v = 0.01;
		d_spilt_percent_h = 0.01;
		n_iter_v = 5;
		n_iter_h = 5;
		n_iter_re_h = 2;
		d_least_width = 0.75;
		d_most_width = 1.25;
		n_thd = 127;
		least_size = 3;
	}

	appc_print_spilt(string arg_filename,string arg_path)
	{
		str_result_path = arg_path;
		str_filename = arg_filename;

		d_spilt_percent_v = 0.005;
		d_spilt_percent_h = 0.005;
		n_iter_v = 2;
		n_iter_h = 3;
		n_iter_re_h = 2;
		d_least_width = 0.66;
		d_most_width = 1.25;
		n_thd = 127;
		least_size = 3;
	}

	void run(list<cls_image_ot<unsigned int> > & v_img_all,bool b_save = false)
	{
		cls_image_ot<unsigned int> img_cool_read;
		if (false == a_load(img_cool_read,str_filename))
			cerr<<"reading failed"<<endl;

		cls_image_ot<unsigned> img_cool(img_cool_read.get_width() + 6,img_cool_read.get_height() + 6);
		img_cool.tools_fill(get_rgb(255,255,255));
		img_cool.tools_place(img_cool_read,3,3);

		vector<cls_image_ot<unsigned int> > v_img;
		img_cool.filter_threshod(n_thd);

		img_cool.app_get_project_v(v_img,d_spilt_percent_v,n_iter_v);
		for(int cnt=0;cnt<v_img.size();cnt++)
		{
			vector<cls_image_ot<unsigned int> > v_img_tmp;
			v_img[cnt].filter_threshod(n_thd);
			v_img[cnt].app_get_project_h(v_img_tmp,d_spilt_percent_h,n_iter_h);

			for(int cnti=0;cnti<v_img_tmp.size();cnti++)
			{
				if (v_img_tmp[cnti].get_width() >= least_size && v_img_tmp[cnti].get_height() >= least_size)
					v_img_all.push_back(v_img_tmp[cnti]);
			}
		}

		double ave_width = 0;
		list<cls_image_ot<unsigned int> >::iterator itor;
		list<cls_image_ot<unsigned int> >::iterator itor_inc;

		itor = v_img_all.begin();
		while(itor != v_img_all.end())
		{
			*itor = itor->tools_part(itor->tools_get_real_rect().dilate(1));
			ave_width += itor->get_width();
			itor++;
		}
		ave_width /= v_img_all.size();

		double ave_least_width = ave_width * d_least_width;
		double ave_most_width = ave_width * d_most_width;

		if (v_img_all.size() < 2)
			return;

		itor = v_img_all.begin();
		itor_inc = itor;
		itor_inc++;
		while(itor_inc != v_img_all.end())
		{
			if (itor->get_width() < ave_least_width && itor_inc->get_width() < ave_least_width)
			{
				itor->tools_merge(*itor_inc);
				v_img_all.erase(itor_inc);
				itor_inc = itor;
				itor_inc++;
			}
			else if (itor->get_width() > ave_most_width)
			{
				vector<cls_image_ot<unsigned int> > v_img;
				itor->app_get_project_h(v_img,d_spilt_percent_h,n_iter_re_h);

				if (v_img.size() >= 2)
				{
					v_img_all.insert(itor_inc,v_img[v_img.size()-2]);
					v_img_all.insert(itor_inc,v_img[v_img.size()-1]);
					v_img_all.erase(itor);
				}

				itor = itor_inc;
				itor_inc++;
			}
			else
			{
				itor_inc++;
				itor++;
			}
		}

		int cnt = 0;
		itor = v_img_all.begin();
		while(itor != v_img_all.end())
		{
			itor->filter_threshod(n_thd);
			*itor = itor->tools_part(itor->tools_get_real_rect().dilate(1));

			if (b_save == true)
				if (itor->get_width() > least_size && itor->get_height() > least_size)
					a_save(*itor,str_result_path - cnt - ".bmp");

			itor++;
			cnt++;
		}
	}
};
