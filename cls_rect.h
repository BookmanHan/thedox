#pragma  once
#include "lib_import.h"

class cls_rect
{
public:

	int top;
	int left;
	int bottom;
	int right;

	cls_rect()
	{

	}

	cls_rect(int t,int l,int b,int r)
	{
		top = t;
		left = l;
		bottom = b;
		right = r;
	}

	inline int get_width() const
	{
		return right - left;
	}

	inline int get_height() const
	{
		return bottom - top;
	}

	inline cls_rect& dilate(int n)
	{
		top -= n;
		left -= n;
		right += n;
		bottom += n;

		return *this;
	}

	inline cls_rect& shrink(int n)
	{
		top += n;
		left += n;
		right -= n;
		bottom -= n;

		return *this;
	}

	bool pt_in_rect(int x,int y)
	{
		if (x >= left && x <= right && y <= bottom && y >= top)
			return true;

		return false;
	}

	bool rect_in_rect(cls_rect& that)
	{
		return pt_in_rect(that.left,that.top) && pt_in_rect(that.right,that.bottom);
	}
};