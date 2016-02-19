#include "lib_import.h"

template <typename T>
class cls_search_breadth
	:cls_search<T>
{
public:

	virtual double measure( T& item ) = 0;
	virtual bool goal( T& item ) = 0;
	virtual vector<T*> next( T& item_cur ) = 0;
	virtual bool repeated_and_prunk(T& item) = 0;

	virtual T* search( T& item_init ) 
	{
		if (goal(item_init))
			return &item_init;

		list<T> que;
		que.push_back(item_init);

		while(!que.empty())
		{
			T item_cur = que.front();
			que.pop_front();

			vector<T*> items_next = next(item_cur);

			vector<T*>::iterator itor;
			for(itor = items_next.begin();
				  itor != items_next.end();
				  ++itor)
			{
				if (goal(*(*itor)))
				{
					return (*itor);
				}
				else
				{
					if (repeated_and_prunk(*(*itor)) == false)
						que.push_back(*(*itor));
				}

				delete *itor;
			}
		}

		return null;
	}
};