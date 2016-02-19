#include "lib_import.h"

void test_lalr_1()
{
	try
	{
		cls_lalr lalr;
		vector<string> v_product;
		v_product.push_back("#start#@#S#");
		v_product.push_back("#S#@#A##B#");
		v_product.push_back("#A#@#A#$a$");
		v_product.push_back("#A#@$a$");
		v_product.push_back("#B#@#B#$b$");
		v_product.push_back("#B#@$b$");
		lalr.add_product(v_product);
		lalr.run();
		lalr.save("D:\\a.txt");

		vector<cls_lalr_table_item> table;
		lalr.get_output(table);

		cls_a_lalr_dealing_void<int> func;
		cls_lalr_frame<int> lalr_frame(table,lalr.info,lalr.get_kernel_size(),&func);
		vector<string> v_src;
		v_src.push_back("a");
		v_src.push_back("a");
		v_src.push_back("a");
		v_src.push_back("b");
		v_src.push_back("b");

		lalr_frame.run_map(v_src);
	}
	catch (cls_exception &exc)
	{
		cout<<exc.display()<<endl;
	}
}

void test_cava_1()
{
	try
	{
		cls_lalr lalr;
		vector<string> v_product;
		ifstream fin("config\\in.txt");
		while(!fin.eof())
		{
			string str;
			getline(fin,str);
			v_product.push_back(str);
		}
		fin.close();

		lalr.add_product(v_product);

		lalr.add_join_right("");
		lalr.add_join_right("=");
		lalr.add_join_right("+=");
		lalr.add_join_right("-=");
		lalr.add_join_right("*=");
		lalr.add_join_right("/=");
		lalr.add_join_right("%=");
		lalr.add_join_right("&=");
		lalr.add_join_right("^=");
		lalr.add_join_right("!=");
		lalr.add_join_right("<<=");
		lalr.add_join_right("> >=");
		lalr.add_join_right("<<:=");
		lalr.add_join_right("> >:=");

		vector<string> v_rank;
		ifstream find("config\\rank.txt");
		while(!find.eof())
		{
			string str;
			find>>str;
			v_rank.push_back(str);
		}
		find.close();

		lalr.run();
		lalr.save("D:\\a.txt");

		vector<cls_lalr_table_item> table;
		lalr.get_output(table);

		cls_a_lalr_dealing_void<int> func;
		cls_lalr_frame<int > lalr_frame(table,lalr.info,lalr.get_kernel_size(),&func);

		vector<string> src;
		src.push_back("int");
		src.push_back("var");
		src.push_back("(");
		src.push_back(")");
		src.push_back("{");
		src.push_back("return");
		src.push_back("const_num");
		src.push_back(";");
		src.push_back("}");

		lalr_frame.run(src);
	}
	catch (cls_exception &exc)
	{
		cout<<exc.display()<<endl;
	}
}

void test_lalr_xml()
{
	cls_xml_parser xml_parser;
	cls_xml xml;
	xml_parser.run("D:\\trys\\try.html",xml);
}

void test_parser_math_express()
{
	cls_parser_math_expr parser;
	parser.set_var_values("a",2);
	parser.set_var_values("c",3);
	cout<<*parser.run("1+2.5*a+c")<<endl;

	double sum = 0.0;
	for(double cur=1.0;cur<=2.0;cur+=0.001)
	{
		parser.set_var_values("x",cur);
		sum += *parser.run("x^2") * 0.001;
	}

	cout<<sum<<endl;
}

void test_parser_math_express_wide()
{
	cls_parser_math_expr_wide parser;
	parser.set_var_values(L"a",2);
	parser.set_var_values(L"c",3);
	cout<<*parser.run(L"1+2.5*a+c")<<endl;

	double sum = 0.0;
	for(double cur=1.0;cur<=2.0;cur+=0.001)
	{
		parser.set_var_values(L"x",cur);
		sum += *parser.run(L"x^2") * 0.001;
	}

	cout<<sum<<endl;
}

void test_parser_xml_wide()
{
	cls_xml_parser_wide xml_parser;
	cls_xml_wide xml;
	
	xml_parser.run(L"D:\\trys\\lalr.xml");
	xml = xml_parser.get_xml();
}

void test_parser_html()
{
	cls_html_parser_wide xml_parser;
	cls_xml_wide xml;

	xml_parser.run(L"D:\\trys\\lalr.xml");
	xml = xml_parser.get_xml();
}

void test_parser_css()
{
}