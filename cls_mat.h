#include "lib_import.h"

/*
	the cls_mat is used for basic matrix struct and
	the element of the matrix needs no computing
	function.
	if you want a math matrix,please use cls_mat-
	rix instead;
*/

template<typename T>
class cls_mat
	:cls_serialized_object
{
protected:

	//the width and height of the matrix.
	unsigned width;
	unsigned height;

	//check if the data is sharable;
	bool b_shared;

	//the data of matrix.
	T* data;

public:

	//the debug function of cls_mat;
#ifdef CLS_MAT_DISPLAY
	virtual void display()
	{
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				cout<<(*this)(i,j)<<"\t";
			}

			cout<<endl;
		}
	}
#endif

public:
	//the copy constructor;
	cls_mat( const cls_mat& src )
	{
		width = src.width;
		height = src.height;
		b_shared = false;

		data = new T[width * height];

		memcpy(data,src.data,sizeof(T)*width*height);
	}

	//the default constructor;
	cls_mat()
	{
		width = 0;
		height = 0;
		b_shared = 0;

		data = null;
	}

	//construct the matrix with a size of w*h;
	cls_mat(int w,int h)
	{
		width = w;
		height = h;

		data = new T[width * height];
	}

	//constructed from vector;
	cls_mat(const vector<T>& src)
	{
		cls_mat(1,src.size());

		for(int cnt=0;cnt<src.size();cnt++)
		{
			data[cnt] = src[cnt];
		}
	}

	//the deconstructor of the cls_mat;
	virtual ~cls_mat()
	{
		delete[] data;
		data = null;
	}

public:

	//get the copy of data for row-i.
	inline
	cls_mat<T> at(unsigned i)
	{
		cls_matrix<T> mat_re(1,get_height());
		for(int cnt=0;cnt<get_height();cnt++)
		{
			mat_re(0,cnt) = (*this)(i,cnt);
		}

		return mat_re;
	}

	//get the share position of data for row-i.
	inline 
	cls_mat<T> share_at(unsigned i)
	{
		cls_mat<T> mat_re;
		mat_re.width = 1;
		mat_re.height = height;
		mat_re.data = data + i*height;
		mat_re.b_shared = true;

		return mat_re;
	}

	//get the copy of data for row-i as the vector.
	inline 
	vector<T> operator[](int i) const
	{
		vector<T> re;
		re.resize(height);
		for(int cnt=0;cnt<get_height();cnt++)
		{
			re[cnt] = (*this)(i,cnt);
		}

		return re;
	}

	//get the share position of data for row-i as vector.
	inline
	cls_mat<T> operator()(int i) const
	{
		return share_at(i);
	}

	//resize all the mat without original memory.
	void resize(int w,int h)
	{
		if (data != null)
			delete [] data;

		data = new T[w*h];
		width = w;
		height = h;
		b_shared = false;
	}

	//resize all the mat with original memory.
	void resize_memory(int w,int h)
	{
		T* data_new = new T[w*h];
		for(int i=0;i<min(w,width);i++)
		{
			for(int j=0;j<min(h,height);j++)
			{
				data_new(i,j) = data(i,j);
			}
		}

		if (data != null)
			delete [] data;

		data = data_new;
		width = w;
		height = h;
	}

	//get the raw data of mat;
	T * const get_raw_data() const
	{
		return data;
	}

	//set the raw data of mat;
	void set_raw_data(const void* arg_data)
	{
		if ((void*)arg_data == (void*)data)
		{
			data = (T*)arg_data;
		}
		else
		{
			if (data != null)
				delete[] data;

			data = (T*)arg_data;
		}
	}

public:
	//get the width of the mat;
	inline int get_width() const
	{
		return width;
	}

	//get the height of the mat;
	inline int get_height() const
	{
		return height;
	}

public:

	//get the corresponding (x,y) of the data;
	inline
	T& operator()(int x,int y)
	{
		return data[x*height + y];
	}

	//get the corresponding (x,y) of the data;
	inline
	T& operator()(int x,int y) const
	{
		return data[x*height + y];
	}

public:

	//backup a copy for a matrix;
	void assign_copy(const cls_mat<T>& src)
	{
		width = src.width;
		height = src.height;
		b_shared = false;

		if (data != null)
			delete[] data;

		data = new T[width * height];

		memcpy(data,src.data,sizeof(T)*width*height);
	}

	void copy_assign(const cls_mat& src)
	{
		assign_copy(src);
	}

	//share a copy for a matrix;
	void share_copy(const cls_mat<T>& src)
	{
		width = src.width;
		height = src.height;
		b_shared = true;

		data = src.data;
	}

	void share_assign(const cls_mat& src)
	{
		share_copy(src);
	}

	//the operator= 
	cls_mat<T>& operator=( const cls_mat & src )
	{
		width = src.width;
		height = src.height;
		b_shared = false;

		data = new T[width * height];

		memcpy(data,src.data,sizeof(T)*width*height);

		return *this;
	}

	//fill the matrix of the value T;
	void fill(T value)
	{
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
			{
				(*this)(i,j) = value;
			}
		}
	}

	//swap the row src,des for the mat;
	void swap_row(unsigned src,unsigned des)
	{
		for(int cnt=0;cnt<height;cnt++)
		{
			T tmp = (*this)(src,cnt);
			(*this)(src,cnt) = (*this)(des,cnt);
			(*this)(des,cnt) = tmp;
		}
	}

public:
	//save the matrix for serialized_object;
	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_mat::save() - the file is bad for output.");

		fout.write((char*)&width,sizeof(width));
		fout.write((char*)&height,sizeof(height));
		fout.write((char*)data,sizeof(T) * width * height);
	}

	//load the matrix for serialized_object;
	virtual void load( ifstream& fin )
	{
		if (data != null)
			delete [] data;

		if (!fin)
			throw cls_exception_io("cls_mat::save() - the file is bad for output.");

		b_shared = false;
		fin.read((char*)&width,sizeof(width));
		fin.read((char*)&height,sizeof(height));

		data = new T[width*height];
		fin.read((char*)data,sizeof(T) * width * height);
	}

public:
	cls_mat<T>& transpose()
	{		
		cls_mat<T> res(height,width);

		for(int cnt_w=0;cnt_w<width;cnt_w++)
		{
			for(int cnt_h=0;cnt_h<height;cnt_h++)
			{
				res(cnt_h,cnt_w) = (*this)(cnt_w,cnt_h);
			}
		}

		this->assign_copy(res);

		return *this;
	}

	cls_mat<T> get_transpose()
	{
		cls_mat<T> re = (*this);
		
		return re.transpose();
	}
};