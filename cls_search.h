#include "lib_import.h"

template <typename T>
//T must override the operator ==
class cls_search
{
public:

	//judge if the item is repeated;
	virtual bool repeated_and_prunk(T& item) = 0;

	//how to measure a node;
	virtual double measure(T& item) = 0;

	//check if an item is a goal;
	virtual bool goal(T& item) = 0;

	//search, the core of the algrithm;
	virtual T* search(T& item_init) = 0;

	//get the next items,order the element by their measurement.
	virtual vector<T*> next(T& item_cur) = 0;
};