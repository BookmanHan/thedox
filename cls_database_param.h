#include "dlib_import.h"

class cls_database_create_item
{
public:
	string name;
	string type;
	bool not_null;
	bool primary_key;
};

class cls_database_create
{
public:
	string name;
	vector<cls_database_create_item> items;

public:
	cls_database_create(string a_name)
		:name(a_name)
	{
		;
	}

	virtual void add_field(string name,string type,bool not_null = false,bool pk = false)
	{
		cls_database_create_item item;
		item.name = name;
		item.type = type;
		item.not_null = not_null;
		item.primary_key = pk;

		items.push_back(item);
	}
};

class cls_database_insert_item
{
public:
	string name;
	string val;
};

class cls_database_insert
{
public:
	string name;
	vector<cls_database_insert_item> items;

public:
	cls_database_insert(string a_table_name)
		:name(a_table_name)
	{
		;
	}

	virtual void add_field(string name,string val)
	{
		cls_database_insert_item item;
		item.name = name;
		item.val = val;

		items.push_back(item);
	}
};