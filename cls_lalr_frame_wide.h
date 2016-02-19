#include "lib_import.h"

template<typename T>
class cls_lalr_frame_item_wide
{
public:
	int status;
	wstring str;
	wstring str_pre;
	en_lalr_type type;

	T user_data;
};

template<typename T>
class cls_a_lalr_dealing_void_wide
{
public:
	virtual void operator()(vector<cls_lalr_frame_item_wide<T> > & now,
		int next_status,
		cls_lalr_frame_item_wide<T> &re,
		vector<cls_lalr_frame_item_wide<T> >& stack)
	{
		;
	}

	virtual wstring operator()(int item)
	{
		return L"";
	}

	virtual wstring operator()(const wstring& item)
	{
		return L"";
	}
};

template<typename T>
class cls_lalr_frame_wide
{
protected:
	vector<cls_lalr_table_item>& tables;
	vector<vector<int> > context;

	cls_lalr_info &info;
	int ter;

	cls_a_lalr_dealing_void_wide<T> * func;

protected:
	int get_next(int row,int sym)
	{
		if (sym >= 10000)
		{
			sym -= 10000;
			sym += ter;
		}

		if (row > context.size() || sym > context[row].size())
			throw cls_exception_execution(string(__FUNCTION__) + "() - out of subsrcipt.\n");

		return context[row][sym];
	}

public:

	cls_lalr_frame_wide(vector<cls_lalr_table_item>& r_tables,
		cls_lalr_info & r_info,int kernels,cls_a_lalr_dealing_void_wide<T> * a_func)
		:tables(r_tables),
		info(r_info),
		func(a_func)
	{
		ter = info.map_ter_name_id.size() + 1;

		context.resize(kernels);
		for(int cnt=0;cnt<kernels;cnt++)
		{
			context[cnt].resize(info.map_ter_name_id.size() + info.map_non_name_id.size() + 2);
			fill(context[cnt].begin(),context[cnt].end(),-1);

			//context[cnt][1] = -1;
		}

		int size = tables.size();
		for(int cnt=0;cnt<size;cnt++)
		{
			int row = tables[cnt].cur;
			int next = tables[cnt].next;

			if (next >= 10000)
			{
				fill(context[row].begin(),context[row].end(),next);
			}
		}

		for(int cnt=0;cnt<size;cnt++)
		{
			int row = tables[cnt].cur;
			int next = tables[cnt].next;
			int sym = tables[cnt].oper;

			if (sym >= 10000)
			{
				sym -= 10000;
				sym += ter;
			}

			context[row][sym] = next;
		}
	}

public:

	virtual void run_map(vector<wstring>& src)
	{
		vector<cls_lalr_frame_item_wide<T> > stack;
		src.push_back(L"__inner_end");

		cls_lalr_frame_item_wide<T> item;
		item.status = 0;
		item.type = en_lalr_ter;
		item.str = L"__inner_end";

		stack.push_back(item);

		en_lalr_type type;
		bool b_get_next = true;
		int cur_status = 0;
		int next_status = 0;
		int pre_status = 0;

		src.insert(src.begin(),L"start");

		vector<wstring>::iterator itor;
		for(itor=src.begin()+1;
			itor != src.end() || stack.size()>=2;
			itor++)
		{
			if (itor == src.end())
			{
				itor--;
			}

			wstring cur = L"";
			while(cur == L"")
			{
				cur = func->operator ()(*itor);

				if (cur == L"")
					itor++;
			}

			pre_status = cur_status;
			cur_status = stack.back().status;

			if (cur_status >= 10000)
			{
				next_status = cur_status;
			}
			else
			{
				next_status = get_next(cur_status,info.get_str_num_ter(wstring_to_string(cur)));
			}

			if (next_status == -1)
			{
				if (stack.size() == 1 || *itor== L"__inner_end")
					break;

				throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(*itor) + ".\n");
			}

			if (next_status < 10000)
			{
				cls_lalr_frame_item_wide<T> item;
				item.status = next_status;
				item.type = en_lalr_ter;
				item.str = cur;
				item.str_pre = *itor;
				stack.push_back(item);
			}
			else
			{
				vector<cls_lalr_frame_item_wide<T> > now;
				for(int cnt=0;cnt<info.map_prod_size[next_status-10000];cnt++)
				{
					now.push_back(stack.back());
					stack.pop_back();
				}

				cls_lalr_frame_item_wide<T> needed;

				now.push_back(needed);
				reverse(now.begin(),now.end());

				int tmp_status = stack.back().status;
				int real_status = get_next(tmp_status,info.map_prod_left[next_status-10000]);

				cls_lalr_frame_item_wide<T> re;

				re.status = real_status;
				re.type = en_lalr_non;
				re.str_pre = *itor;
				re.str = func->operator ()(*itor);

				func->operator ()(now,next_status,re,stack);

				if (real_status == -1)
				{
					if (stack.size() == 1 || *itor == L"__inner_end")
						break;

					throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(*itor) + ".\n");
				}

				stack.push_back(re);
				itor--;
			}
		}
	}

	virtual void run(vector<wstring>& src)
	{
		vector<cls_lalr_frame_item_wide<T> > stack;
		src.push_back(L"__inner_end");

		cls_lalr_frame_item_wide<T> item;
		item.status = 0;
		item.type = en_lalr_ter;
		item.str = L"__inner_end";

		stack.push_back(item);

		en_lalr_type type;
		bool b_get_next = true;
		int cur_status = 0;
		int next_status = 0;
		int pre_status = 0;

		src.insert(src.begin(),L"start");

		vector<wstring>::iterator itor;
		for(itor=src.begin()+1;
			itor != src.end() || stack.size()>=3;
			itor++)
		{
			if (itor == src.end())
			{
				itor--;
			}

			wstring cur = *itor;

			pre_status = cur_status;
			cur_status = stack.back().status;

			if (cur_status >= 10000)
			{
				next_status = cur_status;
			}
			else
			{
				next_status = get_next(cur_status,info.get_str_num_ter(wstring_to_string(cur)));
			}

			if (next_status == -1)
			{
				if (stack.size() <= 2)
					break;

				throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(*itor) + ".\n");
			}

			if (next_status < 10000)
			{
				cls_lalr_frame_item_wide<T> item;
				item.status = next_status;
				item.type = en_lalr_ter;
				item.str = cur;
				item.str_pre = *itor;
				stack.push_back(item);
			}
			else
			{
				vector<cls_lalr_frame_item_wide<T> > now;
				for(int cnt=0;cnt<info.map_prod_size[next_status-10000];cnt++)
				{
					now.push_back(stack.back());
					stack.pop_back();
				}

				cls_lalr_frame_item_wide<T> needed;

				now.push_back(needed);
				reverse(now.begin(),now.end());

				int tmp_status = stack.back().status;
				int real_status = get_next(tmp_status,info.map_prod_left[next_status-10000]);

				cls_lalr_frame_item_wide<T> re;

				re.status = real_status;
				re.type = en_lalr_non;
				re.str_pre = *itor;
				re.str = *itor;

				func->operator ()(now,next_status,re,stack);

				if (real_status == -1)
				{
					if (stack.size() <= 2)
						break;

					throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(*itor) + ".\n");
				}

				stack.push_back(re);
				itor--;
			}
		}
	}

	virtual void run(wstring& src)
	{
		vector<cls_lalr_frame_item_wide<T> > stack;

		cls_lalr_frame_item_wide<T> item;
		item.status = 0;
		item.type = en_lalr_ter;
		item.str = L"__inner_end";

		stack.push_back(item);

		en_lalr_type type;
		bool b_get_next = true;
		int cur_status = 0;
		int next_status = 0;
		int pre_status = 0;

		src.insert(src.begin(),'s');

		for(int cnt=1;cnt<src.size() || stack.size() >= 3;cnt++)
		{
			wstring cur;
			if (cnt >= src.size())
			{
				cur = L"__inner_end";
			}
			else
			{
				cur = get_wstr_from_type(src[cnt]);

				if (cur == L"")
					cur = L" ";
			}

			pre_status = cur_status;
			cur_status = stack.back().status;

			if (cur_status >= 10000)
			{
				next_status = cur_status;
			}
			else
			{
				next_status = get_next(cur_status,info.get_str_num_ter(wstring_to_string(cur)));
			}

			if (next_status == -1)
			{
				if (cur == L"__inner_end" && stack.size() <= 2)
					break;

				throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(cur) + ".\n");
			}

			if (next_status < 10000)
			{
				cls_lalr_frame_item_wide<T> item;
				item.status = next_status;
				item.type = en_lalr_ter;
				item.str = cur;
				stack.push_back(item);
			}
			else
			{
				vector<cls_lalr_frame_item_wide<T> > now;
				for(int cnt=0;cnt<info.map_prod_size[next_status-10000];cnt++)
				{
					now.push_back(stack.back());
					stack.pop_back();
				}

				cls_lalr_frame_item_wide<T> needed;

				now.push_back(needed);
				reverse(now.begin(),now.end());

				int tmp_status = stack.back().status;
				int real_status = get_next(tmp_status,info.map_prod_left[next_status-10000]);

				cls_lalr_frame_item_wide<T> re;
				re.status = real_status;
				re.type = en_lalr_non;
				re.str = cur;

				func->operator ()(now,next_status,re,stack);

				if (real_status == -1)
				{
					if (cur == L"__inner_end" && stack.size() <= 2)
						break;

					throw  cls_exception_execution(string(__FUNCTION__) + "() - syntex error for " + wstring_to_string(cur) + ".\n");
				}

				stack.push_back(re);
				cnt--;
			}
		}
	}

	//virtual void run_mixed(vector<wstring>& src)
	//{
	//	vector<cls_lalr_frame_item<T> > stack;

	//	cls_lalr_frame_item<T> item;
	//	item.status = 0;
	//	item.type = en_lalr_ter;
	//	item.str = "__inner_end";

	//	stack.push_back(item);

	//	en_lalr_type type;
	//	bool b_get_next = true;
	//	int cur_status = 0;
	//	int next_status = 0;
	//	int pre_status = 0;

	//	src.insert(src.begin(),"__inner_end");

	//	for(int cnt=1;cnt<src.size();cnt++)
	//	{
	//		src[cnt] += " ";
	//	}

	//	for(int cnt=1;cnt<src.size();cnt++)
	//	{
	//		for(int cnti = 0;(cnt == src.size() && stack.size() >= 3) || cnti < src[cnt].size();cnti++)
	//		{
	//			wstring cur;
	//			if (cnt >= src.size())
	//				cur = "__inner_end";
	//			else
	//				cur = get_str_from_type(src[cnt][cnti]);

	//			pre_status = cur_status;
	//			cur_status = stack.back().status;

	//			if (cur_status >= 10000)
	//			{
	//				next_status = cur_status;
	//			}
	//			else
	//			{
	//				next_status = get_next(cur_status,info.get_str_num_ter(cur,true));
	//			}

	//			if (next_status == -1)
	//			{
	//				if (cur == " ")
	//				{
	//					continue;
	//				}

	//				if (cur == "__inner_end" && stack.size() == 2)
	//					break;

	//				throw  cls_exception_execution(wstring(__FUNCTION__) + "() - syntex error for " + cur + ".\n");
	//			}

	//			if (next_status < 10000)
	//			{
	//				cls_lalr_frame_item<T> item;
	//				item.status = next_status;
	//				item.type = en_lalr_ter;
	//				item.str = cur;
	//				stack.push_back(item);
	//			}
	//			else
	//			{
	//				vector<cls_lalr_frame_item<T> > now;
	//				for(int cnt=0;cnt<info.map_prod_size[next_status-10000];cnt++)
	//				{
	//					now.push_back(stack.back());
	//					stack.pop_back();
	//				}

	//				cls_lalr_frame_item<T> needed;

	//				now.push_back(needed);
	//				reverse(now.begin(),now.end());

	//				int tmp_status = stack.back().status;
	//				int real_status = get_next(tmp_status,info.map_prod_left[next_status-10000]);

	//				cls_lalr_frame_item<T> re;
	//				re.status = real_status;
	//				re.type = en_lalr_non;
	//				re.str = cur;

	//				func->operator ()(now,next_status,re,stack);

	//				if (real_status == -1)
	//				{
	//					if (cur == "\1")
	//					{
	//						continue;
	//					}

	//					if (cur == "__inner_end" && stack.size() == 2)
	//						break;

	//					throw  cls_exception_execution(wstring(__FUNCTION__) + "() - syntex error for " + cur + ".\n");
	//				}

	//				stack.push_back(re);
	//				cnti--;
	//			}
	//		}
	//	}
	//}
};
