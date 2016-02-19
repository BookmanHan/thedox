#include "lib_import.h"

class cls_lalr_info
{
public:
	map<string,int> map_non_name_id;
	map<string,int> map_ter_name_id;
	map<int,set<int> > follow;
	map<int,set<int> > first;
	vector<string> v_prod_exp;
	map<int,int> map_prod_size;
	map<int,int> map_prod_left;

private:
	int max_non_id;
	int max_ter_id;

public:

	void load(const string& str_filename)
	{
		map_non_name_id.clear();
		map_ter_name_id.clear();
		map_prod_left.clear();
		map_prod_size.clear();

		int n;
		ifstream fin(str_filename.c_str());

		fin>>max_non_id;
		fin>>max_ter_id;

		fin>>n;
		for(int cnt=0;cnt<n;cnt++)
		{
			string f;
			int s;

			fin>>f>>s;
			map_non_name_id.insert(make_pair(f,s));
		}

		fin>>n;
		for(int cnt=0;cnt<n;cnt++)
		{
			string f;
			int s;

			fin>>f>>s;
			map_ter_name_id.insert(make_pair(f,s));
		}

		fin>>n;
		for(int cnt=0;cnt<n;cnt++)
		{
			int f;
			int s;

			fin>>f>>s;
			map_prod_size.insert(make_pair(f,s));
		}

		fin>>n;
		for(int cnt=0;cnt<n;cnt++)
		{
			int f;
			int s;

			fin>>f>>s;
			map_prod_left.insert(make_pair(f,s));
		}

		fin.close();
	}

	void save(const string& str_filename)
	{
		ofstream fout(str_filename.c_str());

		fout<<max_non_id<<endl;
		fout<<max_ter_id<<endl;

		map<string,int>::iterator itor;

		fout<<map_non_name_id.size()<<endl;
		for(itor=map_non_name_id.begin();itor!=map_non_name_id.end();itor++)
		{
			fout<<itor->first<<endl;
			fout<<itor->second<<endl;
		}

		int n = 0;
		for(itor=map_ter_name_id.begin();itor!=map_ter_name_id.end();itor++)
		{
			if (a_string_trim(itor->first) == "")
				continue;

			n++;
		}

		fout<<n<<endl;
		for(itor=map_ter_name_id.begin();itor!=map_ter_name_id.end();itor++)
		{
			if (a_string_trim(itor->first) == "")
				continue;

			fout<<itor->first<<endl;
			fout<<itor->second<<endl;
		}

		map<int,int>::iterator itor_nn;

		fout<<map_prod_size.size()<<endl;
		for(itor_nn=map_prod_size.begin();itor_nn!=map_prod_size.end();itor_nn++)
		{
			fout<<itor_nn->first<<endl;
			fout<<itor_nn->second<<endl;
		}

		fout<<map_prod_left.size()<<endl;
		for(itor_nn=map_prod_left.begin();itor_nn!=map_prod_left.end();itor_nn++)
		{
			fout<<itor_nn->first<<endl;
			fout<<itor_nn->second<<endl;
		}

		fout.close();
	}

	cls_lalr_info()
	{
		max_ter_id = 0;
		max_non_id = 0;
		get_str_num_ter("__inner_end");
	}

	int get_str_num_ter(const string &str,bool b_mode_run = false)
	{
		map<string,int>::iterator itor;
		itor = map_ter_name_id.find(str);
		if (itor != map_ter_name_id.end())
		{
			return itor->second;
		}
		else
		{
			if (b_mode_run == true)
				return -1;

			map_ter_name_id[str] = max_ter_id;

			return  max_ter_id++;
		}
	}

	int get_str_num_non(const string& str)
	{
		map<string,int>::iterator itor;
		itor = map_non_name_id.find(str);
		if (itor != map_non_name_id.end())
		{
			return itor->second;
		}
		else
		{
			map_non_name_id[str] = const_lalr_spilt + max_non_id;

			return const_lalr_spilt + max_non_id++;
		}
	}

	string& get_product(int item)
	{
		return v_prod_exp[item];
	}
};

class cls_lalr_product_item
{
public:
	const static int const_spilt = 10000;
	bool b_non;
	int item;

public:
	operator int()
	{
		return item;
	}

	bool operator == (const cls_lalr_product_item& that) const
	{
		return this->item == that.item;
	}

	cls_lalr_product_item()
	{
		;
	}

	void set_item(int a_item)
	{
		item = a_item;
		if (item < const_spilt)
			b_non = false;
		else
			b_non = true;
	}

	cls_lalr_product_item(int a_item)
		:item(a_item),
		 b_non(a_item>=const_spilt)
	{
		;
	}

	inline bool check_non()
	{
		return b_non;
	}

	inline int get_item()
	{
		return item;
	}
};

class cls_lalr_product
{
public:
	int item;
	cls_lalr_product_item *left;
	vector<cls_lalr_product_item*> right;
	cls_lalr_info * info;

public:
	int oper;
	int order;

private:
	void analyze_product()
	{
		int status = 0;
		bool b_do_left = false;
		string str_get;
		string& str_prod = info->get_product(item);

		string::iterator itor;
		for(itor=str_prod.begin();itor != str_prod.end();itor++)
		{
			switch(*itor)
			{
			case '$':
				{
					if (status != 2)
					{
						status = 2;
					}
					else
					{
						right.push_back(
							new cls_lalr_product_item(info->get_str_num_ter(str_get))
							);

						str_get.clear();

						oper = right.back()->get_item();
						status = 0;
					}
				}
				break;

			case '#':
				{
					if (status != 1)
					{
						status = 1;
					}
					else
					{
						if (b_do_left == false)
						{
							left = new cls_lalr_product_item(info->get_str_num_non(str_get));
							str_get.clear();
						}
						else
						{
							right.push_back(new cls_lalr_product_item(info->get_str_num_non(str_get)));
							str_get.clear();
						}

						status = 0;
					}
				}
				break;

			case '@':
				b_do_left = true;
				break;

			default:
				//TODO:这里速度会很慢的。
				str_get += *itor;
				break;
			}
		}
	}

public:
	cls_lalr_product()
	{
		left = null;
	}

	void make_deleted()
	{
		if (left != null)
			delete left;

		vector<cls_lalr_product_item*>::iterator itor;
		for(itor = right.begin();itor != right.end();itor++)
		{
			delete (*itor);
		}
	}

	cls_lalr_product(int a_item,int a_pos,cls_lalr_info* a_info)
		:info(a_info),
		oper(-1),
		order(a_pos),
		item(a_item)
	{
		analyze_product();
	}
};

class cls_lalr_kernel_item
	:public cls_lalr_product
{
public:
	int pos;

public:
	cls_lalr_kernel_item(const cls_lalr_product& p)
		:cls_lalr_product(p)
	{
		pos = 0;
	}

	bool operator == (const cls_lalr_kernel_item& that) const
	{
		if (this->item == that.item)
		{
			if (this->pos == that.pos)
				return true;
			else
				return false;
		}
		else
		{
			return false;
		}
	}

	bool operator < (const cls_lalr_kernel_item& that) const
	{
		if (this->item < that.item)
		{
			return true;
		}
		else if (this->item == that.item)
		{
			return this->pos < that.pos;
		}
		else
		{
			return false;
		}
	}

	bool check_over() const
	{
		return pos == right.size();
	}
};

class cls_lalr_move
{
public:
	int pre;
	int cur;
	int item;

	cls_lalr_move()
	{
		;
	}

	cls_lalr_move(int a_pre,int a_cur,int a_item)
		:pre(a_pre),
		 cur(a_cur),
		 item(a_item)
	{
		;
	}
};

class cls_lalr_table_item
{
public:
	int cur;
	int next;
	int oper;

public:
	cls_lalr_table_item(int ac,int ao,int an)
		:cur(ac),
		 next(an),
		 oper(ao)
	{
		;
	}
};

class cls_lalr_kernel
{
public:
	set<cls_lalr_kernel_item> items;
	vector<cls_lalr_move*> moves;

public:
	cls_lalr_kernel()
	{

	}

	void get_output(int n_cur,ofstream& fout,cls_lalr_info& info,vector<int>& rank,set<int>& join)
	{
		bool b_reduce = false;
		int order = 0;

		map<int,int> v_rank;

		set<cls_lalr_kernel_item>::iterator itor;
		for(itor=items.begin();itor!=items.end();itor++)
		{
			if (itor->check_over())
			{
				set<int>::iterator itor_s,itor_end;
				itor_end = info.follow[itor->left->get_item()].end();

				for(itor_s = info.follow[itor->left->get_item()].begin();
					itor_s != itor_end;
					itor_s++)
				{
					if (*itor_s == -1)
					{
						continue;
					}

					if (itor->oper != -1)
					{
						v_rank[*itor_s] = rank[itor->oper];
					}


					fout<<n_cur<<"\t";
					fout<<*itor_s<<"\t";
					fout<<(10000 + itor->order)<<endl;
				}
			}
		}

		vector<cls_lalr_move*>::iterator itor_move;
		for(itor_move=moves.begin();itor_move != moves.end();itor_move++)
		{
			if ((*itor_move)->item <= const_lalr_spilt)
			{
				map<int,int>::iterator itor_map = v_rank.find((*itor_move)->item);

				if (itor_map != v_rank.end() && itor_map->second >rank[(*itor_move)->item])
				{
					continue;
				}
				else if (itor_map != v_rank.end() && itor_map->second == rank[(*itor_move)->item])
				{
					if (join.find((*itor_move)->item) != join.end())
					{
						fout<<(*itor_move)->pre<<"\t";
						fout<<(*itor_move)->item<<"\t";
						fout<<(*itor_move)->cur<<endl;
					}
					else
					{
						continue;
					}
				}
				else
				{
					fout<<(*itor_move)->pre<<"\t";
					fout<<(*itor_move)->item<<"\t";
					fout<<(*itor_move)->cur<<endl;
				}
			}
			else
			{
				fout<<(*itor_move)->pre<<"\t";
				fout<<(*itor_move)->item<<"\t";
				fout<<(*itor_move)->cur<<endl;
			}
		}
	}

	void get_output(int n_cur,vector<cls_lalr_table_item>& table,cls_lalr_info& info,vector<int>& rank,set<int>& join)
	{
		bool b_reduce = false;
		int order = 0;

		map<int,int> v_rank;

		set<cls_lalr_kernel_item>::iterator itor;
		for(itor=items.begin();itor!=items.end();itor++)
		{
			if (itor->check_over())
			{
				set<int>::iterator itor_s,itor_end;
				itor_end = info.follow[itor->left->get_item()].end();

				for(itor_s = info.follow[itor->left->get_item()].begin();
					itor_s != itor_end;
					itor_s++)
				{
					if (*itor_s == -1)
					{
						continue;
					}

					if (itor->oper != -1)
					{
						v_rank[*itor_s] = rank[itor->oper];
					}

					table.push_back(cls_lalr_table_item(n_cur,*itor_s,10000+itor->order));
				}
			}
		}

		vector<cls_lalr_move*>::iterator itor_move;
		for(itor_move=moves.begin();itor_move != moves.end();itor_move++)
		{
			if ((*itor_move)->item <= const_lalr_spilt)
			{
				map<int,int>::iterator itor_map = v_rank.find((*itor_move)->item);

				if (itor_map != v_rank.end() && itor_map->second >rank[(*itor_move)->item])
				{
					continue;
				}
				else if (itor_map != v_rank.end() && itor_map->second == rank[(*itor_move)->item])
				{
					if (join.find((*itor_move)->item) != join.end())
					{
						table.push_back(cls_lalr_table_item((*itor_move)->pre,(*itor_move)->item,(*itor_move)->cur));
					}
					else
					{
						continue;
					}
				}
				else
				{
					table.push_back(cls_lalr_table_item((*itor_move)->pre,(*itor_move)->item,(*itor_move)->cur));
				}
			}
			else
			{
				table.push_back(cls_lalr_table_item((*itor_move)->pre,(*itor_move)->item,(*itor_move)->cur));
			}
		}
	}

	bool operator == (const cls_lalr_kernel& that) const
	{
		if (that.items.size() != this->items.size())
			return false;

		if (that.items != this->items)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void move(vector<cls_lalr_kernel>& kenrels,vector<cls_lalr_product>& src,const int n_cur,int &n_total)
	{
		vector<bool> b_used(items.size());

		fill(b_used.begin(),b_used.end(),false);

		set<cls_lalr_kernel_item>::iterator itor,itor_next;
		vector<bool>::iterator itor_b,itor_b_next;

		for(itor_b = b_used.begin(),itor = items.begin();
			itor != items.end();
			itor_b++,itor++)
		{
			if (itor->check_over())
				continue;

			if (*itor_b == true)
				continue;

			*itor_b = true;

			cls_lalr_kernel tmp_kernel;
			cls_lalr_kernel_item tmp_item = *itor;

			tmp_item.pos++;
			tmp_kernel.items.insert(tmp_item);

			cls_lalr_product_item item = *(itor->right[itor->pos]);

			itor_next = items.begin();
			for(itor_b_next=b_used.begin();
				itor_next != items.end();
				itor_b_next++,itor_next++)
			{
				if (itor_next->check_over())
					continue;

				if (itor_next->right[itor_next->pos]->get_item() == item.get_item())
				{
					*itor_b_next = true;
					cls_lalr_kernel_item n_item = *itor_next;
					n_item.pos++;

					tmp_kernel.items.insert(n_item);
				}
			}

			tmp_kernel.make_closure(src);
			bool b_found = false;

			int cnt_c = 0;
			vector<cls_lalr_kernel>::iterator itor;
			for(itor=kenrels.begin();itor != kenrels.end();itor++,cnt_c++)
			{
				if (*itor == tmp_kernel)
				{
					b_found = true;
					break;
				}
			}

			if (b_found == false)
			{
				cls_lalr_move* move_new = new cls_lalr_move();
				move_new->pre = n_cur;
				move_new->cur = n_total;
				move_new->item = item.item;
				moves.push_back(move_new);

				kenrels[n_total] = tmp_kernel;
				n_total++;
			}
			else
			{
				cls_lalr_move* move_new = new cls_lalr_move();
				move_new->pre = n_cur;
				move_new->cur = cnt_c;
				move_new->item = item.item;
				moves.push_back(move_new);

			}
		}
	}

	void make_closure(const vector<cls_lalr_product>& src)
	{
		int pos_last = 0;
		bool b_change = true;
		while(b_change)
		{
			b_change = false;
			int pos_last_r = items.size();

			set<cls_lalr_kernel_item>::iterator itor;
			for(itor=items.begin();itor!=items.end();itor++)
			{
				if (itor->check_over())
					continue;

				cls_lalr_product_item cur_item = *(itor->right[itor->pos]);

				if (cur_item.check_non())
				{
					int size = src.size();
					for(int cnt=0;cnt<size;cnt++)
					{
						if (src[cnt].left->item == cur_item.item)
						{
							cls_lalr_kernel_item k_item = src[cnt];
							k_item.pos = 0;

							if (find(items.begin(),items.end(),k_item) == items.end())
							{
								items.insert(k_item);
								b_change = true;
							}
						}
					}
				}
			}

			pos_last = pos_last_r;
		}
	}
};

class cls_lalr
{
public:
	cls_lalr_info info;
	set<int> join_right;
	vector<cls_lalr_product> src;
	vector<cls_lalr_kernel> kernels;
	vector<int> rank;
	int n_total;

public:
	int get_kernel_size()
	{
		return n_total;
	}

	void add_join_right(string str)
	{
		join_right.insert(info.map_ter_name_id[str]);
	}

	bool check_product_empty(int n)
	{
		for(int cnt=0;cnt<src.size();cnt++)
		{
			if (src[cnt].left->item == n && src[cnt].right.size() == 0)
				return true;
		}

		return false;
	}

	bool get_first_set_r()
	{
		map<int,set<int> > pre = info.first;
		map<string,int>::iterator itor = info.map_non_name_id.begin();

		for(itor=info.map_non_name_id.begin();itor != info.map_non_name_id.end();itor++)
		{
			vector<cls_lalr_product>::iterator itor_src;
			for(itor_src=src.begin();itor_src != src.end();itor_src++)
			{
				if (itor_src->left->item == itor->second)
				{
					int cnt_k = 0;
					int size =  itor_src->right.size();
					for(cnt_k = 0;cnt_k < size;cnt_k++)
					{
						if (itor_src->right[cnt_k]->item >= const_lalr_spilt)
						{
							int cur = itor_src->right[cnt_k]->get_item();

							set<int>::iterator itor_set;
							for(itor_set=info.first[cur].begin();itor_set != info.first[cur].end();itor_set++)
							{
								info.first[itor->second].insert(*itor_set);
							}

							if (!check_product_empty(itor_src->right[cnt_k]->get_item()))
							{
								break;
							}
						}
						else
						{
							info.first[itor->second].insert(itor_src->right[cnt_k]->get_item());
						}
					}

					if (cnt_k == itor_src->right.size())
					{
						info.first[itor->second].insert(-1);
					}
				}
			}
		}

		return pre != info.first;
	}

	void make_first_set()
	{
		while(get_first_set_r())
		{
			;
		}
	}

	bool get_follow_set_r()
	{
		map<int,set<int> > pre = info.follow;
		vector<cls_lalr_product>::iterator itor;

		for(itor=src.begin();itor!=src.end();itor++)
		{
			if (itor->right.size() <=1)
			{
				continue;
			}

			vector<cls_lalr_product_item*>::iterator itor_vi,itor_t;
			for(itor_vi=itor->right.begin();itor_vi != itor->right.end()-1;itor_vi++)
			{
				if ((*itor_vi)->check_non())
				{
					itor_t = itor_vi;
					do
					{
						itor_t++;
						if ((*itor_t)->check_non())
						{
							set<int>::iterator itor_set = info.first[(*itor_t)->get_item()].begin();
							for(itor_set = info.first[(*itor_t)->get_item()].begin();
								itor_set != info.first[(*itor_t)->get_item()].end();
								itor_set++)
							{
								info.follow[(*itor_vi)->get_item()].insert(*itor_set);
							}
						}
						else
						{
							info.follow[(*itor_vi)->get_item()].insert((*itor_t)->get_item());
						}

					}
					while (itor_t < itor->right.end()-1 && (*itor_vi)->check_non() && check_product_empty((*itor_t)->get_item()));

					if (itor_t == itor->right.end() - 1)
					{
						set<int>::iterator itor_set;
						for(itor_set = info.follow[(itor)->left->get_item()].begin();
							itor_set != info.follow[(itor)->left->get_item()].begin();
							itor_set++)
						{
							info.follow[(*itor_vi)->get_item()].insert((*itor_set));
						}
					}
				}
			}
		}

		for(itor=src.begin();itor!=src.end();itor++)
		{
			if (itor->right.size() == 0)
			{
				continue;
			}

			if (itor->right.back()->check_non())
			{
				set<int>::iterator itor_set;
				for(itor_set = info.follow[itor->left->get_item()].begin();
					itor_set != info.follow[itor->left->get_item()].end();
					itor_set++)
				{
					info.follow[itor->right.back()->get_item()].insert(*itor_set);
				}
			}
		}

		//TODO 可以考虑用b_changed来代替。
		return pre != info.follow;
	}

	void make_follow_set()
	{
		info.follow[src[0].left->get_item()].insert(info.map_ter_name_id["__inner_end"]);
		while(get_follow_set_r())
		{
			;
		}
	}

	cls_lalr()
		:kernels(2000),
		 rank(200),
		 src(1000)
	{
		info.get_str_num_ter(" ");

		n_total = 0;
	}

	void run()
	{
		make_first_set();
		make_follow_set();
		analyze_lalr();
	}

	void add_rank(vector<string>& rank)
	{
		int cnt_total = 1000;
		vector<string>::iterator itor;
		for(itor = rank.begin();itor != rank.end();itor++)
		{
			if (*itor == "@")
			{
				cnt_total--;
			}
			else
			{
				(this->rank)[info.map_ter_name_id[*itor]] = cnt_total;
			}
		}
	}

	void save(string str_filename)
	{
		ofstream fout(str_filename.c_str());
		fout<<kernels.size()<<endl;

		for(int cnt=0;cnt<n_total;cnt++)
		{
			kernels[cnt].get_output(cnt,fout,info,rank,join_right);
		}

		fout.close();
	}

	void get_output(vector<cls_lalr_table_item>& table)
	{
		for(int cnt=0;cnt<n_total;cnt++)
		{
			kernels[cnt].get_output(cnt,table,info,rank,join_right);
		}
	}

	void add_product(vector<string>& product)
	{
		info.v_prod_exp = product;
		int size = product.size();

		for(int cnt=0;cnt<size;cnt++)
		{
			src[cnt] = cls_lalr_product(cnt,n_total++,&info);
		}

		src.resize(size);

		for(int cnt=0;cnt<size;cnt++)
		{
			info.map_prod_size[src[cnt].item] = src[cnt].right.size();
			info.map_prod_left[src[cnt].item] = src[cnt].left->get_item();
		}
	}

	void analyze_lalr()
	{
		cls_lalr_kernel_item org_item(src[0]);
		cls_lalr_kernel k;

		int n_last = 0;
		n_total = 1;

		org_item.right.push_back(new cls_lalr_product_item(info.get_str_num_ter("__inner_end")));
		kernels[0].items.insert(org_item);
		kernels[0].make_closure(src);

		for(int cnt=0;cnt<n_total;cnt++)
		{
			//cout<<cnt<<" - finished."<<endl;
			kernels[cnt].move(kernels,src,cnt,n_total);
		}
	}
};
