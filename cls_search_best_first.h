#include "lib_import.h"

template <typename T>
class cls_search_best_first
	:cls_search<T>
{
public:

	virtual double measure( T& item ) = 0;
	virtual bool goal( T& item ) = 0;
	virtual vector<T*> next( T& item_cur ) = 0;

	virtual T* search( T& item_init ) 
	{
		priority_queue<pair<double,T*>> que;
		que.push(&item_init);

		while(!que.empty())
		{
			T* item_cur = que.top().second;
			que.pop();

			vector<T*> items_next = next(item_cur);

			vector<T*>::iterator itor;
			for(itor = items_next.begin();
				itor != items_next.end();
				++itor)
			{
				if (goal(*(*itor)))
				{
					while(!que.empty())
					{
						delete que.top().second;
						que.pop();
					}

					return *itor;
				}
				else
				{
					queue.push(make_pair(measure(*(*itor)),*itor));
				}
			}

			delete item_cur;
		}

		return null;
	}
};