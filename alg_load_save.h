#include "lib_import.h"
#define WIDTHBYTES(bits) (((bits) + 31)/32*4)

inline
string a_get_file_extension(string str_filename)
{
	int pos = str_filename.find_last_of('.');

	if (pos != -1)
		return str_filename.substr(pos+1);
	else
		return "";
}

inline
wstring a_get_file_extension(wstring str_filename)
{
	int pos = str_filename.find_last_of('.');

	if (pos != -1)
		return str_filename.substr(pos+1);
	else
		return L"";
}

template<typename T>
bool a_load(cls_image_ot<T> &re,string str_filename)
{
	ifstream fin(str_filename.c_str(),ios::binary);
	if (!fin)
		return false;

	bool reb = a_load(re,a_string_to_lower(a_get_file_extension(str_filename)),fin);
	fin.close();

	return reb;
}

template<typename T>
bool a_save(cls_image_ot<T> &re,string str_filename)
{
	ofstream fout(str_filename.c_str(),ios::binary);
	if(!fout)
		return false;

	bool reb = a_save(re,a_string_to_lower(a_get_file_extension(str_filename)),fout);
	fout.close();

	return reb;
}

template<typename T>
bool a_load(cls_image_ot<T>& re,string str_ext,istream & fin)
{
	if (str_ext == "bmp")
	{
		tag_bmp_file_header bfh;
		tag_bmp_info_header pbih;

		short u;
		fin.read((char*)&u,2);
		fin.read((char*)&bfh,sizeof(tag_bmp_file_header));

		//if (bfh.bfType != a_make_word('B','M'))
		//	return false;

		fin.read((char*)&pbih,sizeof(tag_bmp_info_header));

		if (pbih.biBitCount != 24)
			return false;

		re = cls_image_ot<T>(pbih.biWidth,pbih.biHeight);
		for(int i=re.get_height()-1;i>=0;i--)
		{
			unsigned char * p_uc_read = new unsigned char[WIDTHBYTES(24 * pbih.biWidth)];
			fin.read((char*)p_uc_read,WIDTHBYTES(24 * pbih.biWidth));

			for(int j=0;j<re.get_width();j++)
			{
				re.set_channel_value_0(j,i,0);
				re.set_channel_value_1(j,i,p_uc_read[j*3 + 2]);
				re.set_channel_value_2(j,i,p_uc_read[j*3 + 1]);
				re.set_channel_value_3(j,i,p_uc_read[j*3 + 0]);
			}

			delete [] p_uc_read;
		}
	}

	return true;
}

template<typename T>
bool a_save(cls_image_ot<T>& re,string str_ext,ostream & fout)
{
	if (str_ext == "bmp")
	{
		tag_bmp_file_header bfh;
		tag_bmp_info_header bih;

		short u = a_make_word('B','M');
		fout.write((char*)&u,2);

		long size_image = WIDTHBYTES(24 * re.get_width()) * re.get_height();

		bfh.bfOffBits = 54;
		bfh.bfReserved1 = 0;
		bfh.bfReserved2 = 0;
		bfh.bfSize = size_image + 54;
		fout.write((char*)&bfh,sizeof(tag_bmp_file_header));

		bih.biBitCount = 24;
		bih.biClrImportant = 0;
		bih.biClrUsed = 0;
		bih.biCompression = 0;
		bih.biWidth = re.get_width();
		bih.biHeight = re.get_height();
		bih.biXPelsPerMeter = 9449;
		bih.biYPelsPerMeter = 9449;
		bih.biPlanes = 1;
		bih.biSize = 40;
		bih.biSizeImage = size_image;
		fout.write((char*)&bih,sizeof(tag_bmp_info_header));

		for(int i=re.get_height()-1;i>=0;i--)
		{
			unsigned char* ar_write = new unsigned char[WIDTHBYTES(re.get_width()*24)];
			memset(ar_write,0,WIDTHBYTES(re.get_width()*24));

			for(int j=0;j<re.get_width();j++)
			{
				ar_write[3*j + 0] = re.get_channel_value_3(j,i);
				ar_write[3*j + 1] = re.get_channel_value_2(j,i);
				ar_write[3*j + 2] = re.get_channel_value_1(j,i);
			}

			fout.write((char*)ar_write,WIDTHBYTES(re.get_width()*24));
			delete [] ar_write;
		}
	}

	return true;
}

class b_ofstream : public ofstream
{
public:
	b_ofstream(string str_filename):ofstream(str_filename.c_str(),ios::binary)
	{

	}

	template<typename T>
	b_ofstream& operator << (T src)
	{
		this->write((char*)&src,sizeof(T));
		return *this;
	}
};

class b_ifstream : public ifstream
{
public:
	b_ifstream(string str_filename):ifstream(str_filename.c_str(),ios::binary)
	{

	}

	template<typename T>
	b_ifstream & operator >> (T src)
	{
		this->read((char*)&src,sizeof(T));
		return *this;
	}
};

#ifndef __USING_GCC__
template<typename T>
bool a_load(cls_image_ot<T> &re,wstring str_filename)
{
	ifstream fin(str_filename.c_str(),ios::binary);
	if (!fin)
		return false;

	bool reb = a_load(re,a_get_file_extension(str_filename),fin);
	fin.close();

	return reb;
}

template<typename T>
bool a_save(cls_image_ot<T> &re,wstring str_filename)
{
	ofstream fout(str_filename.c_str(),ios::binary);
	if(!fout)
		return false;

	bool reb = a_save(re,a_get_file_extension(str_filename),fout);
	fout.close();

	return reb;
}



template<typename T>
bool a_load(cls_image_ot<T>& re,wstring str_ext,istream & fin)
{
	if (str_ext == L"bmp")
	{
		tag_bmp_file_header bfh;
		tag_bmp_info_header pbih;

		short u;
		fin.read((char*)&u,2);
		fin.read((char*)&bfh,sizeof(tag_bmp_file_header));

		//if (bfh.bfType != a_make_word('B','M'))
		//	return false;

		fin.read((char*)&pbih,sizeof(tag_bmp_info_header));

		if (pbih.biBitCount != 24)
			return false;

		re = cls_image_ot<T>(pbih.biWidth,pbih.biHeight);
		for(int i=re.get_height()-1;i>=0;i--)
		{
			unsigned char * p_uc_read = new unsigned char[WIDTHBYTES(24 * pbih.biWidth)];
			fin.read((char*)p_uc_read,WIDTHBYTES(24 * pbih.biWidth));

			for(int j=0;j<re.get_width();j++)
			{
				re.set_channel_value_0(j,i,0);
				re.set_channel_value_1(j,i,p_uc_read[j*3 + 2]);
				re.set_channel_value_2(j,i,p_uc_read[j*3 + 1]);
				re.set_channel_value_3(j,i,p_uc_read[j*3 + 0]);
			}

			delete [] p_uc_read;
		}
	}

	return true;
}

template<typename T>
bool a_save(cls_image_ot<T>& re,wstring str_ext,ostream & fout)
{
	if (str_ext == L"bmp")
	{
		tag_bmp_file_header bfh;
		tag_bmp_info_header bih;

		short u = a_make_word('B','M');
		fout.write((char*)&u,2);

		long size_image = WIDTHBYTES(24 * re.get_width()) * re.get_height();

		bfh.bfOffBits = 54;
		bfh.bfReserved1 = 0;
		bfh.bfReserved2 = 0;
		bfh.bfSize = size_image + 54;
		fout.write((char*)&bfh,sizeof(tag_bmp_file_header));

		bih.biBitCount = 24;
		bih.biClrImportant = 0;
		bih.biClrUsed = 0;
		bih.biCompression = 0;
		bih.biWidth = re.get_width();
		bih.biHeight = re.get_height();
		bih.biXPelsPerMeter = 9449;
		bih.biYPelsPerMeter = 9449;
		bih.biPlanes = 1;
		bih.biSize = 40;
		bih.biSizeImage = size_image;
		fout.write((char*)&bih,sizeof(tag_bmp_info_header));

		for(int i=re.get_height()-1;i>=0;i--)
		{
			unsigned char* ar_write = new unsigned char[WIDTHBYTES(re.get_width()*24)];
			memset(ar_write,0,WIDTHBYTES(re.get_width()*24));

			for(int j=0;j<re.get_width();j++)
			{
				ar_write[3*j + 0] = re.get_channel_value_3(j,i);
				ar_write[3*j + 1] = re.get_channel_value_2(j,i);
				ar_write[3*j + 2] = re.get_channel_value_1(j,i);
			}

			fout.write((char*)ar_write,WIDTHBYTES(re.get_width()*24));
			delete [] ar_write;
		}
	}

	return true;
}

#endif

inline
cls_image a_load_image(string str_filename)
{
	string str_ext = a_get_file_extension(str_filename);
	if (str_ext == "bmp")
	{
		return a_load_bmp(str_filename);
	}
	else if (str_ext == "jpg" || str_ext == "jpeg")
	{
		return a_load_jpg(str_filename);
	}
	else
	{
		throw cls_exception_not_support("thedox don't support your file format, by now.");
	}
}

inline
cls_image& a_load_image(string str_filename,cls_image& img)
{
	string str_ext = a_get_file_extension(str_filename);
	if (str_ext == "bmp")
	{
		return a_load_bmp(str_filename,img);
	}
	else if (str_ext == "jpg" || str_ext == "jpeg")
	{
		return a_load_jpg(str_filename,img);
	}
	else
	{
		throw cls_exception_not_support("thedox don't support your file format, by now.");
	}
}

inline
void a_save_image(string str_filename,cls_image& img)
{
	string str_ext = a_get_file_extension(str_filename);
	if (str_ext == "bmp")
	{
		a_save_bmp(str_filename,img);
	}
	else
	{
		throw cls_exception_not_support("thedox don't support your file format, by now.");
	}
}
