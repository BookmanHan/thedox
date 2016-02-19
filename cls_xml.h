#include "lib_import.h"

class cls_xml_attribut_item
{
public:

	enum {en_string,en_int,en_double} type;
	struct
	{
		string val_str;
		int val_int;
		double val_double;
	}val;
};

class cls_xml_node
{
public:

	string tag;

	map<string,cls_xml_attribut_item> attributes;
	vector<cls_xml_node*> children;
	string context;

	~cls_xml_node()
	{
		vector<cls_xml_node*>::iterator itor;
		for(itor=children.begin();itor!=children.end();itor++)
		{
			delete (*itor);
		}
	}

	void set_attribute(string name,string str)
	{
		cls_xml_attribut_item item;
		item.type = cls_xml_attribut_item::en_string;
		item.val.val_str = str;

		attributes[name] = item;
	}

	void set_attribute(string name,int str)
	{
		cls_xml_attribut_item item;
		item.type = cls_xml_attribut_item::en_int;
		item.val.val_int = str;

		attributes[name] = item;
	}

	void set_attribute(string name,cls_xml_attribut_item& item)
	{
		attributes[name] = item;
	}

	void set_attribute(string name,double str)
	{
		cls_xml_attribut_item item;
		item.type = cls_xml_attribut_item::en_double;
		item.val.val_double = str;

		attributes[name] = item;
	}

	cls_xml_attribut_item& get_attribute(string name)
	{
		if (attributes.find(name) != attributes.end())
		{
			throw cls_exception_not_found(string(__FUNCTION__) + "() - the attribute with the name is not found.\n");
		}

		return attributes[name];
	}

	vector<cls_xml_node*>& get_children()
	{
		return children;
	}

	void add_children(cls_xml_node* new_node)
	{
		children.push_back(new_node);
	}

	void delete_children(vector<cls_xml_node*>::iterator itor)
	{
		delete (*itor);
		children.erase(itor);
	}

	vector<cls_xml_node*>::iterator begin()
	{
		return children.begin();
	}

	vector<cls_xml_node*>::iterator end()
	{
		return children.end();
	}

	unsigned size()
	{
		return children.size();
	}

	void find_node_by_tag(const string& tag,vector<cls_xml_node*>& re)
	{
		if (this->tag == tag)
		{
			re.push_back(this);
		}

		vector<cls_xml_node*>::iterator itor;
		for(itor=children.begin();itor!=children.end();itor++)
		{
			(*itor)->find_node_by_tag(tag,re);
		}
	}
};

class cls_xml
{
public:
	cls_xml_node* root;

public:
	vector<cls_xml_node*> find_node_by_tag(const string & tag)
	{
		vector<cls_xml_node*> re;
		root->find_node_by_tag(tag,re);

		return re;
	}
};
