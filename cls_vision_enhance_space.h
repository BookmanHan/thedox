#include "lib_import.h"

class cls_vision_enhance_space
	:public virtual cls_vision_basis
{
public:

	cls_vision_enhance_space()
	{
		;
	}

	cls_vision_enhance_space(const cls_image& img)
		:cls_vision_basis(img)
	{

	}

	cls_vision_enhance_space& operator=(const cls_image& img)
	{
		this->mat_image = img.mat_image;

		return *this;
	}

	void enhance_channel_map(const vector<unsigned>& vb_map,int channel = -1)
	{
		if (vb_map.size() != 256)
			throw cls_exception_execution("the gray map must use 256 steps");

		if (channel >= 0)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_channel_value(i,j,vb_map[get_channel_value(i,j,channel)],channel);
				}
			}
		}
		else if (channel == -1)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_gray(i,j,vb_map[get_gray(i,j)]);
				}
			}
		}
		else if (channel == -2)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_channel_value_0(i,j,vb_map[get_channel_value_0(i,j)]);
					set_channel_value_1(i,j,vb_map[get_channel_value_1(i,j)]);
					set_channel_value_2(i,j,vb_map[get_channel_value_2(i,j)]);
					set_channel_value_3(i,j,vb_map[get_channel_value_3(i,j)]);
				}
			}
		}
	}

	void enhance_channel_reverse(int channel = -1)
	{
		vector<unsigned> vb_map(256);
		for(int cnt=0;cnt<256;cnt++)
		{
			vb_map[cnt] = 255 - cnt;
		}

		enhance_channel_map(vb_map,channel);
	}

	void enhance_channel_linear(double d_k,double d_b,int channel = -1)
	{
		vector<unsigned> vb_map(256);
		for(int cnt=0;cnt<256;cnt++)
		{
			vb_map[cnt] = get_suitable_byte(cnt * d_k + d_b);
		}

		enhance_channel_map(vb_map,channel);
	}

	//the first point means the segment([a,b)) , the second means the params;
	void enhance_channel_multi_linear(const vector<pair<cls_point<int>,cls_point<double>  > >& v_ml,int channel = -1)
	{
		vector<unsigned> vb_map(256);
		
		for(int cnt=0;cnt<v_ml.size();cnt++)
		{
			for(int i=v_ml[cnt].first.x; i < v_ml[cnt].first.y;i++)
			{
				vb_map[i] = v_ml[cnt].second.x * i + v_ml[cnt].second.y;
			}
		}

		enhance_channel_map(vb_map,channel);
	}

	void enhance_channel_log(int channel = -1)
	{
		vector<unsigned> vb_map(256);
		double d_c = 255.0 / log(1.0 + 256);

		for(int cnt=0;cnt<255;cnt++)
		{
			vb_map[cnt] = d_c * log(1.0 + cnt);
		}

		enhance_channel_map(vb_map,channel);
	}

	void enhance_channel_gamma(double gamma,int channel = -1)
	{
		vector<unsigned> vb_map(256);

		for(int cnt=0;cnt<255;cnt++)
		{
			vb_map[cnt] = get_suitable_byte( pow(cnt/255.0,gamma) + 0.5 );
		}

		enhance_channel_map(vb_map,channel);
	}

	void enhance_histogram_equalization(int channel = -1)
	{
		vector<unsigned> vu(256);
		vector<unsigned> vb(256);
		get_histogram(vu,channel);

		double d_cur = 0;
		double d_total = get_width() * get_height();
		for(int cnt=0;cnt<256;cnt++)
		{
			d_cur += vu[cnt];
			vb[cnt] = get_suitable_byte((d_cur/d_total + 0.5) * 255);
		}

		enhance_channel_map(vb,channel);
	}

	void enhance_histogram_specification(const cls_image& that,int channel = -1)
	{
		vector<unsigned> vb_this(256);
		vector<unsigned> vb_that(256);
		vector<unsigned> vb_r_that(256);

		this->get_histogram(vb_this,channel);
		that.get_histogram(vb_that,channel);

		vector<int> vb(256);
		fill(vb.begin(),vb.end(),0);
		fill(vb_r_that.begin(),vb_r_that.end(),-1);

		for(int cnt=0;cnt<256;cnt++)
		{
			vb_r_that[vb_that[cnt]] = cnt;
		}

		for(int cnt=0;cnt<256;cnt++)
		{
			vb[cnt] = vb_r_that[vb_this[cnt]];
		}

		vector<unsigned> vb_use(256);

		vb_use[0] = min(vb[0],0);
		vb_use[255] = vb[255] == -1?255:vb[255];

		byte pre = 0;
		for(int cnt=0;cnt<255;cnt++)
		{
			if (vb[cnt] == -1)
				vb_use[cnt] = pre;
			else 
				vb_use[cnt] = vb[cnt];

			pre = vb_use[cnt];
		}

		enhance_channel_map(vb_use,channel);
	}

	//the is the adaptive convolution's template enhance.
	//if center_x_y = -1 that use the real center;
	//der means the dealing derta,if the two pixels < der,it will be dealed.
	void enhance_template_convolution_adaptive(const cls_matrix<double>& mat,int der,short channel = -1,short center_x = -1,short center_y = -1)
	{
		cls_vision_enhance_space vision = *this;

		int w = mat.get_width();
		int h = mat.get_height();

		if (center_x == -1)
			center_x = w/2;
		if (center_y == -1)
			center_y = h/2;

		int limit_w = min(get_width(),get_width()-w + center_x);
		int limit_h = min(get_height(),get_height() - h + center_y);

		if (channel >= 0)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_max = -1;
					double d_min =300;
					double d_r = 0.0;
					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							byte v = vision.get_channel_value(i + ci - center_x,j + cj - center_y,channel);
							d_max = max(d_max,v);
							d_min = min(d_min,v);

							d_r += mat(ci,cj) * v;
						}
					}

					if (d_max - d_min <= der)
						set_channel_value(i,j,get_suitable_byte(d_r),channel);
				}
			}
		}
		if (channel == -1)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_max = -1;
					double d_min =300;
					double d_r = 0.0;
					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							byte v = vision.get_gray(i + ci - center_x,j + cj - center_y);
							d_max = max(d_max,v);
							d_min = min(d_min,v);

							d_r += mat(ci,cj) * v;
						}
					}

					if (d_max - d_min <= der)
						set_gray(i,j,get_suitable_byte(d_r));
				}
			}
		}
		else if (channel == -2)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_0 = 0.0,max_0=0.0,min_0=300.0;
					double d_1 = 0.0,max_1=0.0,min_1=300.0;
					double d_2 = 0.0,max_2=0.0,min_2=300.0;
					double d_3 = 0.0,max_3=0.0,min_3=3000.0;

					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							byte v0 = vision.get_channel_value_0(i + ci - center_x,j + cj - center_y);
							byte v1 = vision.get_channel_value_1(i + ci - center_x,j + cj - center_y);
							byte v2 = vision.get_channel_value_2(i + ci - center_x,j + cj - center_y);
							byte v3 = vision.get_channel_value_3(i + ci - center_x,j + cj - center_y);
							
							max_0 = max(max_0,v0);min_0 = min(min_0,v0);
							max_1 = max(max_1,v1);min_1 = min(min_1,v1);
							max_2 = max(max_2,v2);min_2 = min(min_2,v2);
							max_3 = max(max_3,v3);min_3 = min(min_3,v3);
		
							d_0 += mat(ci,cj) * v0;
							d_1 += mat(ci,cj) * v1;
							d_2 += mat(ci,cj) * v2;
							d_3 += mat(ci,cj) * v3;
						}
					}

					if (max_0 - min_0 <= der)
						set_channel_value_0(i,j,get_suitable_byte(d_0));

					if (max_1 - min_1 <= der)
						set_channel_value_1(i,j,get_suitable_byte(d_1));
					
					if (max_2 - min_2 <= der)
						set_channel_value_2(i,j,get_suitable_byte(d_2));
				
					if (max_3 - min_3 <= der)
						set_channel_value_3(i,j,get_suitable_byte(d_3));
				}
			}
		}
	}

	////the is the convolution's template enhance.
	//if center_x_y = -1 that use the real center;
	void enhance_template_convolution(const cls_matrix<double>& mat,short channel = -1,short center_x = -1,short center_y = -1)
	{
		cls_vision_enhance_space vision = *this;

		int w = mat.get_width();
		int h = mat.get_height();

		if (center_x == -1)
			center_x = w/2;
		if (center_y == -1)
			center_y = h/2;

		int limit_w = min(get_width(),get_width()-w + center_x);
		int limit_h = min(get_height(),get_height() - h + center_y);

		if (channel >= 0)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_r = 0.0;
					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							d_r += mat(ci,cj) * vision.get_channel_value(i + ci - center_x,j + cj - center_y,channel);
						}
					}

					set_channel_value(i,j,get_suitable_byte(d_r),channel);
				}
			}
		}
		else if (channel == -1)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_r = 0.0;
					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							d_r += mat(ci,cj) * vision.get_gray(i + ci - center_x,j + cj - center_y);
						}
					}

					set_gray(i,j,get_suitable_byte(d_r));
				}
			}
		}
		else if (channel == -2)
		{
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					double d_0 = 0.0;
					double d_1 = 0.0;
					double d_2 = 0.0;
					double d_3 = 0.0;

					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							d_0 += mat(ci,cj) * vision.get_channel_value_0(i + ci - center_x,j + cj - center_y);
							d_1 += mat(ci,cj) * vision.get_channel_value_1(i + ci - center_x,j + cj - center_y);
							d_2 += mat(ci,cj) * vision.get_channel_value_2(i + ci - center_x,j + cj - center_y);
							d_3 += mat(ci,cj) * vision.get_channel_value_3(i + ci - center_x,j + cj - center_y);

						}
					}

					set_channel_value_0(i,j,get_suitable_byte(d_0));
					set_channel_value_1(i,j,get_suitable_byte(d_1));
					set_channel_value_2(i,j,get_suitable_byte(d_2));
					set_channel_value_3(i,j,get_suitable_byte(d_3));
				}
			}
		}
	}

	//the enhance of common average weights.
	void enhance_average(unsigned short d_w=3,unsigned short d_h=3,int channel = -1,int der=-1)
	{
		cls_matrix<double> mat(d_w,d_h);
		double d_elem = 1.0 / (d_w * d_h);

		for(int i=0;i<d_w;i++)
		{
			for(int j=0;j<d_h;j++)
			{
				mat(i,j) = d_elem;
			}
		}

		if (der == -1)
			enhance_template_convolution(mat,channel);
		else
			enhance_template_convolution_adaptive(mat,der,channel);
	}

	//the enhance of common guassian.
	void enhance_gaussian(unsigned short d_w = 3,unsigned short d_h = 3,double d_sigma = 0.85,int channel = -1,int der=-1)
	{
		cls_matrix<double> mat(d_w,d_h);
		double d_factor = 1.0/(sqrt(2*pi)*fabs(d_sigma));
		double d_all = 0.0;
		for(int i=0;i<d_w;i++)
		{
			for(int j=0;j<d_h;j++)
			{
				mat(i,j) = d_factor * pow(math_e,(-(i-d_w/2)*(i-d_w/2) - (j-d_h/2)*(j-d_h/2))/2.0/d_sigma/d_sigma);
				d_all += mat(i,j);		
			}
		}

		for(int i=0;i<d_w;i++)
		{
			for(int j=0;j<d_h;j++)
			{
				mat(i,j) /= d_all;
			}
		}
		if (der == -1)
			enhance_template_convolution(mat,channel);
		else
			enhance_template_convolution_adaptive(mat,der,channel);
	}

	//the stander guassian enhance;
	void enhance_guassian_standard(int channel = -1,int der=-1)
	{
		cls_matrix<double> mat(3,3);
		mat(0,0) = 1;mat(0,1) = 2;mat(0,2) = 1;
		mat(1,0) = 2;mat(1,1) = 4;mat(1,2) = 2;
		mat(2,0) = 1;mat(2,1) = 2;mat(2,2) = 1;

		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				mat(i,j) /= 16;
			}
		}

		if (der == -1)
			enhance_template_convolution(mat,channel);
		else
			enhance_template_convolution_adaptive(mat,der,channel);
	}

	//the stander guassian enhance (big);
	void enhance_guassian_standard_big(int channel = -1,int der = -1)
	{
		cls_matrix<double> mat(5,5);
		mat(0,0) = 1;mat(0,1) = 4;mat(0,2) = 7;mat(0,3) = 4;mat(0,4) = 1;
		mat(1,0) = 4;mat(1,1) = 16;mat(1,2) = 26;mat(1,3) = 16;mat(1,4) = 4;
		mat(2,0) = 7;mat(2,1) = 26;mat(2,2) = 41;mat(2,3) = 26;mat(2,4) = 7;
		mat(3,0) = 4;mat(3,1) = 16;mat(3,2) = 26;mat(3,3) = 16;mat(3,4) = 4;
		mat(4,0) = 1;mat(4,1) = 4;mat(4,2) = 7;mat(4,3) = 4;mat(4,4) = 1;

		for(int i=0;i<5;i++)
		{
			for(int j=0;j<5;j++)
			{
				mat(i,j) /= 273.0;
			}
		}

		if (der == -1)
			enhance_template_convolution(mat,channel);
		else
			enhance_template_convolution_adaptive(mat,der,channel);
	}

	//the sequence of template convolution's enhance.
	//if center_x_y = -1 that use the real center;
	void enhance_template_convolution_sequence(const cls_matrix<bool>& mat,int anum,short channel = -1,short center_x = -1,short center_y = -1)
	{
		cls_vision_enhance_space vision = *this;

		int w = mat.get_width();
		int h = mat.get_height();

		if (center_x == -1)
			center_x = w/2;
		if (center_y == -1)
			center_y = h/2;

		int limit_w = min(get_width(),get_width()-w + center_x);
		int limit_h = min(get_height(),get_height() - h + center_y);

		int num = 0;
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if (mat(i,j))
					num ++;
			}
		}

		if (num <= anum)
		{
			throw cls_exception_execution("the num is overflow.");
			return;
		}

		if (channel >= 0)
		{
			unsigned acc[256];
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					memset(acc,0,sizeof(unsigned)*256);

					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							if (mat(ci,cj))
								acc[vision.get_channel_value(i + ci - center_x,j + cj - center_y,channel)]++;
						}
					}

					int cnt = 0;
					unsigned total = 0;
					for(cnt=0;cnt<255 && total < anum;cnt++)
					{
						total += acc[cnt];
					}

					set_channel_value(i,j,cnt,channel);
				}
			}
		}
		else if (channel == -1)
		{
			unsigned acc[256];
			for(int i=center_x;i<limit_w;i++)
			{
				for(int j=center_y;j<limit_h;j++)
				{
					memset(acc,0,sizeof(unsigned)*256);
					for(int ci = 0;ci<w;ci++)
					{
						for(int cj=0;cj<h;cj++)
						{
							if (mat(ci,cj))
								acc[vision.get_gray(i + ci - center_x,j + cj - center_y)] ++ ;
						}
					}

					int cnt = 0;
					unsigned total = 0;
					for(cnt=0;cnt<255 && total < anum;cnt++)
					{
						total += acc[cnt];
					}

					set_gray(i,j,cnt);
				}
			}
		}
		else if (channel == -2)
		{
			enhance_template_convolution_sequence(mat,anum,0);
			enhance_template_convolution_sequence(mat,anum,1);
			enhance_template_convolution_sequence(mat,anum,2);
			enhance_template_convolution_sequence(mat,anum,3);
		}
	}

	//the rectangle sequence's enhance.
	void enhance_sequence_rectangle(int w,int h,int channel=-1,int pos=-1)
	{
		if (pos == -1)
		{
			pos = w,h/2;
		}

		cls_matrix<bool> mat(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				mat(i,j) = true;
			}
		}

		enhance_template_convolution_sequence(mat,pos,channel);
	}

	//the sequence convolution's enhance.in many styles;
	void enhance_sequence_median(unsigned short style,int channel=-1)
	{
		if (style >=6)
			throw cls_exception_execution("there is not your style!");

		cls_matrix<bool> mat(5,5);
		mat.fill(false);
		switch(style)
		{
		case 0:
			mat(2,0) = true;
			mat(2,1) = true;
			mat(2,2) = true;
			mat(2,3) = true;
			mat(2,4) = true;
			mat(0,2) = true;
			mat(1,2) = true;
			mat(3,2) = true;
			mat(4,2) = true;
			break;

		case 1:
			mat(0,0) = true;
			mat(1,1) = true;
			mat(2,2) = true;
			mat(3,3) = true;
			mat(4,4) = true;
			mat(0,4) = true;
			mat(1,3) = true;
			mat(3,1) = true;
			mat(4,0) = true;

			enhance_template_convolution_sequence(mat,4,channel);
			break;

		case 2:
			mat(2,2) = true;
			mat(0,1) = true;
			mat(0,3) = true;
			mat(1,0) = true;
			mat(1,4) = true;
			mat(3,0) = true;
			mat(3,4) = true;
			mat(4,1) = true;
			mat(4,3) = true;

			enhance_template_convolution_sequence(mat,4,channel);
			break;

		case 3:
			mat(2,2) = true;
			mat(0,1) = true;
			mat(0,3) = true;
			mat(1,0) = true;
			mat(1,4) = true;
			mat(3,0) = true;
			mat(3,4) = true;
			mat(4,1) = true;
			mat(4,3) = true;
			mat(1,2) = true;
			mat(2,1) = true;
			mat(2,3) = true;
			mat(3,2) = true;

			enhance_template_convolution_sequence(mat,6,channel);
			break;

		case 4:
			mat(2,2) = true;
			mat(0,1) = true;
			mat(0,3) = true;
			mat(1,0) = true;
			mat(1,4) = true;
			mat(3,0) = true;
			mat(3,4) = true;
			mat(4,1) = true;
			mat(4,3) = true;
			mat(1,1) = true;
			mat(3,3) = true;
			mat(1,3) = true;
			mat(3,1) = true;
			enhance_template_convolution_sequence(mat,6,channel);
			break;

		case 5:
			mat(0,1) = true;
			mat(0,2) = true;
			mat(0,3) = true;
			mat(1,0) = true;
			mat(2,0) = true;
			mat(3,0) = true;
			mat(1,4) = true;
			mat(2,4) = true;
			mat(3,4) = true;
			mat(2,2) = true;
			mat(4,1) = true;
			mat(4,2) = true;
			mat(4,3) = true;
			enhance_template_convolution_sequence(mat,6,channel);
			break;
		}
	}

	//the robet enhance for edge;
	void enhance_robert(int channel = -1)
	{
		cls_matrix<double> mat_left(2,2);
		mat_left(0,0) = -1;mat_left(0,1) = 0;
		mat_left(1,0) = 0; mat_left(1,1) = 1;

		cls_matrix<double> mat_right(2,2);
		mat_right(0,0) = 0;mat_right(0,1) = -1;
		mat_right(1,0) = 1; mat_right(1,1) = 0;

		cls_vision_enhance_space left = *this;

		left.enhance_template_convolution(mat_left,channel);
		enhance_template_convolution(mat_right,channel);

		*this = *this + left;
	}

	//the sobel enhance for edge;
	void enhance_sobel(int channel = -1)
	{
		cls_matrix<double> mat_left(3,3);
		mat_left(0,0) = -1;mat_left(0,1) = -2;mat_left(0,2) = -1;
		mat_left(1,0) = 0; mat_left(1,1) = 0;mat_left(1,2) = 0;
		mat_left(2,0) = 1; mat_left(2,1) = 2;mat_left(2,2) = 1;

		cls_matrix<double> mat_right(3,3);
		mat_right(0,0) = -1;mat_right(0,1) = 0;mat_right(0,2) = 1;
		mat_right(1,0) = -2; mat_right(1,1) = 0;mat_right(1,2) = 2;
		mat_right(2,0) = -1; mat_right(2,1) = 0;mat_right(2,2) = 1;

		cls_vision_enhance_space left = *this;

		left.enhance_template_convolution(mat_left,channel);
		enhance_template_convolution(mat_right,channel);

		*this = *this + left;
	}

	//the laplace enhance for edge;
	void enhance_laplace(int channel=-1)
	{
		 cls_matrix<double> mat(3,3);
		 mat(0,0) = 0;mat(0,1) = 1;mat(0,2) = 0;
		 mat(1,0) = 1;mat(1,1) = -4;mat(1,2) = 1;
		 mat(2,0) = 0;mat(2,1) = 1;mat(2,2) = 0;

		 enhance_template_convolution(mat,channel);
	}
	
	//the laplace extension enhance for edge;
	void enhance_laplace_ext(int channel = -1)
	{
		cls_matrix<double> mat(3,3);
		mat(0,0) = 1;mat(0,1) = 1;mat(0,2) = 1;
		mat(1,0) = 1;mat(1,1) = -8;mat(1,2) = 1;
		mat(2,0) = 1;mat(2,1) = 1;mat(2,2) = 1;

		enhance_template_convolution(mat,channel);
	}

	//the Laplace guassian's enhance for edge;
	void enhance_laplace_guassian(int channel = -1)
	{
		cls_matrix<double>mat(3,3);
		mat(0,0) = -1;mat(0,1) = -1;mat(0,2) = -1;
		mat(1,0) = -1;mat(1,1) = 8;mat(1,2) = -1;
		mat(2,0) = -1;mat(2,1) = -1;mat(2,2) = -1;

		enhance_template_convolution(mat,channel);
	}
};