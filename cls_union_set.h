#include "lib_import.h"

class cls_union_set
{
private:
	vector<int> main;

protected:
	inline
	int union_run(int i)
	{
		if (i != main[i])
			main[i] = union_run(main[i]);

		return main[i];
	}

public:
	cls_union_set(unsigned size)
		:main(size)
	{
		fill(main.begin(),main.end(),0);
	}

	void do_union(int i,int j)
	{
		int x = union_run(i);
		int y = union_run(j);

		main[x] = y;
	}

	bool check_equal(int i,int j)
	{
		int x = union_run(i);
		int y = union_run(j);

		return x == y;
	}
};