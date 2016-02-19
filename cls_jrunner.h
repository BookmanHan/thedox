#include "jrunner_import.h"
#include <cstdio>

#include <windows.h>

class cls_jrunner
	:public cls_cap_runner
{
protected:

	typedef void(cls_jrunner::*func_load)(const cls_cap_command& cmd);
	cls_jrunner_parser &parser;
	map<wstring,func_load> map_load;

public:
	cls_jrunner(cls_jrunner_parser& aparser)
		:cls_cap_runner(aparser.cmds),parser(aparser)
	{
		map_load[L"alert"] = &cls_jrunner::command_load_alert;
		map_load[L"wait"] = &cls_jrunner::command_load_wait;
		map_load[L"alert_num"] = &cls_jrunner::command_load_alert_num;
		map_load[L"exit"] = &cls_jrunner::command_load_exit;
		map_load[L"console.gets"] = &cls_jrunner::command_load_console_gets;
		map_load[L"console.puts"] = &cls_jrunner::command_load_console_puts;
		map_load[L"console.get_num"] = &cls_jrunner::command_load_console_gets_num;
		map_load[L"console.put_num"] = &cls_jrunner::command_load_console_puts_num;
		map_load[L"console.endline"] = &cls_jrunner::command_load_console_puts_endline;
		map_load[L"return_from_callback"] = &cls_jrunner::command_ret;
		map_load[L"sleep"] = &cls_jrunner::command_sleep;
		map_load[L"create_thread"] = &cls_jrunner::command_create_thread;
		map_load[L"wait_thread"] = &cls_jrunner::command_wait_thread;
		map_load[L"start_thread"] = &cls_jrunner::command_start_thread;
		map_load[L"get_thread_id"] = &cls_jrunner::command_get_thread_id;
		map_load[L"kill_thread"] = &cls_jrunner::command_kill_thread;
		map_load[L"open_sema"] = &cls_jrunner::command_open_sema;
		map_load[L"close_sema"] = &cls_jrunner::command_close_sema;
		map_load[L"wait_sema"] = &cls_jrunner::command_wait_sema;
		map_load[L"signal_sema"] = &cls_jrunner::command_signal_sema;
	}

	void util_callback(int*obj)
	{
		obj[5] = 1;

		vector<cls_cap_command>::iterator itor_pre = itor;
		itor_pre++;

		load_util_call_func(obj);
		itor++;
		while(itor != itor_pre)
		{
			run_command(*itor);
			itor++;
		}

		itor = itor_pre;
		itor--;
	}

	int* util_get_this()
	{
		return load_util_get_this();
	}

	int* util_get_var(wstring wstr)
	{
		return load_util_get_var(wstr);
	}

	void util_ret_int(int n)
	{
		int* p = (int*)alloc_new(4);
		*p = n;
		load_util_return_direct((byte*)p);
	}

	int util_get_int(wstring str)
	{
		return *util_get_var(str);
	}

	void util_ret_object(void* n)
	{
		int* p = (int*)alloc_new(4);
		*p = (int)n;
		p[-2] = 1;
		p[-1] = 1;

		load_util_return_direct((byte*)p);
	}

	void* util_get_object(wstring str)
	{
		return (void*)*util_get_var(str);
	}

	void util_ret_str(wstring str)
	{
		load_util_return((byte*)str.c_str(),(str.size()+1)*sizeof(wchar_t));
	}

	wchar_t* util_get_str(wstring str)
	{
		return (wchar_t*)*util_get_var(str);
	}

	void command_signal_sema(const cls_cap_command& cmd)
	{
		wchar_t* str = (wchar_t*)*load_util_get_var(L"param_0");

		signal_sema(str);
	}

	void command_wait_sema(const cls_cap_command& cmd)
	{
		wchar_t* str = (wchar_t*)*load_util_get_var(L"param_0");

		wait_sema(str);
	}

	void command_close_sema(const cls_cap_command& cmd)
	{
		wchar_t* str = (wchar_t*)*load_util_get_var(L"param_0");

		delete_sema(str);
	}

	void command_open_sema(const cls_cap_command& cmd)
	{
		wchar_t* str = (wchar_t*)*load_util_get_var(L"param_0");
		int num = *load_util_get_var(L"param_1");

		open_sema(str,num);
	}

	void command_kill_thread(const cls_cap_command& cmd)
	{
		int tid = *(int*)load_util_get_var(L"param_0");

		delete_thread(tid);
		switch_thread(get_next_thread_id());
	}

	void command_get_thread_id(const cls_cap_command& cmd)
	{
		int* ptr =(int*)alloc_new(4);
		*ptr = thread_cur;

		load_util_return_direct((byte*)ptr);
	}

	void command_start_thread(const cls_cap_command& cmd)
	{
		int tid = *(int*)load_util_get_var(L"param_0");

		if (get_num_for_tid(tid) != -1)
			thread_pool[get_num_for_tid(tid)].b_wait = false;

		switch_thread(get_next_thread_id());
	}

	void command_wait_thread(const cls_cap_command& cmd)
	{
		int tid = *(int*)load_util_get_var(L"param_0");

		if (get_num_for_tid(tid) != -1)
		{
			if (get_num_for_tid(thread_cur) != -1)
				thread_pool[get_num_for_tid(thread_cur)].b_wait = true;

			thread_pool[get_num_for_tid(tid)].tid_wait.push_back(thread_cur);
		}

		switch_thread(get_next_thread_id());
	}

	void command_create_thread(const cls_cap_command& cmd)
	{
		int*p_func = (int*)*load_util_get_var(L"param_0");
		
		p_func[-1] = 1000;
		p_func[5] = 1;

		cls_jrunner_symbol_item* item = parser.sm.get_func(L"jrunner_main")->get_class_by_id(p_func[1]);
		int offset = item->addr.offset;
		add_thread(offset);

		*((int*)(thread_pool.back().pseg)) = (int)p_func;

		int* tid = (int*)alloc_new(4);
		*tid = thread_pool.back().tid;

		load_util_return_direct((byte*)tid);
	}

	void command_sleep(const cls_cap_command& cmd)
	{
		Sleep(1000);
	}

	void command_ret(const cls_cap_command& cmd)
	{
		stack_pointer -= limit_stack/2;
		itor = stk_function_call.top();
		stk_function_call.pop();
	}

	virtual void load_util_call_func(int *ptr_this)
	{
		int id = ptr_this[1];
		cls_jrunner_symbol_item* item = parser.sm.get_func(L"jrunner_main")->get_class_by_id(id);

		stk_function_call.push(itor);
		jump(item->addr.offset);

		stack_pointer = limit_stack/2;

		*(int*)(pseg_stack + stack_pointer) = (int)ptr_this;
	}

	virtual bool command_externed(const cls_cap_command& cmd)
	{
		if (cmd.op == cap_command_pointer_set)
		{
			if (cmd.operand[0].segment == cap_segment_code)
			{
				memcpy(get_pointer(cmd.result),&cmd.operand[0].offset,cmd.operand[0].length);
			}
			else
			{
				cls_cap_runner::command_pointer_set(cmd);
			}

			return true;
		}
		else if (cmd.op == cap_command_new)
		{
			if (cmd.operand[0].length != 0)
			{
				byte* ptr = alloc_new(cmd.operand[0].length + 20);
				int *pn = (int*)ptr;
				pn[0] = (int)pn;
				pn[1] = cmd.operand[0].offset;
				pn[2] = *(int*)(pseg_stack + stack_pointer);
				pn[3] = 0;
				pn[4] = 0;

				ptr += 20;

				int p = (int)ptr;
				set_memory((byte*)&p,cmd.result);
			}
			else
			{
				int length = get_externed_value(cmd.operand[1]);
				int* ptr = (int*)alloc_new(length * 4);
				int p = (int)ptr;

				ptr[-2] = 1;

				set_memory((byte*)&p,cmd.result);
			}

			return true;
		}
		else if (cmd.op == cap_command_memory_delete_ref)
		{
			if (cmd.result.segment == null)
				return true;

			int *pthis = (int*)*(int*)get_memory(cmd.result);
			cmd_delete((byte*)pthis);

			return true;
		}

		return false;
	}

	void cmd_delete(byte* ptr)
	{
		int* ptr_this = (int*) ptr;
		if (check_alloc(ptr))
		{
			ptr_this[-1]--;
			if (ptr_this[-1] > 0)
				return;

			if (ptr_this[-2] == 0)
			{
				cls_jrunner_symbol_item * item = parser.sm.get_func(L"jrunner_main")->get_class_by_id(ptr_this[1]);
				if (item == null)
				{
					alloc_delete(ptr);
					return;
				}

				int cnt = item->childs.size();

				for(int i=0;i<cnt;i++)
				{
					cmd_delete((byte*)ptr_this[5 + i]);
				}
			}
			
			alloc_delete(ptr);
		}
	}

	void command_load_file_text_write(const cls_cap_command& cmd)
	{
		wfstream* file = (wfstream*)*(int*)load_util_get_var(L"param_0");
		byte* pspace = (byte*)*(int*)load_util_get_var(L"param_1");
		int * num = load_util_get_var(L"param_2");
		
		file->write((wchar_t*)pspace,*num);
	}

	void command_load_file_text_read(const cls_cap_command& cmd)
	{
		wfstream* file = (wfstream*)*(int*)load_util_get_var(L"param_0");
		int * num = load_util_get_var(L"param_1");
		wchar_t* pspace = (wchar_t*)alloc_new(*num * sizeof(wchar_t));
		file->read((wchar_t*)pspace,*num * sizeof(wchar_t));

		load_util_return((byte*)pspace,*num * sizeof(wchar_t));
	}

	void command_load_file_text_close(const cls_cap_command& cmd)
	{
		wfstream* file = (wfstream*)*(int*)load_util_get_var(L"param_0");
		file->close();
		alloc_delete((byte*)file);
	}

	void command_load_file_text_open(const cls_cap_command& cmd)
	{
		wchar_t* ptr_filename = (wchar_t*)*(int*)load_util_get_var(L"param_0");
		void *file_space = (void*)alloc_new(sizeof(wfstream));
		wfstream* file = new(file_space) wfstream();
		file->open(ptr_filename);

		load_util_return((byte*)file,sizeof(wfstream));
	}

	void command_load_console_puts_endline(const cls_cap_command& cmd)
	{
		int* ptr_var = load_util_get_var(L"param_0");
		cout<<endl;
	}

	void command_load_console_puts_num(const cls_cap_command& cmd)
	{
		int* ptr_var = load_util_get_var(L"param_0");
		cout<<*ptr_var;
	}

	void command_load_console_gets_num(const cls_cap_command& cmd)
	{
		int *n = (int*)alloc_new(4);
		cin>>*n;

		load_util_return_direct((byte*)n);
	}

	void command_load_console_puts(const cls_cap_command& cmd)
	{
		command_load_alert(cmd);
	}

	void command_load_console_gets(const cls_cap_command& cmd)
	{
		wstring wstr;
		getline(wcin,wstr);

		load_util_return((byte*)wstr.c_str(),wstr.size() * 2 + 2);
	}

	void run_debug(const wstring filename)
	{
		parser.run(filename,true);
		make_const();
	
		int n = (int)alloc_new(parser.sm.get_func(L"jrunner_main")->get_size() + 4);			
		*(int*)pseg_stack = n;

		int *ptr_this = (int*)n;
		ptr_this[0] = n;
		ptr_this[1] = parser.sm.get_func(L"jrunner_main")->class_id;
		ptr_this[2] = null;

		cout<<"[Codes]"<<endl;
		parser.display();
		cout<<"[Outputs]"<<endl;
		cls_cap_runner::run();
		cout<<"[Over]"<<endl;
	}

	void run()
	{
		//duplicate of consts!
		make_const();

		int n = (int)alloc_new(parser.sm.get_func(L"jrunner_main")->get_size() + 20);			
		*(int*)(get_memory(make_cap_operand(cap_segment_stack,0,4))) = n;

		int *ptr_this = (int*)n;
		ptr_this[0] = n;
		ptr_this[1] = parser.sm.get_func(L"jrunner_main")->class_id;
		ptr_this[2] = null;
		ptr_this[3] = null;
		ptr_this[4] = null;

		cls_cap_runner::run();
	}

	int* load_util_get_this()
	{
		return  (int*)*((int*)get_memory(make_cap_operand(cap_segment_stack,0,4)));
	}

	int* load_util_get_super(int* pthis)
	{
		return (int*)(pthis[2]);
	}

	cls_jrunner_symbol_item* load_util_get_item(int *pthis,wstring str)
	{
		return parser.sm.get_func(L"jrunner_main")->get_class_by_id(pthis[1])->get_item(str);
	}

	cls_jrunner_symbol_item* load_util_get_func(int *pthis,wstring str)
	{
		return parser.sm.get_func(L"jrunner_main")->get_class_by_id(pthis[1])->get_item_func(str);
	}

	int* load_util_get_var(wstring str)
	{
		int* pthis = load_util_get_this();
		while(pthis != null)
		{
			cls_jrunner_symbol_item* item = load_util_get_item(pthis,str);
			if (item != null)
			{
				return (int*)((byte*)pthis + item->addr.offset);
			}

			pthis = load_util_get_super(pthis);
		}

		return null;
	}

	void load_util_return(byte* things,int size,bool b_obj = false)
	{
		int *p = (int*)alloc_new(4);
		p[-1] = 1;
		p[-2] = 1;

		*p = (int)alloc_new(size);
		((int*)(*p))[-1] = 1;

		if (b_obj == false)
		{
			((int*)(*p))[-2] = 1;

		}

		return_value = (byte*)p;

		memcpy((byte*)*(int*)p,things,size);

		itor = stk_function_call.top();
		stk_function_call.pop();
	}

	void load_util_return_direct(byte* things)
	{
		return_value = things;

		itor = stk_function_call.top();
		stk_function_call.pop();
	}

	void command_load_alert(const cls_cap_command& cmd)
	{
		int* ptr_this = (int*)*((int*)get_memory(make_cap_operand(cap_segment_stack,0,4)));
		wchar_t* w_begin = (wchar_t*)*(int*)(ptr_this + 5);
		wchar_t* w_end = w_begin;

		while(*w_end != null)
			w_end++;

		wstring wstr(w_begin,w_end);
		wcout<<wstr;
	}

	void command_load_exit(const cls_cap_command& cmd)
	{
		itor = cmds.end();
	}

	void command_load_wait(const cls_cap_command& cmd)
	{
		system("pause");
	}

	void command_load_alert_num(const cls_cap_command& cmd)
	{
		int* ptr_this = (int*)*((int*)get_memory(make_cap_operand(cap_segment_stack,0,4)));
		int w_begin = *(int*)(ptr_this + 5);

		cout<<w_begin;
	}

	virtual bool system_load_dynamic(const cls_cap_command& cmd)
	{
		return false;
	}

	virtual void command_load_dynamic( const cls_cap_command& cmd ) 
	{
		if (this->system_load_dynamic(cmd) == false)
			(this->*map_load[cmd.tag])(cmd);
	}

	virtual void make_const() 
	{
		map<wstring,cls_jrunner_symbol_item*>::iterator itor;
		for(itor = parser.sm.table_const.items.begin();
			  itor != parser.sm.table_const.items.end();
			  ++itor)
		{
			if (itor->second->b_str == false)
			{
				int things = get_type_from_wstring<int>(itor->first);
				set_memory_const((byte*)&things,itor->second->addr);
			}
			else
			{
				int things = (int)alloc_new((itor->first.size() + 2) * sizeof(wchar_t));
				int* space = (int*)things;
				space[-2] = 1;
				space[-1] = 100000;
				memcpy((byte*)things,itor->first.c_str(),(itor->first.size() + 1) * sizeof(wchar_t));
				set_memory_const((byte*)&things,itor->second->addr);
			}
		}
	}

	virtual void command_syscall( const cls_cap_command& cmd ) 
	{
		if (cmd.tag == L"dynamic_get_id_from_object")
		{
			int* ptr_this = (int*)(*(int*)(get_memory(cmd.operand[1])));
 			wstring str_id = get_string_from_const(cmd.operand[0]);
			byte* ptr_cur = null;

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item(str_id);

				if (item != null)
				{
					ptr_cur = ((byte*)ptr_this) + item->addr.offset; 
					break;
				}

				ptr_this = super;
			}

			if (ptr_cur != null)
			{
				int n = (int)ptr_cur;
				set_memory((byte*)&n,cmd.result);

				return;
			}
			
			ptr_this = (int*)(*(int*)(get_memory(cmd.operand[1])));

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item_func(str_id);

				if (item != null)
				{
					int n = item->class_id; 
					set_memory((byte*)&n,cmd.result);
					return;
				}

				ptr_this = super;
			}
		}
		else if (cmd.tag == L"dynamic_get_id_from_object_indirect")
		{
			int* ptr_this = (int*)*((int*)(*(int*)(get_memory(cmd.operand[1]))));
			wstring str_id = get_string_from_const(cmd.operand[0]);
			byte* ptr_cur = null;

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item(str_id);

				if (item != null)
				{
					ptr_cur = ((byte*)ptr_this) + item->addr.offset; 
					break;
				}

				ptr_this = super;
			}

			if (ptr_cur != null)
			{
				int n = (int)(ptr_cur);
				set_memory((byte*)&n,cmd.result);
				return;
			}
			
			ptr_this = (int*)*((int*)(*(int*)(get_memory(cmd.operand[1]))));

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item_func(str_id);

				if (item != null)
				{
					int n = item->class_id; 
					set_memory((byte*)&n,cmd.result);
					return;
				}

				ptr_this = super;
			}

			cout<<"[error]dynamic_get_id_from_object_indirect!"<<endl;
			exit(1);
		}
		else if (cmd.tag == L"dynamic_build_func")
		{
			int* ptr_this = (int*)*(int*)(*(int*)(get_memory(cmd.operand[1])));
			int str_id = *(int*)get_memory(cmd.operand[0]);
			byte* ptr_cur = null;

			cls_jrunner_symbol_item* item = parser.sm.get_func(L"jrunner_main")->get_class_by_id(str_id);

			byte* ptr = alloc_new(item->get_size() + 20);
			int *pn = (int*)ptr;
			pn[0] = (int)pn;
			pn[1] = item->class_id;
			pn[2] = (int)ptr_this;

			int p = (int)pn;

			set_memory((byte*)&p,cmd.result);
			cls_cap_addr addr = cmd.result;
			addr.offset += 4;

			p = item->addr.offset;
			set_memory((byte*)&p,addr);
		}
		else if (cmd.tag == L"dynamic_build_func_asfunc")
		{
			int* ptr_this = (int*)*(int*)(*(int*)(get_memory(cmd.operand[1])));
			int str_id = *(int*)*(int*)get_memory(cmd.operand[0]);
			byte* ptr_cur = null;

			cls_jrunner_symbol_item* item = parser.sm.get_func(L"jrunner_main")->get_class_by_id(str_id);

			byte* ptr = alloc_new(item->get_size() + 20);
			int *pn = (int*)ptr;
			pn[0] = (int)pn;
			pn[1] = item->class_id;
			pn[2] = (int)ptr_this;

			int p = (int)pn;

			set_memory((byte*)&p,cmd.result);
			cls_cap_addr addr = cmd.result;
			addr.offset += 4;

			p = item->addr.offset;
			set_memory((byte*)&p,addr);
		}
		else	 if (cmd.tag == L"dynamic_get_id_from_object_indirect_as_ar")
		{
			int* ptr_this = (int*)*(int*)*((int*)(*(int*)(get_memory(cmd.operand[1]))));
			wstring str_id = get_string_from_const(cmd.operand[0]);
			byte* ptr_cur = null;

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item(str_id);

				if (item != null)
				{
					ptr_cur = ((byte*)ptr_this) + item->addr.offset; 
					break;
				}

				ptr_this = super;
			}

			if (ptr_cur != null)
			{
				int n = (int)(ptr_cur);
				set_memory((byte*)&n,cmd.result);
				return;
			}

			ptr_this = (int*)*((int*)(*(int*)(get_memory(cmd.operand[1]))));

			while(ptr_this != null && ptr_cur == null)
			{
				int n_id = ptr_this[1];
				int* super = (int*)(ptr_this[2]);

				cls_jrunner_symbol_item* item_pa = parser.sm.get_func(L"jrunner_main")->get_class_by_id(n_id);
				if (item_pa == null)
				{
					ptr_this = super;
					continue;
				}

				cls_jrunner_symbol_item* item = item_pa->get_item_func(str_id);

				if (item != null)
				{
					int n = item->class_id; 
					set_memory((byte*)&n,cmd.result);
					return;
				}

				ptr_this = super;
			}

			cout<<"[error]dynamic_get_id_from_object_indirect_as_ar!"<<endl;
			exit(1);
		}
	}
};