#include "lib_import.h"

class cls_xml_parser_item_wide
{
public:
	wstring tag;
	vector<cls_xml_node_wide*> nodes;
	vector<pair<wstring,cls_xml_attribut_item_wide> > attributes;
	wstring context;
};

class cls_xml_parser_wide
	:cls_parser_wide<cls_xml_parser_item_wide>
{
private:
	enum en_spilt{en_var,en_num,en_oper};

protected:
	cls_lalr yacc;
	cls_lalr_frame_wide<cls_xml_parser_item_wide> * frame;

	cls_xml_parser_item_wide xml;

	vector<wstring> v_str;
public:

	cls_xml_parser_item_wide* run(const wstring& str_filename)
	{
		wifstream fin(str_filename.c_str());

		if (fin.bad())
			throw cls_exception_not_found("File is not found.");

		wstring str;
		fin.imbue(locale("chs"));
		while(!fin.eof())
		{
			wstring str_tmp;
			getline(fin,str_tmp);

			str += str_tmp;
		}

		fin.close();

		vector<wstring> v_str_tmp;
		spilt(str,v_str_tmp);

		vector<wstring>::iterator itor;
		for(itor=v_str_tmp.begin();itor!=v_str_tmp.end();itor++)
		{
			*itor = a_wstring_trim(*itor);
			if (*itor != L"")
			{
				v_str.push_back(*itor);
			}
		}

		frame->run_map(v_str);

		v_str.clear();

		return &xml;
	}

	virtual void operator()(vector<cls_lalr_frame_item_wide<cls_xml_parser_item_wide> > & now,
		int next_status,
		cls_lalr_frame_item_wide<cls_xml_parser_item_wide> &re,
		vector<cls_lalr_frame_item_wide<cls_xml_parser_item_wide> >& stack)
	{
		next_status -= 10000;
		switch(next_status)
		{
		case 1:
			{
				;
			}
			break;
		case 2:
			{
				re.user_data = now[1].user_data;
				re.user_data.context += now[2].str_pre;
			}
			break;
		case 3:
			{
				re.user_data = now[1].user_data;
				re.user_data.nodes.push_back(now[2].user_data.nodes[0]);
			}
			break;
		case 4:
			{
				if (now[1].user_data.tag != now[3].user_data.tag)
				{
					throw cls_exception_execution(string(__FUNCTION__) + "() - the syntex is erro for not matching.\n");
				}

				cls_xml_node_wide * new_node = new cls_xml_node_wide;
				new_node->children = now[2].user_data.nodes;

				vector<pair<wstring,cls_xml_attribut_item_wide> >::iterator itor;
				for(itor=now[1].user_data.attributes.begin();
					itor != now[1].user_data.attributes.end();
					itor++)
				{
					new_node->set_attribute(itor->first,itor->second);
				}

				new_node->tag = now[1].user_data.tag;
				new_node->context = now[2].user_data.context;
				re.user_data.nodes.push_back(new_node);
			}
			break;
		case 5:
			{
				re.user_data = now[3].user_data;
				re.user_data.tag = now[2].str_pre;
			}
			break;
		case 6:
			{
				re.user_data.tag = now[3].str_pre;
			}
			break;
		case 7:
			{
				;
			}
			break;
		case 8:
			{
				re.user_data = now[1].user_data;
				re.user_data.attributes.push_back(
					make_pair(now[2].user_data.attributes[0].first,
					now[2].user_data.attributes[0].second));
			}
			break;
		case 9:
			{
				cls_xml_attribut_item_wide item;
				item.type = cls_xml_attribut_item_wide::en_string;
				item.val.val_str = now[4].str_pre;

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 10:
			{
				cls_xml_attribut_item_wide item;
				item.type = cls_xml_attribut_item_wide::en_string;
				item.val.val_str = now[4].str_pre;

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 11:
			{
				cls_xml_attribut_item_wide item;

				if (now[3].str_pre.find('.') == -1)
				{
					item.type = cls_xml_attribut_item_wide::en_int;
					item.val.val_int = get_type_from_wstring<int>(now[3].str_pre);
				}
				else
				{
					item.type = cls_xml_attribut_item_wide::en_double;
					item.val.val_double = get_type_from_wstring<double>(now[3].str_pre);
				}

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 12:
			{
				xml = now[1].user_data;
			}
			break;

		case 13:
			{
				cls_xml_node_wide *new_node = new cls_xml_node_wide;
				new_node->tag = now[2].str_pre;

				vector<pair<wstring,cls_xml_attribut_item_wide> >::iterator itor;
				for(itor=now[3].user_data.attributes.begin();
					itor!=now[3].user_data.attributes.end();
					itor++)
				{
					new_node->set_attribute(itor->first,itor->second);
				}

				re.user_data.nodes.push_back(new_node);
			}
			break;

		case 14:
			{
				cls_xml_attribut_item_wide item;
				item.type = cls_xml_attribut_item_wide::en_int;
				wstring& str = now[4].str_pre;
				for(int cnt=0;cnt<str.size();cnt++)
				{
					if (!isdigit(str[cnt]) && str[cnt] != '.')
					{
						item.type = cls_xml_attribut_item_wide::en_string;
					}
				}

				if (item.type == cls_xml_attribut_item_wide::en_int)
				{
					if (str.find('.') != -1)
					{
						item.type = cls_xml_attribut_item_wide::en_double;
					}
				}

				switch(item.type)
				{
				case cls_xml_attribut_item_wide::en_double:
					{
						item.val.val_double = get_type_from_wstring<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item_wide::en_int:
					{
						item.val.val_int = get_type_from_wstring<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item_wide::en_string:
					{
						item.val.val_str = now[4].str_pre;
					}
					break;
				}

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;

		case 15:
			{
				cls_xml_attribut_item_wide item;
				item.type = cls_xml_attribut_item_wide::en_int;
				wstring& str = now[4].str_pre;
				for(int cnt=0;cnt<str.size();cnt++)
				{
					if (!isdigit(str[cnt]) && str[cnt] != '.')
					{
						item.type = cls_xml_attribut_item_wide::en_string;
					}
				}

				if (item.type == cls_xml_attribut_item_wide::en_int)
				{
					if (str.find('.') != -1)
					{
						item.type = cls_xml_attribut_item_wide::en_double;
					}
				}

				switch(item.type)
				{
				case cls_xml_attribut_item_wide::en_double:
					{
						item.val.val_double = get_type_from_wstring<double>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item_wide::en_int:
					{
						item.val.val_int = get_type_from_wstring<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item_wide::en_string:
					{
						item.val.val_str = now[4].str_pre;
					}
					break;
				}

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		}
	}

	virtual cls_xml_wide get_xml()
	{
		cls_xml_wide tmp_xml;
		tmp_xml.root = xml.nodes[0];

		return tmp_xml;
	}

	virtual wstring operator()(const wstring& item)
	{
		if (check_digit(item[0]))
		{
			return wstring(L"num");
		}
		else if (check_alpha(item[0]))
		{
			return wstring(L"var");
		}
		else
		{
			return item;
		}
	}

	cls_xml_parser_wide()
	{
		vector<string> v_product;

		v_product.push_back("#start#@#tuple_start#");
		v_product.push_back("#tuple_list#@");
		v_product.push_back("#tuple_list#@#tuple_list#$var$");
		v_product.push_back("#tuple_list#@#tuple_list##tuple#");
		v_product.push_back("#tuple#@#tuple_pre##tuple_list##tuple_post#");
		v_product.push_back("#tuple_pre#@$<$$var$#attribute_list#$>$");
		v_product.push_back("#tuple_post#@$<$$/$$var$$>$");
		v_product.push_back("#attribute_list#@");
		v_product.push_back("#attribute_list#@#attribute_list##attribute#");
		v_product.push_back("#attribute#@$var$$=$$\"$$var$$\"$");
		v_product.push_back("#attribute#@$var$$=$$\'$$var$$\'$");
		v_product.push_back("#attribute#@$var$$=$$num$");
		v_product.push_back("#tuple_start#@#tuple#");
		v_product.push_back("#tuple#@$<$$var$#attribute_list#$/$$>$");
		v_product.push_back("#attribute#@$var$$=$$\"$$num$$\"$");
		v_product.push_back("#attribute#@$var$$=$$\'$$num$$\'$");

		yacc.add_join_right("=");
		yacc.add_product(v_product);
		yacc.run();

		vector<cls_lalr_table_item> table_yacc;
		yacc.get_output(table_yacc);

		frame = new cls_lalr_frame_wide<cls_xml_parser_item_wide>(table_yacc,yacc.info,yacc.get_kernel_size(),this);
	}

	virtual bool check_alpha(const wchar_t c)
	{
		return !check_digit(c) && c != '=' && c != '>' && c != '<' && c != '/' && c != '\"' && c != '\'' && c!=' ' && c != '\t';
	}

	virtual bool check_digit(const wchar_t c)
	{
		return c >= '0' && c <= '9';
	}
};
