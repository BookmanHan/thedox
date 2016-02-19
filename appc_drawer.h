#include "lib_import.h"

class appc_drawer_item
{
public:
	unsigned line_no;
	float fdata;
};

class appc_drawer_cmds
{
public:
	//1=front;2=back;3=left;4=right;5=repeat_begin;6=repeat_end
	//7=pen up;8=pen down;
	//9=angel;
	unsigned char op;
	double things;

	appc_drawer_cmds(unsigned char aop,double athings)
	{
		op = aop;
		things = athings;
	}
};

class appc_drawer_runner
{
protected:
	vector<appc_drawer_cmds>& cmds;
	stack<pair<vector<appc_drawer_cmds>::iterator,int>> stk_repeat;
	
	cls_point<double> pt_cur;
	int angel;
	bool b_pen_down;

public:
	appc_drawer_runner(vector<appc_drawer_cmds>& a_cmd,cls_image& image)
		:cmds(a_cmd)
	{		
		image.fill_pixel(get_rgb(255,255,255));
	
		b_pen_down  = true;
		pt_cur.x = image.get_width()/2;
		pt_cur.y = image.get_height()/2;
		angel = 0;

		vector<appc_drawer_cmds>::iterator itor;
		for(itor = cmds.begin();itor != cmds.end(); itor++)
		{
			switch(itor->op)
			{
			case 1:
				{
					if (b_pen_down)
					{
						for(int cnt=0;cnt<=itor->things;cnt++)
						{
							image.set_gray(pt_cur.x + cnt* cos(angel/180.0*pi) + 0.5,pt_cur.y + cnt*sin(angel/180.0*pi) + 0.5,0);
						}
					}

					pt_cur.x += itor->things*cos(angel/180.0*pi);
					pt_cur.y += itor->things*sin(angel/180.0*pi);
				}
				break;
			case 2:
				{
					if (b_pen_down)
					{
						for(int cnt=0;cnt<=itor->things;cnt++)
						{
							image.set_gray(pt_cur.x - cnt* cos(angel/180.0*pi) + 0.5,pt_cur.y - cnt*sin(angel/180.0*pi) + 0.5,0);
						}
					}

					pt_cur.x -= itor->things*cos(angel/180.0*pi);
					pt_cur.y -= itor->things*sin(angel/180.0*pi);
				}
				break;
			case 3:
				{
					angel -= itor->things;
				}
				break;
			case 4:
				{
					angel += itor->things;
				}
				break;
			case 5:
				{
					stk_repeat.push(make_pair(itor,itor->things));
				}
				break;
			case 6:
				{
					if (stk_repeat.top().second <= 1)
					{
						stk_repeat.pop();
					}
					else
					{
						itor = stk_repeat.top().first;
						stk_repeat.top().second--;
					}
				}
				break;
			case 7:
				b_pen_down = false;
				break;
			case 8:
				b_pen_down = true;
				break;
			case 9:
				angel = itor->things;
				break;
			}
		}
	}
};

class appc_drawer
	:public cls_parser<appc_drawer_item>
{
protected:
	vector<string> tokens;

public:
	vector<appc_drawer_cmds> cmds;

public:
	virtual void operator()(vector<cls_lalr_frame_item<appc_drawer_item> > & now,
		int next_status,
		cls_lalr_frame_item<appc_drawer_item> &re,
		vector<cls_lalr_frame_item<appc_drawer_item> >& stack) 
	{
		next_status -= 10000;

		switch(next_status)
		{
		case 1:
		case 2:
		case 3:
			break;

		case 4:
			cmds.push_back(appc_drawer_cmds(6,0));
			cmds[now[3].user_data.line_no].things = now[2].user_data.fdata;
			break;
		case 5:
			cmds.push_back(appc_drawer_cmds(1,now[2].user_data.fdata));
			break;
		case 6:
			cmds.push_back(appc_drawer_cmds(2,now[2].user_data.fdata));
			break;
		case 7:
			cmds.push_back(appc_drawer_cmds(3,now[2].user_data.fdata));
			break;
		case 8:
			cmds.push_back(appc_drawer_cmds(4,now[2].user_data.fdata));
			break;
		case 9:
			cmds.push_back(appc_drawer_cmds(5,0));
			re.user_data.line_no = cmds.size() - 1;
			break;
		case 10:
			cmds.push_back(appc_drawer_cmds(7,0));
			break;
		case 11:
			cmds.push_back(appc_drawer_cmds(8,0));
			break;
		case 12:
			cmds.push_back(appc_drawer_cmds(9,get_type_from_string<double>(now[2].str_pre)));
			break;
		case 13:
			re.user_data.fdata = get_type_from_string<double>(now[1].str_pre);
			break;
		case 14:
			re.user_data.fdata = - now[1].user_data.fdata;
			break;
		}
	}

	appc_drawer()
	{
		vector<string> v_product;
		v_product.push_back("#start#@#drawer_init#");
		v_product.push_back("#drawer_init#@#express_list#");
		v_product.push_back("#express_list#@");
		v_product.push_back("#express_list#@#express#$;$#express_list#");
		v_product.push_back("#express#@$REPEAT$#number##repeat_begin#$[$#express_list#$]$");
		v_product.push_back("#express#@$FRONT$#number#");
		v_product.push_back("#express#@$BACK$#number#");
		v_product.push_back("#express#@$LEFT$#number#");
		v_product.push_back("#express#@$RIGHT$#number#");
		v_product.push_back("#repeat_begin#@");
		v_product.push_back("#express#@$PENUP$");
		v_product.push_back("#express#@$PENDOWN$");
		v_product.push_back("#express#@$ANGEL$#number#");
		v_product.push_back("#number#@$num$");
		v_product.push_back("#number#@$-$#number#");

		yacc.add_product(v_product);

		vector<string> v_rank;
		v_rank.push_back("^");
		v_rank.push_back("@");
		v_rank.push_back("*");
		v_rank.push_back("%");
		v_rank.push_back("/");
		v_rank.push_back("@");
		v_rank.push_back("+");
		v_rank.push_back("-");
		v_rank.push_back("@");

		yacc.add_rank(v_rank);

		yacc.run();

		vector<cls_lalr_table_item> table;
		yacc.get_output(table);

		frame = new cls_lalr_frame<appc_drawer_item>(table,yacc.info,yacc.get_kernel_size(),this);
	}

	virtual appc_drawer_item* run( const string& str_context ) 
	{
		tokens.clear();

		vector<string> v_tmp_str;
		spilt(str_context,v_tmp_str);

		vector<string>::iterator itor;
		for(itor=v_tmp_str.begin();itor!=v_tmp_str.end();itor++)
		{
			*itor = a_string_trim(*itor);
			if (*itor != "")
			{
				tokens.push_back(*itor);
			}
		}

		frame->run_map(tokens);

		return null;
	}

	virtual string operator()(const string& item)
	{
		if (check_digit(item[0]))
			return "num";
		else if (item[0] == '-')
			return "num";
		else
			return item;
	}

	virtual bool check_alpha( const char c ) 
	{
		return isalpha(c) || c== '-';
	}

	virtual bool check_digit( const char c ) 
	{
		return isdigit(c);
	}

};