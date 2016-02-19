#include "jrunner_import.h"

using namespace cap_standard;

#define MEMEORY_FOR_SYSTERM_PER_OBJECT 20
/*
	this is the core parsing class for javascript;
	it uses the LALR frame to do the work.

	TODO:
		1.for(... in ...);
		2.triple expression;
		3.typeof;
		4.basic type recognition;
		5.MEMORY MANAGEMENT IS DANGOUROUS;
*/

class cls_jrunner_parser_item
{
public:
	cls_jrunner_symbol_item *item;
	wstring name;
	int line_num;
	vector<cls_jrunner_symbol_item*> params;
	vector<wstring> reg_param;

	bool b_left;
	bool b_func;
	bool b_as_func;
	bool b_ar;

	cls_jrunner_parser_item()
	{
		b_ar = false;
		b_as_func = false;
		b_func  = false;
		b_left = false;
		line_num = 0;
	}
};

class cls_jrunner_parser
	:public cls_parser_wide<cls_jrunner_parser_item>
{
private:
	vector<wstring> tokens;
	set<wstring> key_words;

protected:
	cls_lalr yacc;
	cls_lalr_frame_wide<cls_jrunner_parser_item> * frame;

public:
	cls_jrunner_symbol_manager& sm;
	vector<cls_cap_command>& cmds;

public:
	const static unsigned int cap_command_load = cap_command_users + 1;

public:
	void display()
	{
		for(int cnt=0;cnt<cmds.size();cnt++)
		{
			cout<<cnt<<" => ";
			cmds[cnt].display();
		}
	}

	cls_jrunner_parser(vector<cls_cap_command>& acmds,cls_jrunner_symbol_manager& cjsm)
		:cmds(acmds),sm(cjsm)
	{
		ifstream find("config/jrunner.lalr.kernel.config");
		if (!find)
		{
			find.close();
			
			vector<string> v_product;
			ifstream fin("config/js.bnf.config");

			while(!fin.eof())
			{
				string str_get;
				getline(fin,str_get);
				v_product.push_back(str_get);
			}

			fin.close();

			yacc.add_join_right("=");
			yacc.add_join_right("return");
			yacc.add_product(v_product);

			vector<string> v_rank;
			v_rank.push_back(".");
			v_rank.push_back("@");
			v_rank.push_back("[");
			v_rank.push_back("]");
			v_rank.push_back("@");
			v_rank.push_back("!");
			v_rank.push_back("@");
			v_rank.push_back("%");
			v_rank.push_back("/");
			v_rank.push_back("*");
			v_rank.push_back("@");
			v_rank.push_back("+");
			v_rank.push_back("-");
			v_rank.push_back("@");
			v_rank.push_back("==");
			v_rank.push_back("@");
			v_rank.push_back("?");
			v_rank.push_back("@");
			v_rank.push_back(":");
			v_rank.push_back("@");
			v_rank.push_back("&");
			v_rank.push_back("|");
			v_rank.push_back("@");
			v_rank.push_back("=");
			v_rank.push_back("+=");
			v_rank.push_back("-=");
			v_rank.push_back("*=");
			v_rank.push_back("/=");
			v_rank.push_back("%=");
			v_rank.push_back("@");
			v_rank.push_back(",");
			v_rank.push_back("@");
			v_rank.push_back("return");
			v_rank.push_back("new");

			yacc.add_rank(v_rank);
			yacc.run();

			yacc.info.save("config\\jrunner.lalr.info.config");

			vector<cls_lalr_table_item> table_yacc;
			yacc.get_output(table_yacc);


			ofstream fout("config\\jrunner.lalr.kernel.config");
			fout<<table_yacc.size()<<endl;
			fout<<yacc.get_kernel_size()<<endl;
			for(int cnt=0;cnt<table_yacc.size();cnt++)
			{
				fout<<table_yacc[cnt].cur<<endl;
				fout<<table_yacc[cnt].next<<endl;
				fout<<table_yacc[cnt].oper<<endl;
			}
			fout.close();

			frame = new cls_lalr_frame_wide<cls_jrunner_parser_item>(table_yacc,yacc.info,yacc.get_kernel_size(),this);
		}
		else
		{
			find.close();
			yacc.info.load("config\\jrunner.lalr.info.config");

			vector<cls_lalr_table_item> table_yacc;
			int n;
			int kn;

			ifstream fin("config\\jrunner.lalr.kernel.config");
			fin>>n;
			fin>>kn;
			for(int cnt=0;cnt<n;cnt++)
			{
				cls_lalr_table_item item(0,0,0);
				fin>>item.cur>>item.next>>item.oper;

				table_yacc.push_back(item);
			}
			fin.close();

			frame = new cls_lalr_frame_wide<cls_jrunner_parser_item>(table_yacc,yacc.info,kn,this);
		}

		key_words.insert(L"if");
		key_words.insert(L"else");
		key_words.insert(L"for");
		key_words.insert(L"while");
		key_words.insert(L"in");
		key_words.insert(L"function");
		key_words.insert(L"var");
		key_words.insert(L"do");
		key_words.insert(L"continue");
		key_words.insert(L"break");
		key_words.insert(L"new");
		key_words.insert(L"naive");
		key_words.insert(L"as");
		key_words.insert(L"return");
		key_words.insert(L"typeof");
		key_words.insert(L"case");
		key_words.insert(L"switch");
		key_words.insert(L"try");
		key_words.insert(L"catch");
		key_words.insert(L"throw");
		key_words.insert(L"load");

		be_in_quote = false;
	}

	virtual bool check_alpha( const wchar_t c ) 
	{
		return isalpha(c) || c=='_' || c=='$';
	}

	virtual bool check_digit( const wchar_t c ) 
	{
		return isdigit(c);
	}

	int pos_parsing;

	virtual cls_jrunner_parser_item* run( const wstring& str_context ) 
	{
		be_in_quote = false;
		pos_parsing = 0;
		cmds.clear();

		sm.enter(null);
		sm.enter(sm.get_func(L"jrunner_main"));

		tokens.clear();

		vector<wstring> v_tmp_str;
		spilt(str_context,v_tmp_str);

		vector<wstring>::iterator itor;
		for(itor=v_tmp_str.begin();itor!=v_tmp_str.end();itor++)
		{
			*itor = a_wstring_trim(*itor);
			if (itor->size() != 0)
			{
				if (itor!=v_tmp_str.end()-1 && *(itor+1) == L"=" && (*itor == L"+" || *itor == L"-" || *itor == L"*" || *itor == L"/" || *itor == L"%" || *itor == L"="))
				{
					tokens.push_back(*itor + L"=");
					itor++;
				}
				else if (itor!=v_tmp_str.end()-1 && *(itor+1) == L"+" && (*itor == L"+"))
				{
					tokens.push_back(L"++");
					itor++;
				}
				else if (itor!=v_tmp_str.end()-1 && *(itor+1) == L"-" && (*itor == L"-"))
				{
					tokens.push_back(L"--");
					itor++;
				}
				else
				{
					tokens.push_back(*itor);
				}
			}
		}

		try
		{
			frame->run_map(tokens);
		}
		catch (cls_exception& e)
		{
			cout<<e.display()<<endl;
			for(int cnt=max(0,pos_parsing-5);cnt<=min((int)tokens.size()-1,pos_parsing+5);cnt++)
			{
				wcout.imbue(locale("chs"));
				wcout<<tokens[cnt]<<" ";
			}
		}

		return null;
	}

	virtual void error(const wstring& info)
	{
		wcout<<info<<endl;
	}

	virtual void run( const wstring& str_context,bool b)
	{
		wstring str;
		wifstream fin(str_context.c_str());
		
		while(!fin.eof())
		{
			wstring str_cur;
			getline(fin,str_cur);

			wstringstream wss(str_cur);
			wstring str_a;

			wss>>str_a;
			if (str_a == L"load")
			{
				wstring wfn;
				wss>>wfn;

				int fisrt = wfn.find_first_of('\"');
				int last = wfn.find_last_of('\"');

				wifstream find(wfn.substr(fisrt+1,last-fisrt-1).c_str());
				
				if (find.bad())
				{
					cout<<"BAD LOAD FILE;"<<endl;
					exit(0);
				}

				while(!find.eof())
				{
					getline(find,str_cur);
					str += str_cur;
				}

				find.close();
			}
			else
			{
				str += str_cur;
			}
		}

		fin.close();

		run(str);
	}

	virtual void operator()(vector<cls_lalr_frame_item_wide<cls_jrunner_parser_item> > & now,
		int next_status,
		cls_lalr_frame_item_wide<cls_jrunner_parser_item> &re,
		vector<cls_lalr_frame_item_wide<cls_jrunner_parser_item> >& stack) 
	{
		next_status -= 9999;
		re.user_data.line_num = cmds.size();

		switch(next_status)
		{
		case 8:
			re.user_data.params.push_back(now[1].user_data.item);
			break;

		case 9:
			re.user_data = now[3].user_data;
			re.user_data.params.push_back(now[1].user_data.item);
			break;

		case 10:
				re.user_data.name = now[1].str_pre;
				re.user_data.item = new cls_jrunner_symbol_item;
				sm.add_item(re.user_data.name,re.user_data.item);
				re.user_data.item = sm.get_item(re.user_data.name);
			break;

		case 11:
		case 12:
			{
				re.user_data.name = now[1].str_pre;
				re.user_data.item = new cls_jrunner_symbol_item;
				sm.add_item(re.user_data.name,new cls_jrunner_symbol_item);

				re.user_data.item->addr.offset = sm.get_space();
				re.user_data.item->addr.segment = cap_segment_stack;
				re.user_data.item->addr.length = 4;

				cls_cap_command cmd;
				cmd.tag = L"dynamic_get_id_from_object";
				cmd.op = cap_command_syscall;
				cmd.operand[0] = make_cap_operand(cap_segment_const,sm.add_const(re.user_data.name),4); 
				cmd.operand[1] = make_cap_operand(cap_segment_stack,0,4);
				cmd.result = re.user_data.item->addr;
				cmds.push_back(cmd);

				cls_cap_addr addr;
				addr.segment = cap_segment_stack;
				addr.offset = sm.get_space();
				addr.length = 4;

				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_memory_delete_ref,addr));
				cmds.push_back(make_cap_command(cap_command_memory_add_ref,now[3].user_data.item->addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			break;

		case 16:
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				cmds.push_back(make_cap_command(cap_command_add,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 17:
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				cmds.push_back(make_cap_command(cap_command_minus,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 18:
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				cmds.push_back(make_cap_command(cap_command_mutiply,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 19:
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				cmds.push_back(make_cap_command(cap_command_divide,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 20:
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				cmds.push_back(make_cap_command(cap_command_mod,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;

		case 21:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_inc,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_inc,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 22:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_dec,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_dec,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 23:
			if (now[2].user_data.b_left  == false)
			{
				re.user_data = now[2].user_data;
				cmds.push_back(make_cap_command(cap_command_inc,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[2].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_inc,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 24:
			if (now[2].user_data.b_left  == false)
			{
				re.user_data = now[2].user_data;
				cmds.push_back(make_cap_command(cap_command_dec,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[2].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_dec,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;

		case 25:
			if (now[1].user_data.b_left == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_memory_add_ref,now[3].user_data.item->addr));
				cmds.push_back(make_cap_command(cap_command_memory_delete_ref,now[1].user_data.item->addr));
				cmds.push_back(make_cap_command(cap_command_set,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;
				cls_cap_addr addr;
				addr.segment = cap_segment_stack;
				addr.offset = sm.get_space();
				addr.length = 4;

				cmds.push_back(make_cap_command(cap_command_get,now[1].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_memory_delete_ref,addr));
				cmds.push_back(make_cap_command(cap_command_memory_add_ref,now[3].user_data.item->addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			break;
		
		case 26:
			if (now[1].user_data.b_left == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_set,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_pointer_set,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			break;

		case 27:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_add,re.user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_add,addr,now[3].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 28:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_minus,re.user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_minus,addr,now[3].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 29:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_mutiply,re.user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_mutiply,addr,now[3].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 30:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_divide,re.user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_divide,addr,now[3].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
		case 31:
			if (now[1].user_data.b_left  == false)
			{
				re.user_data = now[1].user_data;
				cmds.push_back(make_cap_command(cap_command_mod,re.user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			}
			else 
			{
				re.user_data = now[1].user_data;

				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_mod,addr,now[3].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_pointer_set,addr,re.user_data.item->addr));
			}
			break;
				
		case 32:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset= sm.get_space();
			cmds.push_back(make_cap_command(cap_command_equ,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 33:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_equ,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 34:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_non_equ,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 35:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_non_equ,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 36:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_larger,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 37:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_smaller,now[1].user_data.item->addr,now[3].user_data.item->addr,re.user_data.item->addr));
			break;
		case 38:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_larger_eql,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 39:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_smaller_eql,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 40:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_and,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 41:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_or,now[1].user_data.item->addr,now[4].user_data.item->addr,re.user_data.item->addr));
			break;
		case 42:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item->addr.offset = sm.get_space();
			cmds.push_back(make_cap_command(cap_command_neg,now[2].user_data.item->addr,re.user_data.item->addr));
			break;

		/*
			triple-operator
		*/

		case 45:
			{
				if (now[1].user_data.b_left)
				{
					re.user_data = now[1].user_data;
					cls_cap_addr addr_get;
					addr_get.segment = cap_segment_stack;
					addr_get.offset = sm.get_space();
					addr_get.length = 4;

					cmds.push_back(make_cap_command(cap_command_get,re.user_data.item->addr,addr_get));
					re.user_data.item->addr = addr_get;
				}
				else
				{
					re.user_data = now[1].user_data;
				}
			}
			break;

		case 46:
			{
				cls_jrunner_symbol_item* item = sm.get_func(now[1].str_pre);
				if (item != null)
				{
					re.user_data.name = now[1].str_pre;
					re.user_data.b_left = false;
					re.user_data.item = item;
					re.user_data.b_as_func = true;

					break;
				}

				re.user_data.b_left = true;
				re.user_data.name = now[1].str_pre;

				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				re.user_data.item->addr.segment = cap_segment_stack;
				re.user_data.item->addr.length = 4;

				cls_cap_command cmd;
				cmd.tag = L"dynamic_get_id_from_object";
				cmd.op = cap_command_syscall;
				cmd.operand[0] = make_cap_operand(cap_segment_const,sm.add_const(re.user_data.name),4); 
				cmd.operand[1] = make_cap_operand(cap_segment_stack,0,4);
				cmd.result = re.user_data.item->addr;
				cmds.push_back(cmd);
			}
			break;

		case 47:
			{
				re.user_data.name = now[1].str_pre;
				re.user_data.b_left = false;
				cls_jrunner_symbol_item *item = sm.get_const(now[1].str_pre);
				if (item == null)
				{
					re.user_data.item = new cls_jrunner_symbol_item;
					sm.add_const(now[1].str_pre,&re.user_data.item,false);
				}
				else
				{
					re.user_data.item = item;
				}
			}
			break;

		case 48:
			{
				re.user_data.b_left = false;
				cls_jrunner_symbol_item *item = sm.get_const(now[2].str_pre);
				if (item == null)
				{
					re.user_data.item = new cls_jrunner_symbol_item;
					sm.add_const(now[2].user_data.name,&re.user_data.item);
				}
				else
				{
					re.user_data.item = item;
				}
			}
			break;

		case 49:
			re.user_data.name = L"";
			break;

		case 50:
			re.user_data.name = now[1].str_pre + now[2].user_data.name;
			break;

		case 51:
			{
				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cls_cap_addr addr_c = make_cap_operand(cap_segment_stack,sm.get_space(),4);

				cmds.push_back(make_cap_command(cap_command_get,now[1].user_data.item->addr,addr));
				cmds.push_back(make_cap_command(cap_command_mutiply,now[3].user_data.item->addr,make_cap_operand(cap_segment_const,sm.add_const(get_wstr_from_type(4),null,false),4),addr_c));
				cmds.push_back(make_cap_command(cap_command_add,addr,addr_c,addr));
				
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.b_left = true;
				re.user_data.item->addr = addr;
				re.user_data.b_ar = true;
			}
			break;

		case 52:
			{
				re.user_data.name = now[3].str_pre;

				if (now[1].user_data.params.size() == 0)
					re.user_data.params.push_back(now[1].user_data.item);
				else
					re.user_data.params.push_back(now[1].user_data.params[0]);
				
				re.user_data.b_func = true;
				re.user_data.b_left = true;

				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr.offset = sm.get_space();
				re.user_data.item->addr.length  = 4;
				re.user_data.item->addr.segment = cap_segment_stack;

				cls_cap_command cmd;
				cmd.tag = L"dynamic_get_id_from_object_indirect";
				cmd.op = cap_command_syscall;
				cmd.operand[0] = make_cap_operand(cap_segment_const,sm.add_const(now[3].str_pre),4); 
				cmd.operand[1] = now[1].user_data.item->addr;
				cmd.result =re.user_data.item->addr;

				cmds.push_back(cmd);
			}
			break;

		case 53:
			re.user_data.item = now[1].user_data.item;
			break;

		case 54:
		case 55:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[6].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size(),4);
			cmds[now[6].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[6].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].result = now[4].user_data.item->addr;
			break;

		case 56:
		case 114:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[6].user_data.line_num].operand[0]= make_cap_operand(cap_segment_code,now[8].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[6].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].result = now[4].user_data.item->addr;
			cmds[now[8].user_data.line_num].result = make_cap_operand(cap_segment_code,cmds.size(),4);
			break;

		case 57:
		case 115:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[6].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,now[9].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[6].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].result = now[4].user_data.item->addr;
			cmds[now[9].user_data.line_num].result= make_cap_operand(cap_segment_code,cmds.size(),4);
			break;
		
		case 60:
		case 61:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[6].user_data.line_num].result = now[4].user_data.item->addr;
			cmds[now[6].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[6].user_data.line_num+1,4);
			cmds[now[6].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size()+1,4);
			cmds.push_back(make_cap_command(cap_command_abs_jump,make_cap_operand(cap_segment_code,now[1].user_data.line_num,0)));
			break; 

		case 62:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[8].user_data.line_num].result = now[6].user_data.item->addr;
			cmds[now[8].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size(),4);
			cmds[now[8].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[1].user_data.line_num,4);
			break;

		case 63:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[9].user_data.line_num].result = now[7].user_data.item->addr;
			cmds[now[9].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size(),4);
			cmds[now[9].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[1].user_data.line_num,4);
			break;

		case 58:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[9].user_data.line_num].result = now[7].user_data.item->addr;
			cmds[now[9].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size(),4);
			cmds[now[9].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[11].user_data.line_num+1,4);
			cmds[now[11].user_data.line_num].result = make_cap_operand(cap_segment_code,now[6].user_data.line_num,4);
			cmds[now[14].user_data.line_num].result = make_cap_operand(cap_segment_code,now[9].user_data.line_num + 1,4);
			break;

		case 59:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[9].user_data.line_num].result = now[7].user_data.item->addr;
			cmds[now[9].user_data.line_num].operand[0] = make_cap_operand(cap_segment_code,cmds.size(),4);
			cmds[now[9].user_data.line_num].operand[1] = make_cap_operand(cap_segment_code,now[11].user_data.line_num+1,4);
			cmds[now[11].user_data.line_num].result = make_cap_operand(cap_segment_code,now[6].user_data.line_num,4);
			cmds[now[15].user_data.line_num].result = make_cap_operand(cap_segment_code,now[9].user_data.line_num + 1,4);
			break;

		case 68:
			re.user_data = now[1].user_data;
			re.user_data.params.insert(re.user_data.params.end(),now[2].user_data.params.begin(),now[2].user_data.params.end());
			break;

		case 69:
			now[2].user_data.item->addr.length = now[4].user_data.line_num;
			re.user_data.params.push_back(now[2].user_data.item);
			break;

		case 66:
			re.user_data.line_num = now[1].user_data.line_num;
			cmds[now[2].user_data.line_num].result = make_cap_operand(cap_segment_code,cmds.size() + 1,4);
			cmds.push_back(make_cap_command(cap_command_abs_jump,make_cap_operand(cap_segment_code,cmds.size() + now[8].user_data.params.size()*2 + 1,4)));

			for(int cnt=0;cnt<now[8].user_data.params.size();cnt++)
			{
				cls_jrunner_symbol_item* item = now[8].user_data.params[cnt];

				cls_cap_addr addr_func;
				addr_func.offset = item->addr.length;
				addr_func.segment = cap_segment_code;
				addr_func.length = 4;
				
				cls_cap_addr addr_result;
				addr_result.segment = cap_segment_stack;
				addr_result.length = 4;
				addr_result.offset = sm.get_offset();

				cmds.push_back(make_cap_command(cap_command_equ,item->addr,now[5].user_data.item->addr,addr_result));
				cmds.push_back(make_cap_command(cap_command_jump,make_cap_operand(cap_segment_code,cmds.size()+1,4),addr_func,addr_result));
			}
			break;

		case 70:
		case 71:
			re.user_data.line_num = now[1].user_data.line_num;
			break;

		case 72:
			re.user_data.line_num = cmds.size() ;
			break;

		case 73:
			break;

		case 74:
			re.user_data.b_as_func = true;
			re.user_data.item = now[1].user_data.item;
			re.user_data.name = now[1].user_data.name;

			cmds[now[1].user_data.line_num - 1].result = make_cap_operand(cap_segment_code,cmds.size() + 1,0);
			cmds.push_back(make_cap_command(cap_command_return));
			sm.leave();

			break;

		case 112:
			cmds.push_back(make_cap_command(cap_command_abs_jump));

			re.user_data.line_num = cmds.size();
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.name = now[2].str_pre;
			re.user_data.item->addr.offset = cmds.size();

			re.user_data.item->add_child(L"this",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"super",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_count",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_bakup",new cls_jrunner_symbol_item);

			sm.get_cur_func()->add_func(re.user_data.name,re.user_data.item);
			re.user_data.item = sm.get_func(re.user_data.name);

			sm.enter(re.user_data.item);

			
			break;

		case 75:
				cmds.push_back(make_cap_command(cap_command_abs_jump));
				re.user_data.line_num = cmds.size();
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.name = now[2].str_pre;
				re.user_data.item->addr.offset = cmds.size();

				re.user_data.item->add_child(L"this",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"super",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_count",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_bakup",new cls_jrunner_symbol_item);

			{
				for(int cnt=now[4].user_data.reg_param.size()-1;cnt>=0;cnt--)
				{
						re.user_data.item->add_child(now[4].user_data.reg_param[cnt],new cls_jrunner_symbol_item);
				}
			}
				
				now[4].user_data.item->detach_child();
				delete now[4].user_data.item;


				sm.get_cur_func()->add_func(re.user_data.name,re.user_data.item);
				re.user_data.item =  sm.get_func(re.user_data.name);

				sm.enter(re.user_data.item);
			break;

		case 76:
			cmds.push_back(make_cap_command(cap_command_abs_jump));

			re.user_data.line_num = cmds.size();
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.name = L"afunc_" + a_get_random_wstring(15);
			re.user_data.item->addr.offset = cmds.size();

			re.user_data.item->add_child(L"this",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"super",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_count",new cls_jrunner_symbol_item);
			re.user_data.item->add_child(L"____runtime_bakup",new cls_jrunner_symbol_item);

			{
				for(int cnt=now[3].user_data.reg_param.size()-1;cnt>=0;cnt--)
				{
					re.user_data.item->add_child(now[3].user_data.reg_param[cnt],new cls_jrunner_symbol_item);
				}
			}

			now[3].user_data.item->detach_child();
			delete now[3].user_data.item;

			sm.get_cur_func()->add_func(re.user_data.name,re.user_data.item);
			re.user_data.item = sm.get_func(re.user_data.name);

			sm.enter(re.user_data.item);
			break;

		case 78:
			cmds.push_back(make_cap_command(cap_command_abs_jump));

				re.user_data.line_num = cmds.size();
				re.user_data.item = new cls_jrunner_symbol_item;

				re.user_data.item->add_child(L"this",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"super",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_count",new cls_jrunner_symbol_item);
				re.user_data.item->add_child(L"____runtime_bakup",new cls_jrunner_symbol_item);

				re.user_data.item->addr.offset = cmds.size();

				re.user_data.name = L"afunc_" + a_get_random_wstring(15);


				sm.get_cur_func()->add_func(re.user_data.name,re.user_data.item);
				re.user_data.item = sm.get_func(re.user_data.name);

				sm.enter(re.user_data.item);
			break;

		case 77:
			re.user_data.b_as_func = true;
			re.user_data = now[2].user_data;
			break;

		case 79:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.reg_param.push_back(now[1].str_pre);
			break;

		case 80:
			re.user_data = now[3].user_data;
			re.user_data.reg_param.push_back(now[1].str_pre);
			break;

		case 81:
			cmds.push_back(make_cap_command(cap_command_return,now[2].user_data.item->addr));
			break;

		case 82:
			cmds.push_back(make_cap_command(cap_command_break));
			break;

		case 83:
			cmds.push_back(make_cap_command(cap_command_continue));
			break;

		case 84:
			{
				cmds[now[2].user_data.line_num - 1].operand[0] = make_cap_operand(null,null,null);
				cmds[now[2].user_data.line_num].result = make_cap_operand(null,null,null);

				re.user_data.item = now[2].user_data.params[0];
			}
			break;

		case 85:
		case 86:
			{

				if (now[1].user_data.b_func == true)
				{
					re.user_data.item = new cls_jrunner_symbol_item;

					cls_cap_addr addr_re;
					addr_re.segment = cap_segment_stack;
					addr_re.offset = sm.get_space();
					addr_re.length = 4;

					re.user_data.item = new cls_jrunner_symbol_item;
					re.user_data.item->addr.offset = sm.get_space();
					
					//used for function address.
					sm.get_space();
					re.user_data.item->addr.segment = cap_segment_stack;
					re.user_data.item->addr.length = 4;

					cls_cap_addr addr;
					addr.segment = cap_segment_relative;
					addr.offset = sm.get_offset();
					addr.length = 4;

					cls_cap_command cmd;
					cmd.tag = L"dynamic_build_func";
					cmd.op = cap_command_syscall;
					cmd.operand[0] = now[1].user_data.item->addr;
					cmd.operand[1] = now[1].user_data.params[0]->addr;
					cmd.result = re.user_data.item->addr;
					cmds.push_back(cmd);

					cmds.push_back(make_cap_command(cap_command_pre_call,addr));
					
					addr_re.offset -= addr.offset;
					re.user_data.item->addr.offset -= addr.offset;

					cmds.push_back(make_cap_command(cap_command_set,re.user_data.item->addr,make_cap_operand(cap_segment_stack,0,4)));

					int tp = now[3].user_data.params.size()-1;
					for(int cnt=0;cnt<now[3].user_data.params.size();cnt++)
					{
						cls_jrunner_symbol_item * item = now[3].user_data.params[cnt];

						cmds.push_back(make_cap_command(cap_command_set,re.user_data.item->addr,addr_re));
						cmds.push_back(make_cap_command(cap_command_add,addr_re,make_cap_operand(cap_segment_const,sm.add_const(get_wstr_from_type((tp - cnt)*4 + MEMEORY_FOR_SYSTERM_PER_OBJECT),null,false),4),addr_re));
						cmds.push_back(make_cap_command(cap_command_memory_add_ref,addr_re));

						if (item->addr.segment == cap_segment_stack || item->addr.segment == cap_segment_relative)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(cap_segment_stack,item->addr.offset - addr.offset,item->addr.length),addr_re));
						else if (item->addr.segment == cap_segment_code)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->class_id,item->addr.length),addr_re));
						else
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->addr.offset,item->addr.length),addr_re));
					}

					cls_jrunner_symbol_item* echo = new cls_jrunner_symbol_item;
					echo->addr = re.user_data.item->addr;
					echo->addr.offset += addr.offset;

					re.user_data.params.push_back(echo);
					now[1].user_data.item->addr.offset -= addr.offset;

					cmds.push_back(make_cap_command(cap_command_call,make_cap_operand(re.user_data.item->addr.segment,re.user_data.item->addr.offset + 4,4)));
					cmds.push_back(make_cap_command(cap_command_post_call,re.user_data.item->addr,addr));
					re.user_data.item->addr.offset += addr.offset;
					re.user_data.line_num = cmds.size();

					cmds.push_back(make_cap_command(cap_command_memory_delete_ref,make_cap_operand(cap_segment_stack,addr.offset,4)));
					break;
				}
				else if (now[1].user_data.b_left == true)
				{
					re.user_data.item = new cls_jrunner_symbol_item;

					cls_cap_addr addr_re;
					addr_re.segment = cap_segment_stack;
					addr_re.offset = sm.get_space();
					addr_re.length = 4;

					re.user_data.item = new cls_jrunner_symbol_item;
					re.user_data.item->addr.offset = sm.get_space();

					//used for function address.
					sm.get_space();
					re.user_data.item->addr.segment = cap_segment_stack;
					re.user_data.item->addr.length = 4;

					cls_cap_addr addr;
					addr.segment = cap_segment_relative;
					addr.offset = sm.get_offset();
					addr.length = 4;

					cls_cap_command cmd;
					cmd.tag = L"dynamic_build_func_asfunc";
					cmd.op = cap_command_syscall;
					cmd.operand[0] = now[1].user_data.item->addr;
					cmd.operand[1] = make_cap_operand(cap_segment_stack,0,4);
					cmd.result = re.user_data.item->addr;
					cmds.push_back(cmd);

					cmds.push_back(make_cap_command(cap_command_pre_call,addr));

					addr_re.offset -= addr.offset;
					re.user_data.item->addr.offset -= addr.offset;

					cmds.push_back(make_cap_command(cap_command_set,re.user_data.item->addr,make_cap_operand(cap_segment_stack,0,4)));

					int tp = now[3].user_data.params.size()-1;
					for(int cnt=0;cnt<now[3].user_data.params.size();cnt++)
					{
						cls_jrunner_symbol_item * item = now[3].user_data.params[cnt];

						cmds.push_back(make_cap_command(cap_command_set,re.user_data.item->addr,addr_re));
						cmds.push_back(make_cap_command(cap_command_add,addr_re,make_cap_operand(cap_segment_const,sm.add_const(get_wstr_from_type((tp - cnt)*4 + MEMEORY_FOR_SYSTERM_PER_OBJECT),null,false),4),addr_re));
						cmds.push_back(make_cap_command(cap_command_memory_add_ref,addr_re));

						if (item->addr.segment == cap_segment_stack || item->addr.segment == cap_segment_relative)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(cap_segment_stack,item->addr.offset - addr.offset,item->addr.length),addr_re));
						else if (item->addr.segment == cap_segment_code)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->class_id,item->addr.length),addr_re));
						else
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->addr.offset,item->addr.length),addr_re));
					}

					cls_jrunner_symbol_item* echo = new cls_jrunner_symbol_item;
					echo->addr = re.user_data.item->addr;
					echo->addr.offset += addr.offset;

					re.user_data.params.push_back(echo);
					now[1].user_data.item->addr.offset -= addr.offset;

					cmds.push_back(make_cap_command(cap_command_call,make_cap_operand(re.user_data.item->addr.segment,re.user_data.item->addr.offset + 4,4)));
					cmds.push_back(make_cap_command(cap_command_post_call,re.user_data.item->addr,addr));

					re.user_data.item->addr.offset += addr.offset;
					re.user_data.line_num = cmds.size();

					cmds.push_back(make_cap_command(cap_command_memory_delete_ref,make_cap_operand(cap_segment_stack,addr.offset,4)));
					break;
				}
				else
				{
					re.user_data.item = new cls_jrunner_symbol_item;
					re.user_data.item->addr.offset = sm.get_space();
					re.user_data.item->addr.segment = cap_segment_stack;
					re.user_data.item->addr.length = 4;

					cls_cap_addr addr_re;
					addr_re.segment = cap_segment_stack;
					addr_re.offset = sm.get_space();
					addr_re.length = 4;

					cls_cap_addr addr;
					addr.segment = cap_segment_relative;
					addr.offset = sm.get_offset();
					addr.length = now[1].user_data.item->get_size();

					cmds.push_back(make_cap_command(cap_command_new,make_cap_operand(null,sm.get_func(now[1].user_data.name)->class_id,addr.length),make_cap_operand(cap_segment_null,now[1].user_data.item->class_id,0),re.user_data.item->addr));
					cmds.push_back(make_cap_command(cap_command_pre_call,addr));

					re.user_data.item->addr.offset -= addr.offset;
					addr_re.offset -= addr.offset;

					cmds.push_back(make_cap_command(cap_command_minus,re.user_data.item->addr,make_cap_operand(cap_segment_const,sm.add_const(get_wstr_from_type(20),null,false),4),make_cap_operand(cap_segment_stack,0,4)));
					cmds.push_back(make_cap_command(cap_command_set,make_cap_operand(cap_segment_stack,0,4),re.user_data.item->addr));

					int tp = now[3].user_data.params.size()-1;
					for(int cnt=0;cnt<now[3].user_data.params.size();cnt++)
					{
						cls_jrunner_symbol_item * item = now[3].user_data.params[cnt];

						cmds.push_back(make_cap_command(cap_command_set,re.user_data.item->addr,addr_re));
						cmds.push_back(make_cap_command(cap_command_add,addr_re,make_cap_operand(cap_segment_const,sm.add_const(get_wstr_from_type((tp - cnt)*4 + MEMEORY_FOR_SYSTERM_PER_OBJECT),null,false),4),addr_re));
						cmds.push_back(make_cap_command(cap_command_memory_add_ref,addr_re));

						if (item->addr.segment == cap_segment_stack || item->addr.segment == cap_segment_relative)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(cap_segment_stack,item->addr.offset - addr.offset,item->addr.length),addr_re));
						else if (item->addr.segment == cap_segment_code)
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->class_id,item->addr.length),addr_re));
						else
							cmds.push_back(make_cap_command(cap_command_pointer_set,make_cap_operand(item->addr.segment,item->addr.offset,item->addr.length),addr_re));
					}

					cmds.push_back(make_cap_command(cap_command_call,sm.get_func(now[1].user_data.name)->addr));
					cmds.push_back(make_cap_command(cap_command_post_call,re.user_data.item->addr,addr));

					cls_jrunner_symbol_item* echo = new cls_jrunner_symbol_item;
					echo->addr = re.user_data.item->addr;
					echo->addr.offset += addr.offset;
					re.user_data.params.push_back(echo);

					re.user_data.item->addr.offset += addr.offset;
					re.user_data.line_num = cmds.size();

					cmds.push_back(make_cap_command(cap_command_memory_delete_ref,make_cap_operand(cap_segment_stack,addr.offset,4)));
					break;
				}
			}
			break;

		case 88:
		case 90:
			re.user_data.params.push_back(now[1].user_data.item);
			break;

		case 89:
		case 91:
			re.user_data = now[3].user_data;
			re.user_data.params.push_back(now[1].user_data.item);
			break;

		case 92:
			{
				sm.add_func(now[3].str_pre,new cls_jrunner_symbol_item);
				cls_jrunner_symbol_item* item = sm.get_func(now[3].str_pre);

				item->add_child(L"this",new cls_jrunner_symbol_item);
				item->add_child(L"____runtime_type",new cls_jrunner_symbol_item);
				item->add_child(L"super",new cls_jrunner_symbol_item);
				item->add_child(L"____runtime_count",new cls_jrunner_symbol_item);
				item->add_child(L"____runtime_bakup",new cls_jrunner_symbol_item);

				for(int cnt=0;cnt<10;cnt++)
				{
					item->add_child(wstring(L"param_") + cnt,new cls_jrunner_symbol_item);
				}

				cmds.push_back(make_cap_command(cap_command_abs_jump,make_cap_operand(cap_segment_code,cmds.size() + 3,0)));
				
				cls_cap_command cmd = make_cap_command(cap_command_load_dynamic);
				cmd.tag = now[7].str_pre;
				cmds.push_back(cmd);

				cmds.push_back(make_cap_command(cap_command_return));

				sm.get_func(now[3].str_pre)->addr.offset = cmds.size() - 2;
			}

			break;

		case 93:
			re.user_data = now[2].user_data;
			break;

		case 94:
			re.user_data = now[2].user_data;
			cmds.push_back(make_cap_command(cap_command_neg,re.user_data.item->addr));
			break;

		case 95:
			/*
				Here is the typeof function. 
			*/
			break;

		case 97:
			re.user_data =  now[2].user_data;
			break;

		case 99:
			re.user_data.item->add_child(now[1].user_data.name,now[1].user_data.item);
			break;

		case 100:
			re.user_data = now[1].user_data;
			re.user_data.item->add_child(now[3].user_data.name,now[3].user_data.item);
			break;

		case 101:
		case 102:
		case 103:
		case 104:
			re.user_data.item = new cls_jrunner_symbol_item;
			re.user_data.item = now[3].user_data.item;
			re.user_data.name = now[1].str_pre;
			break;

		case 105:
			sm.add_item(now[7].str_pre,new cls_jrunner_symbol_item);
			cmds[now[1].user_data.line_num].result = make_cap_operand(cap_segment_code,now[10].user_data.line_num,4);
			cmds[now[4].user_data.line_num].result = make_cap_operand(cap_segment_code,cmds.size(),4);
			break;

		case 106:
			cmds.push_back(make_cap_command(cap_command_throw,now[2].user_data.item->addr));
			break;

		case 108:
		case 109:
			re.user_data = now[1].user_data;
			break;

		case 110:
			re.user_data.item = new cls_jrunner_symbol_item;
			
			for(int cnt=0;cnt<now[2].user_data.params.size();cnt++)
			{
				re.user_data.item->add_child(get_wstr_from_type(cnt),now[2].user_data.params[cnt]);
			}
			break;

		case 111:
			break;

		case 113:
			re.user_data.line_num = cmds.size();
			break;

		case 117:
			re.user_data.line_num = cmds.size();
			cmds.push_back(make_cap_command(cap_command_abs_jump));
			break;

		case 118:
		case 119:
		case 116:
			re.user_data.line_num = cmds.size();
			cmds.push_back(make_cap_command(cap_command_jump));
			break;

		case 120:
			re.user_data.line_num = cmds.size();
			break;

		case 121:
			re.user_data.line_num = cmds.size();
			cmds.push_back(make_cap_command(cap_command_jump));
			break;

		case 122:
		case 123:
		case 124:
		case 125:
			re.user_data.line_num = cmds.size();
			cmds.push_back(make_cap_command(cap_command_abs_jump));
			break;

		case 126:
			re.user_data.line_num = cmds.size();
			cmds.push_back(make_cap_command(cap_command_aserror));
			break;

		case 127:
			sm.add_item((stack.rbegin() + 1)->str_pre,new cls_jrunner_symbol_item);
			break;

		case 128:
			{
				cls_cap_addr addr = make_cap_operand(cap_segment_stack,sm.get_space(),4);
				cmds.push_back(make_cap_command(cap_command_new,make_cap_operand(cap_segment_null,0,0),now[4].user_data.item->addr,addr));
				
				re.user_data.item = new cls_jrunner_symbol_item;
				re.user_data.item->addr = addr;
			}
			break;
		}
	}

protected:
	bool be_in_quote;

public:
	virtual wstring operator()(const wstring& item)
	{
		pos_parsing++;

		if (item.size() == 1 && (item[0] == '\"' || item[0] == '\''))
		{
			be_in_quote = !be_in_quote;
			return L"\"";
		}
		else if (be_in_quote == true)
		{
			return L"str";
		}
		else if (key_words.find(item) != key_words.end())
		{
			return item;
		}
		else if (check_digit(item[0]))
		{
			return L"num";
		}
		else if (check_alpha(item[0]))
		{
			return L"id";
		}
		else
		{
			return item;
		}
	}
};