#include "lib_import.h"

class cls_vision_basis
	:public virtual cls_image
{
public:
	cls_vision_basis(const cls_image& img)
		:cls_image(img)
	{
		;
	}

	cls_vision_basis()
	{
		;
	}

public:
	const static unsigned int neighborhood_four = 1;
	const static unsigned int neighborhood_diag = 2;
	const static unsigned int neighborhood_eight = 3;

	static double get_distance_as_euclidean(cls_point<int> src,cls_point<int> des)
	{
		return sqrt((src.x - des.x)*(src.x - des.x) + (src.y - des.y)*(src.y - des.y));
	}

	static double get_distance_as_city_block(cls_point<int> src,cls_point<int> des)
	{
		return labs(src.x - des.x) + labs(src.y - des.y);
	}

	static double get_distance_chessboard(cls_point<int> src,cls_point<int> des)
	{
		return max(labs(src.x - des.x) , labs(src.y - des.y));
	}

	void remove_color()
	{
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				set_gray(i,j,get_gray(i,j));
			}
		}
	}

	void fourier_transform(cls_matrix<complex<double> >& mat,int channel = -1) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);

		complex<double>* space_data = new complex<double>[w * h];
		complex<double>* freq_data = new complex<double>[w * h];

		if (channel == -1)
		{
			for(int i=0;i<w;i++)
			{
				for(int j=0;j<h;j++)
				{
					if (i < get_width() && j < get_height())
					{
						space_data[i + w*j] = complex<double>(get_gray(i,j),0);
					}
					else
					{
						space_data[i + w*j] = complex<double>(0,0);
					}
				}
			}
		}
		else
		{
			for(int i=0;i<w;i++)
			{
				for(int j=0;j<h;j++)
				{
					if (i < get_width() && j < get_height())
					{
						space_data[i + w*j] = complex<double>(get_channel_value(i,j,channel),0);
					}
					else
					{
						space_data[i + w*j] = complex<double>(0,0);
					}
				}
			}
		}

		for(int cnt=0;cnt<h;cnt++)
		{
			scls_signal_trans::fourier(&space_data[w*cnt],&freq_data[w*cnt],w);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				space_data[j + h * i] = freq_data[i + j * w];
			}
		}

		for(int cnt=0;cnt<w;cnt++)
		{
			scls_signal_trans::fourier(&space_data[cnt*h],&freq_data[cnt*h],h);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				mat(i,j) = freq_data[i*h + j];
			}
		}

		delete [] space_data;
		delete [] freq_data;
	}

	void i_fourier_transform(const cls_matrix<complex<double> >& mat,int channel = -1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		complex<double> *space_data = new complex<double>[w* h];
		complex<double> *freq_data = new complex<double>[w*h];

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				freq_data[i + w*j] = mat(i,j);
			}
		}

		for(int cnt=0;cnt<h;cnt++)
		{
			scls_signal_trans::i_fourier(&freq_data[w *cnt],&space_data[w *cnt],w);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				freq_data[j + h*i] = space_data[i + w*j];
			}
		}

		for(int cnt=0;cnt<w;cnt++)
		{
			scls_signal_trans::i_fourier(&freq_data[cnt*h],&space_data[cnt*h],h);
		}

		if (channel == -1)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					int d_cur =sqrt(space_data[i*h + j].real()*space_data[i*h + j].real()  +  space_data[i*h + j].imag()*space_data[i*h + j].imag()) + 0.5;
					set_gray(i,j,get_suitable_byte(d_cur));
				}
			}
		}
		else
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					int d_cur =sqrt(space_data[i*h + j].real()*space_data[i*h + j].real()  +  space_data[i*h + j].imag()*space_data[i*h + j].imag()) + 0.5;
					set_channel_value(i,j,get_suitable_byte(d_cur),channel);
				}
			}
		}

		delete[] freq_data;
		delete[] space_data;
	}

	void cosine_transform(cls_matrix<double>& mat,int channel = -1) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);

		double* space_data = new double[w * h];
		double* freq_data = new double[w * h];

		if (channel == -1)
		{
			for(int i=0;i<w;i++)
			{
				for(int j=0;j<h;j++)
				{
					if (i < get_width() && j < get_height())
					{
						space_data[i + w*j] = get_gray(i,j);
					}
					else
					{
						space_data[i + w*j] = 0;
					}
				}
			}
		}
		else
		{
			for(int i=0;i<w;i++)
			{
				for(int j=0;j<h;j++)
				{
					if (i < get_width() && j < get_height())
					{
						space_data[i + w*j] = get_channel_value(i,j,channel);
					}
					else
					{
						space_data[i + w*j] = 0;
					}
				}
			}
		}

		for(int cnt=0;cnt<h;cnt++)
		{
			scls_signal_trans::cosine(&space_data[w*cnt],&freq_data[w*cnt],w);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				space_data[j + h * i] = freq_data[i + j * w];
			}
		}

		for(int cnt=0;cnt<w;cnt++)
		{
			scls_signal_trans::cosine(&space_data[cnt*h],&freq_data[cnt*h],h);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				mat(i,j) = freq_data[i*h + j];
			}
		}

		delete [] space_data;
		delete [] freq_data;
	}

	void i_cosine_transform(const cls_matrix<double>& mat,int channel = -1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		double *space_data = new double[w* h];
		double *freq_data = new double[w*h];

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				freq_data[i + w*j] = mat(i,j);
			}
		}

		for(int cnt=0;cnt<h;cnt++)
		{
			scls_signal_trans::i_cosine(&freq_data[w *cnt],&space_data[w *cnt],w);
		}

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				freq_data[j + h*i] = space_data[i + w*j];
			}
		}

		for(int cnt=0;cnt<w;cnt++)
		{
			scls_signal_trans::i_cosine(&freq_data[cnt*h],&space_data[cnt*h],h);
		}

		if (channel == -1)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					int d_cur = space_data[i*h + j] + 0.5;
					set_gray(i,j,get_suitable_byte(d_cur));
				}
			}
		}
		else
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					int d_cur = space_data[i*h + j] + 0.5;
					set_channel_value(i,j,get_suitable_byte(d_cur),channel);
				}
			}
		}

		delete[] freq_data;
		delete[] space_data;
	}

	void get_identical_frequency_matrix(cls_matrix<double>& mat) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				mat(i,j) = 1.0;
			}
		}
	}
};