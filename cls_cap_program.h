#include "cap_import.h"
#pragma once

class cls_cap_program
{
public:
	vector<cls_cap_command>& cmds;
	vector<pair<wstring,cls_cap_addr> > consts;

public:

	cls_cap_program(vector<cls_cap_command>& vcmds):cmds(vcmds)
	{
		;
	}

	virtual void save(ofstream& fout)
	{		
		int n = cmds.size();
		fout.write((char*)&n,sizeof(int));

		vector<cls_cap_command>::iterator itor;
		for(itor=cmds.begin();itor!=cmds.end();itor++)
		{
			itor->save(fout);
		}

		n = consts.size();
		fout.write((char*)&n,sizeof(int));
		vector<pair<wstring,cls_cap_addr> >::iterator itorp;
		for(itorp=consts.begin();itorp!=consts.end();itorp++)
		{
			int len = itorp->first.size() * sizeof(wchar_t);
			fout.write((char*)&len,sizeof(len));
			fout.write((char*)itorp->first.c_str(),len);
			fout.write((char*)&(itorp->second),sizeof(cls_cap_addr));
		}
	}

	virtual void load(ifstream& fout)
	{
		int n;

		fout.read((char*)&n,sizeof(int));
		cmds.resize(n);
		for(int cnt=0;cnt<n;cnt++)
		{
			cmds[cnt].load(fout);
		}

		fout.read((char*)&n,sizeof(int));
		consts.resize(n);
		for(int cnt=0;cnt<n;cnt++)
		{
			wstring str;
			cls_cap_addr addr;

			int len;
			fout.read((char*)&len,sizeof(int));
			wchar_t* p = new wchar_t(len/sizeof(wchar_t) + 1);
			fout.read((char*)p,len);
			p[len/sizeof(wchar_t)] = 0;
			str = wstring(p);
			delete [] p;

			fout.read((char*)&addr,sizeof(cls_cap_addr));

			consts.push_back(make_pair(str,addr));
		}
	}

	virtual void run_command(const cls_cap_command& cmd) = 0;

	vector<cls_cap_command>::iterator itor;

	void jump(int cnt)
	{
		itor = cmds.begin() + (cnt - 1);
	}

	virtual void run()
	{
		for(itor=cmds.begin();itor<cmds.end();itor++)
		{
			//cout<<itor - cmds.begin()<<endl;
			run_command(*itor);
			if (itor == cmds.end())
				break;
		}
	}
};