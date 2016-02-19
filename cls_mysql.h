#include "dlib_import.h"

class cls_mysql
	:public cls_database
{
private:
	cls_mysql_operator mysql;

public:
	cls_mysql(string host,unsigned port,string username,string password,string database)
		:mysql(host,port,username,password,database)
	{
		;
	}

	virtual void create_table( cls_database_create& param ) 
	{
		string sql = "";
		sql = sql + "create table " + param.name + "(";

		vector<cls_database_create_item>::iterator itor;
		for(itor=param.items.begin();itor < param.items.end()-1;itor++)
		{
			sql = sql + itor->name + " " + itor->type;

			if (itor->not_null)
				sql += " not null ";

			if (itor->primary_key)
				sql += "primary key";

			sql += " , ";
		}

		sql = sql + itor->name + " " + itor->type;

		if (itor->not_null)
			sql += " not null ";

		if (itor->primary_key)
			sql += "primary key";


		sql += ")";

		mysql.execute_non_query(sql);

		cout<<sql;
	}

	virtual void insert_row( cls_database_insert& param ) 
	{
		string sql = "";

		sql += "insert into " + param.name;
		sql += "(";

		vector<cls_database_insert_item>::iterator itor;
		for(itor = param.items.begin();itor < param.items.end()-1;itor++)
		{
			sql += itor->name;
			sql += ",";
		}

		sql += itor->name;
		sql += ") values(";

		for(itor=param.items.begin();itor < param.items.end()-1;itor++)
		{
			sql = sql + "'" + itor->val + "',";
 		}

		sql += + "'" + itor->val + "'";
		sql += ")";

		mysql.execute_non_query(sql);
	}
};