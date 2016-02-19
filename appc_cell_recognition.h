#include "lib_import.h"

class appc_cell_recognition
{
public:
	double d_threshod_percent;
	double d_max_radio;
	double d_min_radio;
	double d_primeter_precent;
	double d_join_percent;

private:
	cls_image_ot<unsigned> img_backup;
	cls_image_ot<unsigned> img_main;
	vector<cls_circle<double> > v_cir_re;

public:
	appc_cell_recognition()
	{
		;
	}

	void load(string str_filename)
	{
		a_load(img_main,str_filename);
		img_backup = img_main;
	}

	appc_cell_recognition(string str_filename)
	{
		a_load(img_main,str_filename);
		img_backup = img_main;
	}

	vector<cls_circle<double> >& result()
	{
		return v_cir_re;
	}

	void run()
	{
		v_cir_re.clear();
		img_main = img_backup;
		pre_dealing();
		post_dealing();
	}

	void save_circle(string str_filename)
	{
		cls_image_ot<unsigned> img_draw = img_backup;
		for(int cnt=0;cnt<v_cir_re.size();cnt++)
		{
			img_draw.draw_circle(v_cir_re[cnt]);
		}

		a_save(img_draw,str_filename);
	}

	void pre_dealing()
	{
		img_main.filter_threshod(img_main.tools_get_mid_hist(d_threshod_percent));
		img_main.tools_fill_connect_regin();
		img_main.filter_threshod(255);
		img_main.filter_space_laplace();
		img_main.tools_reverse();
		img_main.filter_threshod(255);
		img_main.filter_thin();
	}

	void post_dealing()
	{
		vector<cls_circle<double> > v_circle;
		img_main.trans_hough_circle(v_circle,d_max_radio,1,d_primeter_precent);

		//去除重复的圆...
		cls_image_ot<unsigned> img_mark(img_main.get_width(),img_main.get_height());
		cls_image_ot<unsigned> img_r(img_main.get_width(),img_main.get_height());

		img_mark.tools_fill((255<<8) + (255<<16) + (255<<24));

		for(int cnt=0;cnt<v_circle.size();cnt++)
		{
			if (v_circle[cnt].d_r >= d_min_radio)
			{
				img_r.set_pixel(v_circle[cnt].pt_center.x,v_circle[cnt].pt_center.y,v_circle[cnt].d_r);
				img_mark.set_gray(v_circle[cnt].pt_center.x,v_circle[cnt].pt_center.y,0);
			}
		}

		int num_block;
		img_mark.tools_mark_connection_region(num_block);
		vector<cls_circle<double> > v_re(num_block);
		vector<int> v_n(num_block);
		for(int i=0;i<img_main.get_width();i++)
		{
			for(int j=0;j<img_main.get_height();j++)
			{
				if (img_mark.get_pixel(i,j) !=0 && img_mark.get_pixel(i,j) != ((255<<8) + (255<<16) + (255<<24)))
				{
					v_re[img_mark.get_pixel(i,j)].pt_center.x += i;
					v_re[img_mark.get_pixel(i,j)].pt_center.y += j;
					v_re[img_mark.get_pixel(i,j)].d_r += img_r.get_pixel(i,j);
					v_n[img_mark.get_pixel(i,j)]++;
				}
			}
		}

		vector<cls_circle<double> > v_real;
		for(int cnt=0;cnt<num_block;cnt++)
		{
			if (v_n[cnt] != 0)
			{
				v_re[cnt].pt_center.x /= v_n[cnt];
				v_re[cnt].pt_center.y /= v_n[cnt];
				v_re[cnt].d_r /= v_n[cnt];
			}
		}

		//去除相邻的重复度比较高的圆。
		vector<bool> v_b(v_re.size());
		fill(v_b.begin(),v_b.end(),true);
		for(int i=0;i<v_re.size();i++)
		{
			if (v_b[i] == false)
				continue;

			for(int j=i+1;j<v_re.size();j++)
			{
				if (distance_euclid(v_re[i].pt_center,v_re[j].pt_center) < v_re[i].d_r
					|| distance_euclid(v_re[i].pt_center,v_re[j].pt_center) < v_re[j].d_r
					|| distance_euclid(v_re[i].pt_center,v_re[j].pt_center) < d_join_percent * (v_re[i].d_r + v_re[j].d_r ))
				{
					if (v_re[i].d_r > v_re[j].d_r)
					{
						v_b[j] = false;
					}
					else
					{
						v_b[i] = false;
					}
				}
			}
		}

		for(int cnt=1;cnt<v_b.size();cnt++)
		{
			if (v_b[cnt] == true)
			{
				v_cir_re.push_back(v_re[cnt]);
			}
		}
	}
};
