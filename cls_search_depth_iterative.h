#include "lib_import.h"

template <typename T>
class cls_search_depth_iterative
	:cls_search<T>
{
protected:
	int n_level;
	int n_add_level;

public:
	cls_search_depth_iterative(int add_level)
	{
		n_add_level = add_level;
		n_level = n_add_level;
	}

	virtual double measure( T& item ) = 0;
	virtual bool goal( T& item ) = 0;
	virtual vector<T*> next( T& item_cur ) = 0;

	virtual T* search( T& item_init ) 
	{
		stack<pair<T*,int> > stk;
		stack<pair<T*,int> > stk_back;

		stk.push(make_pair(&item_init,0));

		while(true)
		{
			while(!stk.empty())
			{
				T* item_cur = stk.top().first;
				int level = stk.top().second;

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
							delete stk.top().first;
							stk.pop();
						}
					}
					else if (level <= n_level)
					{
						stk.push(make_pair(*itor,level+1));
					}
					else
					{
						stk_back.push(make_pair(*itor,level++));
					}
				}

				delete item_cur;
			}

			stk = stk_back;
			n_level += n_add_level;
		}

		return null;
	}

};