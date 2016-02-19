#include "lib_import.h"
#pragma  once

inline
double sqrt(int src)
{
	return sqrt(1.0*src);
}

template<typename T>
double distance_euclid(cls_point<T> pt_src,cls_point<T> pt_des)
{
	return sqrt((pt_src.x - pt_des.x)*(pt_src.x - pt_des.x) + (pt_src.y - pt_des.y)*(pt_src.y - pt_des.y));
}

template<typename T>
void a_fast_fourier_trans(complex<T>*TD,complex<T>*FD,int r)
{
	complex<T> *W,*X1,*X2,*X;
	int count = 1<<r;

	W = new complex<T>[count/2];
	X1 = new complex<T>[count];
	X2 = new complex<T>[count];

	for(int i = 0;i<count/2;i++)
	{
		T angle = -i * pi *2 /count;
		W[i] = complex<T>(cos(angle),sin(angle));
	}

	for(int i=0;i<count;i++)
	{
		X1[i] = TD[i];
	}

	memcpy(X1,TD,sizeof(complex<T>) * count);

	for(int k = 0;k < r;k++)
	{
		for(int j =0;j<(1<<k);j++)
		{
			int bfsize = 1 << (r-k);
			for(int i = 0;i<bfsize /2 ;i++)
			{
				int p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize/2];
				X2[i + p +bfsize/2] = (X1[i + p] - X1[i + p + bfsize/2])* W[i * (1<<k)];
			}
		}

		X = X1;
		X1 = X2;
		X2 = X;
	}

	for(int j=0;j < count;j++)
	{
		int p = 0;
		for(int i = 0;i <r ;i++)
		{
			if (j & (1 << i))
			{
				p += 1 << (r - i -1);
			}
		}

		FD[j] = X1[p];
	}

	delete[] W;
	delete[] X1;
	delete[] X2;
}

template<typename T>
void a_inverse_fast_fourier_trans(complex<T>* cpx_td,complex<T>* cpx_fd,int r)
{
	complex<T> *X;
	int count = 1 << r;
	X = new complex<T>[count];

	memcpy(X,cpx_td,sizeof(complex<T>) * count);

	for(int i=0;i<count;i++)
	{
		X[i] = complex<T>(X[i].real(),-X[i].imag());
	}

	a_fast_fourier_trans(X,cpx_fd,r);

	for(int i=0;i<count;i++)
	{
		cpx_fd[i] = complex<T>( cpx_fd[i].real() / count , - cpx_fd[i].imag() / count );
	}

	delete[] X;
}

inline
int a_get_pow_num(int num)
{
	int rep = 0;
	int re = 1;

	while(re < num)
	{
		re *= 2;
		rep ++;
	}

	return rep;
}

template<typename T>
void a_fast_fourier_trans_in_2d(complex<T>* cpx_in,complex<T>* cpx_out,int width,int height)
{
	int pow_w = a_get_pow_num(width);
	int pow_h = a_get_pow_num(height);

	for(int cnt=0;cnt<height;cnt++)
	{
		a_fast_fourier_trans(&cpx_in[cnt*width],&cpx_out[cnt*width],pow_w);
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			cpx_in[i*height + j] = cpx_out[i + width*j];
		}
	}

	for(int cnt=0;cnt<width;cnt++)
	{
		a_fast_fourier_trans(&cpx_in[cnt*height],&cpx_out[cnt*height],pow_h);
	}
}

template<typename T>
void a_inverse_fast_fourier_trans_in_2d(complex<T>* cpx_in,complex<T>* cpx_out,int width,int height)
{
	int pow_w = a_get_pow_num(width);
	int pow_h = a_get_pow_num(height);


	for(int cnt=0;cnt<height;cnt++)
	{
		a_inverse_fast_fourier_trans(&cpx_in[cnt*width],&cpx_out[cnt*width],pow_w);
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			cpx_in[i*height + j] = cpx_out[i + width*j];
		}
	}

	for(int cnt=0;cnt<width;cnt++)
	{
		a_inverse_fast_fourier_trans(&cpx_in[cnt*height],&cpx_out[cnt*height],pow_h);
	}
}

template<typename T>
static T a_angel_to_radian(T angle)
{
	return ((angle)*pi/180.0);
}

template<typename T>
static T a_radian_to_angel(T radian)
{
	return (180.0/pi * radian);
}


template<typename T>static void a_fast_cosine_trans(complex<T>* cpx_in,complex<T>*cpx_out,int r)
{
	long	dotCount;
	double	d_coff;

	dotCount = 1<<r;
	complex<double>* temp = new complex<double>[dotCount*2];

	for(int cnt=0;cnt<dotCount;cnt++)
	{
		temp[cnt] = complex<T>(cpx_in[cnt]);
	}

	a_fast_fourier_trans(temp,temp,r+1);

	d_coff = 1/sqrt((double)dotCount);
	cpx_out[0] = temp[0].real()*d_coff;
	d_coff *= sqrt(2.0f);

	for(int cnt = 1; cnt < dotCount; cnt ++)
	{
		cpx_out[cnt]=(temp[cnt].real()* cos(cnt*pi/(dotCount*2)) + temp[cnt].imag()* sin(cnt*pi/(dotCount*2))) * d_coff;
	}

	delete[] temp;
}

template<typename T>
static void a_fast_cosine_trans_in_2d(complex<T> *cpx_in,complex<T> *cpx_out,int pow_w,int pow_h)
{
	int height = 1<<pow_h;
	int width  = 1<<pow_w;

	for(int cnt=0;cnt<height;cnt++)
	{
		a_fast_cosine_trans(&cpx_in[width * cnt],&cpx_out[width * cnt],pow_w);
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			cpx_in[i*height + j] = cpx_out[i + width*j];
		}
	}

	for(int cnt=0;cnt<width;cnt++)
	{
		a_fast_cosine_trans(&cpx_in[cnt * height],&cpx_out[cnt * height],pow_h);
	}
}

template<typename T>
void a_walshei_har_trans(T* tin,T* tout,int r)
{
	int cnt_dot = 1 << r;

	for(int k=0; k<r ;k++)
	{
		for(int j=0;j<(1<<k);j++)
		{
			int bf_size = 1 <<(r - k);
			for(int i=0; i < bf_size/2;i++)
			{
				int p = j*bf_size;
				tout[i + p] = tin[i + p] + tin[i + p + bf_size/2];
				tout[i + p + bf_size/2] = tin[i + p] - tin[i + p + bf_size / 2];
			}
		}

		T * ttmp = tin;
		tin = tout;
		tout = ttmp;
	}

	for(int j=0;j<cnt_dot;j++)
	{
		int p = 0;
		for(int i=0;i<r;i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r - i - 1);
			}
		}

		tout = tin[p] / cnt_dot;
	}
}

template<typename T>
void a_walshei_har_trans_in_2d(T* tin,T* tout,int pow_w,int pow_h)
{
	int height = 1<<pow_h;
	int width = 1<<pow_w;

	for(int cnt=0;cnt<height;cnt++)
	{
		walshei_har_trans(&tin[width*cnt],&tout[width*cnt],pow_w);
	}

	for(int i=0;i<width;i++)
	{
		for(int j=0;j<height;j++)
		{
			tin[i*height + j] = tout[i + width * j];
		}
	}

	for(int cnt=0;cnt<width;cnt++)
	{
		walshei_har_trans(&tin[cnt*height],&tout[cnt*height],pow_h);
	}
}

template<typename T>
void a_wavelet_trans(T* src,int level,bool forword_trans,int step,int support)
{
	const double hCoef[10][20] =
	{
		{ .707106781187,  .707106781187},

		{ .482962913145,  .836516303738,  .224143868042, -.129409522551 },

		{ .332670552950,  .806891509311,  .459877502118, -.135011020010, -.085441273882,  .035226291882 },

		{ .230377813309,  .714846570553,  .630880767930, -.027983769417,
		-.187034811719,  .030841381836,  .032883011667, -.010597401785 },

		{ .160102397974,  .603829269797,  .724308528438,  .138428145901, -.242294887066,
		-.032244869585,  .077571493840, -.006241490213, -.012580751999,  .003335725285 },

		{ .111540743350,  .494623890398,  .751133908021,  .315250351709, -.226264693965,
		-.129766867567,  .097501605587,  .027522865530, -.031582039318,  .000553842201,
		.004777257511, -.001077301085 },

		{ .077852054085,  .396539319482,  .729132090846,  .469782287405, -.143906003929,
		-.224036184994,  .071309219267,  .080612609151, -.038029936935, -.016574541631,
		.012550998556,  .000429577973, -.001801640704,  .000353713800 },

		{ .054415842243,  .312871590914,  .675630736297,  .585354683654, -.015829105256,
		-.284015542962,  .000472484574,  .128747426620, -.017369301002, -.044088253931,
		.013981027917,  .008746094047, -.004870352993, -.000391740373,  .000675449406,
		-.000117476784 },

		{ .038077947364,  .243834674613,  .604823123690,  .657288078051,  .133197385825,
		-.293273783279, -.096840783223,  .148540749338,  .030725681479, -.067632829061,
		.000250947115,  .022361662124, -.004723204758, -.004281503682,  .001847646883,
		.000230385764, -.000251963189,  .000039347320 },

		{ .026670057901,  .188176800078,  .527201188932,  .688459039454,  .281172343661,
		-.249846424327, -.195946274377,  .127369340336,  .093057364604, -.071394147166,
		-.029457536822,  .033212674059,  .003606553567, -.010733175483,  .001395351747,
		.001992405295, -.000685856695, -.000116466855,  .000093588670, -.000013264203 }
	};

	double s = sqrt((double)2);
	double* h = (double*)hCoef[support-1];

	int CurN = 1<<level;
	if (forword_trans) CurN <<= 1;

	if (support<1 || support>10 || CurN<2*support)
		return;

	double *ptemp = new double[CurN];
	if (!ptemp) return;

	double	s1, s2;
	int	Index1, Index2;

	if (forword_trans)
	{	// DWT
		Index1=0;
		Index2=2*support-1;


		for (int i=0; i<CurN/2; i++)
		{
			s1 = s2 = 0;
			double t = -1;
			for (int j=0; j<2*support; j++, t=-t)
			{
				s1 += h[j]*src[(Index1 & CurN-1) * step];
				s2 += t*h[j]*src[(Index2 & CurN-1) * step];

				Index1++;
				Index2--;
			}

			ptemp[i] = s1/s;
			ptemp[i+CurN/2] = s2/s;

			Index1 -= 2*support;
			Index2 += 2*support;
			Index1 += 2;
			Index2 += 2;
		}
	}
	else
	{
		Index1 = CurN/2;
		Index2 = CurN/2-support+1;

		for (int i=0; i<CurN/2; i++)
		{
			s1 = s2 = 0;
			int Index3 = 0;
			for (int j=0; j<support; j++)
			{
				s1 += h[Index3]*src[(Index1 & CurN/2-1) * step]
				+h[Index3+1]*src[((Index2 & CurN/2-1) + CurN/2) * step];
				s2 += h[Index3+1]*src[(Index1 & CurN/2-1) * step]
				-h[Index3]*src[((Index2 & CurN/2-1) + CurN/2) * step];

				Index3+=2;
				Index1--,		Index2++;
			}

			ptemp[2*i] = s1*s;
			ptemp[2*i+1] = s2*s;

			Index1 += support;
			Index2 -= support;
			Index1++;
			Index2++;
		}
	}

	for (int i=0; i<CurN; i++)
		src[i*step] = ptemp[i];

	delete[] ptemp;
}

template<typename T>
void a_wavelet_trans_in_2d(T* src,int level_w,int level_h,int max_w_level,int max_h_level,bool forword_trans,int step,int support)
{
	int W = 1<<max_w_level, H = 1<<max_h_level;
	int CurW = 1<<level_w, CurH = 1<<level_h;

	if (forword_trans)
	{
		int i = 0;
		for (i=0; i<CurH; i++)
			wavelet_trans(src+(int)i*W*step, level_w , forword_trans, step, support);
		for (i=0; i<CurW; i++)
			wavelet_trans(src+i*step, level_h, forword_trans, W*step, support);
	}
	else
	{
		CurW <<= 1;
		CurH <<= 1;

		int i = 0;
		for (i=0; i<CurW; i++)
			wavelet_trans(src+i*step, level_h, forword_trans, W*step, support);
		for (i=0; i<CurH; i++)
			wavelet_trans(src+(int)i*W*step, level_w , forword_trans, step, support);
	}

	return;
}
