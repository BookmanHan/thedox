#include "lib_import.h"

template <typename T>
class cls_encorder_hamming
{
private:
	vector<int> v_n_d2;
	cls_matrix<int> v_n_d1;

public:
	cls_encorder_hamming(int max_hamming)
		:v_n_d1(max_hamming,max_hamming),
		v_n_d2(max_hamming)
	{
		int max_coder = a_a_get_pow_num(max_hamming);
		for(int i=0;i<max_coder;i++)
		{
			v_n_d1(i,0) = (int)pow(2,i);
			for(int j=1;j<max_hamming;j++)
			{
				for(int k=0;k<((int)pow(2,i)-1);k++)
				{
					v_n_d1(i,j) = v_n_d1(i,j-1) + 1;
					j++;
				}
			}

			v_n_d1[i][j] = v_n_d1[i][j-1] + ((int)pow(2,i)+1);
		}
	}
};
