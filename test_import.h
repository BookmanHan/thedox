#pragma  once
#include "lib_import.h"

void test_ann_add();
void test_ann_xor();
void test_ann_hopfield();
void test_ann_radial_basis();
void test_ann_kohonen();
void test_linear_sensor();
void test_linear_sensor_gnz();
void test_linear_sensor_inc();
void test_linear_sensor_wh();
void test_linear_fisher();
void test_linear_lmse();
void test_linear_multiply_lmse();
void test_svm();
void test_svm_c_svc();
void test_svm_nu_svc();
void test_svm_one_class();
void test_svm_ev();
void test_svm_nu_svr();
void test_hmm();				//切记序号从0开始。
void test_boosting_first();
void test_boosting_second();
void test_bagging();
void test_bayes_basis();
void test_bayes_least_error();

/*
问题：
1.模糊聚类没有完成。
*/

void test_mysql_create();

void test_union_set();

void test_genetic_algorthim_frame();
void test_genetic_group_stable();

void test_lalr_1();
void test_cava_1();
void test_lalr_xml();
void test_parser_math_express();
void test_parser_math_express_wide();
void test_parser_xml_wide();
void test_parser_html();
void test_parser_css();

void test_cluster_k_means();
void test_cluster_hierarchical();
void test_cluster_max_min_distance();
void test_cluster_isoda();

void test_search_breath();

void test_birepair(cls_vision& vision);
void test_birepair_simple(cls_vision& vision);
void test_vision_zoom(cls_vision& vision);