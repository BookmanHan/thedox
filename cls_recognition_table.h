#include "lib_import.h"

class cls_recognition_table
{
private:
	cls_image_ot<unsigned> img_main;

public:
	
	void load_image(const string& str_name)
	{
		a_load(img_main,str_name);
	}

	void save_image(const string& str_name)
	{
		a_save(img_main,str_name);
	}

	void mark_table(vector<unsigned>& vn_finnal_row,vector<unsigned>& vn_finnal_col)
	{
		cls_image_ot<unsigned> img_dealed = img_main;

		img_dealed.filter_threshod(180);
		img_dealed.filter_dilate_cross(3);

		vector<cls_rect> v_rect;
		vector<int> v_num;

		img_dealed.tools_mark_connection_region(v_rect,v_num);

		vector<cls_rect>::iterator itor;
		
		int mark_num = 0;
		for(itor=v_rect.begin();itor != v_rect.end(); ++itor)
		{
			if (itor->get_width() > img_dealed.get_width()*0.8 && itor->get_height() > img_dealed.get_height() * 0.8)
			{
				mark_num = itor - v_rect.begin();
			}
		}

		for(int i=0;i<img_dealed.get_width();i++)
		{
			for(int j=0;j<img_dealed.get_height();j++)
			{
				if (img_dealed.get_pixel(i,j) != mark_num)
					img_dealed.set_gray(i,j,255);
				else
					img_dealed.set_gray(i,j,0);
			}
		}

		vector<pair<double,double> > re;
		img_dealed.trans_hough_line(re,max(img_main.get_height(),img_main.get_width()),2.5,5);

		vector<unsigned> vn_row;
		vector<unsigned> vn_col;

		for(int cnt=0;cnt<re.size();cnt++)
		{ 
			if (fabs(re[cnt].first) < 3 && re[cnt].second > 0)
			{
				vn_row.push_back(re[cnt].second);
			}
			else if ((re[cnt].first < -85 || re[cnt].first > 85))
			{
				vn_col.push_back(fabs(re[cnt].second));
			}
		}

		int num = 1;
		sort(vn_row.begin(),vn_row.end());

		int sum = vn_row[0];
		for(int cnt=1;cnt<vn_row.size();cnt++)
		{
			if (vn_row[cnt] - vn_row[cnt-1] > 10)
			{
				vn_finnal_row.push_back(sum/num);
				sum = vn_row[cnt];
				num = 1;
			}
			else
			{
				sum += vn_row[cnt];
				num ++;
			}
		}

		vn_finnal_row.push_back(sum/num);

		num = 1;
		sum = vn_col[0];
		sort(vn_col.begin(),vn_col.end());

		for(int cnt=1;cnt<vn_col.size();cnt++)
		{
			if (vn_col[cnt] - vn_col[cnt-1] > 5)
			{
				vn_finnal_col.push_back(sum/num);
				sum = vn_col[cnt];
				num = 1;
			}
			else
			{
				sum += vn_col[cnt];
				num ++;
			}
		}

		vn_finnal_col.push_back(sum/num);
	}

	void draw_table(vector<unsigned>& vn_row,vector<unsigned>& vn_col)
	{
		for(int cnt=0;cnt<vn_row.size();cnt++)
		{
			img_main.draw_line_for_col(vn_row[cnt]);
		}

		for(int cnt=0;cnt<vn_col.size();cnt++)
		{
			img_main.draw_line_for_row(vn_col[cnt]);
		}
	}
};