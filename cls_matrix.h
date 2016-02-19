#include "lib_import.h"
#pragma once

/*
	this class is used for math matrix!
*/

template <typename T>
class cls_matrix
	:public cls_mat<T>
{
public:
	//the constructor of the matrix;
	cls_matrix():cls_mat<T>()
	{
		;
	}

	//the constructor of the matrix;
	cls_matrix(int w,int h):cls_mat<T>(w,h)
	{
		;
	}

	//the constructor of the matrix;
	cls_matrix(const cls_matrix<T>& src):cls_mat<T>(src)
	{
		;
	}

	cls_matrix(const vector<T>& src):cls_mat(src)
	{
		;
	}

	cls_matrix& operator = (const cls_matrix<T>& src)
	{
		cls_mat::operator =(src);
		return *this;
	}

public:
	//the plus operation of the two matrix!
	cls_matrix<T> operator+(const cls_matrix<T> & des) const
	{
		if (get_width() != des.get_width() || get_height() != des.get_height())
		{
			throw cls_exception_execution("The step of matrix is different!");
		}

		cls_matrix<T> re(get_width(),get_height());
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				re(i,j) = (*this)(i,j) + des(i,j);
			}
		}

		return re;
	}

	//the minus operation of the two matrix!
	cls_matrix<T> operator-(const cls_matrix<T> & des) const
	{
		if (get_width() != des.get_width() || get_height() != des.get_height())
		{
			throw cls_exception_execution("The step of matrix is different!");
		}

		cls_matrix<T> re(get_width(),get_height());
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				re(i,j) = (*this)(i,j) - des(i,j);
			}
		}

		return re;
	}

	//the multiply operation of the two matrix!
	cls_matrix<T> operator*(const cls_matrix & des) const
	{
		if (get_height() != des.get_width())
		{
			throw cls_exception_execution("The two matrix can't be multiplied!");
		}

		cls_matrix<T> re(get_width(),des.get_height());
		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<des.get_height();j++)
			{
				re(i,j) = 0;
				for(int k=0;k<get_height();k++)
				{
					re(i,j) += (*this)(i,k) * des(k,j);
				}
			}
		}

		return re;
	}

public:

	//get the inverse of the matrix;
	cls_matrix<T> inverse()
	{
		if (width != height)
		{
			throw cls_exception_io("not a square matrix");
		}

		int i,j,k;

		cls_matrix diag(width,height);
		cls_matrix m = *this;
		diag.make_identity();
		double anchor = 0.0;
		double factor = 0.0;

		for(i=0;i<width-1;i++)
		{
			for(k=i ; k<width && m(k,i) == 0.0;k++)
				;

			if (k == width)
			{
				throw cls_exception_io("cls_mat::get_inverse() - Inverse dosn't exists.");
			}
			else if (k != i)
			{
				m.swap_row(k,i);
				diag.swap_row(k,i);
			}

			anchor = m(i,i);
			if (anchor == 0)
				throw cls_exception_io("cls_mat::get_inverse() - Inverse dosn't exists.");

			for(j=i+1;j<width;j++)
			{
				factor = m(j,i) / anchor;
				for(k=0;k<width;k++)
				{
					m(j,k) -= m(i,k) * factor;
					diag(j,k) -= diag(i,k) * factor;
				}
			}
		}

		for(i = width-1;i>=0;i--)
		{
			if (m(i,i) != 1.0)
			{
				factor = m(i,i);

				if (factor == 0)
					throw cls_exception_io("cls_mat::get_inverse() - Inverse dosn't exists.");

				for(k=0;k<width;k++)
				{
					m(i,k) /= factor;
					diag(i,k) /= factor;
				}
			}

			for(j=width-1;j>i;j--)
			{
				if (m(i,j) == 0)
					continue;

				if(m(j,j) == 0)
					throw cls_exception_io("cls_mat::get_inverse() - Inverse dosn't exists.");

				factor = m(i,j) / m(j,j);
				m(i,j) = 0;
				for(k=0;k<width;k++)
				{
					diag(i,k) -= diag(j,k) * factor;
				}
			}
		}

		return diag;
	}

	//get the inverse of the matrix.
	/*
		this is an old version!
	*/
	cls_matrix<T> get_inverse()
	{
		return inverse();
	}

	//trans the matrix to a general_reverse;
	cls_matrix<T>& general_reverse()
	{
		cls_mat<T> mat = get_transpose();
		*this = mat * (*this);

		this->inverse();

		return *this;
	}

	//get the matrix's a general_reverse;
	cls_matrix<T> get_general_reverse()
	{
		cls_mat<T> mat = get_transpose();
		cls_mat<T> mat_g = mat * (*this);

		mat_g.inverse();

		return mat_g;
	}

	//get the det of the matrix;
	T get_det()
	{
		if (width != height)
			return 0;

		int n = width;

		cls_mat<T> re = *this;

		T det = 1.0;
		int n_is,n_js;

		T q = 0.0;
		T d = 0.0;
		T f = 0.0;

		for(int k=0;k<n-1;k++)
		{

			for(int i=k;i<n;i++)
			{
				for(int j=k;j<k;j++)
				{
					d = fabs(re(i,j));
					if (d > q)
					{
						q = d;
						n_is = i;
						n_js = j;
					}
				}
			}

			if (q + 1.0 == 1.0)
			{
				det = 0.0;
			}
			else
			{
				if (n_is != k)
				{
					f = -f;
					for(int j=k;j<n;j++)
					{
						d = re(k,j);
						re(k,j) = re(n_is,j);
						re(n_is,j) = d;
					}
				}

				if (n_js != k)
				{
					f = -f;
					for(int i=k;i<n;i++)
					{
						d = re(i,n_js);
						re(i,n_js) = re(i,k);
						re(i,k) = d;
					}
				}

				det = det * re(k,k);
				for(int i=k+1;i<n;i++)
				{
					d = re(i,k) / re(k,k);
					for(int j=k+1;j<n;j++)
					{
						re(i,j) = re(i,j) - d*re(k,j);
					}
				}
			}
		}

		det = f*det*re(n-1,n-1);

		return det;
	}

	//get the norm of the matrix with step 'step';
	T cls_matrix<T>::get_norm(int n_step = 2)
	{
		T res = 0;
		for(int cnt_w=0;cnt_w<width;cnt_w++)
		{
			for(int cnt_h=0;cnt_h<height;cnt_h++)
			{
				res += pow((*this)(cnt_w,cnt_w),n_step);
			}
		}

		return pow(res,1.0/n_step);
	}

public:
	//get the LR depose of the matrix;
	bool get_depose_lr(cls_matrix<T>& l,cls_matrix<T>& r)
	{
		cls_matrix<T> re = (*this);

		if (get_width() != get_height())
		{
			throw cls_exception_execution("only the identity matrix can be deposed.");
		}

		int n = get_width();
		
		l.resize(n,n);
		r.resize(n,n);
		
		l.fill(0);
		r.fill(0);

		for(int cnt=0;cnt<n;cnt++)
		{
			if (fabs(re(cnt,cnt)) < e)
			{
				return false;
			}

			for(int i=cnt+1;i<n;i++)
			{
				re(i,cnt) /=  re(cnt,cnt);
			}

			for(int i=cnt+1;i<n;i++)
			{
				for(int j=cnt+1;j<n;j++)
				{
					re(i,j) -= re(i,cnt) * re(cnt,j);
				}
			}
		}

		for(int i=0;i<n;i++)
		{
			for(int j=0;j<i;j++)
			{
				l(i,j) = re(i,j);
			}

			l(i,i) = 1.0;
			r(i,i) = re(i,i);

			for(int j=i+1;j<n;j++)
			{
				r(i,j) = re(i,j);
			}
		}

		return true;
	}
	
	//get the CHOLESKY despose of the matrix;
	bool get_depose_cholesky(cls_matrix& cur)
	{
		cur = (*this);

		if (get_width() != get_height())
		{
			throw cls_exception_execution("only the identity matrix can be deposed.");
		}

		int n = get_width();

		if (fabs(cur(0,0)) < e || cur(0,0) < 0.0)
		{
			return false;
		}

		cur(0,0) = sqrt(cur(0,0));

		double d = cur(0,0);

		for(int i=0;i<n;i++)
		{
			cur[i][0] = cur[i][0] / cur[0][0];
		}

		for(int j=0;j<n;j++)
		{
			for(int k=0;k<j;k++)
			{
				cur(j,j) -= cur(j,k)*cur(j,k);
			}

			if (fabs(cur(j)(j)) < e || cur(j,j) <0.0)
			{
				return false;
			}

			cur(j,j) = sqrt(cur(j,j));
			d *= cur(j,j);

			for(int i=j+1;i<n;i++)
			{
				for(int k=0;k<j;k++)
					cur(i,j) -= cur(i,k) * cur(j,k);

				cur(i,j) /= cur(j,j);
			}
		}

		for(int i=0;i<n-1;i++)
		{
			for(int j=i+1;j<n;j++)
				cur(i,j) = 0.0;
		}
	}

	//get the QR depose of the matrix;
	pair<cls_matrix,cls_matrix> get_depose_qr()
	{

	}

	//get the full-rank depose of the matrix;
	pair<cls_matrix,cls_matrix> get_depose_rank()
	{

	}

	//get the SVR depose of the matrix;
	pair<pair<cls_matrix,cls_matrix>,cls_matrix> get_depose_svd()
	{

	}

public:
	//get the rank of the matrix;
	int get_rank()
	{
		int max_r = min(get_width(),get_height());

		int is = 0;
		int js = 0;
		int k = 0;
		for(int cnt=0;cnt<max_r-1;cnt++)
		{
			double q = 0.0;
			for(int i=1;i<get_width();i++)
			{
				for(int j=1;j<get_height();j++)
				{
					double d = fabs((*this)(i,j));
					if (d > q)
					{
						q = d;
						is = i;
						js = j;
					}
				}
			}

			if (q < e)
			{
				break;
			}

			++k;

			if (is != cnt)
			{
				swap_row(cnt,is);
			}

			if (js != cnt)
			{
				swap_row(cnt,js);
			}

			for(int i=cnt+1;i<get_height();i++)
			{
				double d= (*this)(i,cnt) / (*this)(cnt,cnt);
				for(int j=cnt+1;j<get_height();j++)
				{
					(*this)(i,j) -= d * (*this)(cnt,j);
				}
			}
		}

		return k;
	}

public:
	//get the max element in the matrix;
	T get_max()
	{
		T val = (*this)(0,0);

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				val = max(val,(*this)(i,j));
			}
		}

		return val;
	}

	//get the min element in the matrix;
	T get_min()
	{
		T val = (*this)(0,0);

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				val = min(val,(*this)(i,j));
			}
		}

		return val;
	}

	//get the absolute max element in the matrix;
	T 	get_abs_max()
	{
		T val = fabs((*this)(0,0));

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				val = max(val,fabs((*this)(i,j)));
			}
		}

		return val;
	}

	//get the absolute min element in the matrix;
	T 	get_abs_min()
	{
		T val = fabs((*this)(0,0));

		for(int i=0;i<get_width();i++)
		{
			for(int j=0;j<get_height();j++)
			{
				val = min(val,fabs((*this)(i,j)));
			}
		}

		return val;
	}

public:
	//make the matrix an identity one;
	void make_identity()
	{
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				(*this)(i,j) = (i == j)?1:0;
			}
		}
	}

	//make the matrix transpose;
	cls_matrix<T>& transpose()
	{
		return (cls_matrix<T>&)cls_mat<T>::transpose();
	}

	//get the matrix's transpose;
	cls_matrix<T> get_transpose()
	{
		cls_matrix<T> mat = *this;
		mat.transpose();

		return mat;
	}
};
