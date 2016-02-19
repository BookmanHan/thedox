#include "cis_import.h"

class cls_cis_symbol_item
{
public:
	cls_cdox_addr addr;
	unsigned int class_id;
	cls_cis_symbol_item* parent;
	
	vector<cls_cis_symbol_item*> extends;
	map<wstring,cls_jrunner_symbol_item*> mb_vars;
	map<wstring,cls_jrunner_symbol_item*> mb_funcs;

private:
	int offset;

	static unsigned int get_class_id()
	{
		static unsigned int class_id = 10;
		return class_id++;
	}

public:
	cls_cis_symbol_item(cls_cis_symbol_item* parent)
	{
		class_id = get_class_id();
		this->parent = parent;
	}

	cls_cis_symbol_item get_mem_var(cls_cis_symbol_item* parent)
	{

	}
};