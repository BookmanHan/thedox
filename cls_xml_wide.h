#include "lib_import.h"

class cls_xml_attribut_item_wide
{
public:

	enum {en_string,en_int,en_double} type;
	struct
	{
		wstring val_str;
		int val_int;
		double val_double;
	}val;
};

class cls_xml_node_wide
{
public:

	wstring tag;

	map<wstring,cls_xml_attribut_item_wide> attributes;
	vector<cls_xml_node_wide*> children;
	wstring context;

	~cls_xml_node_wide()
	{
		vector<cls_xml_node_wide*>::iterator itor;
		for(itor=children.begin();itor!=children.end();itor++)
		{
			delete (*itor);
		}
	}

	void set_attribute(wstring name,wstring str)
	{
		cls_xml_attribut_item_wide item;
		item.type = cls_xml_attribut_item_wide::en_string;
		item.val.val_str = str;

		attributes[name] = item;
	}

	void set_attribute(wstring name,int str)
	{
		cls_xml_attribut_item_wide item;
		item.type = cls_xml_attribut_item_wide::en_int;
		item.val.val_int = str;

		attributes[name] = item;
	}

	void set_attribute(wstring name,cls_xml_attribut_item_wide& item)
	{
		attributes[name] = item;
	}

	void set_attribute(wstring name,double str)
	{
		cls_xml_attribut_item_wide item;
		item.type = cls_xml_attribut_item_wide::en_double;
		item.val.val_double = str;

		attributes[name] = item;
	}

	cls_xml_attribut_item_wide& get_attribute(wstring name)
	{
		if (attributes.find(name) == attributes.end())
		{
			throw cls_exception_not_found(string(__FUNCTION__) + "() - the attribute with the name is not found.\n");
		}

		return attributes[name];
	}

	vector<cls_xml_node_wide*>& get_children()
	{
		return children;
	}

	void add_children(cls_xml_node_wide* new_node)
	{
		children.push_back(new_node);
	}

	void delete_children(vector<cls_xml_node_wide*>::iterator itor)
	{
		delete (*itor);
		children.erase(itor);
	}

	vector<cls_xml_node_wide*>::iterator begin()
	{
		return children.begin();
	}

	vector<cls_xml_node_wide*>::iterator end()
	{
		return children.end();
	}

	unsigned size()
	{
		return children.size();
	}

	void find_node_by_tag(const wstring& tag,vector<cls_xml_node_wide*>& re)
	{
		if (this->tag == tag)
		{
			re.push_back(this);
		}

		vector<cls_xml_node_wide*>::iterator itor;
		for(itor=children.begin();itor!=children.end();itor++)
		{
			(*itor)->find_node_by_tag(tag,re);
		}
	}

	cls_xml_node_wide* get_node_by_id(const wstring& id)
	{
		map<wstring,cls_xml_attribut_item_wide>::iterator itor = attributes.find(L"id");

		if (itor != attributes.end() && itor->second.val.val_str == id)
		{	
			return this;
		}
		else
		{
			vector<cls_xml_node_wide*>::iterator itor;
			for(itor=children.begin();itor!=children.end();++itor)
			{
				cls_xml_node_wide* node = (*itor)->get_node_by_id(id);
				if (node != null)
					return node;
			}
		}	

		return null;
	}
};

class cls_xml_wide
{
public:
	cls_xml_node_wide* root;

public:
	vector<cls_xml_node_wide*> find_node_by_tag(const wstring & tag)
	{
		vector<cls_xml_node_wide*> re;
		root->find_node_by_tag(tag,re);

		return re;
	}

	cls_xml_node_wide* find_node_by_id(const wstring& id)
	{
		cls_xml_node_wide* node;
		node = root->get_node_by_id(id);

		return node;
	}
};
