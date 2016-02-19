#include "lib_import.h"

template <typename T>
class cls_search_depth_limit
	:cls_search<T>
{
protected:
	int n_level;

public:
	cls_search_depth_limit(int level)
	{
		n_level = level;
	}

	virtual double measure( T& item ) = 0;
	virtual bool goal( T& item ) = 0;
	virtual vector<T*> next( T& item_cur ) = 0;

	virtual T* search( T& item_init ) 
	{
		stack<pair<T*,int>> stk;
		stk.push(make_pair(&item_init,0));

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
			}

			delete item_cur;
		}

		return null;
	}

};