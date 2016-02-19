#include "lib_import.h"

class cls_vision_geometry
	:public virtual cls_vision_basis
{
public:

	//the project transformation in a forward implement;
	void geometry_project_forward(const cls_matrix<double>& mat)
	{
		if (mat.get_width() !=3 || mat.get_height() != 3)
			throw cls_exception_execution("the matrix for affine is bad.");

		cls_vision_basis basis = *this;
		cls_matrix<double> mat_this(3,1);
		cls_matrix<double> mat_that(3,1);

		this->fill_pixel(get_rgb(255,255,255));

		mat_this(2,0) = 1;
		for(int i=0;i<get_width();i++)
		{
			mat_this(0,0) = i;
			for(int j=0;j<get_height();j++)
			{
				mat_this(1,0) = j;
				mat_that = mat * mat_this;

				mat_that(0,0) /= mat_that(2,0);
				mat_that(1,0) /= mat_that(2,0);

				if (mat_that(0,0) < 0 || mat_that(0,0) >= get_width() - 1)
					continue;
				else if (mat_that(1,0) <0 || mat_that(1,0) >= get_height() - 1)
					continue;

				int a = (int)(mat_that(0,0));
				int b = (int)(mat_that(1,0));

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_0(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_0(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_0(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_1(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_1(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_2(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_2(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_3(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_3(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);

				this->set_pixel(a,b,basis.get_pixel(i,j));
			}
		}
	}

	//the project transformation in a backward implement;
	void geometry_project_backward(const cls_matrix<double>& amat)
	{
		if (amat.get_width() !=3 || amat.get_height() != 3)
			throw cls_exception_execution("the matrix for affine is bad.");

		cls_vision_basis basis = *this;
		cls_matrix<double> mat_this(3,1);
		cls_matrix<double> mat_that(3,1);
		cls_matrix<double> mat = amat;

		mat = mat.inverse();

		this->fill_pixel(get_rgb(255,255,255));

		mat_this(2,0) = 1;
		for(int i=0;i<get_width();i++)
		{
			mat_this(0,0) = i;
			for(int j=0;j<get_height();j++)
			{
				mat_this(1,0) = j;
				mat_that = mat * mat_this;

				mat_that(0,0) /= mat_that(2,0);
				mat_that(1,0) /= mat_that(2,0);

				if (mat_that(0,0) < 0 || mat_that(0,0) >= get_width() - 1)
					continue;
				else if (mat_that(1,0) <0 || mat_that(1,0) >= get_height() - 1)
					continue;

				int a = (int)(mat_that(0,0));
				int b = (int)(mat_that(1,0));

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					 + basis.get_channel_value_0(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					 + basis.get_channel_value_0(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					 + basis.get_channel_value_0(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_1(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_1(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_2(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_2(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - mat_that(0,0)) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_3(a+1,b) * (mat_that(0,0) - a) * (b + 1 - mat_that(1,0))
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - mat_that(0,0)) * (mat_that(1,0) - b)
					+ basis.get_channel_value_3(a+1,b+1) * (mat_that(0,0) - a) *  (mat_that(1,0) - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);
			}
		}
	}

	//the similarity transformation 
	void geometry_similarity(double s,double angel,double d_x,double d_y)
	{
		cls_matrix<double> mat(3,3);
		mat(0,0) = s * cos(angel);mat(0,1) = - s * sin(angel);mat(0,2) = d_x;
		mat(1,0) = s * sin(angel);mat(1,1) = s * cos(angel);mat(1,2) = d_y;
		mat(2,0) = 0;mat(2,1) = 0;mat(2,2) = 1;

		geometry_project_backward(mat);
	}

	//the isometry transformation
	void geometry_isometry(bool e,double angel,double d_x,double d_y)
	{
		double s = e?1:-1;

		cls_matrix<double> mat(3,3);
		mat(0,0) = s * cos(angel);mat(0,1) = - s * sin(angel);mat(0,2) = d_x;
		mat(1,0) = s * sin(angel);mat(1,1) = s * cos(angel);mat(1,2) = d_y;
		mat(2,0) = 0;mat(2,1) = 0;mat(2,2) = 1;

		geometry_project_backward(mat);
	}

	//move the image;
	void geometry_move(const double x,const double y)
	{
		geometry_similarity(1,0,x,y);
	}

	//rotate the image;
	void geometry_rotation(const double angel)
	{
		geometry_similarity(1,angel,0,0);
	}

	//rotate the image in center;
	void geometry_rotation_center(const double angel)
	{
		cls_matrix<double> mat_1(3,3),mat_2(3,3),mat_3(3,3);
		mat_1(0,0) = cos(angel);mat_1(0,1) = -sin(angel);mat_1(0,2) = 0;
		mat_1(1,0) = sin(angel);mat_1(1,1) = cos(angel);mat_1(1,2) = 0;
		mat_1(2,0) = 0;mat_1(2,1) = 0;mat_1(2,2) = 1;

		mat_2(0,0) = 1;mat_2(0,1) = 0;mat_2(0,2) = +1.0*get_width()/2;
		mat_2(1,0) = 0;mat_2(1,1) = 1;mat_2(1,2) = +1.0*get_height()/2;
		mat_2(2,0) = 0;mat_2(2,1) = 0;mat_2(2,2) = 1;

		mat_3 = mat_2;
		mat_3(0,2) = -1.0 * get_width()/2;
		mat_3(1,2) = -1.0 * get_height()/2;

		cls_matrix<double> mat =mat_2 * mat_1 * mat_3;
		//mat.display();

		geometry_project_backward(mat);
	}

	//rotate the image with image in point(i,j);
	void geometry_rotation(int i,int j,const double angel)
	{
		cls_matrix<double> mat_1(3,3),mat_2(3,3),mat_3(3,3);
		mat_1(0,0) = cos(angel);mat_1(0,1) = -sin(angel);mat_1(0,2) = 0;
		mat_1(1,0) = sin(angel);mat_1(1,1) = cos(angel);mat_1(1,2) = 0;
		mat_1(2,0) = 0;mat_1(2,1) = 0;mat_1(2,2) = 1;

		mat_2(0,0) = 1;mat_2(0,1) = 0;mat_2(0,2) = +1.0*i;
		mat_2(1,0) = 0;mat_2(1,1) = 1;mat_2(1,2) = +1.0*j;
		mat_2(2,0) = 0;mat_2(2,1) = 0;mat_2(2,2) = 1;

		mat_3 = mat_2;
		mat_3(0,2) = -1.0 * j;
		mat_3(1,2) = -1.0 * j;

		cls_matrix<double> mat =mat_2 * mat_1 * mat_3;
		//mat.display();

		geometry_project_backward(mat);
	}

	//zoom the image;
	void geometry_zoom(const cls_rect& rect_src,const cls_rect& rect_des)
	{
		cls_vision_basis basis = *this;

		double d_scaler_x = double(rect_src.get_width()) / rect_des.get_width();
		double d_scaler_y = double(rect_src.get_height()) / rect_des.get_height();

		this->resize(rect_des.get_width(),rect_des.get_height());
		for(int i=0;i<rect_des.get_width();i++)
		{
			for(int j=0;j<rect_des.get_height();j++)
			{
				double d_x = i*d_scaler_x;
				double d_y = j*d_scaler_y;

				if (d_x < 0 || d_x >= rect_src.get_width() - 1)
					continue;
				else if (d_y <0 || d_y >= rect_src.get_height() - 1)
					continue;

				int a = (int)(d_x);
				int b = (int)(d_y);

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - d_x) * (b + 1 - d_y)
					+ basis.get_channel_value_0(a+1,b) * (d_x - a) * (b + 1 - d_y)
					+ basis.get_channel_value_0(a,b+1) * (a + 1 - d_x) * (d_y - b)
					+ basis.get_channel_value_0(a+1,b+1) * (d_x - a) *  (d_y - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - d_x) * (b + 1 - d_y)
					+ basis.get_channel_value_1(a+1,b) * (d_x - a) * (b + 1 - d_y)
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - d_x) * (d_y - b)
					+ basis.get_channel_value_1(a+1,b+1) * (d_x - a) *  (d_y - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - d_x) * (b + 1 - d_y)
					+ basis.get_channel_value_2(a+1,b) * (d_x - a) * (b + 1 - d_y)
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - d_x) * (d_y - b)
					+ basis.get_channel_value_2(a+1,b+1) * (d_x - a) *  (d_y - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - d_x) * (b + 1 - d_y)
					+ basis.get_channel_value_3(a+1,b) * (d_x - a) * (b + 1 - d_y)
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - d_x) * (d_y - b)
					+ basis.get_channel_value_3(a+1,b+1) * (d_x - a) *  (d_y - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);
			}
		}
	}

	//zoom the image from all the page to rect_des;
	void geometry_zoom(const cls_rect& rect_des)
	{
		cls_rect rect(0,0,get_height(),get_width());
		geometry_zoom(rect,rect_des);
	}

	//paint anthor image in this image;
	void geometry_locate(const cls_vision_geometry& vision,int ci,int cj)
	{
		for(int i=0;i<vision.get_width();i++)
		{
			for(int j=0;j<vision.get_height();j++)
			{
				set_pixel(ci + i,cj + j,vision.get_pixel(i,j));
			}
		}
	}

	//paint anthor image in this image;
	void geometry_locate(const cls_vision_geometry& vision,int ci,int cj,int channel = -1)
	{
		if (channel == -1)
		{
			for(int i=0;i<vision.get_width();i++)
			{
				for(int j=0;j<vision.get_height();j++)
				{
					set_gray(ci + i,cj + j,vision.get_gray(i,j));
				}
			}
		}
		else
		{
			for(int i=0;i<vision.get_width();i++)
			{
				for(int j=0;j<vision.get_height();j++)
				{
					set_channel_value(ci + i,cj + j,vision.get_channel_value(i,j,channel),channel);
				}
			}
		}
	}

	//locate the image on the image.
	void geometry_locate(const cls_point<unsigned>& pt,const cls_image& img)
	{
		for(int i=pt.x;i<=pt.x + img.get_width();i++)
		{
			for(int j=pt.y;j<pt.y + img.get_height();j++)
			{
				this->set_pixel(i,j,img.get_pixel(i,j));
			}
		}
	}

	//cut part of the image in this image;
	void geometry_part(const cls_rect& arect,cls_image& img)
	{
		cls_rect rect = arect;

		rect.left = max(0,rect.left);
		rect.top = max(0,rect.top);
		rect.bottom = min(rect.bottom,get_height());
		rect.right = min(rect.right,get_width());

		img.resize(rect.get_width(),rect.get_height());

		for(int i=0;i<rect.get_width();i++)
		{
			for(int j=0;j<rect.get_height();j++)
			{
				img.set_pixel(i,j,get_pixel(rect.left + i, rect.top + j));
			}
		}
	}

	//cut part of the image in this image;
	cls_image geometry_part(const cls_rect& rect)
	{
		cls_image img;
		geometry_part(rect,img);
		
		return img;
	}

	//image mathing in affine;
	void geometry_match_affine(const vector<cls_point<int> >& pts_src,const vector<cls_point<int> >& pts_des)
	{
		if (pts_src.size() <3 || pts_src.size() != pts_des.size())
		{
			throw cls_exception_execution("the input for birepait is bad.");
		}

		cls_matrix<double> mat_x(3,1);
		cls_matrix<double> mat_y(3,1);
		cls_matrix<double> mat_bx(3,1);
		cls_matrix<double> mat_by(3,1);
		cls_matrix<double> mat_a(3,3);

		cls_vision_basis basis = *this;

		for(int cnt = 0;cnt < 3;cnt ++)
		{
			mat_bx(cnt,0) = pts_des[cnt].x;
			mat_by(cnt,0) = pts_des[cnt].y;

			mat_a(cnt,0) = 1;
			mat_a(cnt,1) = pts_src[cnt].x;
			mat_a(cnt,2) = pts_src[cnt].y;
		}

		mat_a = mat_a.inverse();

		mat_x = mat_a * mat_bx;
		mat_y = mat_a * mat_by;

		this->fill_pixel(get_rgb(255,255,255));
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				double x = mat_x(0,0)  + mat_x(1,0) * i + mat_x(2,0) * j;
				double y = mat_y(0,0)  + mat_y(1,0) * i + mat_y(2,0) * j;

				if (x < 0 || x >= get_width() - 1)
					continue;
				else if (y <0 || y >= get_height() - 1)
					continue;

				int a = (int)(x);
				int b = (int)(y);

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_0(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_0(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_0(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_1(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_1(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_2(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_2(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_3(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_3(a+1,b+1) * (x - a) *  (y - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);
			}
		}
	}

	//image-mathing in project;
	void geometry_match_project(const vector<cls_point<int> >& pts_src,const vector<cls_point<int> >& pts_des)
	{
		if (pts_src.size() <3 || pts_src.size() != pts_des.size())
		{
			throw cls_exception_not_support("the meathod can't be implemented.");
		}
	}

	//image_mathing in binomial;
	void geometry_match_binomial(const vector<cls_point<int> >& pts_src,const vector<cls_point<int> >& pts_des)
	{
		if (pts_src.size() <6 || pts_src.size() != pts_des.size())
		{
			throw cls_exception_execution("the input for birepait is bad.");
		}

		cls_matrix<double> mat_x(6,1);
		cls_matrix<double> mat_y(6,1);
		cls_matrix<double> mat_bx(6,1);
		cls_matrix<double> mat_by(6,1);
		cls_matrix<double> mat_a(6,6);

		cls_vision_basis basis = *this;

		for(int cnt = 0;cnt < 6;cnt ++)
		{
			mat_bx(cnt,0) = pts_des[cnt].x;
			mat_by(cnt,0) = pts_des[cnt].y;

			mat_a(cnt,0) = 1;
			mat_a(cnt,1) = pts_src[cnt].x;
			mat_a(cnt,2) = pts_src[cnt].y;
			mat_a(cnt,3) = pts_src[cnt].x * pts_src[cnt].x;
			mat_a(cnt,4) = pts_src[cnt].x * pts_src[cnt].y;
			mat_a(cnt,5) = pts_src[cnt].y * pts_src[cnt].y;
		}

		mat_a = mat_a.inverse();

		mat_x = mat_a * mat_bx;
		mat_y = mat_a * mat_by;

		this->fill_pixel(get_rgb(255,255,255));
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				double x = mat_x(0,0)  + mat_x(1,0) * i + mat_x(2,0) * j +
					mat_x(3,0) * i * i + mat_x(5,0) * j * j +
					mat_x(4,0) * i * j;

				double y = mat_y(0,0)  + mat_y(1,0) * i + mat_y(2,0) * j +
					mat_y(3,0) * i * i + mat_y(5,0) * j * j +
					mat_y(4,0) * i * j;

				if (x < 0 || x >= get_width() - 1)
					continue;
				else if (y <0 || y >= get_height() - 1)
					continue;

				int a = (int)(x);
				int b = (int)(y);

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_0(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_0(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_0(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_1(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_1(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_2(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_2(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_3(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_3(a+1,b+1) * (x - a) *  (y - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);
			}
		}
	}

	//image_mathing in binomial with simple;
	void geometry_match_binomial_simple(const vector<cls_point<int> >& pts_src,const vector<cls_point<int> >& pts_des)
	{
		if (pts_src.size() <4 || pts_src.size() != pts_des.size())
		{
			throw cls_exception_execution("the input for birepait is bad.");
		}

		cls_matrix<double> mat_x(4,1);
		cls_matrix<double> mat_y(4,1);
		cls_matrix<double> mat_bx(4,1);
		cls_matrix<double> mat_by(4,1);
		cls_matrix<double> mat_a(4,4);

		cls_vision_basis basis = *this;

		for(int cnt = 0;cnt < 4;cnt ++)
		{
			mat_bx(cnt,0) = pts_des[cnt].x;
			mat_by(cnt,0) = pts_des[cnt].y;

			mat_a(cnt,0) = 1;
			mat_a(cnt,1) = pts_src[cnt].x;
			mat_a(cnt,2) = pts_src[cnt].y;
			mat_a(cnt,3) = pts_src[cnt].x * pts_src[cnt].y;
		}

		mat_a = mat_a.inverse();

		mat_x = mat_a * mat_bx;
		mat_y = mat_a * mat_by;

		this->fill_pixel(get_rgb(255,255,255));
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				double x = mat_x(0,0)  + mat_x(1,0) * i + mat_x(2,0) * j +
					mat_x(3,0) * i * j;

				double y = mat_y(0,0)  + mat_y(1,0) * i + mat_y(2,0) * j +
					mat_y(3,0) * i * j;

				if (x < 0 || x >= get_width() - 1)
					continue;
				else if (y <0 || y >= get_height() - 1)
					continue;

				int a = (int)(x);
				int b = (int)(y);

				unsigned node_0 = basis.get_channel_value_0(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_0(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_0(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_0(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_1 = basis.get_channel_value_1(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_1(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_1(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_1(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_2 = basis.get_channel_value_2(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_2(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_2(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_2(a+1,b+1) * (x - a) *  (y - b);

				unsigned node_3 = basis.get_channel_value_3(a,b) * (a + 1 - x) * (b + 1 - y)
					+ basis.get_channel_value_3(a+1,b) * (x - a) * (b + 1 - y)
					+ basis.get_channel_value_3(a,b+1) * (a + 1 - x) * (y - b)
					+ basis.get_channel_value_3(a+1,b+1) * (x - a) *  (y - b);

				this->set_channel_value_0(i,j,node_0);
				this->set_channel_value_1(i,j,node_1);
				this->set_channel_value_2(i,j,node_2);
				this->set_channel_value_3(i,j,node_3);
			}
		}
	}
};
