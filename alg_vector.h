#include "lib_import.h"

template<typename T>
void a_vector_output(ostream& fout,vector<T>& re,string str_inter=" ")
{
	for(int cnt=0;cnt<re.size();cnt++)
	{
		fout<<re[cnt]<<str_inter;
	}
}

template<typename T>
void a_vector_resize(vector<vector<T> > & re,int x,int y)
{
	re.resize(x);

	for(int cnt=0;cnt<x;cnt++)
	{
		re[cnt].resize(y);
	}
}
