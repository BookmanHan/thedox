#include "lib_import.h"

template <typename T>
class cls_multi_array
	:cls_serialized_object
{
private:
	T * data;
	vector<int> v_dim;

public:
	virtual void fill(T val)
	{
		int size = get_size();

		for(int cnt=0;cnt<size;cnt++)
		{
			data[cnt] = val;
		}
	}

	virtual int get_size()
	{
		int re = 1;
		for(int cnt=0;cnt<v_dim.size();cnt++)
		{
			re *= v_dim[cnt];
		}

		return re;
	}

	virtual void save(ofstream& fout) const
	{
		if (!fout)
			throw cls_exception_io("cls_multi_array::save() - the file is bad for output.");

		int num = v_dim.size();
		fout.write((char*)&num,sizeof(num));

		num = 1;
		for(int cnt=0;cnt<v_dim.size();cnt++)
		{
			num *= v_dim[cnt];
			fout.write((char*)&v_dim[cnt],sizeof(v_dim[cnt]));
		}

		fout.write((char*)data,sizeof(T)*num);
	}

	virtual void load(ifstream& fin)
	{
		if (!fin)
			throw cls_exception_io("cls_multi_array::load() - the file is bad for input.");

		int num = 0;
		fin.read((char*)&num,sizeof(num));

		v_dim.resize(num);
		int rnum = 1;
		for(int cnt=0;cnt<num;cnt++)
		{
			int a;
			fin.read((char*)&a,sizeof(int));
			v_dim[cnt] = a;
			
			rnum *= a;
		}

		delete[] data;
		data = new T[rnum];

		fin.read((char*)data,sizeof(T)*rnum);
	}

	~cls_multi_array()
	{
		delete[] data;
	}

	T& operator()(vector<int> & adim)
	{
		if (v_dim.size() != adim.size())
			return data[0];

		int pos = 0;
		for(int cnt=0;cnt<v_dim.size();cnt++)
		{
			pos = pos*v_dim[cnt] + adim[cnt];
		}

		return data[pos];
	}

	T& operator()(int a1,int a2,int a3)
	{
		return data[(a1*v_dim[1] + a2)*v_dim[2] + a3];
	}

	T& operator()(int a1,int a2,int a3,int a4)
	{
		return data[((a1*v_dim[1] + a2)*v_dim[2] + a3)*v_dim[3]+a4];
	}

	T& operator()(int a1,int a2,int a3,int a4,int a5)
	{
		return data[(((a1*v_dim[1] + a2)*v_dim[2] + a3)*v_dim[3]+a4)*v_dim[4] + a5];
	}

	T& operator()(int a1,int a2,int a3,int a4,int a5,int a6)
	{
		return data[((((a1*v_dim[1] + a2)*v_dim[2] + a3)*v_dim[3]+a4)*v_dim[4] + a5)*v_dim[5] + a6];
	}

	T& operator()(int a1,int a2,int a3,int a4,int a5,int a6,int a7)
	{
		return data[(((((a1*v_dim[1] + a2)*v_dim[2] + a3)*v_dim[3]+a4)*v_dim[4] + a5)*v_dim[5] + a6)*v_dim[6] + a7];
	}

	cls_multi_array(vector<int> &adim):v_dim(adim)
	{
		int size = 1;
		for(int cnt=0;cnt<adim.size();cnt++)
		{
			size *= adim[cnt];
		}

		data = new T[size];
		memset(data,0,sizeof(T)*size);
	}

	cls_multi_array(int a1,int a2,int a3)
	{
		v_dim.push_back(a1);
		v_dim.push_back(a2);
		v_dim.push_back(a3);

		data = new T[a1*a2*a3];
		memset(data,0,sizeof(T)*a1*a2*a3);
	}

	cls_multi_array(int a1,int a2,int a3,int a4)
	{
		v_dim.push_back(a1);
		v_dim.push_back(a2);
		v_dim.push_back(a3);
		v_dim.push_back(a4);

		data = new T[a1*a2*a3*a4];
		memset(data,0,sizeof(T)*a1*a2*a3*a4);
	}

	cls_multi_array(int a1,int a2,int a3,int a4,int a5)
	{
		v_dim.push_back(a1);
		v_dim.push_back(a2);
		v_dim.push_back(a3);
		v_dim.push_back(a4);
		v_dim.push_back(a5);

		data = new T[a1*a2*a3*a4*a5];
		memset(data,0,sizeof(T)*a1*a2*a3*a4*a5);
	}

	cls_multi_array(int a1,int a2,int a3,int a4,int a5,int a6)
	{
		v_dim.push_back(a1);
		v_dim.push_back(a2);
		v_dim.push_back(a3);
		v_dim.push_back(a4);
		v_dim.push_back(a5);
		v_dim.push_back(a6);

		data = new T[a1*a2*a3*a4*a5*a6];
		memset(data,0,sizeof(T)*a1*a2*a3*a4*a5*a6);
	}

	cls_multi_array(int a1,int a2,int a3,int a4,int a5,int a6,int a7)
	{
		v_dim.push_back(a1);
		v_dim.push_back(a2);
		v_dim.push_back(a3);
		v_dim.push_back(a4);
		v_dim.push_back(a5);
		v_dim.push_back(a6);
		v_dim.push_back(a7);

		data = new T[a1*a2*a3*a4*a5*a6*a7];
		memset(data,0,sizeof(T)*a1*a2*a3*a4*a5*a6*a7);
	}
};