#include "dlib_import.h"
#include <mysql.h>
#pragma comment(lib,"libmysql.lib")

class cls_mysql_field
{
public:
	string name;
	void *val;
	unsigned len;
};

class cls_mysql_rows
{
public:
	vector<cls_mysql_field> fields;
};

class cls_mysql_operator
{
protected:
	MYSQL m_main;
	MYSQL_STMT * m_stmt;
	MYSQL_RES * m_result;
	vector<cls_mysql_rows*> v_res;

	string str_password;
	string str_host;
	string str_username;
	string str_database;

	unsigned port;

public:
	cls_mysql_operator(string a_h,unsigned a_p,string a_u,string a_pwd,string str_database)
		:str_username(a_u),
		 str_password(a_pwd),
		 str_host(a_h),
		 port(a_p)
	{
		m_result = null;
		mysql_init(&m_main);

		if (mysql_real_connect(&m_main,str_host.c_str(),str_username.c_str(),str_password.c_str(),str_database.c_str(),port,null,0) == null)
		{
			throw cls_exception_connection_error("cls_mysql::cls_mysql() - thedox can't connect to the mysql.");
		}

		//if (mysql_set_character_set(&m_main,"gb2312"))
		//{
		//	throw cls_exception_connection_error("cls_mysql::cls_mysql() - thedox can't connect to the mysql.");
		//}

		mysql_set_server_option(&m_main,MYSQL_OPTION_MULTI_STATEMENTS_ON);
	}

	~cls_mysql_operator()
	{
		close();
	}

	virtual int execute_non_query(vector<string>& sql)
	{
		mysql_autocommit(&m_main,null);

		int re_effected_lines = 0;
		int size = sql.size();
		for(int cnt=0;cnt<size;cnt++)
		{
			mysql_query(&m_main,sql[cnt].c_str());
			re_effected_lines += (unsigned)mysql_affected_rows(&m_main);
		}

		if (mysql_commit(&m_main))
		{
			mysql_rollback(&m_main);
		}

		mysql_autocommit(&m_main,null);

		return re_effected_lines;
	}

	virtual int get_error_no()
	{
		return mysql_errno(&m_main);
	}

	virtual string get_error()
	{
		return mysql_error(&m_main);
	}

	virtual int execute_non_query(string& sql)
	{
		mysql_autocommit(&m_main,null);

		int re_effected_lines = 0;

		mysql_query(&m_main,sql.c_str());
		re_effected_lines += (unsigned)mysql_affected_rows(&m_main);


		if (mysql_commit(&m_main))
		{
			mysql_rollback(&m_main);
			throw cls_exception_execution("cls_mysql_operator::execute_non_query() - the sql can't be commited.\n");
		}

		mysql_autocommit(&m_main,null);

		return re_effected_lines;
	}

	virtual unsigned execute_query(string sql)
	{
		unsigned n_line = 0;
		unsigned n_field = 0;

		MYSQL_ROW row;
		MYSQL_FIELD *p_field;

		free_result();

		mysql_query(&m_main,sql.c_str());
		m_result = mysql_store_result(&m_main);

		if (m_result == null)
			return 0;

		n_line = (unsigned)mysql_num_rows(m_result);
		n_field = (unsigned)mysql_num_fields(m_result);

		for(int cnt=0;cnt<n_line;cnt++)
		{
			cls_mysql_rows* rows = new cls_mysql_rows;

			rows->fields.resize(n_field);

			row = mysql_fetch_row(m_result);

			for(int j=0;j<(int)n_field;j++)
			{
				p_field = mysql_fetch_field_direct(m_result,j);
				rows->fields[j].name = p_field->name;
				rows->fields[j].val = row[j];
				rows->fields[j].len = p_field->max_length;
			}

			v_res.push_back(rows);
		}

		return n_line;
	}

	virtual void free_result()
	{
		vector<cls_mysql_rows*>::iterator itor;
		for(itor=v_res.begin();itor != v_res.end();itor++)
		{
			delete (*itor);
		}

		v_res.clear();

		m_result = null;
	}

	virtual void execute_prepare(string sql)
	{
		m_stmt = mysql_stmt_init(&m_main);
		if (!m_stmt)
		{
			throw cls_exception_build_error("cls_mysql_operator::execute_prepare() - the mysql statement can't be builded.\n");
		}

		if (mysql_stmt_prepare(m_stmt,sql.c_str(),sql.length()))
		{
			throw cls_exception_build_error("cls_mysql_operator::execute_prepare() - the mysql statement can't be executed.\n");
		}
	}

	virtual void bind_param(MYSQL_BIND *bind)
	{
		if (mysql_stmt_bind_param(m_stmt,bind))
		{
			throw cls_exception_build_error("cls_mysql_operator::bind_param() - the mysql statement can't be binded.\n");
		}
	}

	virtual unsigned execute()
	{
		if (mysql_stmt_execute(m_stmt))
		{
			throw cls_exception_execution("cls_mysql_operator::execute() - mysql execution is error.\n");
		}

		return (unsigned)mysql_stmt_affected_rows(m_stmt);
	}

	virtual void close_prepare()
	{
		if (mysql_stmt_close(m_stmt))
		{
			throw cls_exception_execution("cls_mysql_operator::close_prepare() - mysql execution is error.\n");
		}
	}

	virtual void change_user(string a_user,string a_pwd,string a_db)
	{
		str_username = a_user;
		str_password = a_pwd;
		str_database = a_db;

		if (mysql_change_user(&m_main,str_username.c_str(),str_password.c_str(),str_database.c_str()))
		{
			throw cls_exception_execution("cls_mysql_operator::change_user() - mysql execution is error.\n");
		}
	}

	virtual void select_database(string db)
	{
		if (mysql_select_db(&m_main,db.c_str()))
		{
			throw cls_exception_execution("cls_mysql_operator::select_database() - mysql execution is error.\n");
		}
	}
	
	virtual void close()
	{
		free_result();
		mysql_close(&m_main);
	}

	template<typename T>
	int get_value(int cnt,string fd_name)
	{
		if (cnt >= v_res.size() || cnt < 0)
		{
			throw cls_exception_overflow("cls_mysql_operator::get_int_value() - out of subscirpts.");
		}

		cls_mysql_rows *row = v_res[cnt];
		vector<cls_mysql_field>::iterator itor;
		
		for(itor = row->fields.begin();itor != row->fields.end();itor++)
		{
			if (itor->name == fd_name)
			{
				string str=string((char*)(itor->val));

				return get_type_from_string<T>(str);
			}
		}
	}

	template<typename T>
	T get_value(int cnt,int fd_num)
	{
		if (cnt >= v_res.size() || cnt < 0)
		{
			throw cls_exception_overflow("cls_mysql_operator::get_int_value() - out of subscirpts.");
		}

		cls_mysql_rows *row = v_res[cnt];
		
		return get_type_from_string<T>((char*)row->fields[fd_num].val);
	}

	template<typename T>
	pair<int,void*> get(int cnt,string fd_name)
	{
		if (cnt >= v_res.size() || cnt < 0)
		{
			throw cls_exception_overflow("cls_mysql_operator::get_int_value() - out of subscirpts.");
		}

		cls_mysql_rows *row = v_res[cnt];
		vector<cls_mysql_field>::iterator itor;

		for(itor = row->fields.begin();itor != row->fields.end();itor++)
		{
			if (itor->name == fd_name)
			{
				return make_pair(itor->len,itor->val);
			}
		}
	}

	template<typename T>
	int get(int cnt,int fd_num)
	{
		if (cnt >= v_res.size() || cnt < 0)
		{
			throw cls_exception_overflow("cls_mysql_operator::get_int_value() - out of subscirpts.");
		}

		cls_mysql_rows *row = v_res[cnt];

		return make_pair(row->fields[fd_num].len,row->fields[fd_num].val);
	}

	unsigned get_query_size()
	{
		return v_res.size();
	}
};