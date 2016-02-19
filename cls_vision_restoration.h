#include "lib_import.h"

class cls_vision_restoration
	:public virtual cls_vision_basis
{
public:
	void restore_inverse(const cls_matrix<double>& mat,int channel = -1)
	{
		cls_matrix<double> h(scls_signal_trans::get_fourier_size(get_width()),scls_signal_trans::get_fourier_size(get_height()));
		for(int i=0;i<h.get_width();i++)
		{
			for(int j=0;j<h.get_height();j++)
			{
				if (i<mat.get_width() && j < mat.get_height())
				{
					h(i,j) = mat(i,j);
				}
				else
				{
					h(i,j) = 0;
				}
			}
		}

		cls_matrix<complex<double> > h_t;
		scls_signal_trans::fourier_transform_2D(h,h_t);

		cls_matrix<complex<double> > g_t;
		fourier_transform(g_t,channel);

		for(int i=0;i<g_t.get_width();i++)
		{
			for(int j=0;j<g_t.get_height();j++)
			{
				if (h_t(i,j).real()*h_t(i,j).real() +h_t(i,j).imag()*h_t(i,j).imag() > 1e-3)
					g_t(i,j) = g_t(i,j) / h_t(i,j);
			}
		}

		cls_matrix<double> re;
		scls_signal_trans::i_fourier_transform_2D(g_t,re);

		double d_min = inf;
		double d_max = -inf;

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				d_min = min(d_min,re(i,j));
				d_max = max(d_max,re(i,j));
			}
		}

		if (channel == -1)
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_gray(i,j,get_suitable_byte((re(i,j)-d_min)/(d_max-d_min)*255));
				}
			}
		}
		else
		{
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_channel_value(i,j,get_suitable_byte((re(i,j)-d_min)/(d_max-d_min)*255.0),channel);
				}
			}
		}
	}
};