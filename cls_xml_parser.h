#include "lib_import.h"

class cls_xml_parsing_item
{
public:
	string first;
	vector<cls_xml_node*> second;
	vector<pair<string,cls_xml_attribut_item> > attr_items;
};

class cls_xml_parser_item
{
public:
	string tag;
	vector<cls_xml_node*> nodes;
	vector<pair<string,cls_xml_attribut_item> > attributes;
	string context;
};

class cls_xml_parser
	:cls_a_lalr_dealing_void<cls_xml_parser_item>
{
private:
	enum en_spilt{en_var,en_num,en_oper};

protected:
	cls_lalr yacc;
	cls_lalr_frame<cls_xml_parser_item> * frame;

	cls_xml *xml;

	vector<string> v_str;
public:

	void run(string str_filename,cls_xml & a_xml)
	{
		xml = &a_xml;

		ifstream fin(str_filename.c_str());

		string str;
		while(!fin.eof())
		{
			string str_tmp;
			getline(fin,str_tmp);

			str += str_tmp;
		}

		fin.close();

		vector<string> v_str_tmp;
		spilt(str,v_str_tmp);

		vector<string>::iterator itor;
		for(itor=v_str_tmp.begin();itor!=v_str_tmp.end();itor++)
		{
			*itor = a_string_trim(*itor);
			if (*itor != "")
			{
				v_str.push_back(*itor);
			}
		}

		frame->run_map(v_str);

		v_str.clear();

		cout<<"parsing succeed."<<endl;
	}

	virtual void operator()(vector<cls_lalr_frame_item<cls_xml_parser_item> > & now,
		int next_status,
		cls_lalr_frame_item<cls_xml_parser_item> &re,
		vector<cls_lalr_frame_item<cls_xml_parser_item> >& stack)
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

				cls_xml_node * new_node = new cls_xml_node;
				new_node->children = now[2].user_data.nodes;

				vector<pair<string,cls_xml_attribut_item> >::iterator itor;
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
				cls_xml_attribut_item item;
				item.type = cls_xml_attribut_item::en_string;
				item.val.val_str = now[4].str_pre;

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 10:
			{
				cls_xml_attribut_item item;
				item.type = cls_xml_attribut_item::en_string;
				item.val.val_str = now[4].str_pre;

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 11:
			{
				cls_xml_attribut_item item;

				if (now[3].str_pre.find('.') == -1)
				{
					item.type = cls_xml_attribut_item::en_int;
					item.val.val_int = get_type_from_string<int>(now[3].str_pre);
				}
				else
				{
					item.type = cls_xml_attribut_item::en_double;
					item.val.val_double = get_type_from_string<double>(now[3].str_pre);
				}

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		case 12:
			{
				xml->root = now[1].user_data.nodes[0];
			}
			break;

		case 13:
			{
				cls_xml_node *new_node = new cls_xml_node;
				new_node->tag = now[2].str_pre;

				vector<pair<string,cls_xml_attribut_item> >::iterator itor;
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
				cls_xml_attribut_item item;
				item.type = cls_xml_attribut_item::en_int;
				string& str = now[4].str_pre;
				for(int cnt=0;cnt<str.size();cnt++)
				{
					if (!isdigit(str[cnt]) && str[cnt] != '.')
					{
						item.type = cls_xml_attribut_item::en_string;
					}
				}

				if (item.type == cls_xml_attribut_item::en_int)
				{
					if (str.find('.') != -1)
					{
						item.type = cls_xml_attribut_item::en_double;
					}
				}

				switch(item.type)
				{
				case cls_xml_attribut_item::en_double:
					{
						item.val.val_double = get_type_from_string<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item::en_int:
					{
						item.val.val_int = get_type_from_string<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item::en_string:
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
				cls_xml_attribut_item item;
				item.type = cls_xml_attribut_item::en_int;
				string& str = now[4].str_pre;
				for(int cnt=0;cnt<str.size();cnt++)
				{
					if (!isdigit(str[cnt]) && str[cnt] != '.')
					{
						item.type = cls_xml_attribut_item::en_string;
					}
				}

				if (item.type == cls_xml_attribut_item::en_int)
				{
					if (str.find('.') != -1)
					{
						item.type = cls_xml_attribut_item::en_double;
					}
				}

				switch(item.type)
				{
				case cls_xml_attribut_item::en_double:
					{
						item.val.val_double = get_type_from_string<double>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item::en_int:
					{
						item.val.val_int = get_type_from_string<int>(now[4].str_pre);
					}
					break;

				case cls_xml_attribut_item::en_string:
					{
						item.val.val_str = now[4].str_pre;
					}
					break;
				}

				re.user_data.attributes.push_back(make_pair(now[1].str_pre,item));
			}
			break;
		}

		//cout<<next_status<<endl;
	}

	virtual string operator()(const string& item)
	{
		if (isdigit(item[0]))
		{
			return string("num");
		}
		else if (isalpha(item[0]) || item[0] == '_')
		{
			return string("var");
		}
		else
		{
			return item;
		}
	}

	virtual string operator()(int item)
	{
		if (item >= v_str.size())
			return "__inner_end";

		if (isdigit(v_str[item][0]))
		{
			return "num";
		}
		else if (isalpha(v_str[item][0]) || v_str[item][0] == '_')
		{
			return "var";
		}
		else
		{
			return v_str[item];
		}
	}

	cls_xml_parser()
	{
		vector<string> v_product;

		//ifstream fin("config\\xml_parser.config");

		//while(!fin.eof())
		//{
		//	string str_prod;
		//	getline(fin,str_prod);

		//	v_product.push_back(str_prod);
		//}

		//fin.close();

		//v_product.push_back("#start#@#tuple_start#");
		//v_product.push_back("#tuple_list#@#tuple#");
		//v_product.push_back("#tuple_list#@$var$");
		//v_product.push_back("#tuple_list#@#tuple_list##tuple#");
		//v_product.push_back("#tuple#@#tuple_pre##tuple_list##tuple_post#");
		//v_product.push_back("#tuple_pre#@$<$$var$#attribute_list#$>$");
		//v_product.push_back("#tuple_post#@$<$$/$$var$$>$");
		//v_product.push_back("#attribute_list#@");
		//v_product.push_back("#attribute_list#@#attribute_list##attribute#");
		//v_product.push_back("#attribute#@$var$$=$$\"$$var$$\"$");
		//v_product.push_back("#attribute#@$var$$=$$\'$$var$$\'$");
		//v_product.push_back("#attribute#@$var$$=$$num$");
		//v_product.push_back("#tuple_start#@#tuple#");

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

		frame = new cls_lalr_frame<cls_xml_parser_item>(table_yacc,yacc.info,yacc.get_kernel_size(),this);
	}

	bool check_alpha(char c)
	{
		return !isdigit(c) && c != '=' && c != '>' && c != '<' && c != '/' && c != '\"' && c != '\'' && c!=' ' && c != '\t';
	}

	bool check_digit(char c)
	{
		return isdigit(c);
	}

	void spilt(string str,vector<string>& re)
	{
		int status = 0;
		int pos = 0;
		string::iterator itor;

		int cnt = 0;
		for(cnt=0,itor=str.begin();
			itor!=str.end();
			itor++,cnt++)
		{
			switch(status)
			{
			case 0:
				{
					if (check_alpha(*itor) || *itor == '_')
					{
						status = 1;
					}
					else if (check_digit(*itor))
					{
						status = 2;
					}
					else if (*itor == '\"' )
					{
						status = 3;
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
					else if (*itor == '\'')
					{
						status  = 4;
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
					else
					{
						status = 0;

						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
					}
				}
				break;

			case 1:
				{
					if (check_alpha(*itor) || check_digit(*itor) || *itor=='_')
					{
						;
					}
					else
					{
						status = 0;
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt + 1;
					}
				}
				break;

			case 2:
				{
					if (check_digit(*itor) || *itor=='.')
					{
						;
					}
					else
					{
						status = 0;
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt + 1;
					}
				}
				break;

			case 3:
				{
					if (*itor == '\"')
					{
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
						status = 0;
					}
				}
				break;

			case 4:
				{
					if (*itor == '\'')
					{
						re.push_back(str.substr(pos,cnt-pos));
						re.push_back(str.substr(cnt,1));
						pos = cnt+1;
						status = 0;
					}
				}
				break;
			}
		}
	}
};
