#include "lib_import.h"

inline
unsigned int a_make_word(char src,char des)
{
	unsigned int re = 0;
	re = (des<<8) + src;

	return re;
}