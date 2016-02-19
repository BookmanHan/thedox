#pragma  once

#ifndef _MSC_VER
#define __USING_GCC__
#else
#endif

#include <locale>
#include <iostream>
#include <vector>
#include <map>
#include <complex>
#include <cctype>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <list>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <assert.h>
#include <exception>
#include <algorithm>
#include <bitset>
#include <queue>
#include <stack>

#ifdef USING_INTEL_MKL
#include "mkl.h"
#endif

using namespace std;

#ifdef __USING_GCC__
	#include "sys/time.h"
	#include "unistd.h"
#endif

#include "const.h"

//basic algorthm
#include "cls_object.h"
#include "cls_exception.h"
#include "cls_object_cache.h"

#include "alg_externs.h"
#include "alg_time.h"
#include "alg_random.h"
#include "alg_conject_ann.h"
#include "alg_max_min.h"
#include "cls_conject_linear.h"
#include "fd_lib_type.h"
#include "cls_conjector_euclidean.h"

//basic structure
#include "cls_rect.h"
#include "cls_point.h"
#include "cls_circle.h"
#include "cls_multi_array.h"
#include "cls_triple.h"


//math
#include "cls_mat.h"
#include "cls_matrix.h"
#include "alg_math_trans.h"
#include "scls_signal_trans.h"

//basic object
#include "cls_image_ot.h"

#include "cls_image.h"
#include "cls_vision_basis.h"
#include "cls_vision_geometry.h"
#include "cls_vision_mixed.h"
#include "cls_vision_enhance_space.h"
#include "cls_vision_enhance_frequence.h"
#include "cls_vision_restoration.h"
#include "cls_vision.h"

//struct
#include "cls_union_set.h"

//classifier
#include "alg_basic.h"
#include "cls_training_set.h"
#include "cls_classifier.h"
#include "cls_ann_bpnn.h"
#include "cls_ann_boltzman.h"
#include "cls_ann_cpnn.h"
#include "cls_ann_hopfield.h"
#include "cls_nearest.h"
#include "ann_import.h"
#include "cls_ann_link.h"
#include "cls_ann_neuron.h"
#include "cls_ann_abstract_neuron.h"
#include "cls_ann_simple_neuron.h"
#include "cls_ann_center_neuron.h"
#include "cls_ann_input_neuron.h"
#include "cls_ann_layer.h"
#include "cls_ann_layer_input.h"
#include "cls_ann_network.h"
#include "cls_ann_network_mutilayer.h"
#include "cls_ann_network_bilayer.h"
#include "cls_ann_network_trilayer.h"
#include "cls_ann_network_hopfield.h"
#include "cls_ann_neuron_gab.h"
#include "cls_ann_neuron_parametized.h"
#include "cls_ann_neuron_recurrent.h"
#include "cls_ann_network_radial_basis.h"
#include "cls_ann_network_recurrent.h"
#include "cls_ann_neuron_kohonen.h"
#include "cls_ann_eicledian_topology.h"
#include "cls_ann_network_kohonen.h"
#include "cls_linear_unit.h"
#include "cls_linear_unit_abstract.h"
#include "cls_linear_unit_sensor.h"
#include "cls_linear.h"
#include "cls_linear_sensor.h"
#include "cls_linear_unit_normalized_sensor.h"
#include "cls_linear_unit_gradian.h"
#include "cls_linear_sensor_grad_normz.h"
#include "cls_linear_unit_increasement_sensor.h"
#include "cls_linear_sensor_increasement.h"
#include "cls_linear_unit_wh.h"
#include "cls_linear_wh.h"
#include "cls_linear_unit_fisher.h"
#include "cls_linear_fisher.h"
#include "cls_linear_unit_lmse.h"
#include "cls_linear_lmse.h"
#include "cls_linear_multiply.h"
#include "cls_linear_multiply_abstract.h"
#include "cls_linear_multiply_lmse.h"
#include "cls_svm_param.h"
#include "alg_conject_kernel.h"
#include "cls_svm.h"
#include "cls_svm_cache.h"
#include "cls_svm_solver.h"
#include "cls_svm_solver_nu.h"
#include "cls_svm_solve.h"
#include "cls_svm_train.h"
#include "cls_svm_probability.h"
#include "cls_svm_base.h"
#include "cls_svm_child.h"
#include "cls_hmm.h"
#include "cls_bayes_unit.h"
#include "cls_bayes_unit_least_error.h"
#include "cls_bayes.h"
#include "cls_bayes_basis.h"
#include "cls_bayes_least_error.h"
#include "cls_bayes_least_risk.h"
#include "cls_boosting.h"
#include "cls_boosting_first.h"
#include "cls_boosting_second.h"
#include "cls_bagging.h"

//cluster
#include "cls_cluster.h"
#include "cls_cluster_k_means.h"
#include "cls_cluster_hierarchical.h"
#include "cls_cluster_max_min_distance.h"
#include "cls_cluster_threshod.h"
#include "cls_cluster_isoda.h"

//computational intelligence
#include "cls_ci_target.h"
#include "cls_pso_basic.h"
#include "cls_pso_mixed_dim.h"
#include "cls_ci.h"
#include "cls_ci_param.h"
#include "cls_genetic_algorithm_item.h"
#include "cls_genetic_algorithms.h"

//string and type
#include "alg_string.h"
#include "alg_basic_type.h"
#include "alg_vector.h"

//tools
#include "alg_feature.h"
#include "alg_reorder.h"

//data_compression
#include "cls_decoder_huffman.h"
#include "cls_encoder_huffman.h"
#include "cls_decoder_prediction.h"
#include "cls_encoder_prediction.h"

//autometa
#include "cls_lalr.h"
#include "cls_lalr_frame.h"
#include "cls_lalr_frame_wide.h"

//util
#include "cls_log.h"
#include "cls_xml.h"
#include "cls_xml_wide.h"
#include "cls_xml_parser.h"

//parser
#include "cls_parser.h"
#include "cls_parser_math_expr.h"
#include "cls_parser_wide.h"
#include "cls_parser_math_expr_wide.h"
#include "cls_parser_xml_wide.h"
#include "cls_parser_html.h"
#include "cls_parser_css.h"

//searching
#include "cls_search.h"
#include "cls_search_best_first.h"
#include "cls_search_breadth.h"
#include "cls_search_depth.h"
#include "cls_search_depth_iterative.h"
#include "cls_search_depth_limit.h"
#include "cls_search_hill_climbing.h"

//manager
#include "cls_object_manager.h"

//basic format
#include "format_static.h"
#include "cls_format_bmp.h"
#include "cls_format_jpg.h"
//file load and save
#include "alg_load_save.h"

//dependent library.
//#include "dlib_import.h"

//boost library.
//#include "boost_import.h"
