#include "lib_import.h"

void test_birepair(cls_vision& vision)
{
	vector<cls_point<int> > src,des;
	src.push_back(cls_point<int>(0,0));
	src.push_back(cls_point<int>(0,vision.get_height()));
	src.push_back(cls_point<int>(vision.get_width(),0));
	src.push_back(cls_point<int>(vision.get_width(),vision.get_height()));
	src.push_back(cls_point<int>(100,100));
	src.push_back(cls_point<int>(200,200));

	des.push_back(cls_point<int>(0,0));
	des.push_back(cls_point<int>(0,vision.get_height()));
	des.push_back(cls_point<int>(vision.get_width(),0));
	des.push_back(cls_point<int>(vision.get_width(),vision.get_height()));
	des.push_back(cls_point<int>(101,99));
	des.push_back(cls_point<int>(199,201));

	vision.geometry_match_binomial(src,des);
}

void test_birepair_simple(cls_vision& vision)
{
	vector<cls_point<int> > src,des;
	src.push_back(cls_point<int>(0,0));
	src.push_back(cls_point<int>(0,vision.get_height()));
	src.push_back(cls_point<int>(vision.get_width(),0));
	src.push_back(cls_point<int>(vision.get_width(),vision.get_height()));

	des.push_back(cls_point<int>(100,100));
	des.push_back(cls_point<int>(0,vision.get_height()));
	des.push_back(cls_point<int>(vision.get_width(),0));
	des.push_back(cls_point<int>(vision.get_width(),vision.get_height()));

	vision.geometry_match_binomial_simple(src,des);
}

void test_vision_zoom(cls_vision& vision)
{
	vision.geometry_zoom(cls_rect(100,100,200,200),cls_rect(0,0,200,200));
}