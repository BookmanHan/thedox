#include "lib_import.h"

//Make examples of 8-queens.

class test_item_queens
{
public:
	vector<pair<int,int> > vpnn;

	test_item_queens()
	{
		;
	}
};

class test_search_breadth
	:cls_search_breadth<test_item_queens>
{
public:

	virtual double measure( test_item_queens& item ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual bool goal( test_item_queens& item ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual bool repeated_and_prunk( test_item_queens& item ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual vector<test_item_queens*> next( test_item_queens& item_cur ) 
	{
		throw std::exception("The method or operation is not implemented.");
	}

};
