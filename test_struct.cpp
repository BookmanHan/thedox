#include "lib_import.h"

void test_union_set()
{
	cls_union_set st(1000);
	st.do_union(1,2);
	st.do_union(3,4);
	st.do_union(1,3);

	cout<<st.check_equal(1,4)<<endl;
}