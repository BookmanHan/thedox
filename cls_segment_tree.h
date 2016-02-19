#include "lib_import.h"

class cls_segment_tree
{
protected:
	int size;
	vector<int> cnt;
	vector<int> len;

public:
	cls_segment_tree(int t)
		:size(t),
		:cnt(t+1),
		:len(t+1)
	{
		fill(cnt.begin(),cnt.end(),0);
		fill(len.begin(),len.end(),0);
	}

	void update(int l,int r)
	{
		if (cnt[1] || r-l == 1)
			len[1] = r-l;
		else 
			len[1] = len[2] + len[3];

	}

	void inc_seg(int l0,int r0,int l,int r);
	void dec_seg(int l0,int r0,int l,int r);
	int seg_len(int l0,int r0,int l,int r);
};