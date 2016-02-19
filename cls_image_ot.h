#include "lib_import.h"
#pragma once

template <typename T>
class cls_image_ot
	:public cls_serialized_object
{
public:
	cls_matrix<T> mat_image;

public:

	cls_image_ot();
	cls_image_ot(int w,int h);
	cls_image_ot(const cls_image_ot& that);

	~cls_image_ot(void);

	void init_pixel(int w,int h);
	void fill_pixel(int w,int h,unsigned int value = 0);

	inline T get_pixel(int x,int y);
	inline void set_pixel(int x,int y,T value);
	inline void set_pixel(int x,int y,byte r,byte g,byte b);

	inline int get_width() const;
	inline int get_height() const;

	inline void resize(int width,int height);

	inline int get_binary(int x,int y);

	inline unsigned char get_channel_value_0(int x,int y) const;
	inline unsigned char get_channel_value_1(int x,int y) const;
	inline unsigned char get_channel_value_2(int x,int y) const ;
	inline unsigned char get_channel_value_3(int x,int y) const;
	inline unsigned char get_channel_value(int x,int y,int number) const;

	inline void set_channel_value_0(int x,int y,int value);
	inline void set_channel_value_1(int x,int y,int value);
	inline void set_channel_value_2(int x,int y,int value);
	inline void set_channel_value_3(int x,int y,int value);
	inline void set_channel_value(int x,int y,int value,int number);

	inline int get_gray(int x,int y) const;
	inline void set_gray(int x,int y,unsigned char value);

	void operator = (const cls_image_ot& that);

	void get_freq_trans_data(complex<double> *data) const;
	void set_freq_trans_data(complex<double> *data);

	int get_freq_trans_width();
	int get_freq_trans_height();

	void get_hist_normal(vector<double>& hist,int num_hist,int channel) const;
	void get_hist(vector<double>& hist,int num_hist,int channel) const;
	void get_hist_gray_normal(vector<double>& hist,int num_hist) const;
	void get_hist_gray(vector<double>& hist,int num_hist) const;

	void basis_trans_linear(double d_a,double d_b);
	void basis_trans_log(double d_r);
	void basis_trans_gramma(double d_grammer,double dexp);
	void basis_trans_threshold(unsigned char n_threshold);

	void hist_equal();
	void hist_match(cls_image_ot &that);

	void filter_space_template(int n_w,int n_h,int n_mw,int n_mh,double *d_template,double d_coef);
	void filter_space_ave_smooth();
	void filter_space_gauss_smooth();
	void filter_space_adapter_smooth(int n_w,int n_h,int n_mw,int n_mh,double *d_template,double d_coef,unsigned char byt_std);
	void filter_space_ave_smooth_adaptive();
	void filter_space_gauss_smooth_adaptive();
	void filter_space_median(int n_w,int n_h,int n_mw,int n_mh,int order);
	void filter_space_median_mid();
	void filter_space_sobel();
	void filter_space_isotropic_sobel();
	void filter_space_roberts();
	void filter_space_edge();
	void filter_space_log();
	void filter_space_laplace();
	void filter_space_big_log();

	void filter_freq_template(double* d_filter);

	void template_freq_ideal_lpf(double *d_filter,int n_freq);
	void template_freq_gauss_lpf(double *d_filter,double d_freq);
	void template_freq_ideal_hpf(double *d_filter,int n_freq);
	void template_freq_gauss_hpf(double *d_filter,double d_sigma);
	void template_freq_laplace_hpf(double *d_filter);

	void filter_freq_ideal_lpf(int n_freq);
	void filter_freq_gauss_lpf(double d_freq);
	void filter_freq_ideal_hpf(int n_freq);
	void filter_freq_gauss_hpf(double d_sigma);
	void filter_freq_laplace_hpf();

	void filter_erode(int se[3][3]);
	void filter_dilate(int se[3][3]);
	void filter_open(int se[3][3]);
	void filter_close(int se[3][3]);
	void filter_gray_dilate(int n_w,int n_h,int n_mw,int n_mh,int **se);
	void filter_gray_erode(int n_w,int n_h,int n_mw,int n_mh,int **se);
	void filter_gray_open(int n_w,int n_h,int n_mw,int n_mh,int **se);
	void filter_gray_close(int n_w,int n_h,int n_mw,int n_mh,int **se);
	void filter_top_hat(int n_w,int n_h,int n_mw,int n_mh,int **se);
	void filter_trace_boundary();
	void filter_erode_cross(int num=1);
	void filter_dilate_cross(int num=1);
	void filter_open_cross(int num=1);
	void filter_close_cross(int num=1);
	void filter_erode_squr(int num=1);
	void filter_dilate_squr(int num=1);
	void filter_open_squr(int num=1);
	void filter_close_squr(int num=1);

	cls_image_ot<T>& operator & (const cls_image_ot<T> & ib);
	cls_image_ot<T>& operator | (const cls_image_ot<T> & ib);
	cls_image_ot<T>& operator - (const cls_image_ot<T> & ib);
	bool operator == (const cls_image_ot<T> & ib);

	void tools_fill_connect_regin();
	void tools_mark_connection_region(int &pnum);
	void tools_mark_connection_region(vector<cls_rect>&rect,vector<int>& pnum);
	void tools_fill_region(int x,int y);
	void tools_reverse();
	void tools_except_border(byte by_color);
	void tools_get_connect_region_rect(vector<cls_rect> & rect);
	void tools_get_connect_region_pixel_count(byte by_start,byte by_end);
	cls_image_ot tools_part(cls_rect rect);
	cls_rect tools_get_real_rect();
	byte tools_get_mid_hist(double d_mu) const;
	void tools_get_min_hist(byte *ady, int *n) const;
	void tools_merge(cls_image_ot<T>& that);
	void tools_place(cls_image_ot<T>& that,int x,int y);

	void analyze_classic_label_connect_region(int n_conn,int n_min);
	void filter_thin();
	void filter_threshod(int low,int high);
	void filter_threshod(int thd);
	void filter_convex(bool b_constrained);

	void tools_get_project_v(int *ar_n_data);
	void tools_get_project_v_spilt(vector<int> & re,double d_percent,int min_inter);
	void tools_get_project_h(int *ar_n_data);
	void tools_get_project_h_spilt(vector<int> & re,double d_percent,int min_inter);
	void tools_to_gray();
	cls_point<T> tools_get_connect_region_center_point(int num);

	void tools_fill(int color);

	void feature_grids_percent( vector<double>& re,double inter_x,double inter_y );
	void feature_plane(vector<double>&re);

	void feature_cross_full_v(vector<int>& re,double inter);
	void feature_cross_full_h(vector<int>& re,double inter);
	void feature_cross_full(vector<int>&re,double inter);
	void feature_cross_full( vector<double>&re,double inter_x,double inter_y);
	void feature_cross_percent_v(vector<int>&re,int inter,double percent);
	void feature_cross_percent_h(vector<int>&re,int inter,double percent);
	void feature_cross_percent(vector<int>&re,int inter,double percent);

	void app_get_project_v(vector<cls_image_ot<T> > &re,double d_precent,int min_inter);
	void app_get_project_h(vector<cls_image_ot<T> > &re,double d_precent,int min_inter);
	double app_get_likely_percent(cls_image_ot<T>& img);

	cls_image_ot<T> trans_bi_stretch(cls_rect rect_src,cls_rect rect_des);
	cls_image_ot<T> trans_bi_stretch(cls_rect rect_des);

	vector<cls_circle<double> >& trans_hough_circle(vector<cls_circle<double> > & re,int bound_r,int squr,double thd,int xmax=0,int ymax=0,int rmax=0);
	vector<pair<double,double> >& trans_hough_line(vector<pair<double,double> >& re, int search_space ,double thd,int window);

	void draw_circle(cls_circle<double> cir);
	void draw_points(vector<cls_point<int> > & re);
	void draw_line_for_ax_plus_by(double a,double b,unsigned color);
	void draw_line_for_poler(double poler,double angel,unsigned color);
	void draw_line_for_row(int row);
	void draw_line_for_col(int col);

	void trans_cosine(cls_matrix<complex<double> > &re);

	double appc_get_likely_with_same_size(cls_image_ot<T> & that);

	vector<cls_point<int> >& get_author(vector<cls_point<int> >& re);

	vector< vector<double> >& feature_haar_like(vector< vector<double> > & re ,int inc_ment_x,int inc_ment_y,int width_beg,int height_beg,int inc_width,int inc_height);

	int get_sum(const cls_rect& rect);
	int get_binary_sum(const cls_rect& rect);

	void trans_geometry(cls_matrix<double>& mat_trans);
	void trans_geometry_rotation(double d_angle,bool b_extend);

	//trans_xxx;
	//filter_restore_xxx;
	//color_xxx;
	//draw_xxx;

private:
	bool check_connection_region();

public:
	virtual void save( ofstream& fout ) const
	{
		try
		{
			mat_image.save(fout);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_image::save() - the file is not avalible for output.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			mat_image.load(fin);
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_image::load() - the file is not avalible for input.\n");
		}
	}
};

template <typename T>
void cls_image_ot<T>::draw_line_for_col( int col )
{
	for(int cnt=0;cnt<get_height();cnt++)
	{
		set_pixel(col,cnt,get_rgb(255,0,0));
	}
}

template <typename T>
void cls_image_ot<T>::draw_line_for_row( int row )
{
	for(int cnt=0;cnt<get_width();cnt++)
	{
		set_pixel(cnt,row,get_rgb(255,0,0));
	}
}

template <typename T>
void cls_image_ot<T>::draw_line_for_poler( double poler,double angel,unsigned color )
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (fabs(poler - i*cos(angel/180.0*pi) - j*sin(angel/180.0*pi)) < 1)
			{
				set_pixel(i,j,color);
			}
		}
	}
}

template <typename T>
void cls_image_ot<T>::draw_line_for_ax_plus_by( double a,double b,unsigned color )
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (fabs(i/a + j/b - 1) < 0.05)
			{
				set_pixel(i,j,color);
			}
		}
	}
}

template <typename T>
vector<pair<double,double> >& cls_image_ot<T>::trans_hough_line( vector<pair<double,double> >& re , int search_space , double thd,int window )
{
	int len = 2*sqrt(2.0)*search_space;
	cls_matrix<unsigned> mat(180,len);
	mat.fill(0);

	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (get_gray(i,j) == 0)
			{
				for(int cnt=-90;cnt<90;cnt++)
				{
					mat(cnt + 90,cos(cnt/180.0*pi)*i + sin(cnt/180.0*pi)*j + len/2)++;
				}
			}
		}
	}

	for(int i=-90;i<90;i++)
	{
		for(int j=-len/2;j<len/2-window;j+=window)
		{
			int sum = 0;
			for(int jj=j;jj<j+window;jj++)
			{
				sum += mat(i + 90,jj+len/2);
			}

			if (sum > thd * search_space)
			{
				re.push_back(make_pair(i,j + window/2));
			}
		}
	}

	return re;
}

template <typename T>
void cls_image_ot<T>::trans_geometry_rotation( double d_angle,bool b_extend )
{
	if (b_extend)
	{
		double d_lines = sqrt(get_height()*get_height() + get_width()*get_width());
		cls_image_ot that(d_lines,d_lines);
		that.tools_place(*this,0,0);
		*this = that;
	}
	
	cls_matrix<double> trans(3,3);
	cls_matrix<double> tmp(3,3);
	trans.make_identity();

	tmp.make_identity();
	tmp(2,2) = 1;
	tmp(0,2) = -get_width()/2;
	tmp(1,2) = -get_height()/2;
	trans = trans * tmp;

	tmp.make_identity();
	tmp(2,2) = 1;
	tmp(0,0) = cos(d_angle*pi/180.0);
	tmp(0,1) = -sin(d_angle*pi/180.0);
	tmp(1,0) = sin(d_angle*pi/180.0);
	tmp(1,1) = cos(d_angle*pi/180.0);
	trans = trans * tmp;

	tmp.make_identity();
	tmp(2,2) = 1;
	tmp(0,2) = +get_width()/2;
	tmp(1,2) = +get_height()/2;
	trans = trans*tmp;

	trans_geometry(trans);
}

template <typename T>
void cls_image_ot<T>::trans_geometry(cls_matrix<double>&  mat_trans )
{
	cls_matrix<double> mat_re(3,get_width()*get_height());

	int cnt = 0;
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			mat_re(0,cnt) = i;
			mat_re(1,cnt) = j;
			mat_re(2,cnt) = 1;
			cnt++;
		}
	}

	mat_re = mat_trans * mat_re;

	cls_image<unsigned> img_org = *this;
	tools_fill(get_rgb(255,255,255));

	cnt = 0;
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (mat_re(2,cnt) == 0)
				continue;

			mat_re(0,cnt) /= mat_re(2,cnt) ;
			mat_re(1,cnt) /= mat_re(2,cnt) ;

			cout<<mat_re(0,cnt)<<" - "<<mat_re(1,cnt)<<endl;

			if (mat_re(0,cnt) >=0 && mat_re(0,cnt) < get_width() 
				&& mat_re(1,cnt) >=0 && mat_re(1,cnt) < get_height())
			{
				set_pixel(mat_re(0,cnt),mat_re(1,cnt),img_org.get_pixel(i,j));
			}
			
			cnt++;
		}
	}
}

template <typename T>
int cls_image_ot<T>::get_binary_sum( const cls_rect& rect )
{
	int sum = 0;
	for(int i=rect.left;i<rect.right;i++)
	{
		for(int j=rect.top;j<rect.bottom;j++)
		{
			sum += get_binary(i,j);
		}
	}

	return sum;
}

template <typename T>
int cls_image_ot<T>::get_sum( const cls_rect& rect )
{
	int sum = 0;
	for(int i=rect.left;i<rect.right;i++)
	{
		for(int j=rect.top;j<rect.bottom;j++)
		{
			sum += get_pixel(i,j);
		}
	}

	return sum;
}

inline 
int get_sum_from_acc(cls_matrix<int>& mat_acc,cls_rect& rect)
{
	return mat_acc(rect.right,rect.bottom) + mat_acc(rect.left,rect.top) - 
		mat_acc(rect.right,rect.top) - mat_acc(rect.left,rect.bottom);
}

template <typename T>
vector<vector<double> >& cls_image_ot<T>::feature_haar_like(vector< vector<double> > & re ,
														 int inc_ment_x,int inc_ment_y,int width_beg,int height_beg,int inc_width,int inc_height)
{
	cls_matrix<int> mat_acc(get_width(),get_height());
	mat_acc.fill(0);

	mat_acc(0,0) = get_binary(0,0);
	for(int j=1;j<get_height();j++)
	{
		mat_acc(0,j) += mat_acc(0,j-1) + get_binary(0,j);
	}

	for(int i=1;i<get_width();i++)
	{
		mat_acc(i,0) += mat_acc(i-1,0) + get_binary(i,0);
	}

	for(int i=1;i<get_width();i++)
	{
		for(int j=1;j<get_height();j++)
		{
			mat_acc(i,j) += -mat_acc(i-1,j-1) + mat_acc(i,j-1) + mat_acc(i-1,j) + get_binary(i,j);
		}
	}


	vector<double> v_re(9);
	int width = width_beg;
	int height = height_beg;

	while(width < get_width() && height < get_height())
	{
		for(int j=0;j<get_height() - height;j+=inc_ment_y)
		{
			for(int i=0;i<get_width() - width;i+=inc_ment_x)
			{
				int sum1 = get_sum_from_acc(mat_acc,cls_rect(j,i,j+height/2,i+width/2));
				int sum2 = get_sum_from_acc(mat_acc,cls_rect(j+height/2,i,j+height,i+width/2));
				int sum3 = get_sum_from_acc(mat_acc,cls_rect(j,i+width/2,j+height/2,i+width));
				int sum4 = get_sum_from_acc(mat_acc,cls_rect(j+height/2,i+width/2,j+height,i+width));

				v_re[0] = 1.0* (sum1 + sum2 - sum3 - sum4) / (width*height) ;
				v_re[1] = 1.0* (sum1 + sum3 - sum4 - sum2) / (width*height) ;
				v_re[2] = 1.0* (sum1 + sum4 - sum2 - sum3) /  (width*height);

				int sum5 = get_sum_from_acc(mat_acc,cls_rect(j,i,height,i+width/3));
				int sum6 = get_sum_from_acc(mat_acc,cls_rect(j,i + width/3,height,i+2*width/3));
				int sum7 = get_sum_from_acc(mat_acc,cls_rect(j,i+2*width/3,height,i+width));

				v_re[3] = 1.0*(sum6 - (sum5 + sum7)) /  (width*height);

				int sum_8 = get_sum_from_acc(mat_acc,cls_rect(j,i,j+height,i+width));
				int sum_9 = get_sum_from_acc(mat_acc,cls_rect(j+height/3,i+width/3,j+height*2/3,i+width*2/3));

				v_re[4] = 1.0*(sum_8 - sum_9) / (width*height);

				v_re[5] = 1.0 * (sum1 + sum2 + sum3 - sum4) / (width*height);
				v_re[6] = 1.0 * (sum1 + sum2 - sum3 + sum4) / (width*height);
				v_re[7] = 1.0 * (sum1 - sum2 + sum3 + sum4) / (width*height);
				v_re[8] = 1.0 * ( - sum1 + sum2 + sum3 + sum4) / (width*height);

				re.push_back(v_re);
			}
		}

		width += inc_width;
		height += inc_height;
	}

	return re;
}

template <typename T>
void cls_image_ot<T>::draw_points( vector<cls_point<int> > & re )
{
	vector<cls_point<int> >::iterator itor;
	for(itor = re.begin();itor != re.end(); itor++)
	{
		set_pixel(itor->x,itor->y,get_rgb(255,0,0));
	}
}

template <typename T>
vector<cls_point<int>>& cls_image_ot<T>::get_author(vector<cls_point<int>>& re)
{
	for(int i=1;i<get_width()-1;i++)
	{
		for(int j=1;j<get_height()-1;j++)
		{
			int num = get_binary(i,j) + get_binary(i-1,j) + get_binary(i+1,j) +
				get_binary(i,j-1) + get_binary(i-1,j-1) + get_binary(i+1,j-1) +
				get_binary(i,j+1) + get_binary(i-1,j+1) + get_binary(i+1,j+1);
		
			if (num == 2 && get_binary(i,j) == 1)
			{
				re.push_back(cls_point<int>(i,j));
			}
		}
	}

	return re;
}

template <typename T>
int cls_image_ot<T>::get_binary( int x,int y )
{
	return get_gray(x,y) == 0?1:0;
}

template <typename T>
double cls_image_ot<T>::appc_get_likely_with_same_size( cls_image_ot<T> & that )
{
	int sum = 0;
	for(int i=0;i<that.get_width();i++)
	{
		for(int j=0;j<that.get_height();j++)
		{
			sum += pow(1.0*(get_gray(i,j) - that.get_gray(i,j)),2.0);
		}
	}

	return sum;
}

template <typename T>
void cls_image_ot<T>::feature_cross_full( vector<double>&re,double inter_x,double inter_y)
{
	vector<int> v_n;
	feature_cross_full_v(v_n,inter_x);
	feature_cross_full_h(v_n,inter_y);

	for(int cnt=0;cnt<v_n.size();cnt++)
	{
		re.push_back(double(v_n[cnt])/10.0);
		if (re.back() > 1.0)
			re.back() = 1.0;
	}
}

template <typename T>
void cls_image_ot<T>::trans_cosine( cls_matrix<complex<double> > &re )
{
	complex<double> *trans_in = new complex<double>[get_freq_trans_width()*get_freq_trans_height()];
	complex<double> *trans_out = new complex<double>[get_freq_trans_width()*get_freq_trans_height()];

	get_freq_trans_data(trans_in);
	a_fast_cosine_trans_in_2d(trans_in,trans_out,a_get_pow_num(get_width()),a_get_pow_num(get_height()));

	int width = get_freq_trans_width();
	int height = get_freq_trans_height();

	re.resize(width,height);
	re.set_raw_data(trans_out);

	delete []trans_in;
}

template <typename T>
void cls_image_ot<T>::feature_cross_percent( vector<int>&re,int inter,double percent )
{
	feature_cross_percent_v(re,inter,percent);
	feature_cross_percent_h(re,inter,percent);
}

template <typename T>
void cls_image_ot<T>::feature_cross_percent_h( vector<int>&re,int inter,double percent )
{
	for(int cnt=0;cnt<get_height();cnt+=inter)
	{
		int change = 0;
		int cur = get_gray(0,cnt);
		for(int i=1;i<get_width()*percent;i++)
		{
			if (cur != get_gray(i,cnt))
			{
				cur = get_gray(i,cnt);
				change++;
			}
		}

		re.push_back(change);
	}
}

template <typename T>
void cls_image_ot<T>::feature_cross_percent_v( vector<int>&re,int inter,double percent )
{
	for(int cnt=0;cnt<get_width();cnt+=inter)
	{
		int change = 0;
		int cur = get_gray(cnt,0);
		for(int i=1;i<get_height()*percent;i++)
		{
			if (cur != get_gray(cnt,i))
			{
				cur = get_gray(cnt,i);
				change++;
			}
		}

		re.push_back(change);
	}
}

template <typename T>
void cls_image_ot<T>::feature_cross_full( vector<int>&re,double inter )
{
	feature_cross_full_v(re,inter);
	feature_cross_full_h(re,inter);
}

template <typename T>
void cls_image_ot<T>::feature_cross_full_h( vector<int>& re,double inter )
{
	for(int cnt=0;cnt<get_height();cnt+=inter)
	{
		int change = 0;
		int cur = get_binary(0,cnt);
		for(int i=1;i<get_width();i++)
		{
			if (cur != get_binary(i,cnt))
			{
				cur = get_binary(i,cnt);
				change++;
			}
		}

		re.push_back(change);
	}
}

template <typename T>
void cls_image_ot<T>::feature_cross_full_v( vector<int>& re,double inter )
{
	for(int cnt=0;cnt<get_width();cnt+=inter)
	{
		int change = 0;
		int cur = get_binary(cnt,0);
		for(int i=1;i<get_height();i++)
		{
			if (cur != get_binary(cnt,i))
			{
				cur = get_binary(cnt,i);
				change++;
			}
		}

		re.push_back(change);
	}
}

template <typename T>
void cls_image_ot<T>::feature_grids_percent( vector<double>& re,double inter_x,double inter_y )
{
	double x_len = get_width()/inter_x;
	double y_len = get_height()/inter_y;

	for(int i=0;i<x_len;i++)
	{
		for(int j=0;j<y_len;j++)
		{
			double sum = 0.0;
			for(int ii=0;ii<inter_x;ii++)
			{
				for(int jj=0;jj<inter_y;jj++)
				{
					if (get_gray(int(i*inter_x+ii),int(j*inter_y+jj)) < 127)
					{
						sum++;
					}
				}
			}

			re.push_back(sum/inter_x/inter_y);
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_fill( int color )
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			set_pixel(i,j,color);
		}
	}
}

template <typename T>
void cls_image_ot<T>::set_pixel( int x,int y,byte r,byte g,byte b )
{
	set_channel_value_1(x,y,r);
	set_channel_value_1(x,y,g);
	set_channel_value_2(x,y,b);
}

template <typename T>
void cls_image_ot<T>::draw_circle( cls_circle<double> cir )
{
	for(int ang=0;ang<360;ang++)
	{
		double d_ang = ang/360.0*2*pi;
		int x = cir.pt_center.x + cir.d_r*cos(d_ang) ;
		int y = cir.pt_center.y + cir.d_r*sin(d_ang) ;
		if (x <0 || x >get_width())
			continue;
		if (y < 0 || y > get_height())
			continue;

		set_pixel(x,y,100,100,100);
	}
}

template <typename T>
vector<cls_circle<double> >& cls_image_ot<T>::trans_hough_circle(vector<cls_circle<double> > & re,int bound_r,int squr,double thd,int xmax/*=0*/,int ymax/*=0*/,int rmax/*=0*/ )
{
	if (xmax == 0)
		xmax = get_width();
	if (ymax == 0)
		ymax = get_height();
	if (rmax == 0)
		rmax = bound_r*2.2;

	cls_multi_array<double> hough_space(xmax,ymax,rmax);

	for(int w=0;w<get_width();w++)
	{
		for(int h=0;h<get_height();h++)
		{
			if (get_gray(w,h) == 0)
			{
				for(int x=max(w-bound_r,0);x<min(xmax,w+bound_r);x++)
				{
					for(int y=max(h-bound_r,0);y<min(ymax,h+bound_r);y++)
					{
						hough_space(x,y,sqrt((x-w)*(x-w) + (y-h)*(y-h))) ++;
					}
				}
			}
		}
	}

	for(int i=0;i<xmax/squr;i++)
	{
		for(int j=0;j<ymax/squr;j++)
		{
			for(int k=0;k<rmax/squr;k++)
			{
				int sum = 0;
				for(int thi=0;thi<squr;thi++)
				{
					for(int thj=0;thj<squr;thj++)
					{
						for(int thk=0;thk<squr;thk++)
						{
							sum += hough_space(i*squr+thi,j*squr+thj,k*squr+thk);
						}
					}
				}

				if (sum>2*(k+1)*squr*squr*squr*squr*thd*pi)
				{
					re.push_back(cls_circle<double>(i*squr+squr/2,j*squr+squr/2,k*squr+squr/2));
				}
			}
		}
	}

	return re;
}

template <typename T>
void cls_image_ot<T>::filter_space_roberts()
{
	double d_template_h_sobel[4] =
	{
		0,1,-1,0
	};

	double d_template_v_sobel[4] =
	{
		1,0,0,-1
	};

	filter_space_template(2,2,1,1,d_template_h_sobel,1);
	filter_space_template(2,2,1,1,d_template_v_sobel,1);
}

template <typename T>
void cls_image_ot<T>::filter_space_edge()
{
	double d_template_h_sobel[9] =
	{
		-1,0,1,
		-1,0,1,
		-1,0,1
	};

	double d_template_v_sobel[9] =
	{
		-1,-1,-1,
		0,0,0,
		1,1,1
	};

	filter_space_template(3,3,1,1,d_template_h_sobel,1);
	filter_space_template(3,3,1,1,d_template_v_sobel,1);
}

template <typename T>
void cls_image_ot<T>::filter_space_isotropic_sobel()
{
	double d_template_h_sobel[9] =
	{
		-1,0,1,
		-1.414,0,1.414,
		-1,0,1
	};

	double d_template_v_sobel[9] =
	{
		-1,-1.414,-1,
		0,0,0,
		1,1.414,1
	};

	filter_space_template(3,3,1,1,d_template_h_sobel,1);
	filter_space_template(3,3,1,1,d_template_v_sobel,1);
}

template <typename T>
void cls_image_ot<T>::init_pixel( int w,int h )
{
	mat_image = cls_matrix<T>(w,h);
}

template <typename T>
void cls_image_ot<T>::filter_close_squr(int num)
{
	int se[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};

	while(num--)
		filter_close(se);
}

template <typename T>
void cls_image_ot<T>::filter_open_squr(int num)
{
	int se[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};

	while(num--)
		filter_open(se);
}

template <typename T>
void cls_image_ot<T>::filter_dilate_squr(int num)
{
	int se[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};

	while(num--)
		filter_dilate(se);
}

template <typename T>
void cls_image_ot<T>::filter_erode_squr(int num)
{
	int se[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};

	while(num--)
		filter_erode(se);
}

template <typename T>
void cls_image_ot<T>::filter_close_cross(int num)
{
	int se[3][3] = {
		{-1,1,-1},
		{1,1,1},
		{-1,1,-1}
	};

	while(num--)
		filter_close(se);
}

template <typename T>
void cls_image_ot<T>::filter_open_cross(int num)
{
	int se[3][3] = {
		{-1,1,-1},
		{1,1,1},
		{-1,1,-1}
	};

	while(num--)
		filter_open(se);
}

template <typename T>
void cls_image_ot<T>::filter_dilate_cross(int num)
{
	int se[3][3] = {
		{-1,1,-1},
		{1,1,1},
		{-1,1,-1}
	};

	while(num--)
		filter_dilate(se);
}

template <typename T>
void cls_image_ot<T>::filter_erode_cross(int num)
{
	int se[3][3] = {
		{-1,1,-1},
		{1,1,1},
		{-1,1,-1}
				   };

	while(num--)
		filter_erode(se);
}

template <typename T>
void cls_image_ot<T>::tools_fill_connect_regin()
{
	vector<cls_rect> v_rect;
	vector<int> v_n;
	vector<cls_rect> v_r_rect;
	vector<int> v_r_n;

	cls_image_ot<T> img_barkup = *this;
	cls_image_ot<T> img_src = *this;
	img_src.tools_mark_connection_region(v_rect,v_n);

	img_barkup.tools_reverse();
	img_barkup.tools_mark_connection_region(v_r_rect,v_r_n);

	for(int i=0;i<v_rect.size();i++)
	{
		for(int j=0;j<v_r_rect.size();j++)
		{
			if (v_rect[i].rect_in_rect(v_r_rect[j]))
			{
				for(int ii=0;ii<get_width();ii++)
				{
					for(int ij=0;ij<get_height();ij++)
					{
						if (img_barkup.get_pixel(ii,ij) == j)
							img_src.set_pixel(ii,ij,i);
					}
				}
			}
		}
	}

	*this = img_src;
}

template <typename T>
void cls_image_ot<T>::tools_mark_connection_region(int &pnum)
{
	int sum = 1;
	for(int c=0;c<get_height();c++)
	{
		bool check_in_region = false;
		for(int r=0;r<get_width();r++)
		{
			if (get_gray(r,c) == 0)
			{
				set_pixel(r,c,sum);
				check_in_region = true;
			}
			else if (check_in_region == true)
			{
				sum++;
				check_in_region = false;
			}
		}
	}

	while(!check_connection_region())
	{
		bool check_in_region = false;
		int n_pos = 0;
		int n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));
		for(int r=0;r<get_width();r++)
		{
			if (check_in_region == true)
			{
				for(int cnt=n_pos;cnt<get_height();cnt++)
				{
					set_pixel(r-1,cnt,n_min);
				}
			}

			check_in_region = false;
			n_pos = 0;
			n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

			for(int c=0;c<get_height();c++)
			{
				if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)))
				{
					if (check_in_region == false)
					{
						n_min = get_pixel(r,c);
						n_pos = c;
						check_in_region = true;
					}

					if (n_min >= get_pixel(r,c))
					{
						n_min = get_pixel(r,c);
					}
				}
				else if (check_in_region == true)
				{
					for(int cnt=n_pos;cnt<c;cnt++)
					{
						set_pixel(r,cnt,n_min);
					}

					check_in_region = false;
				}
			}
		}

		check_in_region = false;
		n_pos = 0;
		n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

		for(int c=0;c<get_height();c++)
		{
			if (check_in_region == true)
			{
				for(int cnt=n_pos;cnt<get_width();cnt++)
				{
					set_pixel(cnt,c-1,n_min);
				}
			}

			check_in_region = false;
			n_pos = 0;
			n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

			for(int r=0;r<get_width();r++)
			{
				if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)))
				{
					if (check_in_region == false)
					{
						n_min = get_pixel(r,c);
						n_pos = r;
						check_in_region = true;
					}

					if (n_min >= get_pixel(r,c))
					{
						n_min = get_pixel(r,c);
					}
				}
				else if (check_in_region == true)
				{
					for(int cnt=n_pos;cnt<r;cnt++)
					{
						set_pixel(cnt,c,n_min);
					}

					check_in_region = false;
				}
			}
		}
	}

	int cur_num = 0;
	map<int,int> reset_num;

	for(int c=0;c<get_height();c++)
	{
		for(int r=0;r<get_width();r++)
		{
			if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)) && reset_num.find(get_pixel(r,c)) == reset_num.end())
			{
				reset_num.insert(make_pair(get_pixel(r,c),cur_num++));
			}
		}
	}

	for(int c=0;c<get_height();c++)
	{
		for(int r=0;r<get_width();r++)
		{
			if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)) )
			{
				int num = reset_num.find((int)get_pixel(r,c))->second;
				set_pixel(r,c,num);
			}
		}
	}

	pnum = cur_num;
}

template <typename T>
cls_point<T> cls_image_ot<T>::tools_get_connect_region_center_point( int num )
{
	double x = 0.0,y = 0.0;
	int num_to = 0;

	for(int col=0;col<get_height();col++)
	{
		for(int row=0;row<get_width();row++)
		{
			int cur_num = get_pixel(row,col);
			if (cur_num == num)
			{
				num_to ++;
				x += row;
				y += col;
			}
		}
	}

	return cls_point<T>(x/num_to,y/num_to);
}

template <typename T>
bool cls_image_ot<T>::check_connection_region()
{
	for(int c=1;c<get_height()-1;c++)
	{
		for(int r=1;r<get_width()-1;r++)
		{
			if (get_pixel(r,c) == (unsigned)((255<<24) + (255<<8) + (255<<16)))
				continue;

			if (get_pixel(r+1,c) !=  (unsigned)((255<<24) + (255<<8) + (255<<16)) && get_pixel(r+1,c)!= get_pixel(r,c))
			{
				return false;
			}

			if (get_pixel(r,c+1) !=  (unsigned)((255<<24) + (255<<8) + (255<<16)) && get_pixel(r,c+1)!= get_pixel(r,c))
			{
				return false;
			}

			if (get_pixel(r-1,c) !=  (unsigned)((255<<24) + (255<<8) + (255<<16)) && get_pixel(r-1,c)!= get_pixel(r,c))
			{
				return false;
			}

			if (get_pixel(r,c-1) !=  (unsigned)((255<<24) + (255<<8) + (255<<16)) && get_pixel(r,c-1)!= get_pixel(r,c))
			{
				return false;
			}
		}
	}

	return true;
}

template <typename T>
void cls_image_ot<T>::tools_mark_connection_region( vector<cls_rect>&rect,vector<int>& pnum )
{
	int sum = 1;
	for(int c=0;c<get_height();c++)
	{
		bool check_in_region = false;
		for(int r=0;r<get_width();r++)
		{
			if (get_gray(r,c) == 0)
			{
				set_pixel(r,c,sum);
				check_in_region = true;
			}
			else if (check_in_region == true)
			{
				sum++;
				check_in_region = false;
			}
		}
	}

	while(!check_connection_region())
	{
		bool check_in_region = false;
		int n_pos = 0;
		int n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));
		for(int r=0;r<get_width();r++)
		{
			if (check_in_region == true)
			{
				for(int cnt=n_pos;cnt<get_height();cnt++)
				{
					set_pixel(r-1,cnt,n_min);
				}
			}

			check_in_region = false;
			n_pos = 0;
			n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

			for(int c=0;c<get_height();c++)
			{
				if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)))
				{
					if (check_in_region == false)
					{
						n_min = get_pixel(r,c);
						n_pos = c;
						check_in_region = true;
					}

					if (n_min >= get_pixel(r,c))
					{
						n_min = get_pixel(r,c);
					}
				}
				else if (check_in_region == true)
				{
					for(int cnt=n_pos;cnt<c;cnt++)
					{
						set_pixel(r,cnt,n_min);
					}

					check_in_region = false;
				}
			}
		}

		check_in_region = false;
		n_pos = 0;
		n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

		for(int c=0;c<get_height();c++)
		{
			if (check_in_region == true)
			{
				for(int cnt=n_pos;cnt<get_width();cnt++)
				{
					set_pixel(cnt,c-1,n_min);
				}
			}

			check_in_region = false;
			n_pos = 0;
			n_min = (unsigned)((255<<24) + (255<<8) + (255<<16));

			for(int r=0;r<get_width();r++)
			{
				if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)))
				{
					if (check_in_region == false)
					{
						n_min = get_pixel(r,c);
						n_pos = r;
						check_in_region = true;
					}

					if (n_min >= get_pixel(r,c))
					{
						n_min = get_pixel(r,c);
					}
				}
				else if (check_in_region == true)
				{
					for(int cnt=n_pos;cnt<r;cnt++)
					{
						set_pixel(cnt,c,n_min);
					}

					check_in_region = false;
				}
			}
		}
	}

	int cur_num = 0;
	map<int,int> reset_num;

	for(int c=0;c<get_height();c++)
	{
		for(int r=0;r<get_width();r++)
		{
			if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)) && reset_num.find(get_pixel(r,c)) == reset_num.end())
			{
				cls_rect tmp;
				rect.push_back(tmp);

				rect[cur_num].left = get_width();
				rect[cur_num].right = 0;
				rect[cur_num].top = get_height();
				rect[cur_num].bottom = 0;

				reset_num.insert(make_pair(get_pixel(r,c),cur_num++));
			}
		}
	}

	rect.resize(cur_num);
	pnum.resize(cur_num);

	for(int c=0;c<get_height();c++)
	{
		for(int r=0;r<get_width();r++)
		{
			if (get_pixel(r,c) != (unsigned)((255<<24) + (255<<8) + (255<<16)) )
			{
				int num = reset_num.find((int)get_pixel(r,c))->second;
				set_pixel(r,c,num);

				pnum[num]++;

				if (rect[num].left > r)
					rect[num].left = r;

				if (rect[num].right < r)
					rect[num].right = r;

				if (rect[num].top > c)
					rect[num].top = c;

				if (rect[num].bottom < c)
					rect[num].bottom = c;
			}
		}
	}
}

template <typename T>
cls_image_ot<T> cls_image_ot<T>::trans_bi_stretch( cls_rect rect_des )
{
	return trans_bi_stretch(cls_rect(0,0,get_height(),get_width()),rect_des);
}

template <typename T>
cls_image_ot<T> cls_image_ot<T>::trans_bi_stretch( cls_rect rect_src,cls_rect rect_des )
{
	cls_image_ot<T> img_barkup = *this;
	img_barkup.resize(rect_des.get_width(),rect_des.get_height());
	img_barkup.tools_fill(get_rgb(255,255,255));

	for(int i=0;i<rect_des.get_width();i++)
	{
		for(int j=0;j<rect_des.get_height();j++)
		{
			byte p;
			float x = float(i)/float(rect_des.get_width())*float(rect_src.get_width()) + rect_src.left;
			float y = float(j)/float(rect_des.get_height())*float(rect_src.get_height()) + rect_src.top;

			int a = int(x);
			int b = int(y);

			if (a>=rect_src.right)
				x = rect_src.right - 2;
			if (b>=rect_src.bottom)
				y = rect_src.bottom - 2;

			byte b00 = get_gray(int(x),int(y));
			byte b01 = get_gray(int(x),int(y)+1);
			byte b10 = get_gray(int(x)+1,int(y));
			byte b11 = get_gray(int(x)+1,int(y)+1);

			double re = 0.0;
			double re1 = 0.0;
			double re2 = 0.0;

			re1 = (x-a)*b11 + (a+1-x)*b01;
			re2 = (x-a)*b10 + (a+1-x)*b00;
			re = (b+1-y)*re2 + (y-b)*re1;

			if (re > 255)
				p = 255;
			else if (re < 0)
				p = 0;
			else
				p = byte(re+0.5);

			img_barkup.set_gray(i,j,p);
		}
	}

	return img_barkup;
}

template <typename T>
void cls_image_ot<T>::feature_plane( vector<double>&re )
{
	re.resize(get_width() * get_height());

	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (get_gray(i,j) == 255)
				re[i*get_height() + j] = 1;
			else
				re[i*get_height() + j] = 0;
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_place( cls_image_ot<T>& that,int x,int y )
{
	int len_width = min(that.get_width(),get_width()-x);
	int len_height = min(that.get_height(),get_height()-y);

	for(int i=0;i<len_width;i++)
	{
		for(int j=0;j<len_height;j++)
		{
			this->set_pixel(x+i,y+j,that.get_pixel(i,j));
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_merge( cls_image_ot & that )
{
	cls_image_ot<T> img(get_width() + that.get_width(),get_height() + that.get_height());
	img.tools_place(*this,0,0);
	img.tools_place(that,get_width(),0);

	*this = img;
}

template <typename T>
void cls_image_ot<T>::filter_threshod( int thd )
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			if (get_gray(i,j) < thd)
				set_gray(i,j,0);
			else
				set_gray(i,j,255);
		}
	}
}

template <typename T>
void cls_image_ot<T>::app_get_project_h( vector<cls_image_ot<T> > &re,double d_precent ,int min_inter)
{
	vector<int> v_n;
	tools_get_project_h_spilt(v_n,d_precent,min_inter);

	int pos = 0;
	for(int cnt=0;cnt<v_n.size();cnt++)
	{
		while(cnt <v_n.size() && pos == v_n[cnt])
			cnt++;

		if (cnt == v_n.size())
			break;
		re.push_back(tools_part(cls_rect(0,pos,get_height(),v_n[cnt])));
		pos = v_n[cnt];
	}

	re.push_back(tools_part(cls_rect(0,pos,get_height(),get_width())));
}

template <typename T>
void cls_image_ot<T>::app_get_project_v( vector<cls_image_ot<T> > &re,double d_precent ,int min_inter)
{
	vector<int> v_n;
	tools_get_project_v_spilt(v_n,d_precent,min_inter);

	int pos = 0;
	for(int cnt=0;cnt<v_n.size();cnt++)
	{
		while(cnt <v_n.size() && pos == v_n[cnt])
			cnt++;

		if (cnt == v_n.size())
			break;

		re.push_back(tools_part(cls_rect(pos,0,v_n[cnt],get_width())));
		pos = v_n[cnt];
	}

	re.push_back(tools_part(cls_rect(pos,0,get_height(),get_width())));
}

template <typename T>
void cls_image_ot<T>::tools_to_gray()
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			set_gray(i,j,get_gray(i,j));
		}
	}
}

template <typename T>
cls_image_ot<T>::cls_image_ot( int w,int h ):mat_image(w,h)
{
	;
}

template <typename T>
void cls_image_ot<T>::tools_get_project_h_spilt(vector<int> & re,double d_percent,int min_inter)
{
	int* ar_n_data = new int[get_width()];
	tools_get_project_h(ar_n_data);

	int n_least = d_percent *get_height();

	vector<int> v_n;
	for(int cnt=0;cnt<get_width();cnt++)
	{
		if (ar_n_data[cnt] <= n_least)
			v_n.push_back(cnt);
	}

	if (v_n.size() == 0)
		return ;

	re.push_back(v_n[0]);
	for(int cnt=1;cnt<v_n.size();cnt++)
	{
		if (v_n[cnt] - v_n[cnt-1] > min_inter)
			re.push_back(v_n[cnt]);
	}

	delete [] ar_n_data;
}

template <typename T>
void cls_image_ot<T>::tools_get_project_h( int *ar_n_data )
{
	memset(ar_n_data,0,sizeof(int)*get_width());

	for(int cnt=0;cnt<get_width();cnt++)
	{
		for(int cnti=0;cnti<get_height();cnti++)
		{
			if (get_gray(cnt,cnti) == 0)
				ar_n_data[cnt]++;
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_get_project_v_spilt(vector<int> & re,double d_percent,int min_inter)
{
	int * ar_n_data = new int[mat_image.get_height()];
	tools_get_project_v(ar_n_data);

	int n_least = d_percent * get_width();

	vector<int> v_n;
	for(int cnt=0;cnt<get_height();cnt++)
	{
		if (ar_n_data[cnt] < n_least)
			v_n.push_back(cnt);
	}

	if (v_n.size() == 0)
		return ;

	re.push_back(v_n[0]);
	for(int cnt=1;cnt<v_n.size();cnt++)
	{
		if (v_n[cnt] - v_n[cnt-1] > min_inter)
			re.push_back(v_n[cnt]);
	}

	delete [] ar_n_data;
}

template <typename T>
void cls_image_ot<T>::tools_get_project_v( int *ar_n_data )
{
	memset(ar_n_data,0,sizeof(int)*get_height());

	for(int cnt=0;cnt<get_height();cnt++)
	{
		for(int cnti=0;cnti<get_width();cnti++)
		{
			if (get_gray(cnti,cnt) == 0)
				ar_n_data[cnt]++;
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_get_min_hist( byte *ady, int *n ) const
{
	vector<double> dHist(256);
	get_hist_gray_normal(dHist,1);

	double Pre = 0.0;
	double Post;
	*n=0;

	for(int i=1;i<255;i++)
	{
		int j=i + 1;
		for(j=i + 1;j<256;j++)
		{
			if (dHist[j] > 0.005)
			{
				Post = dHist[j];
				break;
			}
		}

		if (j == 256)
			break;

		if (dHist[i] > 0.005)
		{
			if (dHist[i] < Pre && dHist[i] > Post)
			{
				ady[(*n)++] = i;
			}
			Pre = dHist[i];
		}
		else
		{
			continue;
		}
	}
}

template <typename T>
byte cls_image_ot<T>::tools_get_mid_hist( double d_mu ) const
{
	vector<double> dHist;
	get_hist_gray_normal(dHist,1);
	double dMutiHist[256];

	dMutiHist[0] = dHist[0];
	for(int i=1;i<256;i++)
	{
		dMutiHist[i] = dMutiHist[i-1] + dHist[i];
	}

	double dMid = 1.0;
	int nMid = 0;
	for(int i=1;i<256;i++)
	{
		if (fabs(dMutiHist[i] - d_mu) < dMid)
		{
			dMid = fabs(dMutiHist[i] - d_mu);
			nMid = i;
		}
	}

	return nMid - 1;
}

template <typename T>
double cls_image_ot<T>::app_get_likely_percent(cls_image_ot<T>& img)
{
	cls_rect rctImg = img.tools_get_real_rect();
	cls_rect rct = tools_get_real_rect();

	int sum = 0;
	for(int i=rct.left;i<rct.right;i++)
	{
		for(int j=rct.top;j<rct.bottom;j++)
		{
			byte byOrg = get_gray(i,j);
			byte byImg = img.get_gray(int(float(i-rct.left)/float(rct.get_width())*float(rctImg.get_width()))+rctImg.left,int(float(j-rct.top)/float(rct.get_height())*float(rctImg.get_height()))+rctImg.top);

			sum += (byImg - byOrg) * (byImg - byOrg) / 256.0 / 256.0;
		}
	}

	double re = double(sum)/double(rct.get_width()*rct.get_height());

	return re;
}

template <typename T>
cls_rect cls_image_ot<T>::tools_get_real_rect()
{
	cls_rect rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = 0;
	rect.bottom = 0;

	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			byte by = get_gray(i,j);

			if (by != 255)
			{
				if (rect.left == 0)
				{
					rect.left = i;
				}

				if (rect.right < i)
				{
					rect.right = i;
				}
			}
		}
	}

	for(int i=0;i<get_height();i++)
	{
		for(int j=0;j<get_width();j++)
		{
			byte by = get_gray(j,i);

			if (by != 255)
			{
				if (rect.top == 0)
				{
					rect.top = i;
				}

				if (rect.bottom < i)
				{
					rect.bottom = i;
				}
			}
		}
	}

	return rect;
}

template <typename T>
cls_image_ot<T>  cls_image_ot<T>::tools_part(cls_rect rect)
{
	if (rect.left < 0)
		rect.left = 0;
	if (rect.top  < 0)
		rect.top = 0;
	if (rect.bottom >= get_height())
		rect.bottom = get_height();
	if (rect.right >= get_width())
		rect.right = get_width();

	cls_image_ot<T> res(rect.get_width(),rect.get_height());

	for(int i=0;i<rect.get_width();i++)
	{
		for(int j=0;j<rect.get_height();j++)
		{
			res.set_pixel(i,j,this->get_pixel(i+rect.left,j+rect.top));
		}
	}

	return res;
}

template <typename T>
void cls_image_ot<T>::tools_get_connect_region_pixel_count( byte by_start,byte by_end )
{
	int sum = 0;

	for(int i=0;i<this->get_width();i++)
	{
		for(int j=0;j<this->get_height();j++)
		{
			byte by = get_gray(i,j);
			if (by >= by_start && by<= by_end)
				sum++;
		}
	}

	return sum;
}

template <typename T>
void cls_image_ot<T>::tools_get_connect_region_rect(vector<cls_rect> & re )
{
	re.clear();

	vector<cls_rect> rect;
	rect.resize(256);
	for(int i=0;i<256;i++)
	{
		rect[i].top = 0;
		rect[i].bottom = 0;
		rect[i].left = 0;
		rect[i].right = 0;
	}

	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			byte by = get_gray(i,j);

			if (rect[by].left == 0)
				rect[by].left = i;

			if (rect[by].right < i)
				rect[by].right = i;
		}
	}

	for(int i=0;i<get_height();i++)
	{
		for(int j=0;j<get_width();j++)
		{
			byte by = get_gray(j,i);

			if (rect[by].top == 0)
				rect[by].top = i;

			if (rect[by].bottom < i)
				rect[by].bottom = i;
		}
	}

	for(int cnt=0;cnt<256;cnt++)
	{
		if (rect[cnt].get_width() != 0 && rect[cnt].get_height() != 0)
			re.push_back(rect[cnt]);
	}
}

template <typename T>
void cls_image_ot<T>::tools_except_border( byte by_color )
{
	for(int i=0;i<this->get_width();i++)
	{
		set_gray(i,0,by_color);
	}

	for(int i=0;i<this->get_width();i++)
	{
		set_gray(i,get_height()-1,by_color);
	}

	for(int j=0;j<this->get_height();j++)
	{
		set_gray(0,j,by_color);
	}

	for(int j=0;j<this->get_height();j++)
	{
		set_gray(get_width()-1,j,by_color);
	}
}

template <typename T>
void cls_image_ot<T>::filter_top_hat( int n_w,int n_h,int n_mw,int n_mh,int **se )
{
	cls_image_ot<T> ib;
	ib = *this;

	ib.filter_gray_open(n_w,n_h,n_mw,n_mh,se);
	*this = *this - ib;
}

template <typename T>
void cls_image_ot<T>::filter_gray_close( int n_w,int n_h,int n_mw,int n_mh,int **se )
{
	cls_image_ot<T> ib;
	ib = *this;

	ib.filter_gray_dilate(n_w,n_h,n_mw,n_mh,se);
	ib.filter_gray_erode(n_w,n_h,n_mw,n_mh,se);

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_gray_open( int n_w,int n_h,int n_mw,int n_mh,int **se )
{
	cls_image_ot<T> ib;
	ib = *this;

	ib.filter_gray_erode(n_w,n_h,n_mw,n_mh,se);
	ib.filter_gray_dilate(n_w,n_h,n_mw,n_mh,se);

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_gray_erode( int n_w,int n_h,int n_mw,int n_mh,int **se )
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ib;
	ib = *this;

	for(int i=n_mh;i<nHeight - n_h + n_mh + 1;i++)
	{
		for(int j=n_mw;j<nWidth - n_w + n_mw + 1;j++)
		{
			byte maxval = 255;
			for(int k=0;k<n_mh;k++)
			{
				for(int l=0;l<n_mw;l++)
				{
					if (se[k][l] == 1)
					{
						byte gray = get_gray(j - n_mh + l,i - n_mw + k);

						if (gray < maxval)
						{
							maxval = gray;
						}
					}
				}

				ib.set_gray(j,i,maxval);
			}
		}
	}

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_gray_dilate( int n_w,int n_h,int n_mw,int n_mh,int **se )
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ib;
	ib = *this;

	for(int i=n_mh;i<nHeight - n_h + n_mh + 1;i++)
	{
		for(int j=n_mw;j<nWidth - n_w + n_mw + 1;j++)
		{
			byte maxval = 0;
			for(int k=0;k<n_mh;k++)
			{
				for(int l=0;l<n_mw;l++)
				{
					if (se[k][l] == 1)
					{
						byte gray = get_gray(j - n_mh + l,i - n_mw + k);

						if (gray > maxval)
						{
							maxval = gray;
						}
					}
				}

				ib.set_gray(j,i,maxval);
			}
		}
	}

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_convex( bool b_constrained )
{
	int se1[3][3] = { {1,-1,-1},{1,0,-1},{1,-1,-1} };
	int se2[3][3] = { {1,1,1},{-1,0,-1},{-1,-1,-1} };
	int se3[3][3] = { {-1,-1,1},{-1,0,1},{-1,-1,1} };
	int se4[3][3] = { {-1,-1,-1},{-1,0,-1},{1,1,1} };

	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ib;
	ib = *this;

	cls_image_ot<T> ibTemp1;
	ibTemp1 = (*this);
	ib = (*this);

	while(true)
	{
		ib.filter_erode(se1);
		ib = ib  | ibTemp1;

		if (ib == ibTemp1)
			break;

		ibTemp1 = ib;
	}

	cls_image_ot<T> ibTemp2;
	ibTemp2 = (*this);
	ib = (*this);

	while(true)
	{
		ib.filter_erode(se2);
		ib = ib  | ibTemp2;

		if (ib == ibTemp2)
			break;

		ibTemp2 = ib;
	}

	cls_image_ot<T> ibTemp3;
	ibTemp3 = (*this);
	ib = (*this);

	while(true)
	{
		ib.filter_erode(se3);
		ib = ib  | ibTemp3;

		if (ib == ibTemp3)
			break;

		ibTemp3 = ib;
	}

	cls_image_ot<T> ibTemp4;
	ibTemp4 = (*this);
	ib = (*this);

	while(true)
	{
		ib.filter_erode(se4);
		ib = ib  | ibTemp4;

		if (ib == ibTemp4)
			break;

		ibTemp4 = ib;
	}

	ib = *this;

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			if ( ibTemp1.get_gray(j,i) == 0 || ibTemp2.get_gray(j,i) == 0 || ibTemp3.get_gray(j,i) == 0 || ibTemp4.get_gray(j,i) == 0)
			{
				ib.set_gray(j,i,0);
			}
		}
	}

	int nTop = nHeight;
	int nBottom = 0;
	int nLeft = nWidth;
	int nRight = 0;

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			if ( get_gray(j,i) == 0 )
			{
				if (i < nTop)
					nTop = i;
				if (i > nBottom)
					nBottom = i;
				if (j < nLeft)
					nLeft = j;
				if (j > nRight)
					nRight = j;
			}
		}
	}

	if (b_constrained)
	{
		for(int i=0;i<nHeight;i++)
		{
			for(int j=0;j<nWidth;j++)
			{
				if (i < nTop || i >nBottom || j <nLeft || j>nRight)
				{
					ib.set_pixel(j,i,255);
				}
			}
		}
	}

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_threshod( int low,int high )
{
	double sumX[256];
	double sumY[256];
	double sumPt[256];

	int nHeight = get_height();
	int nWidth = get_width();

	for(int i=0;i<256;i++)
	{
		sumX[i] = 0;
		sumY[i] = 0;
		sumPt[i] = 0;
	}

	for(int i = 0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			byte gray = get_gray(j,i);
			sumX[gray] += j;
			sumY[gray] += i;
			sumPt[gray] ++;
		}
	}

	cls_image_ot<T> ib;
	ib = *this;

	for(int i=0;i<256;i++)
	{
		if (sumPt[i] != 0)
		{
			if (sumPt[i] > high && sumPt[i] <low)
				break;

			sumX[i] = sumX[i] / sumPt[i];
			sumY[i] = sumY[i] / sumPt[i];

			ib.set_gray((int)(sumX[i] + 0.5),(int)(sumY[i] + 0.5),0);
		}
	}

	this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_thin()
{
	int nHeight = get_height();
	int nWidth = get_width();

	bool b1;
	bool b2;
	bool b3;
	bool b4;

	byte neighbour[5][5];

	cls_image_ot<T> ibPic;

	bool bModified = true;
	while(bModified)
	{
		bModified = false;
		ibPic = *this;

		for(int j=2;j<nHeight - 2;j++)
		{
			for(int i=2;i<nWidth - 2;i++)
			{
				b1 = false;
				b2 = false;
				b3 = false;
				b4 = false;

				byte data = get_gray(i,j);
				if (data == 255)
					continue;

				for(int m=0;m<5;m++)
				{
					for(int n=0;n<5;n++)
					{
						neighbour[m][n] = (get_gray(i+n-2,j+m-2) == 0);
					}
				}

				int nCount = neighbour[1][1] + neighbour[1][2] + neighbour[1][3] +neighbour[2][1]+neighbour[2][3]+neighbour[3][1]+neighbour[3][2]+neighbour[3][3];

				if (nCount >= 2 && nCount <=6)
					b1 = true;

				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					b2 = true;

				if (neighbour[1][2] * neighbour[2][1] * neighbour[2][3] == 0)
				{
					b3 = true;
				}
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount ++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount ++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount ++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount ++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount ++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount ++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount ++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount ++;
					if (nCount != 1)
						b3 = true;
				}

				if (neighbour[1][2] * neighbour[2][1] * neighbour[3][2] == 0)
				{
					b4 = true;
				}
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						b4 = true;
				}

				if (b1 && b2 && b3 && b4)
				{
					ibPic.set_gray(i,j,255);
				}
				else
				{
					ibPic.set_gray(i,j,0);
				}
			}
		}

		*this = ibPic;
	}

}

template <typename T>
void cls_image_ot<T>::analyze_classic_label_connect_region( int n_conn,int n_min )
{
	int se[3][3] = { {1,1,1},{1,1,1},{1,1,1} };

	if (n_conn == 4)
	{
		se[0][0] = -1;
		se[0][2] = -1;
		se[2][0] = -1;
		se[2][2] = -1;
	}

	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ibOrg;
	ibOrg = *this;

	int nConnRgn = 1;
	int nGray ;

	for(int i=0;i<nHeight;i++)
	{
		set_gray(0,i,255);
		set_gray(nWidth - 1,i,255);
	}

	for(int j=0;j<nWidth;j++)
	{
		set_gray(j,0,255);
		set_gray(j,nHeight - 1,255);
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			nGray = get_gray(j,i);

			if (nGray == 0)
			{
				cls_image_ot<T> ibTo;
				cls_image_ot<T> ibTemp;

				ibTo = *this;
				ibTo.set_gray(j,i,0);
				ibTemp = ibTo;

				while(true)
				{
					ibTo.filter_dilate(se);
					ibTo = ibTo | ibOrg;

					if (ibTo == ibTemp)
						break;

					ibTemp = ibTo;
				}

				for(int k=0;k<nHeight;k++)
				{
					for(int l=0;l<nWidth;l++)
					{
						nGray = ibTo.get_gray(l,k);
						if (nGray == 0)
						{
							set_gray(l,k,nConnRgn);
						}
					}
				}

				nConnRgn ++;
				if (nConnRgn > 255)
					nConnRgn = 1;
			}
		}
	}
}

template<typename T>
bool cls_image_ot<T>::operator==(const cls_image_ot<T> & ib)
{
	if (ib.get_height() != get_height() || ib.get_width() != get_width())
		return false;

	for(int cnt_w=0;cnt_w<get_width();cnt_w++)
	{
		for(int cnt_h=0;cnt_h<get_height();cnt_h++)
		{
			if (get_pixel(cnt_w,cnt_h) != ib.get_pixel(cnt_w,cnt_h))
				return false;
		}
	}

	return true;
}

template <typename T>
void cls_image_ot<T>::tools_reverse()
{
	int nHeight = get_height();
	int nWidth = get_width();

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			byte pixel = get_gray(j,i);
			set_gray(j,i,255-pixel);
		}
	}
}

template <typename T>
void cls_image_ot<T>::tools_fill_region( int x,int y )
{
	int se[3][3] = { {-1,1,-1},{1,1,1},{-1,1,-1} };

	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ibRev;
	ibRev = *this;
	ibRev.inverse();

	cls_image_ot<T> ibTo;
	ibTo = *this;
	ibTo.set_gray(x,y,0);

	cls_image_ot<T> ibPreTo;
	ibPreTo = ibTo;

	while(true)
	{
		ibTo.filter_dilate(se);
		ibTo = ibTo | ibRev;

		if (ibTo == ibPreTo)
			break;

		ibPreTo = ibTo;
	}

	*this = ibTo;
}

template <typename T>
cls_image_ot<T>& cls_image_ot<T>::operator -(const cls_image_ot<T> & ib)
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ibt;
	ibt = *this;

	if (ib->get_height() != this->get_height() || this->get_width() != ib->get_width())
	{
		return ;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			byte pixelB = ib->get_gray(j,i);
			byte pixelA = this->get_gray(j,i);

			if (pixelA >= pixelB)
			{
				ibt.set_gray(j,i,get_rgb(pixelA - pixelB,pixelA - pixelB,pixelA - pixelB));
			}
			else
			{
				ibt.set_gray(j,i,get_rgb(0,0,0));
			}
		}
	}

	*this = ibt;
}

template <typename T>
cls_image_ot<T>& cls_image_ot<T>::operator &(const cls_image_ot<T> & ib)
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ibt;
	ibt = *this;

	if (ib.get_height() != this->get_height() || this->get_width() != ib.get_width())
	{
		return *this;
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			byte pixelB = ib.get_gray(j,i);
			byte pixelA = this->get_gray(j,i);

			if (pixelA != 0 && pixelB != 0)
			{
				ibt.set_pixel(j,i,(255<<8) + (255<<16) + (255<<24));
			}
			else
			{
				ibt.set_pixel(j,i,0);
			}
		}
	}

	*this = ibt;
	return *this;
}

template <typename T>
cls_image_ot<T>& cls_image_ot<T>::operator | (const cls_image_ot<T> & ib)
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ibt;
	ibt.init_pixel(nWidth,nHeight);

	if (ib->get_height() != this->get_height() || this->get_width() != ib->get_width())
	{
		return cls_image_ot<T>();
	}

	for(int i=0;i<nHeight;i++)
	{
		for(int j=0;j<nWidth;j++)
		{
			byte pixelB = ib->get_grayl(j,i);
			byte pixelA = this->get_grayl(j,i);

			if (pixelA != 0 || pixelB != 0)
			{
				ibt.set_gray(j,i,255);
			}
			else
			{
				ibt.set_gray(j,i,0);
			}
		}
	}

	*this = ibt;
}

template <typename T>
void cls_image_ot<T>::filter_trace_boundary()
{
	cls_image_ot<T> ib;
	ib = *this;

	int nHeight = get_height();
	int nWidth = get_width();

	int Direction[8][2] =  { {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0} };

	int nBeginDirection;
	int nCurDirection = 0;
	int posX,posY;
	bool bAtStartPt;

	cls_point<int> ptStart(2,2);
	cls_point<int> ptCur(2,2);

	for(int i=0; i<nHeight ;i++)
	{
		ib.set_gray(0,i,255);
		ib.set_gray(nWidth - 1,i,255);
	}

	for(int j=0;j<nWidth;j++)
	{
		ib.set_gray(j,0,255);
		ib.set_gray(nHeight- 1,j,255);
	}

	for(int i=1;i<nHeight;i++)
	{
		for(int j=1;j<nWidth;j++)
		{
			if (get_gray(j,i) == 0)
			{
				ptStart.x = j;
				ptStart.y = i;

				ptCur = ptStart;
				bAtStartPt = true;

				while( ( (ptCur.x != ptStart.x) || (ptStart.y!= ptCur.x) ) || bAtStartPt)
				{
					bAtStartPt = false;

					posX = ptCur.x + Direction[nCurDirection][0];
					posY = ptCur.y + Direction[nCurDirection][1];

					int nSearchTimes = 1;
					while( get_gray(posX,posY) == 255 )
					{
						nCurDirection ++ ;
						if (nCurDirection >=8)
							nCurDirection -= 8;

						posX = ptCur.x + Direction[nCurDirection][0];
						posY = ptCur.y + Direction[nCurDirection][1];

						nSearchTimes++;
						if (nSearchTimes >= 8)
						{
							posX = ptCur.x;
							posY = ptCur.y;
							break;
						}
					}

					ptCur.x = posX;
					ptCur.y = posY;

					ib.set_gray(ptCur.x,ptCur.y,0);

					nCurDirection -= 2;
					if (nCurDirection <0)
						nCurDirection += 8;
				}

				*this = ib;

				return true;
			}
		}
	}
}

template <typename T>
void cls_image_ot<T>::filter_close( int se[3][3] )
{
	this->filter_dilate(se);
	this->filter_erode(se);
}

template <typename T>
void cls_image_ot<T>::filter_open( int se[3][3] )
{
	this->filter_erode(se);
	this->filter_dilate(se);
}

template <typename T>
void cls_image_ot<T>::filter_dilate( int se[3][3] )
{
	int nHeight = get_height();
	int nWidth = get_width();

	int nTemp;
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<3-i;j++)
		{
			nTemp = se[i][j];
			se[i][j] = se[2-i][2-j];
			se[2-i][2-j] = nTemp;
		}
	}

	cls_image_ot<T> ib(get_width(),get_height());

	bool bMatch;
	for(int i=1;i<nHeight - 1;i++)
	{
		for(int j=1;j<nWidth - 1;j++)
		{
			bMatch = false;

			for(int k=0;k<3;k++)
			{
				for(int l=0;l<3;l++)
				{
					if (se[k][l] == -1)
						continue;

					if (se[k][l] == 1)
					{
						if (get_gray(j-1+l,i-1+k) == 0)
						{
							ib.set_gray(j,i,0);
							bMatch = true;
							break;
						}
					}
					else
					{
						;
					}
				}
			}

			if (!bMatch)
				ib.set_gray(j,i,255);
		}
	}

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_erode( int se[3][3] )
{
	int nHeight = get_height();
	int nWidth = get_width();

	cls_image_ot<T> ib(get_width(),get_height());

	for(int i=1;i<nHeight - 1;i++)
	{
		for(int j=1;j<nWidth - 1;j++)
		{
			bool bMatch = true;
			for(int k=0;k<3;k++)
			{
				for(int l=0;l<3;l++)
				{
					if (se[k][l] == -1)
						continue;
					if (se[k][l] == 1)
					{
						if (get_gray(j-1+l,i-1+k) != 0)
						{
							bMatch = false;
							break;
						}
					}
					else if (se[k][l] == 0)
					{
						if (get_gray(j-1+l,i-1+k) != 255)
						{
							bMatch = false;
							break;
						}
					}
					else
					{
						;
					}
				}

			}

			if (bMatch)
				ib.set_gray(j,i,0);
			else
				ib.set_gray(j,i,255);
		}
	}

	*this = ib;
}

template <typename T>
void cls_image_ot<T>::filter_freq_laplace_hpf()
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	double *d_filter = new double[w * h];

	template_freq_laplace_hpf(d_filter);

	filter_freq_template(d_filter);

	delete[] d_filter;
}

template <typename T>
void cls_image_ot<T>::filter_freq_gauss_hpf( double d_sigma )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	double *d_filter = new double[w * h];

	template_freq_gauss_hpf(d_filter,d_sigma);

	filter_freq_template(d_filter);

	delete[] d_filter;
}

template <typename T>
void cls_image_ot<T>::filter_freq_ideal_hpf( int n_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	double *d_filter = new double[w * h];

	template_freq_ideal_hpf(d_filter,n_freq);

	filter_freq_template(d_filter);

	delete[] d_filter;
}

template <typename T>
void cls_image_ot<T>::filter_freq_gauss_lpf( double d_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	double *d_filter = new double[w * h];

	template_freq_gauss_lpf(d_filter,d_freq);

	filter_freq_template(d_filter);

	delete[] d_filter;
}

template <typename T>
void cls_image_ot<T>::filter_freq_ideal_lpf( int n_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	double *d_filter = new double[w * h];

	template_freq_ideal_lpf(d_filter,n_freq);

	filter_freq_template(d_filter);

	delete[] d_filter;
}

template <typename T>
void cls_image_ot<T>::template_freq_laplace_hpf( double *d_filter )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = - (i < h/2 ? i + h/2 : i -h/2)*(i < h/2 ? i + h/2 : i -h/2) - (j < w/2?j+w/2:j-w/2)*(j < w/2?j+w/2:j-w/2);
		}
	}
}

template <typename T>
void cls_image_ot<T>::template_freq_gauss_hpf( double *d_filter,double d_sigma )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 1 - exp( -( pow((double)(i - h/2),2) + pow((double)(j - w/2),2) ) / 2 / pow(d_sigma,2) );
		}
	}
}

template <typename T>
void cls_image_ot<T>::template_freq_ideal_hpf( double *d_filter,int n_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			if (sqrt( ((double)(i - h/2)) * ( i - h/2) + ((double)(j - w/2))*(j - w/2) ) < n_freq)
			{
				d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 0;
			}
			else
			{
				d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 1;
			}
		}
	}

}

template <typename T>
void cls_image_ot<T>::template_freq_gauss_lpf( double *d_filter,double d_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = exp( -( pow((double)(i - h/2),2) + pow((double)(j - w/2),2) ) / 2 / pow(d_freq,2) );
		}
	}
}

template <typename T>
void cls_image_ot<T>::template_freq_ideal_lpf( double *d_filter,int n_freq )
{
	long w = get_freq_trans_width();
	long h = get_freq_trans_height();

	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			if (sqrt( ((double)(i - h/2)) * ( i - h/2) + ((double)(j - w/2))*(j - w/2) ) > n_freq)
			{
				d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 0;
			}
			else
			{
				d_filter[(i < h/2 ? i + h/2 : i -h/2) * w + (j < w/2?j+w/2:j-w/2)] = 1;
			}
		}
	}

}

template <typename T>
cls_image_ot<T>::cls_image_ot(void):mat_image(1,1)
{
	;
}

template <typename T>
cls_image_ot<T>::cls_image_ot(const cls_image_ot<T>& that)
{
	this->mat_image = cls_matrix<T>(that.get_width(),that.get_height());
	this->mat_image.copy_assign(that.mat_image);
}

template <typename T>
cls_image_ot<T>::~cls_image_ot(void)
{
	;
}

template <typename T>
void cls_image_ot<T>::set_pixel(int x,int y,T value)
{
	mat_image(x,y) = value;
}

template <typename T>
T cls_image_ot<T>::get_pixel(int x,int y)
{
	return mat_image(x,y);
}

template <typename T>
int cls_image_ot<T>::get_height() const
{
	return mat_image.get_height();
}

template <typename T>
int cls_image_ot<T>::get_width() const
{
	return mat_image.get_width();
}

template <typename T>
unsigned char cls_image_ot<T>::get_channel_value(int x,int y,int num)  const
{
	unsigned char *pdata = (unsigned char*)mat_image.get_raw_data();

	return pdata[(x*mat_image.get_height()+y)*4 + num];
}

template <typename T>
unsigned char cls_image_ot<T>::get_channel_value_0(int x,int y)  const
{
	return get_channel_value(x,y,0);
}

template <typename T>
unsigned char cls_image_ot<T>::get_channel_value_1(int x,int y) const
{
	return get_channel_value(x,y,1);
}

template <typename T>
unsigned char cls_image_ot<T>::get_channel_value_2(int x,int y) const
{
	return get_channel_value(x,y,2);
}

template <typename T>
unsigned char cls_image_ot<T>::get_channel_value_3(int x,int y) const
{
	return get_channel_value(x,y,3);
}

template <typename T>
void cls_image_ot<T>::set_channel_value(int x,int y,int value,int num)
{
	unsigned char *pdata = (unsigned char*)mat_image.get_raw_data();

	pdata[(x*mat_image.get_height() +y)*4 + num] = value;
}

template <typename T>
void cls_image_ot<T>::set_channel_value_0(int x,int y,int value)
{
	return set_channel_value(x,y,value,0);
}

template <typename T>
void cls_image_ot<T>::set_channel_value_1(int x,int y,int value)
{
	return set_channel_value(x,y,value,1);
}

template <typename T>
void cls_image_ot<T>::set_channel_value_2(int x,int y,int value)
{
	return set_channel_value(x,y,value,2);
}

template <typename T>
void cls_image_ot<T>::set_channel_value_3(int x,int y,int value)
{
	return set_channel_value(x,y,value,3);
}

template <typename T>
int cls_image_ot<T>::get_gray(int x,int y) const
{
	return ( 0.30 * get_channel_value_1(x,y)
		+ 0.59 * get_channel_value_2(x,y)
		+ 0.11 * get_channel_value_3(x,y)
		);
}

template <typename T>
void cls_image_ot<T>::set_gray(int x,int y,unsigned char value)
{
	set_channel_value_1(x,y,value);
	set_channel_value_2(x,y,value);
	set_channel_value_3(x,y,value);
}

template <typename T>
void cls_image_ot<T>::operator = (const cls_image_ot<T>& that)
{
	mat_image = that.mat_image;
}

template <typename T>
void cls_image_ot<T>::get_hist_normal(vector<double>& hist,int num_hist,int channel) const
{
	hist.resize(256/num_hist + 1);

	if (num_hist <= 0 && num_hist > 256)
	{
		fill(hist.begin(),hist.end(),0);
		return;
	}

	fill(hist.begin(),hist.end(),0);

	int num_seg_len = 256.0 / num_hist;

	unsigned char bytGray;
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			bytGray = get_channel_value(i,j,channel);
			hist[(int)(bytGray / num_hist)] ++;
		}
	}

	double d_total_square = mat_image.get_width() * mat_image.get_height();

	for(int i=0;i<256.0/num_hist;i++)
	{
		hist[i] = hist[i] / d_total_square;
	}

	return;
}

template <typename T>
void cls_image_ot<T>::get_hist(vector<double>& hist,int num_hist,int channel) const
{
	hist.resize(256/num_hist + 1);

	if (num_hist <= 0 && num_hist > 256)
	{
		fill(hist.begin(),hist.end(),0);
		return;
	}

	fill(hist.begin(),hist.end(),0);

	int num_seg_len = 256.0 / num_hist;

	unsigned char bytGray;
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			bytGray = get_channel_value(i,j,channel);
			hist[(int)(bytGray / num_hist)] ++;
		}
	}

	return;
}

template <typename T>
void cls_image_ot<T>::get_hist_gray_normal(vector<double>& hist,int num_hist) const
{
	hist.resize(256/num_hist + 1);

	if (num_hist <= 0 && num_hist > 256)
	{
		fill(hist.begin(),hist.end(),0);
		return;
	}

	fill(hist.begin(),hist.end(),0);

	int num_seg_len = 256.0 / num_hist;

	unsigned char bytGray;
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			bytGray = get_gray(i,j);
			hist[(int)(bytGray / num_hist)] ++;
		}
	}

	double d_total_square = mat_image.get_width() * mat_image.get_height();

	for(int i=0;i<256.0/num_hist;i++)
	{
		hist[i] = hist[i] / d_total_square;
	}

	return;
}

template <typename T>
void cls_image_ot<T>::get_hist_gray(vector<double>& hist,int num_hist) const
{
	hist.resize(256/num_hist + 1);

	if (num_hist <= 0 && num_hist > 256)
	{
		fill(hist.begin(),hist.end(),0);
		return;
	}

	fill(hist.begin(),hist.end(),0);

	int num_seg_len = 256.0 / num_hist;

	unsigned char bytGray;
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			bytGray = get_gray(i,j);
			hist[(int)(bytGray / num_hist)] ++;
		}
	}

	return;
}

template <typename T>
void cls_image_ot<T>::basis_trans_linear(double d_a,double d_b)
{
	for(int i=0;i<get_width();i++)
	{
		for(int j=0;j<get_height();j++)
		{
			unsigned char byt_gray = get_gray(i,j);
			int n_re_trans = (int)(byt_gray * d_a + d_b);;

			if (n_re_trans > 255)
				n_re_trans =255;
			if (n_re_trans < 0)
				n_re_trans = 0;

			set_gray(i,j,n_re_trans);
		}
	}
}

template <typename T>
void cls_image_ot<T>::basis_trans_log(double d_r)
{
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			unsigned char byt_gray = get_gray(i,j);

			int n_re_tran = (int)( d_r * log((double)(byt_gray + 1)));

			if (n_re_tran > 255)
				n_re_tran =255;
			if (n_re_tran < 0)
				n_re_tran = 0;

			set_gray(i,j,n_re_tran);
		}
	}
}

template <typename T>
void cls_image_ot<T>::basis_trans_gramma(double d_grammer,double d_exp)
{
	for(int i=0;i<mat_image.get_height();i++)
	{
		for(int j=0;j<mat_image.get_width();j++)
		{
			unsigned char byt_gray = get_gray(i,j);

			int n_re_tran = (int)( pow((byt_gray + d_exp)/255.0,d_grammer) * 255);

			if (n_re_tran > 255)
				n_re_tran =255;
			if (n_re_tran < 0)
				n_re_tran = 0;

			set_gray(i,j,n_re_tran);
		}
	}
}

template <typename T>
void cls_image_ot<T>::basis_trans_threshold(unsigned char n_threshold)
{
	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			unsigned char byt_gray = get_gray(i,j);
			unsigned char byt_re;

			if (byt_gray >= n_threshold)
				byt_re = 255;
			else
				byt_re = 0;

			set_gray(i,j,byt_re);
		}
	}
}

template <typename T>
void cls_image_ot<T>::hist_equal()
{
	vector<double> d_hist;
	double dtmp[256];

	get_hist_gray_normal(d_hist,256);

	for(int i=0;i<256;i++)
	{
		dtmp[i] = 0;

		if (i == 0)
		{
			dtmp[i] = d_hist[i];
		}
		else
		{
			dtmp[i] = dtmp[i - 1] + d_hist[i];
		}

	}

	for(int i=0;i<mat_image.get_width();i++)
	{
		for(int j=0;j<mat_image.get_height();j++)
		{
			unsigned char byt_gray = get_gray(i,j);

			int n_re = dtmp[byt_gray] * 255;

			set_gray(i,j,n_re);
		}
	}
}

template <typename T>
void cls_image_ot<T>::hist_match(cls_image_ot<T> &that)
{
	vector<double> d_hist;
	double d_tran[256];

	memset(d_tran,-1,256);

	that.get_hist_gray_normal(d_hist,256);

	double dtmp;
	for(int i = 0;i<256;i++)
	{
		dtmp = 0;
		for(int k=0;k<i;k++)
		{
			dtmp += d_hist[k];
		}

		d_tran[(int)(0.5 + 255 * dtmp)] = i;
	}

	int i = 0;
	int j = 0;
	while(i < 255)
	{
		if (d_tran[i+1] != -1)
		{
			i ++;
			continue;
		}
		j = 1;
		while((d_tran[i + j] == -1) && (i + j <= 255))
		{
			d_tran[i + j] = d_tran[i];
			j++;
		}
	}

	double datmp[256];

	for(int i=0;i<256;i++)
	{
		datmp[i] = 0;

		if (i == 0)
		{
			datmp[i] = d_hist[i];
		}
		else
		{
			datmp[i] = datmp[i - 1] + d_hist[i];
		}

	}

	for(int i=0;i<mat_image.get_height();i++)
	{
		for(int j=0;j<mat_image.get_width();j++)
		{
			unsigned char byt_gray = get_gray(i,j);

			int n_re = d_tran[(int)(255 * datmp[byt_gray])];

			set_gray(i,j,n_re);
		}
	}
}

//template <typename T>
//void cls_image_ot<T>::fill_pixel(int w,int h,unsigned int value)
//{
//	delete[] data;
//	mat_image.get_width() = w;
//	mat_image.get_height() = h;
//	data = new unsigned int[w * h];
//	unsigned int * pdata = (unsigned int *)data;
//
//	for(int i=0;i<mat_image.get_width();i++)
//	{
//		for(int j=0;j<mat_image.get_height();j++)
//		{
//			pdata[i*h + j] = value;
//		}
//	}
//}

template <typename T>
void cls_image_ot<T>::filter_space_template(int n_w,int n_h,int n_mw,int n_mh,double *d_template,double d_coef)
{
	cls_image_ot img_re;
	img_re.init_pixel(mat_image.get_width(),mat_image.get_height());

	for(int i=n_mw;i<mat_image.get_width() - (n_w - n_mw) + 1;i++)
	{
		for(int j=n_mh;j<mat_image.get_height() - (n_h - n_mh) + 1;j++)
		{
			double d_re = 0.0;

			for(int ii = 0;ii < n_w;ii++)
			{
				for(int jj = 0;jj < n_h;jj++)
				{
					d_re += get_gray(i + ii - n_mw,j + jj - n_mh) * d_template[ii*n_h + jj];
				}
			}

			d_re *= d_coef;
			d_re = fabs(d_re);

			unsigned char byte;
			if (d_re > 255)
				byte = 255;
			else
				byte = d_re + 0.5;

			img_re.set_gray(i,j,byte);
		}
	}

	*this = img_re;
}

template <typename T>
void cls_image_ot<T>::filter_space_ave_smooth()
{
	double d_template[9] =
	{
		1 ,1 ,1,
		1 ,1 ,1,
		1 ,1 ,1
	};

	filter_space_template(3,3,1,1,d_template,1.0/9.0);
}

template <typename T>
void cls_image_ot<T>::filter_space_gauss_smooth()
{
	double d_template[9] =
	{
		1,2,1,
		2,4,2,
		1,2,1
	};

	filter_space_template(3,3,1,1,d_template,1.0/16.0);
}

template <typename T>
void cls_image_ot<T>::filter_space_adapter_smooth(int n_w,int n_h,int n_mw,int n_mh,double *d_template,double d_coef,unsigned char byt_std)
{
	cls_image_ot img_re;
	img_re.init_pixel(mat_image.get_width(),mat_image.get_height());

	for(int i=n_mw;i<mat_image.get_width() - (n_w - n_mw) + 1;i++)
	{
		for(int j=n_mh;j<mat_image.get_height() - (n_h - n_mh) + 1;j++)
		{
			double d_re = 0.0;
			unsigned char byt_max = 0;
			unsigned char byt_min = 255;

			for(int ii = 0;ii < n_w;ii++)
			{
				for(int jj = 0;jj < n_h;jj++)
				{
					unsigned char byt_cur =  get_gray(i + ii - n_mw,j + jj - n_mh) * d_template[ii*n_h + jj];
					d_re += byt_cur;

					if (byt_max < byt_cur)
					{
						byt_max = byt_cur;
					}

					if (byt_min > byt_cur)
					{
						byt_min = byt_cur;
					}
				}
			}

			if (byt_max - byt_min < byt_std)
			{
				img_re.set_gray(i,j,get_gray(i,j));
				continue;
			}

			d_re *= d_coef;
			d_re = fabs(d_re);

			unsigned char byte;
			if (d_re > 255)
				byte = 255;
			else
				byte = d_re + 0.5;

			img_re.set_gray(i,j,byte);
		}
	}

	*this = img_re;
}

template <typename T>
void cls_image_ot<T>::filter_space_ave_smooth_adaptive()
{
	double d_template[9] =
	{
		1 ,1 ,1,

		1 ,1 ,1,
		1 ,1 ,1
	};

	filter_space_template(3,3,1,1,d_template,1.0/9.0);
}

template <typename T>
void cls_image_ot<T>::filter_space_gauss_smooth_adaptive()
{
	double d_template[9] =
	{
		1,2,1,
		2,4,2,
		1,2,1
	};

	filter_space_template(3,3,1,1,d_template,1.0/16.0);
}

template <typename T>
void cls_image_ot<T>::filter_space_median(int n_w,int n_h,int n_mw,int n_mh,int order)
{
	cls_image_ot img_re;
	img_re.init_pixel(mat_image.get_width(),mat_image.get_height());

	unsigned char* ar_pos = new unsigned char[n_w * n_h];

	if (order >= n_w * n_h || order < 0)
	{
		return;
	}

	for(int i=n_mw;i<mat_image.get_width() - (n_w - n_mw) + 1;i++)
	{
		for(int j=n_mh;j<mat_image.get_height() - (n_h - n_mh) + 1;j++)
		{
			double d_re = 0.0;
			int n_pos = 0;

			for(int ii = 0;ii < n_w;ii++)
			{
				for(int jj = 0;jj < n_h;jj++)
				{
					unsigned char byt_cur = get_gray(i + ii - n_mw,j + jj - n_mh);
					ar_pos[n_pos++] = byt_cur;
				}
			}

			sort(ar_pos,ar_pos + n_w*n_h);

			img_re.set_gray(i,j,ar_pos[order-1]);
		}
	}

	*this = img_re;

	delete [] ar_pos;
}

template <typename T>
void cls_image_ot<T>::filter_space_median_mid()
{
	filter_space_median(3,3,1,1,5);
}

template <typename T>
void cls_image_ot<T>::filter_space_sobel()
{
	double d_template_h_sobel[9] =
	{
		-1,0,1,
		-2,0,2,
		-1,0,1
	};

	double d_template_v_sobel[9] =
	{
		-1,-2,-1,
		0,0,0,
		1,2,1
	};

	filter_space_template(3,3,1,1,d_template_h_sobel,1);
	filter_space_template(3,3,1,1,d_template_v_sobel,1);
}

template <typename T>
void cls_image_ot<T>::filter_space_log()
{
	double d_template_log[25] =
	{
		0,0,-1,0,0,
		0,-1,-2,-1,0,
		-1,-2,16,-2,-1,
		0,-1,-2,-1,0,
		0,0,-1,0,0
	};

	filter_space_template(5,5,2,2,d_template_log,1);
}

template <typename T>
void cls_image_ot<T>::filter_space_laplace()
{
	double d_template[9] =
	{
		0,-1,0,
		-1,4,-1,
		0,-1,0
	};

	filter_space_template(3,3,1,1,d_template,1);
}

template <typename T>
void cls_image_ot<T>::filter_space_big_log()
{
	double d_template[9] =
	{
		1,4,1,
		1,-20,1,
		1,4,1
	};

	filter_space_template(3,3,1,1,d_template,1);
}

template <typename T>
void cls_image_ot<T>::get_freq_trans_data(complex<double>* data) const
{
	int width_int = 1<<(int)a_get_pow_num(mat_image.get_width());
	int height_int = 1<<(int)a_get_pow_num(mat_image.get_height());

	for(int i=0;i<width_int;i++)
	{
		for(int j=0;j<height_int;j++)
		{
			if (i <mat_image.get_width() && j< mat_image.get_height())
			{
				data[i * height_int + j] = complex<double>(get_gray(i,j),0);
			}
			else
			{
				data[i * height_int + j] = complex<double>(255.0,0.0);
			}
		}
	}
}

template <typename T>
void cls_image_ot<T>::set_freq_trans_data(complex<double> *data)
{
	int width_int = 1<<(int)a_get_pow_num(mat_image.get_width());
	int height_int = 1<<(int)a_get_pow_num(mat_image.get_height());

	for(int i=0;i<width_int;i++)
	{
		for(int j=0;j<height_int;j++)
		{
			if (i <mat_image.get_width() && j< mat_image.get_height())
			{
				set_gray(i,j,data[i * height_int + j].real()>255?255:data[i * height_int + j].real());
			}
		}
	}
}

template <typename T>
int cls_image_ot<T>::get_freq_trans_width()
{
	int width_int = 1<<(int)a_get_pow_num(mat_image.get_width());

	return width_int;
}

template <typename T>
int cls_image_ot<T>::get_freq_trans_height()
{
	int height_int = 1<<(int)a_get_pow_num(mat_image.get_height());

	return height_int;
}

template <typename T>
void cls_image_ot<T>::filter_freq_template(double* d_filter)
{
	int width_int = get_freq_trans_width();
	int height_int = get_freq_trans_height();

	complex<double> *data_trans = new complex<double>[width_int*height_int];

	get_freq_trans_data(data_trans);

	a_fast_fourier_trans_in_2d(data_trans,data_trans,width_int,height_int);

	for(int cnt=0;cnt<width_int*height_int;cnt++)
	{
		data_trans[cnt] = d_filter[cnt] * data_trans[cnt];
	}

	a_inverse_fast_fourier_trans_in_2d(data_trans,data_trans,width_int,height_int);

	set_freq_trans_data(data_trans);

	delete[] data_trans;
}

template <typename T>
void cls_image_ot<T>::resize(int a_width,int a_height)
{
	mat_image = cls_matrix<T>(a_width,a_height);
}
