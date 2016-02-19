#include "cdox_import.h"

class cls_cdox_addr
{
public:
	unsigned int segment;
	int offset;
	unsigned int length;

public:
	cls_cdox_addr()
	{
		segment = 0;
		offset = 0;
		length = 0;
	}
};

inline
cls_cdox_addr make_cdox_addr(unsigned segment,int offset,int length)
{
	cls_cdox_addr addr;
	addr.segment = segment;
	addr.length = length;
	addr.offset = offset;

	return addr;
}

class cls_cdox_command
{
public:
	wstring tag;
	unsigned op;
	cls_cdox_addr operand[2];
	cls_cdox_addr result;
};

inline
cls_cdox_command make_cdox_command(unsigned op,cls_cdox_addr oprand_0,cls_cdox_addr oprand_1,cls_cdox_addr result)
{
	cls_cdox_command cmd;
	cmd.op = op;
	cmd.operand[0] = oprand_0;
	cmd.operand[1] = oprand_1;
	cmd.result = result;

	return cmd;
}

inline
cls_cdox_command make_cdox_command(unsigned op,cls_cdox_addr oprand_0,cls_cdox_addr result)
{
	cls_cdox_command cmd;
	cmd.op = op;
	cmd.operand[0] = oprand_0;
	cmd.result = result;

	return cmd;
}

inline
cls_cdox_command make_cdox_command(unsigned op,cls_cdox_addr result)
{
	cls_cdox_command cmd;
	cmd.op = op;
	cmd.result = result;

	return cmd;
}

inline
cls_cdox_command make_cdox_command(unsigned op)
{
	cls_cdox_command cmd;
	cmd.op = op;

	return cmd;
}

class cls_cdox_thread
{
public:
	byte* pseg_stack;
	byte* return_value;
	unsigned int stack_point;

	unsigned short tid;
	bool b_wait;

	vector<cls_cdox_command>::iterator itor;
	stack<vector<cls_cdox_command>::iterator> stack_call_pointers;

	list<cls_cdox_thread*> wait_tid; 
};

class cls_cdox_sema
{
public:
	int num;
	list<cls_cdox_thread*> sleep_tid;
};

class cls_cdox_cache
{
protected:
	set<void*> alloc;

public:
	void* alloc_new(int length)
	{
		byte* p = new byte[length + 4];
		memset(p,0,length+4);
		alloc.insert(p + 4);

		return (void*)(p+4);
	}

	bool check_alloc(void* p)
	{
		return alloc.find(p) != alloc.end();
	}

	void alloc_return(void* p)
	{
		if (check_alloc(p))
		{
			delete (p-4);
		}
	}
};