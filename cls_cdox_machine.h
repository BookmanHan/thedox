#include "cdox_import.h"
/*
	THE CDOX VIRTUAL MACHINE;
*/
class cls_cdox_machine
{
public:
	typedef void (cls_cdox_machine::*func_command)(const cls_cdox_command&);
	//the acc function table;
	func_command funcs[500];

protected:
	//the current thread;
	cls_cdox_thread* thread_now;
	//the pool of threads;
	vector<cls_cdox_thread*> threads;
	//the pool of semas;
	map<wstring,cls_cdox_sema> semaphores;

	//the segment of consts;
	byte* pseg_const;
	//the cache system of cdox;
	cls_cdox_cache memory;

	//the time for thread_switch;
	unsigned int time_slice;

	//the commands to execute;
	vector<cls_cdox_command>& cmds;

	//check if the machine is runing;
	bool b_running;

protected:
	unsigned get_tid()
	{
		static unsigned int tid = 0;
		return tid++:
	}

	void set_timeslice(unsigned ts)
	{
		time_slice = ts;
	}

public:
	virtual void jump(int cmd_line_no)
	{
		thread_now->itor = cmds.begin() + (cmd_line_no - 1);
	}

public:
	//open the sema with its name and num;
	virtual void open_sema(wstring wstr,int num)
	{
		semaphores[wstr].num = num;
	}

	//close the sema with its name;
	virtual void close_sema(wstring& wstr,int num)
	{
		if (semaphores.find(wstr) == semaphores.end())
		{
			return;
		}

		list<cls_cdox_thread*>::iterator itor = semaphores[wstr].sleep_tid.begin();
		list<cls_cdox_thread*>::iterator itor_end = semaphores[wstr].sleep_tid.end();
		for(;itor!=itor_end;itor++)
		{
			if (find(threads.begin(),threads.end(),itor) == threads.end())
			{
				continue;
			}

			itor->b_wait = false;
		}

		semaphores.erase(wstr);

		switch_thread(get_next_thread_id());
	}

	//wait the sema with its name;
	virtual wait_sema(wstring wstr)
	{
		map<wstring,cls_cdox_sema>::iterator itor_sema;
		itor_sema = semaphores.find(wstr);

		if (itor_sema == semaphores.end())
			return;

		if (itor_sema->second.num <= 0)
		{
			thread_now->b_wait = true;
			itor_sema->second.sleep_tid.push_back(thread_now);
		}
		else
		{
			itor_sema->second.num--;
		}

		switch_thread(get_next_thread_id());
	}

	//signal the sema with its name;
	virtual void signal_sema(wstring wstr)
	{
		map<wstring,cls_cdox_sema>::iterator itor_sema;
		itor_sema = semaphores.find(wstr);

		if (itor_sema == semaphores.end())
			return;

		itor_sema->second.num++;
		while(itor_sema->second.num >0 && itor_sema->second.sleep_tid.size() > 0)
		{
			if (find(threads.begin(),threads.end(),itor_sema->second.sleep_tid.front()) == threads.end())
			{
				itor_sema->second.sleep_tid.pop_front();
				continue;
			}

			itor_sema->second.sleep_tid.front().b_wait = false;
			itor_sema->second.sleep_tid.pop_front();
			itor_sema->second.num--;
		}

		switch_thread(get_next_thread_id());
	}

public:
	//the corresponding num for tid;
	int get_num_for_tid(int tid)
	{
		vector<cls_cdox_thread*>::iterator itor;
		for(itor=threads.begin();itor!=threads.end();itor++)
		{
			if (itor->tid == tid)
				return (itor - threads.begin());
		}

		return -1;
	}

	//add a thread for the machine.
	virtual void add_thread(int cnt_cmd_line)
	{
		cls_cdox_thread* thd = new cls_cdox_thread;
		thd->itor = cmds.begin() + cnt_cmd_line;
		thd->pseg_stack = new byte[limit_segment_stack];
		thd->tid = get_tid();
		thd->stack_point = 0;
		thd->return_value = null;
		thd->b_wait = false;

		threads.push_back(thd);
	}

	//delete the thread of the tid;
	virtual void delete_thread(int tid)
	{
		int num = get_num_for_tid(tid);
		if (num == -1)
			return;

		cls_cdox_thread* thd = threads[num];
		list<cls_cdox_thread*>::iterator itor;
		for(itor=thd->wait_tid.begin();itor != thd->wait_tid.end();itor++)
		{
			if (find(threads.begin(),threads.end(),itor) !=  threads.end())
				itor->b_wait = false;
		}

		delete thd;
		threads.erase(thd);
	}

	virtual void switch_thread(cls_cdox_thread* thd)
	{
		if (thd == null)
			return;
		
		thread_now = thd;
	}

	virtual cls_cdox_thread* get_next_thread_id()
	{
		int cnt = get_num_for_tid(thread_now);
		if (cnt == -1)
			cnt = 0;

		int cnt_pre = cnt;

		do
		{
			cnt = (cnt+1)%threads.size();
			if (threads[cnt]->b_wait == false)
				break;
		}
		while(cnt != cnt_pre);

		if (threads[cnt]->b_wait == false)
			return threads[cnt];

		if (cnt == cnt_pre)
		{
			if (threads.size() != 1)
			{
				delete_thread(0);
				b_running = false;
				return -1;
			}
			else
			{
				threads[0]->b_wait = false;
				return threads[0];
			}
		}

		return threads[cnt];
	}

public:
	virtual byte* alloc_new(int length,bool b_self_return)
	{
		int* ptr = (int*)memory.alloc_new(length);
		if (b_self_return)	
			ptr[-1] = 2;

		return ptr;
	}

	virtual bool check_alloc(byte* ptr)
	{
		return memory.check_alloc(ptr);
	}

	virtual void alloc_delete(byte* ptr)
	{
		memory.alloc_return(ptr);
	}

public:
	void push(byte* things,unsigned length)
	{
		memcpy(thread_now->pseg_stack + thread_now->stack_point,things,length);
		thread_now->stack_point += length;
	}

	void pop(byte* things,unsigned length)
	{
		thread_now->stack_point -= length;
		memcpy(things,thread_now->pseg_stack + thread_now->stack_point,length);
	}

	void set_memory(byte* things,const cls_cdox_addr& addr)
	{
		if (addr.segment == cdox_segment_const)
		{
			memcpy(pseg_const + addr.offset,things,addr,length);
		}
		else if (addr.segment == cdox_segment_stack)
		{
			memcpy(thread_now->pseg_stack + thread_now->stack_point + addr.offset,things,addr.length);
		}
	}

	byte* get_memory(const cls_cdox_addr& addr)
	{
		if (addr.segment == cdox_segment_const)
		{
			return pseg_const + addr.offset;
		}
		else if (addr.segment == cdox_segment_stack)
		{
			return thread_now->pseg_stack + thread_now->stack_point + addr.offset;
		}
	}

	byte* get_pointer(const cls_cdox_addr& addr)
	{
		return (byte*)(*(int*)get_memory(addr));
	}

	int get_integer_value(const cls_cdox_addr& addr)
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

	unsigned int get_unsigned_value(const cls_cdox_addr& addr)
	{
		int re = 0;
		switch(addr.length)
		{
		case 1:
			{
				unsigned char a = *((unsigned char*)get_memory(addr));
				re = a;
			}
			break;

		case 2:
			{
				unsigned short a = *((unsigned short*)get_memory(addr));
				re = a;
			}
			break;

		case 4:
			{
				unsigned int a = *((unsigned int*)get_memory(addr));
				re = a;
			}
			break;
		}

		return re;
	}

	double get_float_value(const cls_cdox_addr& addr)
	{
		switch(addr.length)
		{
		case 4:
			return *(float*)get_memory(addr);
		case 8:
			return *(float*)get_memory(addr);
		}

		return 0.0;
	}

public:
	cls_cdox_machine(vector<cls_cdox_command>& a_cmds,byte* a_seg_const)
		:cmds(a_cmds),pseg_const(a_seg_const)
	{
		add_thread(0);
		switch_thread(get_next_thread_id());

		memset((byte*)funcs,0,2000);

		funcs[ cdox_command_nop ] = &cls_cdox_machine::command_nop;
		funcs[ cdox_command_set ] = &cls_cdox_machine::command_set;
		funcs[ cdox_command_get ] = &cls_cdox_machine::command_get;
		funcs[ cdox_command_pointer_set ] = &cls_cdox_machine::command_pointer_set;
		funcs[ cdox_command_pointer_get ] = &cls_cdox_machine::command_pointer_get;
		funcs[ cdox_command_set_address ] = &cls_cdox_machine::command_set_address;
		funcs[ cdox_command_add ] = &cls_cdox_machine::command_add;
		funcs[ cdox_command_minus ] = &cls_cdox_machine::command_minus;
		funcs[ cdox_command_mutiply ] = &cls_cdox_machine::command_mutiply;
		funcs[ cdox_command_divide ] = &cls_cdox_machine::command_divide;
		funcs[ cdox_command_unsigned_mutiply ] = &cls_cdox_machine::command_unsigned_mutiply;
		funcs[ cdox_command_unsigned_divide ] = &cls_cdox_machine::command_unsigned_divide;
		funcs[ cdox_command_mod ] = &cls_cdox_machine::command_mod;
		funcs[ cdox_command_or ] = &cls_cdox_machine::command_or;
		funcs[ cdox_command_and ] = &cls_cdox_machine::command_and;
		funcs[ cdox_command_xor ] = &cls_cdox_machine::command_xor;
		funcs[ cdox_command_inc ] = &cls_cdox_machine::command_inc;
		funcs[ cdox_command_dec ] = &cls_cdox_machine::command_dec;
		funcs[ cdox_command_neg ] = &cls_cdox_machine::command_neg;
		funcs[ cdox_command_left ] = &cls_cdox_machine::command_left;
		funcs[ cdox_command_right ] = &cls_cdox_machine::command_right;
		funcs[ cdox_command_roleft ] = &cls_cdox_machine::command_roleft;
		funcs[ cdox_command_roright ] = &cls_cdox_machine::command_roright;
		funcs[ cdox_command_larger ] = &cls_cdox_machine::command_larger;
		funcs[ cdox_command_larger_eql ] = &cls_cdox_machine::command_larger_eql;
		funcs[ cdox_command_smaller ] = &cls_cdox_machine::command_smaller;
		funcs[ cdox_command_smaller_eql ] = &cls_cdox_machine::command_smaller_eql;
		funcs[ cdox_command_equ ] = &cls_cdox_machine::command_equ;
		funcs[ cdox_command_non_equ ] = &cls_cdox_machine::command_non_equ;
		funcs[ cdox_command_not ] = &cls_cdox_machine::command_not;
		funcs[ cdox_command_jump ] = &cls_cdox_machine::command_jump;
		funcs[ cdox_command_abs_jump ] = &cls_cdox_machine::command_abs_jump;
		funcs[ cdox_command_push ] = &cls_cdox_machine::command_push;
		funcs[ cdox_command_pop ] = &cls_cdox_machine::command_pop;
		funcs[ cdox_command_call ] = &cls_cdox_machine::command_call;
		funcs[ cdox_command_pre_call ] = &cls_cdox_machine::command_pre_call;
		funcs[ cdox_command_post_call ] = &cls_cdox_machine::command_post_call;
		funcs[ cdox_command_return ] = &cls_cdox_machine::command_return;
		funcs[ cdox_command_get_retrun ] = &cls_cdox_machine::command_get_return;
		funcs[ cdox_command_float_add ] = &cls_cdox_machine::command_float_add;
		funcs[ cdox_command_float_minus ] = &cls_cdox_machine::command_float_minus;
		funcs[ cdox_command_float_mutiply ] = &cls_cdox_machine::command_float_mutiply;
		funcs[ cdox_command_float_divide ] = &cls_cdox_machine::command_float_divide;

		b_running = true;
	}

public:
	void command_nop(const cls_cdox_command& cmd)
	{
		return;
	}

	void command_set_address(const cls_cdox_command& cmd)
	{
		int n = (int) get_memory(cmd.operand[0]);
		set_memory((byte*)&n,cmd.result);
	}

	void command_set(const cls_cdox_command& cmd)
	{
		set_memory(get_memory(cmd.operand[0]),cmd.result);
	}

	void command_get(const cls_cdox_command& cmd)
	{
		set_memory(get_pointer(cmd.operand[0]),cmd.result);
	}

	void command_pointer_set(const cls_cdox_command& cmd)
	{
		memcpy(get_pointer(cmd.result),get_memory(cmd.operand[0]),cmd.operand[0].length);
	}

	void command_pointer_get(const cls_cdox_command& cmd)
	{
		memcpy(get_pointer(cmd.result),get_pointer(cmd.operand[0]),cmd.result.length);
	}

	void command_add(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) + get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_float_add(const cls_cdox_command& cmd)
	{
		int re = get_float_value(cmd.operand[0]) + get_float_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_minus(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) - get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_float_minus(const cls_cdox_command& cmd)
	{
		int re = get_float_value(cmd.operand[0]) - get_float_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_unsigned_mutiply(const cls_cdox_command& cmd)
	{
		int re = get_unsigned_value(cmd.operand[0]) * get_unsigned_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_unsigned_divide(const cls_cdox_command& cmd)
	{
		int re = get_unsigned_value(cmd.operand[0]) / get_unsigned_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_mutiply(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) * get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_divide(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) / get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_float_mutiply(const cls_cdox_command& cmd)
	{
		int re = get_float_value(cmd.operand[0]) * get_float_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_float_divide(const cls_cdox_command& cmd)
	{
		int re = get_float_value(cmd.operand[0]) / get_float_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_mod(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) % get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_or(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) | get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_and(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) & get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_xor(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) ^ get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_inc(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.result);
		re++;
		set_memory((byte*)&re,cmd.result);
	}

	void command_dec(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.result);
		re--;
		set_memory((byte*)&re,cmd.result);
	}

	void command_neg(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.result);
		re = - re;
		set_memory((byte*)&re,cmd.result);
	}

	void command_left(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) << get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_right(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) >> get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_roleft(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) << get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_roright(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) >> get_integer_value(cmd.operand[1]);
		set_memory((byte*)&re,cmd.result);
	}

	void command_larger(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) > get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_larger_eql(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) >= get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_smaller(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) < get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_smaller_eql(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) <= get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_equ(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) == get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_non_equ(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.operand[0]) != get_integer_value(cmd.operand[1])?1:0;
		set_memory((byte*)&re,cmd.result);
	}

	void command_non_equ(const cls_cdox_command& cmd)
	{
		int re = !get_integer_value(cmd.result);
		set_memory((byte*)&re,cmd.result);
	}

	void command_jump(const cls_cdox_command& cmd)
	{
		int re = get_integer_value(cmd.result);
		re &= 0x00000001;
		jump(cmd.operand[re].offset);
	}

	void command_abs_jump(const cls_cdox_command& cmd)
	{
		jump(cmd.result.offset);
	}

	void command_push(const cls_cdox_command& cmd)
	{
		push(cmd.result);
	}

	void command_pop(const cls_cdox_command& cmd)
	{
		pop(cmd.result);
	}

	void command_call(const cls_cdox_command& cmd)
	{
		jump(cmd.result.offset);
	}

	void command_pre_call(const cls_cdox_command& cmd)
	{
		thread_now->stack_point += cmd.result.offset;
	}

	void command_post_call(const cls_cdox_command& cmd)
	{
		if (cmd.operand[0].segment != null)
		{
			set_memory(thread_now->return_value,cmd.operand[0]);
		}

		if (thread_now->return_value != null)
		{
			alloc_delete(thread_now->return_value);
			thread_now->return_value = null;
		}
		thread_now->stack_point -= cmd.result.offset;
	}

	void command_return(const cls_cdox_command& cmd)
	{
		if (cmd.result.segment == null)
		{
			thread_now->return_value = null;
		}
		else
		{
			byte* space = alloc_new(cmd.result.length);
			memcpy(space,get_memory(cmd.result),cmd.result.length);
		}
	}

	void command_get_return(const cls_cdox_command& cmd)
	{
		return;
	}

	void start()
	{
		b_running = true;
	}
	
	void stop()
	{
		b_running = false;
	}

	virtual void command_externs(const cls_cdox_command& cmd) = 0;

	void run()
	{
		static unsigned int counts = 0;
		while(b_running)
		{
			if (funcs[thread_now->itor->op] != null)
				funcs[thread_now->itor->op](*(thread_now->itor));
			else
				command_externs(*(thread_now->itor));

			thread_now->itor ++;

			if ((++counts)%time_slice == 0)
			{
				switch_thread(get_next_thread_id());
			}
		}
	}
};