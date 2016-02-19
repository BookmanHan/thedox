#include "cap_import.h"
#pragma  once

class cls_cap_thread
{
public:
	bool b_wait;
	unsigned tid;
	byte* pseg;
	unsigned stack_pointer;
	vector<cls_cap_command>::iterator itor;
	byte* return_value;
	stack<vector<cls_cap_command>::iterator> stk_function_call;
	vector<unsigned> tid_wait;
};

class cls_cap_sema
{
public:
	int num;
	list<int> sleep_tid;
};

class cls_cap_runner
	:public cls_cap_program
{
protected:

	typedef void (cls_cap_runner::*func_command)(const cls_cap_command&);
	map<unsigned int,func_command> map_runner;

protected:

	stack<vector<cls_cap_command>::iterator> stk_function_call;
	byte* return_value;

	unsigned int stack_pointer;

	unsigned int limit_stack;
	unsigned int limit_memory;

	unsigned int thread_cur;
	vector<cls_cap_thread> thread_pool;
	map<wstring,cls_cap_sema*> semaphores;

	byte* pseg_stack;
	byte* pseg_const;

	set<byte*> alloc;

	unsigned int time_slice;

public:
	
	virtual byte* get_stack_base(int tid)
	{
		if (get_num_for_tid(tid) != -1)
			return thread_pool[get_num_for_tid(tid)].pseg;
		else
			return null;
	}

	void open_sema(wstring wstr,int num)
	{
		semaphores[wstr] = new cls_cap_sema;
		semaphores[wstr]->num = num;
	}

	void delete_sema(wstring wstr)
	{
		if (semaphores.find(wstr) == semaphores.end())
			return;

		list<int>::iterator itor;
		for(itor = semaphores[wstr]->sleep_tid.begin();
			   itor != semaphores[wstr]->sleep_tid.end();
			   itor++)
		{
			int num = get_num_for_tid(*itor);
			if (num == -1)
				continue;

			thread_pool[num].b_wait = false; 
		}

		delete semaphores[wstr];
		semaphores.erase(wstr);
	}

	void wait_sema(wstring wstr)
	{
		if (semaphores.find(wstr) == semaphores.end())
			return;

		if (semaphores[wstr]->num <= 0)
		{
			thread_pool[get_num_for_tid(thread_cur)].b_wait  = true;
			semaphores[wstr]->sleep_tid.push_back(thread_cur);
		}
		else
		{
			semaphores[wstr]->num--;
		}

		switch_thread(get_next_thread_id());
	}

	void signal_sema(wstring wstr)
	{
		if (semaphores.find(wstr) == semaphores.end())
			return;

		semaphores[wstr]->num++;
		while (semaphores[wstr]->num > 0 && semaphores[wstr]->sleep_tid.size() > 0)
		{
			int tid = semaphores[wstr]->sleep_tid.front();
			semaphores[wstr]->sleep_tid.pop_front();
			int num = get_num_for_tid(tid);

			if (num == -1)
				continue;

			thread_pool[num].b_wait = false;
			semaphores[wstr]->num--;
		}

		switch_thread(get_next_thread_id());
	}

	unsigned get_tid()
	{
		static unsigned tid = 1;

		return tid++;
	}

	void set_timeslice(unsigned ts)
	{
		time_slice = ts;
	}

	virtual void delete_thread(int tid)
	{
		int cntid = get_num_for_tid(tid);

		for(int cnt=0;cnt<thread_pool[cntid].tid_wait.size();cnt++)
		{
			thread_pool[get_num_for_tid(thread_pool[cntid].tid_wait[cnt])].b_wait = false;
		}
		
		delete thread_pool[get_num_for_tid(tid)].pseg;
		thread_pool.erase(thread_pool.begin() + get_num_for_tid(tid));
	}

	virtual void add_thread(int cnt_cmd)
	{
		cls_cap_thread thd;
		thd.itor = cmds.begin() + cnt_cmd - 1;
		thd.pseg = new byte[limit_stack];
		thd.tid = get_tid();
		thd.stack_pointer = 0;
		thd.return_value = null;
		thd.b_wait = false;

		thread_pool.push_back(thd);
	}

	unsigned get_num_for_tid(int tid)
	{
		for(int cnt=0;cnt<thread_pool.size();cnt++)
		{
			if (thread_pool[cnt].tid == tid)
				return cnt;
		}

		return -1;
	}

	virtual void switch_thread(int tid)
	{
		if (tid == -1)
			return;

		int n_cur = get_num_for_tid(thread_cur);
		if (n_cur != -1)
		{
			thread_pool[n_cur].stack_pointer = stack_pointer;
			thread_pool[n_cur].itor = itor;
			thread_pool[n_cur].return_value = return_value;
			thread_pool[n_cur].stk_function_call = stk_function_call;		
		}

		int n_tid = get_num_for_tid(tid);
		if (n_tid == -1)
			return;

		itor = thread_pool[n_tid].itor ;
		pseg_stack = thread_pool[n_tid].pseg;
		stack_pointer = thread_pool[n_tid].stack_pointer;
		return_value = thread_pool[n_tid].return_value;
		stk_function_call = thread_pool[n_tid].stk_function_call;

		thread_cur = tid;
	}

	virtual byte* alloc_new(int length,bool b_self_return = false)
	{
		byte* ptr= new byte[length + 8];

		memset(ptr,0,length + 8);
		alloc.insert(ptr + 8);

		if (b_self_return)
		{
			((int*)ptr)[-1] = 1000;
		}

		return ptr + 8;
	}

	virtual bool check_alloc(byte* ptr)
	{
		return alloc.find(ptr) != alloc.end();
	}

	virtual void alloc_delete(byte *ptr)
	{
		alloc.erase(ptr);
		delete (ptr-8);
	}

	void push(byte* things,unsigned int length)
	{
		memcpy(pseg_stack + stack_pointer,things,length);
		stack_pointer += length;
	}

	void pop(byte* things,unsigned int length)
	{
		stack_pointer -= length;
		memcpy(pseg_stack + stack_pointer,things,length);
	}

	void set_memory_const(byte* things,const cls_cap_addr& addr)
	{
		if (addr.segment == cap_segment_const)
			memcpy(pseg_const+ addr.offset,things,addr.length);
	}

	void set_memory(byte* things,const cls_cap_addr& addr)
	{
		if (addr.segment == cap_segment_stack)
			memcpy(pseg_stack + stack_pointer + addr.offset,things,addr.length);
	}

	byte* get_memory(const cls_cap_addr& addr)
	{
		if (addr.segment == cap_segment_stack)
				return pseg_stack + stack_pointer + addr.offset;
		else if (addr.segment == cap_segment_const)
				return pseg_const + addr.offset;
	}

	byte* get_pointer(const cls_cap_addr& addr)
	{
		return (byte*)(*(int*)get_memory(addr));
	}

	virtual ~cls_cap_runner()
	{
		if (pseg_const != null)
		{
			//?WHY?
			//delete pseg_const;
			pseg_const = null;
		}
	}

	cls_cap_runner(vector<cls_cap_command>& vcmds,int size_seg_stack=1024*1024,int size_seg_memory = 10*1024*1024,int size_seg_const = 1024*1024)
		:cls_cap_program(vcmds)
	{
		time_slice = 100;

		pseg_const = new byte[size_seg_const];

		limit_memory = size_seg_memory;
		limit_stack = size_seg_stack;

		stack_pointer = 0;
		
		pseg_stack = new byte [size_seg_stack];

		thread_cur = 0;

		cls_cap_thread thd;
		thd.stack_pointer = stack_pointer;
		thd.pseg = pseg_stack;
		thd.itor = cmds.begin();
		thd.tid = 0;
		thd.b_wait = false;

		thread_pool.push_back(thd);

		memset(pseg_const,0,size_seg_const);
		memset(pseg_stack,0,size_seg_stack);

		map_runner[cap_command_nop] = &cls_cap_runner::command_nop;
		map_runner[cap_command_set] = &cls_cap_runner::command_set;
		map_runner[cap_command_get] = &cls_cap_runner::command_get;
		map_runner[cap_command_pointer_set] = &cls_cap_runner::command_pointer_set;
		map_runner[cap_command_pointer_get] = &cls_cap_runner::command_pointer_get;
		map_runner[cap_command_add] = &cls_cap_runner::command_add;
		map_runner[cap_command_minus] = &cls_cap_runner::command_minus;
		map_runner[cap_command_mutiply] = &cls_cap_runner::command_mutiply;
		map_runner[cap_command_divide] = &cls_cap_runner::command_divide;
		map_runner[cap_command_mod] = &cls_cap_runner::command_mod;
		map_runner[cap_command_or] = &cls_cap_runner::command_or;
		map_runner[cap_command_and] = &cls_cap_runner::command_and;
		map_runner[cap_command_xor] = &cls_cap_runner::command_xor;
		map_runner[cap_command_inc] = &cls_cap_runner::command_inc;
		map_runner[cap_command_dec] = &cls_cap_runner::command_dec;
		map_runner[cap_command_neg] = &cls_cap_runner::command_neg;
		map_runner[cap_command_pointer_real_add] = &cls_cap_runner::command_pointer_real_add;
		map_runner[cap_command_left] = &cls_cap_runner::command_left;
		map_runner[cap_command_right] = &cls_cap_runner::command_right;
		map_runner[cap_command_roleft] = &cls_cap_runner::command_roleft;
		map_runner[cap_command_roright] = &cls_cap_runner::command_roright;
		map_runner[cap_command_larger] = &cls_cap_runner::command_larger;
		map_runner[cap_command_larger_eql] = &cls_cap_runner::command_larger_eql;
		map_runner[cap_command_smaller] = &cls_cap_runner::command_smaller;
		map_runner[cap_command_smaller_eql] = &cls_cap_runner::command_smaller_eql;
		map_runner[cap_command_equ] = &cls_cap_runner::command_equ;
		map_runner[cap_command_non_equ] = &cls_cap_runner::command_non_equ;
		map_runner[cap_command_assign_set] = &cls_cap_runner::command_assign_set;
		map_runner[cap_command_not] = &cls_cap_runner::command_not;
		map_runner[cap_command_jump] = &cls_cap_runner::command_jump;
		map_runner[cap_command_abs_jump] = &cls_cap_runner::command_abs_jump;
		map_runner[cap_command_push] = &cls_cap_runner::command_push;
		map_runner[cap_command_pop] = &cls_cap_runner::command_pop;
		map_runner[cap_command_call] = &cls_cap_runner::command_call;
		map_runner[cap_command_pre_call] = &cls_cap_runner::command_pre_call;
		map_runner[cap_command_post_call] = &cls_cap_runner::command_post_call;
		map_runner[cap_command_return] = &cls_cap_runner::command_return;
		map_runner[cap_command_new] = &cls_cap_runner::command_new;
		map_runner[cap_command_memory_add_ref] = &cls_cap_runner::command_memory_add_ref;
		map_runner[cap_command_memory_delete_ref] = &cls_cap_runner::command_memory_delete_ref;
		map_runner[cap_command_delete] = &cls_cap_runner::command_delete;
		map_runner[cap_command_syscall] = &cls_cap_runner::command_syscall;
		map_runner[cap_command_load_dynamic] = &cls_cap_runner::command_load_dynamic;
		map_runner[cap_command_set_address] = &cls_cap_runner::command_set_address;
	}

	void command_nop(const cls_cap_command& cmd)
	{
		return;
	}

	void command_set_address(const cls_cap_command& cmd)
	{
		int n = (int) get_memory(cmd.operand[0]);
		set_memory((byte*)&n,cmd.result);
	}

	void command_set(const cls_cap_command& cmd)
	{
		set_memory(get_memory(cmd.operand[0]),cmd.result);
	}

	void command_get(const cls_cap_command& cmd)
	{
		set_memory(get_pointer(cmd.operand[0]),cmd.result);
	}

	void command_pointer_set(const cls_cap_command& cmd)
	{
		memcpy(get_pointer(cmd.result),get_memory(cmd.operand[0]),cmd.operand[0].length);
	}

	void command_pointer_get(const cls_cap_command& cmd)
	{
		memcpy(get_pointer(cmd.result),get_pointer(cmd.operand[0]),cmd.result.length);
	}

	int get_externed_value(const cls_cap_addr& addr)
	{
		int re = 0;
		switch(addr.length)
		{
		case 1:
			{
				char a = *((char*)get_memory(addr));
				re = a;
			}
			break;

		case 2:
			{
				short a = *((short*)get_memory(addr));
				re = a;
			}
			break;

		case 4:
			{
				int a = *((int*)get_memory(addr));
				re = a;
			}
			break;
		}

		return re;
	}

	void command_add (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) + get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_minus (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) - get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_mutiply (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) * get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_divide (const cls_cap_command& cmd)
	{
		int re = get_externed_value(cmd.operand[0]) / get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_mod (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) % get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_or (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) | get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_and (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) & get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_xor (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) ^ get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_inc (const cls_cap_command& cmd) 
	{ 
		int re = get_externed_value(cmd.result);
		++re;
		set_memory((byte*)&re,cmd.result);
	}

	void command_dec (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.result);
		--re;
		set_memory((byte*)&re,cmd.result);
	}

	void command_neg (const cls_cap_command& cmd) 
	{
		int re = - get_externed_value(cmd.result);
		set_memory((byte*)&re,cmd.result);
	}

	void command_pointer_real_add (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]);
		re += cmd.operand[2].offset;

		set_memory((byte*)&re,cmd.result);
	}

	void command_left (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) << get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_right (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) >> get_externed_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_roleft (const cls_cap_command& cmd) 
	{
		command_left(cmd);
	}

	void command_roright (const cls_cap_command& cmd) 
	{
		command_right(cmd);
	}

	void command_larger (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) > get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_larger_eql (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) >= get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_smaller (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) < get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_smaller_eql (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) <= get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_equ (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) == get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_non_equ (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) != get_externed_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_assign_set (const cls_cap_command& cmd) 
	{
		return;
	}

	void command_not (const cls_cap_command& cmd) 
	{
		int re = get_externed_value(cmd.operand[0]) == 0?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_jump (const cls_cap_command& cmd) 
	{ 
		jump(cmd.operand[get_externed_value(cmd.result)].offset);
	}

	void command_abs_jump (const cls_cap_command& cmd) 
	{ 
		jump(cmd.result.offset);
	}

	void command_push (const cls_cap_command& cmd) 
	{ 
		push(get_memory(cmd.result),cmd.result.length);
	}

	void command_pop (const cls_cap_command& cmd)
	{
		pop(get_memory(cmd.result),cmd.result.length);
	}

	void command_call (const cls_cap_command& cmd) 
	{
		if (cmd.result.segment == cap_segment_code)
		{
			stk_function_call.push(itor);
			jump(cmd.result.offset);
		}
		else
		{
			int n = *(int*)get_memory(cmd.result);

			stk_function_call.push(itor);
			jump(n);
		}
	}

	void command_pre_call (const cls_cap_command& cmd)
	{
		stack_pointer += cmd.result.offset;
	}

	void command_post_call (const cls_cap_command& cmd)
	{ 	
		if (cmd.operand[0].segment != null) 
		{
			set_memory(return_value,cmd.operand[0]);
			if (check_alloc((byte*)*(int*)return_value))
			{
				int* nrv = (int*)*(int*)return_value;
				if (nrv[-2] == 0)
				{
					nrv[2] = *(int*)get_memory(make_cap_operand(cap_segment_stack,-cmd.result.offset,4));
				}
			}
		}

		stack_pointer -= cmd.result.offset;
	}

	void command_return (const cls_cap_command& cmd)
	{
		return_value = get_memory(cmd.result);

		if (stk_function_call.size() == 0)
		{
			if (thread_cur == 0)
			{
				delete_thread(0);
				itor = cmds.end();
				return;
			}

			byte* ptr = (byte*)(*(int*)thread_pool[get_num_for_tid(thread_cur)].pseg);
			alloc_delete(ptr);

			int pos = get_num_for_tid(thread_cur);
			for(int cnt=0;cnt<thread_pool[pos].tid_wait.size();cnt++)
			{
				int tid = get_num_for_tid(thread_pool[pos].tid_wait[cnt]);
				if (tid != -1)
					thread_pool[tid].b_wait = false;
			}

			delete_thread(thread_cur);
			switch_thread(get_next_thread_id());
			return;
		}

		itor = stk_function_call.top();
		stk_function_call.pop();
	}

	void command_get_retrun (const cls_cap_command& cmd) 
	{
		return;
	}

	void command_new(const cls_cap_command& cmd)
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

	void command_delete(const cls_cap_command& cmd)
	{
		byte* ptr = (byte*)(*((int*)get_memory(cmd.result)));
		ptr += 20;

		alloc_delete(ptr);
	}

	void command_memory_add_ref(const cls_cap_command& cmd)
	{
		int *pthis = (int*)*(int*)get_memory(cmd.result);

		if (check_alloc((byte*)pthis))
		{
			pthis[-1]++;
		}

		return ;
	}

	void command_memory_delete_ref(const cls_cap_command& cmd)
	{
		int *pthis = (int*)*get_memory(cmd.result);
		if (check_alloc((byte*)pthis))
		{
			pthis[3]--;

			if (pthis[3] <= 0)
			{
				alloc_delete((byte*)pthis);
			}

			return ;
		}
	}

	virtual void make_const() = 0;
	virtual void command_syscall(const cls_cap_command& cmd) = 0;
	virtual void command_load_dynamic(const cls_cap_command& cmd) = 0;
	virtual bool command_externed(const cls_cap_command& cmd) = 0;

	virtual unsigned get_next_thread_id()
	{
		int cnt = get_num_for_tid(thread_cur);
		if (cnt == -1)
			cnt = 0;

		int cnt_pre = cnt;

		do
		{
			cnt = (cnt+1)%thread_pool.size();
			if (thread_pool[cnt].b_wait == false)
				break;
		}
		while(cnt != cnt_pre);

		if (thread_pool[cnt].b_wait == false)
			return thread_pool[cnt].tid;

		if (cnt == cnt_pre)
		{
			if (thread_pool.size() != 1)
			{
				delete_thread(0);
				itor = cmds.end();
				return -1;
			}
			else
			{
				thread_pool[0].b_wait = false;
			}
		}

		return thread_pool[cnt].tid;
	}

	virtual wstring get_string_from_const(const cls_cap_addr& addr)
	{
		wchar_t* w_beg =*((wchar_t**)(pseg_const + addr.offset));
		wchar_t* w_end =w_beg;

		while(*w_end != null)
			w_end++;

		return wstring(w_beg,w_end);
	}

	virtual void run_command(const cls_cap_command& cmd)
	{
		static unsigned int count_cmds = 0;

		if (command_externed(cmd) == false)
			(this->*map_runner[cmd.op])(cmd);

		if (time_slice != 0 && (++count_cmds)%(time_slice) == 0)
		{
			count_cmds = 0;
			switch_thread(get_next_thread_id());
		}
	}
};