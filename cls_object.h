#pragma  once
#include "lib_import.h"

//the basic object class for our the classes in the thedox
class cls_object
{
private:
	
	//the id of the object;
	int id;

public:
	cls_object()
	{
		//get static id number for the current id;
		static int cur_id = 1;

		id = cur_id;
		cur_id ++;
	}

	//the method for getting the id and there is no method for overwriting the id.
	int __get_id()
	{
		return id;
	}
};

//the obj extends from this class will have the ability to save and load.
class cls_serialized_object 
	:virtual public cls_object
{
public:

	//the virtual function for saving.
	virtual void save(ofstream& fout) const = 0;

	//the virtual function for loading.
	virtual void load(ifstream& fin) = 0;
	
	//the basic function for override function;
	void save(const string & str_filename) const
	{
		ofstream fout(str_filename.c_str());
		save(fout);
		fout.close();
	}

	//the basic function for override function;
	void load(const string & str_filename) 
	{
		ifstream fin(str_filename.c_str());
		load(fin);
		fin.close();
	}
};