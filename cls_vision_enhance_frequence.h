#include "lib_import.h"

class cls_vision_enhance_frequence
	:public virtual cls_vision_basis
{
public:
	//enhance the image with the FFT.
	void enhance_frequence_map(const cls_matrix<double>& mat,int channel = -1)
	{
		if (channel == -2)
		{
			enhance_frequence_map(mat,0);
			enhance_frequence_map(mat,1);
			enhance_frequence_map(mat,2);
			enhance_frequence_map(mat,3);
			return;
		}

		if (mat.get_width() != scls_signal_trans::get_fourier_size(get_width()) ||
			mat.get_height() != scls_signal_trans::get_fourier_size(get_height()))
		{
			throw cls_exception_execution("the two matrix is not the same size.");
		}

		cls_matrix<complex<double> > mat_trans;

		fourier_transform(mat_trans,channel);

		for(int i=0;i<mat.get_width();i++)
		{
			for(int j=0;j<mat.get_height();j++)
			{
				mat_trans(i,j) *= mat(i,j);
			}
		}

		i_fourier_transform(mat_trans,channel);
	}

	//enhance the image with the DCT.
	void enhance_cosine_map(const cls_matrix<double>& mat,int channel = -1)
	{
		if (channel == -2)
		{
			enhance_cosine_map(mat,0);
			enhance_cosine_map(mat,1);
			enhance_cosine_map(mat,2);
			enhance_cosine_map(mat,3);
			return;
		}

		if (mat.get_width() != scls_signal_trans::get_fourier_size(get_width()) ||
			mat.get_height() != scls_signal_trans::get_fourier_size(get_height()))
		{
			throw cls_exception_execution("the two matrix is not the same size.");
		}

		cls_matrix<double> mat_trans;

		cosine_transform(mat_trans,channel);

		for(int i=0;i<mat.get_width();i++)
		{
			for(int j=0;j<mat.get_height();j++)
			{
				mat_trans(i,j) *= mat(i,j);
			}
		}

		i_cosine_transform(mat_trans,channel);
	}

	//get ideal low frequance filter;
	void get_enhance_ideal_low_freq(cls_matrix<double>& mat,double d_r) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if ((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2) > d_r*d_r)
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 0;
				}
				else
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1;
				}
			}
		}
	}


	void enhance_ideal_low_freq(double d_r,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_ideal_low_freq(mat,d_r);
		enhance_frequence_map(mat,channel);
	}

	void enhance_ideal_low_freq_percent(double d_r,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_ideal_low_freq(min(w,h)*d_r,channel);
	}

	void get_enhance_butter_worth_low_freq(cls_matrix<double>& mat,double d_freq_std,double d_exp) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				double d_r = sqrt((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2));
				mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1.0/(1.0 + pow(d_r/d_freq_std,2*d_exp));
			}
		}
	}

	void enhance_butter_worth_low_freq(double d_freq_std,double d_exp,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_butter_worth_low_freq(mat,d_freq_std,d_exp);
		enhance_frequence_map(mat,channel);
	}

	void enhance_butter_worth_low_freq_percent(double d_r,double d_exp,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_butter_worth_low_freq(min(w,h)*d_r,d_exp,channel);
	}

	void get_enhance_exponential_low_freq(cls_matrix<double>& mat,double d_freq_std,double d_exp) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				double d_r = sqrt((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2));
				mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = pow(math_e,pow(-(d_r/d_freq_std),d_exp*2));
			}
		}
	}

	void enhance_exponential_low_freq(double d_freq_std,double d_exp,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_exponential_low_freq(mat,d_freq_std,d_exp);
		enhance_frequence_map(mat,channel);
	}

	void enhance_exponential_low_freq_percent(double d_r,double d_exp,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_exponential_low_freq(min(w,h)*d_r,d_exp,channel);
	}

	void result_fourier(cls_image& des,int channel=-1,double d_scaler=100)
	{
		cls_matrix<complex<double> > mat_trans;
		cls_matrix<double> mat_real;

		fourier_transform(mat_trans,channel);

		mat_real.resize(mat_trans.get_width(),mat_trans.get_height());
		des.resize(mat_trans.get_width(),mat_trans.get_height());

		for(int i=0;i<des.get_width();i++)
		{
			for(int j=0;j<des.get_height();j++)
			{
				mat_real(i,j) = sqrt(mat_trans(i,j).real()*mat_trans(i,j).real() + mat_trans(i,j).imag()*mat_trans(i,j).imag()) / d_scaler;
			}
		}

		for(int i=0;i<des.get_width();i++)
		{
			for(int j=0;j<des.get_height();j++)
			{
				des.set_gray(i,j,get_suitable_byte((mat_real(i,j))));
			}
		}
	}

	void get_enhance_ideal_high_freq(cls_matrix<double>& mat,double d_r) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if ((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2) > d_r*d_r)
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1;
				}
				else
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 0;
				}
			}
		}
	}

	void enhance_ideal_high_freq(double d_r,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_ideal_high_freq(mat,d_r);
		enhance_frequence_map(mat,channel);
	}

	void enhance_ideal_high_freq_percent(double d_r,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_ideal_high_freq(min(w,h)*d_r,channel);
	}

	void get_enhance_butter_worth_high_freq(cls_matrix<double>& mat,double d_freq_std,double d_exp) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				double d_r = sqrt((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2));
				mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1.0/(1.0 + pow(d_freq_std / d_r,2*d_exp));
			}
		}
	}

	void enhance_butter_worth_high_freq(double d_freq_std,double d_exp,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_butter_worth_high_freq(mat,d_freq_std,d_exp);
		enhance_frequence_map(mat,channel);
	}

	void enhance_butter_worth_high_freq_percent(double d_r,double d_exp,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_butter_worth_high_freq(min(w,h)*d_r,d_exp,channel);
	}

	void get_enhance_exponential_high_freq(cls_matrix<double>& mat,double d_freq_std,double d_exp) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);
		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				double d_r = sqrt((i-w/2)*(i-w/2) + (j-h/2)*(j-h/2));
				mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1.0 - pow(math_e,pow(-(d_r/d_freq_std),d_exp*2));
			}
		}
	}

	void enhance_exponential_high_freq(double d_freq_std,double d_exp,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_exponential_high_freq(mat,d_freq_std,d_exp);
		enhance_frequence_map(mat,channel);
	}

	void enhance_exponential_high_freq_percent(double d_r,double d_exp,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_exponential_high_freq(min(w,h)*d_r,d_exp,channel);
	}

	void get_enhance_ideal_band_stop(cls_matrix<double>& mat,const cls_point<unsigned>& pt,double d_r) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if ((i-pt.x)*(i-pt.x) + (j-pt.y)*(j-pt.y) > d_r*d_r)
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1;
				}
				else
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 0;
				}
			}
		}
	}

	void enhance_ideal_band_stop(const cls_point<unsigned>& pt,double d_r,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_ideal_band_stop(mat,pt,d_r);

		enhance_frequence_map(mat,channel);
	}

	void enhance_ideal_band_stop_percent(const cls_point<unsigned>& pt,double d_r,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_ideal_band_stop(pt,min(w,h)*d_r,channel);
	}

	void get_enhance_ideal_band_pass(cls_matrix<double>& mat,const cls_point<unsigned>& pt,double d_r) const
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		mat.resize(w,h);

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if ((i-pt.x)*(i-pt.x) + (j-pt.y)*(j-pt.y) < d_r*d_r)
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 1;
				}
				else
				{
					mat(i<w/2?i+w/2:i-w/2,j<h/2?j+h/2:j-h/2) = 0;
				}
			}
		}
	}

	void enhance_ideal_band_pass(const cls_point<unsigned>& pt,double d_r,int channel=-1)
	{
		cls_matrix<double> mat;
		get_enhance_ideal_band_pass(mat,pt,d_r);

		enhance_frequence_map(mat,channel);
	}

	void enhance_ideal_band_pass_percent(const cls_point<unsigned>& pt,double d_r,int channel=-1)
	{
		int w = scls_signal_trans::get_fourier_size(get_width());
		int h = scls_signal_trans::get_fourier_size(get_height());

		enhance_ideal_band_pass(pt,min(w,h)*d_r,channel);
	}

	void enhance_homomorphic(const cls_matrix<double>& mat,int channel = -1)
	{
		if (channel == -2)
		{
			enhance_homomorphic(mat,1);
			enhance_homomorphic(mat,2);
			enhance_homomorphic(mat,3);
			enhance_homomorphic(mat,4);
		}

		{
			double d_factor = 255.0/log(256.0);
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_channel_value(i,j,log(1.0*get_channel_value(i,j,channel)) * d_factor,channel);
				}
			}
		}

		enhance_frequence_map(mat);

		{
			double d_factor = log(256.0)/255.0;
			for(int i=0;i<get_width();i++)
			{
				for(int j=0;j<get_height();j++)
				{
					set_channel_value(i,j,log(1.0*get_channel_value(i,j,channel)) * d_factor,channel);
				}
			}
		}
	}
};