#include "lib_import.h"

class cls_format_psd_header_info
{
public:
	short n_channels;
	int n_height;
	int n_width;
	short n_bits_per_pixel;

	//0:bitmap,1:gray_scale,2:indexed,3:RGB,
	//4:CMYK,7:multichannel,8:Duotone;
	//9:Lab;
	short n_color_mode;

	cls_format_psd_header_info()
	{
		n_channels = -1;
		n_height = -1;
		n_width = -1;
		n_bits_per_pixel = -1;
		n_color_mode = -1;
	}
};

class cls_format_psd_display_info
{
public:
	//0:RGB,1:HSB,2:CMYK;
	//7:LAb
	//8:gray_scale;
	short color_space;
	short color[4];
	short opacity;

	//selected = false; protected = true;
	bool kind;

	//should be zero;
	byte padding;

	cls_format_psd_display_info()
	{
		color_space = -1;
		for(int cnt=0;cnt<4;cnt++)
		{
			color[cnt] = 0;
		}

		opacity = -1;
		kind = false;
		padding = 0x00;
	}
};

class cls_format_psd_image_resource
{
public:
	int n_length;
	byte OSType[4];
	short n_id;
	byte * name;
	int n_size;

	void reset()
	{
		n_length = -1;
		for(int cnt=0;cnt<4;cnt++)
		{
			OSType[cnt] = 0x00;
		}

		n_id = -1;
		n_size = -1;
	}

	cls_format_psd_image_resource()
	{
		reset();
	}
};

class cls_format_psd_resolution_info
{
public:
		short h_res;
		int h_res_unit;
		short width_unit;
		
		short v_res;
		int v_res_unit;
		short height_unit;

		cls_format_psd_resolution_info()
		{
			h_res = -1;
			h_res_unit  = -1;
			width_unit = -1;

			v_res = -1;
			v_res_unit = -1;
			height_unit = -1;
		}
};

class cls_format_psd_thumb_nail
{
public:
		int n_format;
		int n_width;
		int n_height;
		int n_width_bytes;
		int n_size;
		int n_compresswd_size;
		short n_bit_per_pixel;
		short n_planes;
		byte* data;

public:
	cls_format_psd_thumb_nail()
	{
		n_format = -1;
		n_width = -1;
		n_height = -1;
		n_width_bytes = -1;
		n_size = -1;
		n_compresswd_size = -1;
		n_bit_per_pixel = -1;
		n_planes = -1;
	}
};

class cls_format_psd_color_mode_data
{
public:
	int n_length;
	byte* data;

public:
	cls_format_psd_color_mode_data()
	{
		n_length = -1;
		data = null;
	}
};

class cls_format_psd
{
private:
		cls_format_psd_header_info header_info;
		cls_format_psd_color_mode_data color_mode_data;
		cls_format_psd_image_resource image_resource;
		cls_format_psd_resolution_info resolution_info;
		cls_format_psd_display_info display_info;
		cls_format_psd_thumb_nail thumb_nail;

		bool b_resolution_info_filled;
		bool b_thumb_nail_filled;
		bool b_copy_right;

		short n_color_count;
		short n_transparent_index;
		int n_global_angel;
		int n_compression;

		int* bitmap;

		cls_format_psd()
		{
			b_resolution_info_filled = false;
			b_thumb_nail_filled  =  false;
			b_copy_right = false;

			n_color_count = -1;
			n_transparent_index = -1;
			n_global_angel = 30;
			n_compression = -1;

			bitmap = null;
		}

		int load(string str_path_name)
		{
			int error_code = 0;
			ifstream fin(str_path_name.c_str(),ios::binary);

			if (fin.bad())
				throw cls_exception_not_found("The file is not found!");
		}
};