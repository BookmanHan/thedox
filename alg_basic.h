#include "lib_import.h"

template<typename S,typename T>
void clone(T* & des,const S* src,int n)
{
	des = new T[n];
	memcpy((void*)des,(void*)src,sizeof(T)*n);
}