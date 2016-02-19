#include "lib_import.h"

#ifndef USING_INTEL_MKL
class scls_signal_trans
{
public:
	static int get_fourier_size(int count)
	{
		int p = 1;
		while(p < count)
		{
			p <<= 1;
		}

		return p;
	}

	static int get_fourier_power(int count)
	{
		int p = 1;
		int re = 0;
		while(p < count)
		{
			p <<= 1;
			re++;
		}

		return re;
	}

	static void fourier(complex<double> *space_data,complex<double> *freq_data,int count)
	{
		int power = get_fourier_power(count);
		count = get_fourier_size(count);

		complex<double> *weight = new complex<double>[count/2];
		complex<double> *x_1 = new complex<double>[count];
		complex<double> *x_2 = new complex<double>[count];

		for(int cnt=0;cnt<count/2;cnt++)
		{
			double angel = - cnt * pi * 2.0 /count; 
			weight[cnt] = complex<double>(cos(angel),sin(angel));
		}

		copy(space_data,space_data + count,x_1);

		for(int k=0;k<power;k++)
		{
			for(int j=0;j<(1<<k);j++)
			{
				int size_interval = 1<<(power - k);
				for(int i=0;i<size_interval/2;i++)
				{
					int p = j * size_interval;
					x_2[p + i] = x_1[p + i] + x_1[p + i + size_interval/2];
					x_2[p + i +size_interval/2] = (x_1[p + i] - x_1[p + i + size_interval/2]) * weight[i * (1<<k)];
				}
			}

			swap(x_1,x_2);
		}

		for(int j=0;j<count;j++)
		{
			int p = 0;
			for(int i=0;i<power;i++)
			{
				if (j & (1<<i))
					p += 1<<(power - i - 1);
			}

			freq_data[j] = x_1[p];
		}

		delete[] weight;
		delete[] x_1;
		delete[] x_2;
	}

	static void i_fourier(complex<double>* freq_data,complex<double>* space_data,int count)
	{
		int power = get_fourier_power(count);
		count = get_fourier_size(count);

		complex<double> *x = new complex<double>[count];
		copy(freq_data,freq_data+count,x);

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(x[cnt].real(),-x[cnt].imag());
		}

		fourier(x,space_data,count);

		for(int cnt=0;cnt<count;cnt++)
		{
			space_data[cnt] = complex<double>(space_data[cnt].real()/count, - space_data[cnt].imag() / count);
		}

		delete[] x;
	}

	static void cosine(double* space_data,double* time_data,int count)
	{
		count = get_fourier_size(count);
		complex<double>* x = new complex<double>[count * 2];
		fill(x,x+2*count,complex<double>(0,0));

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(space_data[cnt],0);
		}

		fourier(x,x,count*2);

		double factor = 1.0/ sqrt(count);
		time_data[0] = x[0].real() * factor;
		
		factor *= sqrt(2.0);

		for(int cnt=1;cnt<count;cnt++)
		{
			time_data[cnt] = (x[cnt].real() * cos(cnt*pi/(count*2.0)) + x[cnt].imag() * sin(cnt*pi/(count*2.0)))*factor;
		}

		delete [] x;
	}

	static void i_cosine(double* space_data,double* time_data,int count)
	{
		count = get_fourier_size(count);
		complex<double>* x = new complex<double>[count * 2];
		fill(x,x+2*count,complex<double>(0,0));

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(space_data[cnt] * cos(cnt*pi/(count*2.0)), space_data[cnt] * sin(cnt * pi / (count * 2.0)));
		}

		i_fourier(x,x,count*2);

		double factor = 1.0/ sqrt(count);
		for(int cnt=0;cnt<count;cnt++)
		{
			time_data[cnt] = (1- sqrt(2))*factor*space_data[0] + sqrt(2)*factor*x[cnt].real() * count * 2;
		}

		delete [] x;
	}

	static void fourier_transform_2D(const cls_matrix<double>& src,cls_matrix<complex<double> >& mat)
	{
		cls_matrix<complex<double> > dsrc(src.get_width(),src.get_height());

		for(int i=0;i<dsrc.get_width();i++)
		{
			for(int j=0;j<dsrc.get_height();j++)
			{
				dsrc(i,j) = complex<double>(src(i,j),0);
			}
		}

		fourier_transform_2D(dsrc,mat);
	}

	static void fourier_transform_2D(const cls_matrix<complex<double> >& src,cls_matrix<complex<double> >& mat)
	{
		int w = scls_signal_trans::get_fourier_size(src.get_width());
		int h = scls_signal_trans::get_fourier_size(src.get_height());

		mat.resize(w,h);

		complex<double>* space_data = new complex<double>[w * h];
		complex<double>* freq_data = new complex<double>[w * h];

		for(int i=0;i<w;i++)
		{
			for(int j=0;j<h;j++)
			{
				if (i < src.get_width() && j < src.get_height())
				{
					space_data[i + w*j] = src(i,j);
				}
				else
				{
					space_data[i + w*j] = complex<double>(0,0);
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

	static void i_fourier_transform_2D(const cls_matrix<complex<double>>& mat,cls_matrix<double>& des)
	{
		cls_matrix<complex<double> > ddes;
		i_fourier_transform_2D(mat,ddes);

		des.resize(ddes.get_width(),ddes.get_height());

		for(int i=0;i<des.get_width();i++)
		{
			for(int j=0;j<des.get_height();j++)
			{
				des(i,j) = sqrt( ddes(i,j).real()*ddes(i,j).real() + ddes(i,j).imag()*ddes(i,j).imag() );
			}
		}
	}

	static void i_fourier_transform_2D(const cls_matrix<complex<double> >& mat,cls_matrix<complex<double> >& des)
	{
		int w = scls_signal_trans::get_fourier_size(mat.get_width());
		int h = scls_signal_trans::get_fourier_size(mat.get_height());

		des.resize(w,h);

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

		for(int i=0;i<mat.get_width();i++)
		{
			for(int j=0;j<mat.get_height();j++)
			{
				des(i,j) = space_data[i*h + j];
			}
		}

		delete[] freq_data;
		delete[] space_data;
	}
};
#else
class scls_signal_trans
{
public:
	static int get_fourier_size(int count)
	{
		int p = 1;
		while(p < count)
		{
			p <<= 1;
		}

		return p;
	}

	static int get_fourier_power(int count)
	{
		int p = 1;
		int re = 0;
		while(p < count)
		{
			p <<= 1;
			re++;
		}

		return re;
	}

	static void fourier(complex<double> *space_data,complex<double> *freq_data,int count)
	{
		DFTI_DESCRIPTOR_HANDLE hand;

		DftiCreateDescriptor(&hand,DFTI_DOUBLE,DFTI_COMPLEX,1,get_fourier_size(count));
		DftiSetValue( hand, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
		DftiCommitDescriptor(hand);
		DftiComputeForward(hand,freq_data,space_data);
		DftiFreeDescriptor(&hand);
	}

	static void i_fourier(complex<double>* freq_data,complex<double>* space_data,int count)
	{
		int power = get_fourier_power(count);
		count = get_fourier_size(count);

		complex<double> *x = new complex<double>[count];
		copy(freq_data,freq_data+count,x);

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(x[cnt].real(),-x[cnt].imag());
		}

		fourier(x,space_data,count);

		for(int cnt=0;cnt<count;cnt++)
		{
			space_data[cnt] = complex<double>(space_data[cnt].real()/count, - space_data[cnt].imag() / count);
		}

		delete[] x;
	}

	static void cosine(double* space_data,double* time_data,int count)
	{
		count = get_fourier_size(count);
		complex<double>* x = new complex<double>[count * 2];
		fill(x,x+2*count,complex<double>(0,0));

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(space_data[cnt],0);
		}

		fourier(x,x,count*2);

		double factor = 1.0/ sqrt(count);
		time_data[0] = x[0].real() * factor;

		factor *= sqrt(2.0);

		for(int cnt=1;cnt<count;cnt++)
		{
			time_data[cnt] = (x[cnt].real() * cos(cnt*pi/(count*2.0)) + x[cnt].imag() * sin(cnt*pi/(count*2.0)))*factor;
		}

		delete [] x;
	}

	static void i_cosine(double* space_data,double* time_data,int count)
	{
		count = get_fourier_size(count);
		complex<double>* x = new complex<double>[count * 2];
		fill(x,x+2*count,complex<double>(0,0));

		for(int cnt=0;cnt<count;cnt++)
		{
			x[cnt] = complex<double>(space_data[cnt] * cos(cnt*pi/(count*2.0)), space_data[cnt] * sin(cnt * pi / (count * 2.0)));
		}

		i_fourier(x,x,count*2);

		double factor = 1.0/ sqrt(count);
		for(int cnt=0;cnt<count;cnt++)
		{
			time_data[cnt] = (1- sqrt(2))*factor*space_data[0] + sqrt(2)*factor*x[cnt].real() * count * 2;
		}

		delete [] x;
	}
};
#endif