#include "lib_import.h"
#pragma  once

#define get_math_angel(a) (a/180.0*pi)

//the basic class for image.
class cls_image
	:public cls_serialized_object
{
public:

	//the matrix of the color;
	cls_matrix<unsigned> mat_image;

public:

	/*
		if you want to load a bmp or jpg, please use load_image in algorithm.
	*/

	//save the matrix of color;
	virtual void save(ofstream& fout) const
	{
		mat_image.save(fout);
	}

	//load the matrix of color;
	virtual void load(ifstream& fin)
	{
		mat_image.load(fin);
	}

	//the default constuctor will build an image with 1*1 size;
	cls_image():mat_image(1,1)
	{
		;
	}

	//build the image for w*h size;
	cls_image(int w,int h):mat_image(w,h)
	{
		;
	}

	//copy constructor;
	cls_image(const cls_image& that)
	{
		mat_image = that.mat_image;
	}

	//construct from cls_matrix;
	cls_image(const cls_matrix<unsigned>& src)
	{
		mat_image = src;
	}

	//the deconstructor;
	~cls_image()
	{

	}

	//init pixel for a black w*h size;
	void init_pixel(int w,int h,unsigned int value = 0)
	{
		mat_image.resize(w,h);
		mat_image.fill(value);
	}

	//fill the image with the color;
	void fill_pixel(unsigned int value = 0)
	{
		mat_image.fill(value);
	}

	//get the pixel of the point;
	unsigned get_pixel(int x,int y) const
	{
		return mat_image(x,y);
	}

	//set the pixel of the point;
	void set_pixel(int x,int y,unsigned value)
	{
		mat_image(x,y) = value;
	}

	//get the height of the image;
	inline
	unsigned get_height() const
	{
		return mat_image.get_height();
	}

	//get the width of the image;
	inline
	unsigned get_width() const
	{
		return mat_image.get_width();
	}

protected:
	//get the corresponding channel value;
	inline
	unsigned char get_channel_value(int x,int y,int number) const
	{
		unsigned char *pdata = (unsigned char*)mat_image.get_raw_data();

		return pdata[(x*mat_image.get_height()+y)*4 + number];
	}

public:
	//construct from cls_matrix;
	inline 
	void assign_from_matrix(cls_matrix<unsigned>& src)
	{
		mat_image = src;
	}

	//get the byte color for channel 0; 
	inline
	unsigned char get_channel_value_0(int x,int y) const
	{
		return  get_channel_value(x,y,0);
	}

	//get the byte color for channel 1; 
	inline
	unsigned char get_channel_value_1(int x,int y) const
	{
		return  get_channel_value(x,y,1);
	}

	//get the byte color for channel 2; 
	inline
	unsigned char get_channel_value_2(int x,int y) const
	{
		return  get_channel_value(x,y,2);
	}

	//get the byte color for channel 3; 
	inline
	unsigned char get_channel_value_3(int x,int y) const
	{
		return  get_channel_value(x,y,3);
	}

protected:
	//set the corresponding channel value;
	void set_channel_value(int x,int y,unsigned char value,int channel)
	{
		unsigned char *pdata = (unsigned char*)mat_image.get_raw_data();

		pdata[(x*mat_image.get_height() +y)*4 + channel] = value;
	}

public:
	//set the channel 0's value;
	void set_channel_value_0(int x,int y,unsigned char value)
	{
		set_channel_value(x,y,value,0);
	}

	//set the channel 1's value;
	void set_channel_value_1(int x,int y,unsigned char value)
	{
		set_channel_value(x,y,value,1);
	}

	//set the channel 2's value;
	void set_channel_value_2(int x,int y,unsigned char value)
	{
		set_channel_value(x,y,value,2);
	}

	//set the channel 3's value;
	void set_channel_value_3(int x,int y,unsigned char value)
	{
		set_channel_value(x,y,value,3);
	}

	//get the gray value of the corresponding position;
	inline
	int get_gray(int x,int y) const
	{
		return ( 0.30 * get_channel_value_1(x,y)
			+ 0.59 * get_channel_value_2(x,y)
			+ 0.11 * get_channel_value_3(x,y)
			);
	}

	//set the gray value of the corresponding position;
	inline
	void set_gray(int x,int y,unsigned char value)
	{
		set_channel_value_0(x,y,value);
		set_channel_value_1(x,y,value);
		set_channel_value_2(x,y,value);
		set_channel_value_3(x,y,value);
	}

	//the assigned operator;
	cls_image& operator = (const cls_image& that) 
	{
		mat_image = that.mat_image;
		
		return *this;
	}

	//get the suitable x position within the image;
	int get_suitable_posititon_x(int x) const
	{
		if (x <= 0)
			return 0;
		else if (x >= this->get_width())
			return this->get_width() - 2;
		else
			return x;
	}

	//get the suitable y position within the image;
	int get_suitable_posititon_y(int y) const
	{
		if (y <= 0)
			return 0;
		else if (y >= this->get_height())
			return this->get_height() - 2;
		else
			return y;
	}

	//get the suitable byte within the image's color range;
	static byte get_suitable_byte(int p)
	{
		return min(max(p,0),255);
	}

	//the plus operator;
	cls_image operator+(const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,get_suitable_byte(this->get_channel_value_0(i,j) + that.get_channel_value_0(i,j)));
				img.set_channel_value_1(i,j,get_suitable_byte(this->get_channel_value_1(i,j) + that.get_channel_value_1(i,j)));
				img.set_channel_value_2(i,j,get_suitable_byte(this->get_channel_value_2(i,j) + that.get_channel_value_2(i,j)));
				img.set_channel_value_3(i,j,get_suitable_byte(this->get_channel_value_3(i,j) + that.get_channel_value_3(i,j)));
			}
		}

		return img;
	}

	//the minus operator;
	cls_image operator-(const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,get_suitable_byte(this->get_channel_value_0(i,j) - that.get_channel_value_0(i,j)));
				img.set_channel_value_1(i,j,get_suitable_byte(this->get_channel_value_1(i,j) - that.get_channel_value_1(i,j)));
				img.set_channel_value_2(i,j,get_suitable_byte(this->get_channel_value_2(i,j) - that.get_channel_value_2(i,j)));
				img.set_channel_value_3(i,j,get_suitable_byte(this->get_channel_value_3(i,j) - that.get_channel_value_3(i,j)));			}
		}

		return img;
	}

	//the mutiply operator;
	cls_image operator*(const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,get_suitable_byte(this->get_channel_value_0(i,j) * that.get_channel_value_0(i,j)));
				img.set_channel_value_1(i,j,get_suitable_byte(this->get_channel_value_1(i,j) * that.get_channel_value_1(i,j)));
				img.set_channel_value_2(i,j,get_suitable_byte(this->get_channel_value_2(i,j) * that.get_channel_value_2(i,j)));
				img.set_channel_value_3(i,j,get_suitable_byte(this->get_channel_value_3(i,j) * that.get_channel_value_3(i,j)));			}
		}

		return img;
	}

	//the divide operator;
	cls_image operator/(const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,get_suitable_byte(this->get_channel_value_0(i,j) / ( 0.001 +  that.get_channel_value_0(i,j))));
				img.set_channel_value_1(i,j,get_suitable_byte(this->get_channel_value_1(i,j) / ( 0.001 +  that.get_channel_value_1(i,j))));
				img.set_channel_value_2(i,j,get_suitable_byte(this->get_channel_value_2(i,j) / ( 0.001 +  that.get_channel_value_2(i,j))));
				img.set_channel_value_3(i,j,get_suitable_byte(this->get_channel_value_3(i,j) / ( 0.001 +  that.get_channel_value_3(i,j))));
			}
		}

		return img;
	}

	//the and operator;
	cls_image operator & (const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,this->get_channel_value_0(i,j) & that.get_channel_value_0(i,j));
				img.set_channel_value_1(i,j,this->get_channel_value_1(i,j) & that.get_channel_value_1(i,j));
				img.set_channel_value_2(i,j,this->get_channel_value_2(i,j) & that.get_channel_value_2(i,j));
				img.set_channel_value_3(i,j,this->get_channel_value_3(i,j) & that.get_channel_value_3(i,j));
			}
		}

		return img;
	}

	//the or operator;
	cls_image operator | (const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,this->get_channel_value_0(i,j) | that.get_channel_value_0(i,j));
				img.set_channel_value_1(i,j,this->get_channel_value_1(i,j) | that.get_channel_value_1(i,j));
				img.set_channel_value_2(i,j,this->get_channel_value_2(i,j) | that.get_channel_value_2(i,j));
				img.set_channel_value_3(i,j,this->get_channel_value_3(i,j) | that.get_channel_value_3(i,j));			}
		}

		return img;
	}

	//the divide operator;
	cls_image operator ^ (const cls_image& that) const
	{
		cls_image img;

		if (get_width() != that.get_width() || get_height() != that.get_height())
			throw cls_exception_execution("the two matrises must be the same size,");

		img.resize(get_width(),get_height());

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				img.set_channel_value_0(i,j,this->get_channel_value_0(i,j) ^ that.get_channel_value_0(i,j));
				img.set_channel_value_1(i,j,this->get_channel_value_1(i,j) ^ that.get_channel_value_1(i,j));
				img.set_channel_value_2(i,j,this->get_channel_value_2(i,j) ^ that.get_channel_value_2(i,j));
				img.set_channel_value_3(i,j,this->get_channel_value_3(i,j) ^ that.get_channel_value_3(i,j));			}
		}

		return img;
	}

	//resize the image;
	void resize(int width,int height)
	{
		mat_image.resize(width,height);
	}

	//resize with the same image()
	void resize_remain(int width,int height)
	{
		cls_image img = *this;
		resize(width,height);

		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				set_pixel(i,j,img.get_pixel(i,j));
			}
		}
	}

public:
	//check if the two matrix is the same size;
	static void check_identifical(const cls_image& src,const cls_image& des)
	{
		if (src.get_width() != des.get_width() ||
			src.get_height() != des.get_height())
		{
			throw cls_exception_execution("the two matrix is not the same size;");
		}

		return;
	}

public:
	//reverse the image's color
	void reverse()
	{
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				set_channel_value_0(i,j,255 - get_channel_value_0(i,j));
				set_channel_value_1(i,j,255 - get_channel_value_1(i,j));
				set_channel_value_2(i,j,255 - get_channel_value_2(i,j));
				set_channel_value_3(i,j,255 - get_channel_value_3(i,j));
			}
		}
	}

public:
	//get the histogram for the spec channel;
	void get_histogram(vector<unsigned>& vb,int channel = -1) const
	{
		if (vb.size() != 256)
			throw cls_exception_execution("the histogram must be 256 steps");

		fill(vb.begin(),vb.end(),0);

		if (channel == -1)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					vb[get_gray(i,j)]++;
				}
			}
		}
		else
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					vb[get_channel_value(i,j,channel)]++;
				}
			}
		}
	}
};