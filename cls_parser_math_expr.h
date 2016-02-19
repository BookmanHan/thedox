#include "lib_import.h"

class cls_parser_math_expr
	:public cls_parser<double>
{
private:
	vector<string> tokens;
	map<string,double> map_var_to_values;
	double d_re;

public:
	virtual void operator()(vector<cls_lalr_frame_item<double> > & now,
		int next_status,
		cls_lalr_frame_item<double> &re,
		vector<cls_lalr_frame_item<double> >& stack) 
	{
		next_status -= 10000;
		switch(next_status)
		{
		case 0:
			break;

		case 1:
			{
				re.user_data = now[1].user_data + now[3].user_data;
			}
			break;

		case 2:
			{
				re.user_data = now[1].user_data - now[3].user_data;
			}
			break;

		case 3:
			{
				re.user_data = now[1].user_data * now[3].user_data;
			}
			break;

		case 4:
			{
				re.user_data = now[1].user_data / now[3].user_data;
			}
			break;

		case 5:
			{
				re.user_data = int(now[1].user_data) % int(now[3].user_data);
			}
			break;

		case 6:
			{
				re.user_data = now[2].user_data;
			}
			break;
		
		case 7:
			{
				re.user_data = map_var_to_values[now[1].str_pre];
			}
			break;

		case 8:
			{
				re.user_data = get_type_from_string<double>(now[1].str_pre);
			}
			break;

		case 9:
			{
				d_re = now[1].user_data;
			}
			break;

		case 10:
			{
				d_re = pow(now[1].user_data,now[3].user_data);
			}
			break;
		}
	}

	cls_parser_math_expr()
	{
		d_re = 0.0;

		vector<string> v_product;
		v_product.push_back("#start#@#express_init#");
		v_product.push_back("#express#@#express#$+$#express#");
		v_product.push_back("#express#@#express#$-$#express#");
		v_product.push_back("#express#@#express#$*$#express#");
		v_product.push_back("#express#@#express#$/$#express#");
		v_product.push_back("#express#@#express#$%$#express#");
		v_product.push_back("#express#@$($#express#$)$");
		v_product.push_back("#express#@$var$");
		v_product.push_back("#express#@$num$");
		v_product.push_back("#express_init#@#express#");
		v_product.push_back("#express#@#express#$^$#express#");

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

		frame = new cls_lalr_frame<double>(table,yacc.info,yacc.get_kernel_size(),this);
	}

	virtual void set_var_values(string str,double val)
	{
		map_var_to_values[str] = val;
	}

	virtual bool check_alpha( const  char c ) 
	{
		return isalpha(c);
	}

	virtual bool check_digit( const char c ) 
	{
		return isdigit(c);
	}

	virtual double* run( const string& str_context ) 
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

		return &d_re;
	}

	virtual string operator()(const string& item)
	{
		bool b_figure = true;
		string::const_iterator itor;
		for(itor=item.begin();itor!=item.end();itor++)
		{
			if (!check_digit(*itor) && *itor != '.')
			{
				if (!check_alpha(*itor))
				{
					return item;
				}

				b_figure = false;
				break;
			}
		}

		if (b_figure)
			return "num";
		else
			return "var";
	}
};