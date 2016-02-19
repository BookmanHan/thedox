#include "lib_import.h"

template <typename T>
class cls_search_depth
	:cls_search<T>
{
	
public:

	virtual double measure( T& item ) = 0;
	virtual bool goal( T& item ) = 0;
	virtual vector<T*> next( T& item_cur ) = 0;

	virtual T* search( T& item_init ) 
	{
		stack<T*> stk;
		stk.push(&item_init);

		while(!stk.empty())
		{
			T* item_cur = stk.top();
			stk.pop();

			vector<T*> items_next = next(item_cur);
			vector<T*>::iterator itor;

			for(itor=items_next.begin();
				  itor!=items_next.end();
				  ++itor)
			{
				if (goal(*(*itor)))
				{
					while(!stk.empty())
					{
						delete stk.top();
						stk.pop();
					}
				}
				else
				{
					stk.push(*itor);
				}
			}

			delete item_cur;
		}

		return null;
	}

};