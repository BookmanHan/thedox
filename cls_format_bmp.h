#include "lib_import.h"
#define WIDTHBYTES(bits) (((bits) + 31)/32*4)
#define BMP_STRUCT

typedef struct {
	int   bfSize;   // λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
	short bfReserved1;  // λͼ�ļ������֣�����Ϊ0
	short bfReserved2;  // λͼ�ļ������֣�����Ϊ0
	int   bfOffBits;  // λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} tag_bmp_file_header;

typedef struct {            // ����40�ֽ�
	int   biSize;   // ���ṹ��ռ���ֽ���������40�ֽ�
	int   biWidth;   // λͼ�Ŀ�ȣ�������Ϊ��λ
	int   biHeight;   // λͼ�ĸ߶ȣ�������Ϊ��λ
	short biPlanes;   // Ŀ���豸�ļ��𣬱���Ϊ1
	short biBitCount;  // ÿ�����������λ����������1(˫ɫ),4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	int   biCompression; // λͼѹ�����ͣ�������0(��ѹ��),1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	int   biSizeImage;  // λͼ�Ĵ�С�����ֽ�Ϊ��λ
	int   biXPelsPerMeter; // λͼˮƽ�ֱ��ʣ�ÿ��������
	int   biYPelsPerMeter; // λͼ��ֱ�ֱ��ʣ�ÿ��������
	int   biClrUsed;  // λͼʵ��ʹ�õ���ɫ���е���ɫ��
	int   biClrImportant; // λͼ��ʾ��������Ҫ����ɫ��
} tag_bmp_info_header;

inline
cls_image& a_load_bmp(string str_filename,cls_image& re)
{
	tag_bmp_file_header bfh;
	tag_bmp_info_header pbih;

	ifstream fin(str_filename.c_str(),ios::binary);

	short u;
	fin.read((char*)&u,2);
	fin.read((char*)&bfh,sizeof(tag_bmp_file_header));
	fin.read((char*)&pbih,sizeof(tag_bmp_info_header));

	if (pbih.biBitCount != 24)
	{
		throw cls_exception_not_support("thedox only support 24-bit bitmap.");
	}

	re.resize(pbih.biWidth,pbih.biHeight);
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

	return re;
}

inline
cls_image a_load_bmp(string str_filename)
{
	cls_image img;
	a_load_bmp(str_filename,img);

	return img;
}

inline
cls_image& a_save_bmp(string str_filename,cls_image& re)
{
	tag_bmp_file_header bfh;
	tag_bmp_info_header bih;

	ofstream fout(str_filename.c_str(),ios::binary);

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

	return re;
}