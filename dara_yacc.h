#pragma once
class cls_yacc
{
private:
	cls_yacc_info info;
	vector<cls_yacc_kernal> kernals;
	vector<cls_yacc_product> src;
	vector<int> rank;
	set<int> join_right;

	int n_total;

public:
	void load_join_from_file(string str_filename);
	bool check_product_empty(int n);
	bool get_first_set_r();
	void get_first_set();
	bool get_follow_set_r();
	void get_follow_set();
	void display_info(string str_info);
	cls_yacc(string str_product,string str_rank,string str_join);
	void load_rank_from_file(string str_filename);
	void load_product_from_file(string str_filename);
	void analyze_slr();
	void save_to_file(string str_filename);

	friend class cls_yacc_kernal;
};