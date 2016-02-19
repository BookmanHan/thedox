#include "jrunner_import.h"
#pragma  once

class cls_jrunner_symbol_item
{
public:
	int class_id;
	cls_jrunner_symbol_item *parent;
	cls_cap_addr addr;
	bool b_str;

	map<wstring,cls_jrunner_symbol_item*> childs;
	map<wstring,cls_jrunner_symbol_item*> members;

	int offset;

	cls_jrunner_symbol_item* get_class_by_id(int cid)
	{
		if (cid == class_id)
		{
			return this;
		}
		else
		{
			map<wstring,cls_jrunner_symbol_item*>::iterator itor = childs.begin();
			for(itor = childs.begin();itor != childs.end(); itor++)
			{
				cls_jrunner_symbol_item* item = (itor)->second->get_class_by_id(cid);
				if (item != null)
					return item;
			}

			for(itor = members.begin();itor != members.end(); itor++)
			{
				cls_jrunner_symbol_item* item = (itor)->second->get_class_by_id(cid);
				if (item != null)
					return item;
			}
			return null;
		}
	}

	void detach_child()
	{
		childs.clear();
	}

	int get_size_recursive()
	{
		if (childs.size() == 0)
		{
			return 4;
		}
		else 
		{
			int size = 0;
			map<wstring,cls_jrunner_symbol_item*>::iterator itor;
			for(itor=childs.begin();itor!=childs.end();itor++)
			{
				size += itor->second->get_size();
			}

			return size;
		}
	}

	int get_size()
	{
		if (childs.size() == 0)
		{
			return 4;
		}
		else
		{
			return childs.size() * 4;
		}
	}

	cls_jrunner_symbol_item()
	{
		static int static_class_id = 10;

		class_id = static_class_id++;
		parent = null;
		addr.offset = 0;
		addr.length = 4;
		addr.segment = cap_segment_stack;
		offset = 0;
	}
	
	void add_func(const wstring& str, cls_jrunner_symbol_item* that)
	{
		if (get_item(str) != null)
			return;

		that->parent = this;
		that->addr.length = 4;
		that->addr.segment = cap_segment_code;

		members.insert(make_pair(str,that));
	}

	void add_child(const wstring& str, cls_jrunner_symbol_item* that)
	{
		if (get_item(str) != null)
			return;

		that->parent = this;
		that->addr.segment = cap_segment_relative;
		that->addr.length = 4;
		that->addr.offset = offset;
		offset += 4;
		childs.insert(make_pair(str,that));
	}

	cls_jrunner_symbol_item* get_item(const int offset)
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		for(itor=childs.begin();itor!=childs.end();itor++)
		{
			if (itor->second->addr.offset == offset)
				return itor->second;
		}

		return null;
	}

	wstring get_item_string(const int offset)
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		for(itor=childs.begin();itor!=childs.end();itor++)
		{
			if (itor->second->addr.offset == offset)
				return itor->first;
		}

		return L"";
	}

	cls_jrunner_symbol_item* get_item(const wstring& str)
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		itor = childs.find(str);

		if (itor == childs.end())
			return null;
		else
			return itor->second;
	}

	cls_jrunner_symbol_item* get_item_up(const wstring& str)
	{
		cls_jrunner_symbol_item* item = this;
		cls_jrunner_symbol_item* res = null;

		while(item != null)
		{
			res = item->get_item(str);
			if (res != null)
				return res;

			item = item->parent;
		}

		return null;
	}

	cls_jrunner_symbol_item* get_item_func(const wstring& str)
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		itor = members.find(str);

		if (itor == members.end())
			return null;
		else
			return itor->second;
	}

	cls_jrunner_symbol_item* get_item_up_func(const wstring& str)
	{
		cls_jrunner_symbol_item* item = this;
		cls_jrunner_symbol_item* res = null;

		while(item != null)
		{
			res = item->get_item_func(str);
			if (res != null)
				return res;

			item = item->parent;
		}

		return null;
	}

	~cls_jrunner_symbol_item()
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		for(itor=childs.begin();itor!=childs.end();itor++)
		{
			delete (itor->second);
		}
	}
};

class cls_jrunner_symbol_table
{
public:
	map<wstring,cls_jrunner_symbol_item*> items;

public:
	cls_jrunner_symbol_item* cur_func;
	int cur_stack_offset;

public:
	cls_jrunner_symbol_table()
	{
		cur_stack_offset = 12;
	}

	void add_item(const wstring& str,cls_jrunner_symbol_item* that)
	{
		items.insert(make_pair(str,that));
	}

	void delete_item(const wstring& str)
	{
		items.erase(str);
	}

	cls_jrunner_symbol_item* get_item(const wstring& str)
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		itor = items.find(str);

		if (itor == items.end())
			return null;
		else
			return itor->second;
	}
};

class cls_jrunner_symbol_manager
{
public:

	stack<unsigned> stack_offset;

	cls_jrunner_symbol_item *cur_func;
	wstring name_space;
	int cur_const_offset;

	cls_jrunner_symbol_table table_func;
	cls_jrunner_symbol_table table_const;
	
	vector<cls_jrunner_symbol_item*> v_new;

public:
	cls_jrunner_symbol_manager()
	{
		add_func(L"jrunner_main",new cls_jrunner_symbol_item);
		cur_const_offset = 12;
	}

	cls_jrunner_symbol_item* get_cur_func()
	{
		return cur_func;
	}

	cls_jrunner_symbol_item* get_item(const wstring& str)
	{
		cls_jrunner_symbol_item* item = table_func.get_item(str);

		if (item != null)
			return item;
		else 
			return cur_func->get_item(str);
	}

	void add_func(const wstring& str,cls_jrunner_symbol_item* item)
	{
		item->parent = get_cur_func();
		item->addr.segment = cap_segment_code;

		if (get_cur_func() == null)
		{
			item->add_child(L"this",new cls_jrunner_symbol_item);
			item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
			item->add_child(L"super",new cls_jrunner_symbol_item);

			table_func.add_item(str,item);
		}
		else
		{
			get_cur_func()->add_func(str,item);
		}
	}

	void add_item(const wstring& str,cls_jrunner_symbol_item* item)
	{  
			get_cur_func()->add_child(str,item);
	}

	int get_offset()
	{
		return stack_offset.top();
	}

	int get_space()
	{
		stack_offset.top() += 4;

		return stack_offset.top() - 4;
	}

	cls_jrunner_symbol_item* get_const(const wstring& str)
	{
		return table_const.get_item(str);
	}

	int add_const(const wstring& str,cls_jrunner_symbol_item** item = null,bool b_str = true)
	{
		if (table_const.get_item(str) != null)
		{
			if (item != null)
				*item = table_const.get_item(str);
			
			return table_const.get_item(str)->addr.offset;
		}

		if (item == null)
		{
			item = new cls_jrunner_symbol_item*;
		}

		*item = new cls_jrunner_symbol_item;
		(*item)->b_str = b_str;

		(*item)->addr.segment = cap_segment_const;
		(*item)->addr.length = 4;
		(*item)->addr.offset = table_const.cur_stack_offset;

		table_const.add_item(str,(*item));
		table_const.cur_stack_offset += 4;

		return table_const.cur_stack_offset -  4;
	}

	void enter(cls_jrunner_symbol_item* cur_func = null)
	{
		cls_jrunner_symbol_table table;
		this->cur_func = cur_func;

		stack_offset.push(12);
	}

	void leave()
	{
		cur_func = cur_func->parent;
		stack_offset.pop();
	}

	cls_jrunner_symbol_item* get_func(const wstring& str)
	{
		if (str == L"jrunner_main")
		{
			return table_func.get_item(str);
		}
		else if (get_cur_func() == null)
		{
			return table_func.get_item(str);
		}
		else
		{
			return get_cur_func()->get_item_up_func(str);
		}
	}
};