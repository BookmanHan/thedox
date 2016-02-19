#include "lib_import.h"

class appc_image_clearity_dct_enhance
{
protected:
	cls_vision vision;

public:
	appc_image_clearity_dct_enhance(const cls_vision& that)
	{
		vision = that;
	}

	appc_image_clearity_dct_enhance(wstring wstr)
	{
		a_load_image(wstring_to_string(wstr).c_str(),vision);
	}

	void method_scale(double d_factor_1 = 4,double d_factor_2 = 0.6)
	{
		cls_matrix<double> mat;
		vision.get_identical_frequency_matrix(mat);

		double d_max_r = sqrt(mat.get_width()*mat.get_width() 
			+ mat.get_height()*mat.get_height());

		for(int i=0;i<mat.get_width();i++)
		{
			for(int j=0;j<mat.get_height();j++)
			{
				double d_r = sqrt(i*i + j*j);
				mat(i,j) = 1.0 + d_factor_1 * d_r/(d_max_r*d_factor_2 +d_r);
			}
		}

		vision.enhance_cosine_map(mat,-2);
	}

	void method_guassian(double d_factor_1 = 1.2)
	{
		cls_matrix<double> mat;
		vision.get_identical_frequency_matrix(mat);

		double d_max_r = sqrt(mat.get_width()*mat.get_width() 
			+ mat.get_height()*mat.get_height());

		for(int i=0;i<mat.get_width();i++)
		{
			for(int j=0;j<mat.get_height();j++)
			{
				double d_r = sqrt(i*i + j*j);
				mat(i,j) = pow(math_e,d_factor_1 * d_r/d_max_r);
			}
		}

		vision.enhance_cosine_map(mat,-2);
	}

	cls_vision& result()
	{
		return vision;
	}
};