#include "cap_import.h"
#pragma  once

#ifndef	CAP_COMMAND
#define  CAP_COMMAND

struct cls_cap_addr
{
	unsigned short segment;
	int offset;
	unsigned short length;

	cls_cap_addr()
	{
		segment = 0;
		offset = 0;
		length = 0;
	}
};

class cls_cap_command
{
public:
	wstring tag;
	unsigned int op;
	cls_cap_addr operand[2],result;

	void save(ofstream& fout)
	{
		int len = tag.size();
		fout.write((char*)&len,sizeof(int));
		fout.write((char*)tag.c_str(),len*sizeof(wchar_t));
		fout.write((char*)&op,sizeof(op));
		fout.write((char*)&operand[0],sizeof(cls_cap_addr));
		fout.write((char*)&operand[1],sizeof(cls_cap_addr));
		fout.write((char*)&result,sizeof(cls_cap_addr));
	}

	void load(ifstream& fout)
	{
		int len;
		fout.read((char*)&len,sizeof(int));
		wchar_t* p =new wchar_t[len+1];
		fout.read((char*)p,len*sizeof(wchar_t));
		p[len] = 0;
		tag = wstring(p);
		delete[] p;

		fout.read((char*)&op,sizeof(op));
		fout.read((char*)&operand[0],sizeof(cls_cap_addr));
		fout.read((char*)&operand[1],sizeof(cls_cap_addr));
		fout.read((char*)&result,sizeof(cls_cap_addr));
	}

	void display()
	{
		cout<<op<<"\t";
		cout<<operand[0].segment<<":"<<operand[0].offset<<" ["<<operand[0].length<<"]\t";
		cout<<operand[1].segment<<":"<<operand[1].offset<<" ["<<operand[1].length<<"]\t";
		cout<<result.segment<<":"<<result.offset<<" ["<<result.length<<"]\t";

		if (tag != L"")
			wcout<<tag;

		cout<<endl;
	}
};

inline
cls_cap_addr make_cap_operand(unsigned short segment,int offset,unsigned short length)
{
	cls_cap_addr to;
	to.segment = segment;
	to.offset = offset;
	to.length = length;

	return to;
}

inline
cls_cap_command make_cap_command
	(	unsigned int op,
		cls_cap_addr operand_1,
		cls_cap_addr operand_2,
		cls_cap_addr operand_re)
{
	cls_cap_command cmd;
	cmd.op = op;
	cmd.operand[0] = operand_1;
	cmd.operand[1] = operand_2;
	cmd.result = operand_re;

	return cmd;
}

inline
cls_cap_command make_cap_command
(	unsigned int op,
	cls_cap_addr operand_re)
{
	cls_cap_command cmd;
	cmd.op = op;
	cmd.result = operand_re;

	return cmd;
}

inline
cls_cap_command make_cap_command
(	unsigned int op,
	cls_cap_addr operand_1,
	cls_cap_addr operand_re)
{
	cls_cap_command cmd;
	cmd.op = op;
	cmd.operand[0] = operand_1;
	cmd.result = operand_re;

	return cmd;
}

inline
cls_cap_command make_cap_command
(	unsigned int op)
{
	cls_cap_command cmd;
	cmd.op = op;

	return cmd;
}

#endif