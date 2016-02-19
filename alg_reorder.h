#include "lib_import.h"

template <typename T>
void a_reorder_z_order(cls_matrix<T>& src,vector<T>& des)
{
	des.resize(src.get_width() * src.get_height());

	for(int i=1;i<=src.get_width();i++)
	{
		for(int j=1;j<=src.get_height();j++)
		{
			int sum_order = i + j;
			int der_order = sum_order - 1;

			int sum_pre = 0;
			
			if (der_order <= min(src.get_width(),src.get_height()))
			{
				sum_pre = (1 + der_order-1)*(der_order-1)/2;

				if (der_order % 2 == 0)
				{
					int der_norm = der_order - i;
					des[sum_pre + der_norm] = src(i-1,j-1);
				}
				else
				{
					int der_norm = der_order - j;
					des[sum_pre + der_norm] = src(i-1,j-1);
				}
			}
			else
			{
				int i_der_order = src.get_height() + src.get_width() - der_order;
				sum_pre = src.get_height() * src.get_width() - (1+i_der_order)*i_der_order/2;

				if (der_order % 2 == 0)
				{
					int der_n = src.get_width() + src.get_height() - der_order;
					int der_norm = der_n - (src.get_width() - i);
					des[sum_pre + der_norm - 1] = src(i-1,j-1);
				}
				else
				{
					int der_n = src.get_width() + src.get_height() - der_order;
					int der_norm = der_n - (src.get_height() - j);
					des[sum_pre + der_norm - 1] = src(i-1,j-1);
				}
			}
		}
	}
}