#include "lib_import.h"

#include "lib_import.h"

template <typename T>
class cls_search_hill_climbing
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
			vector<pair<double,T*> > items_pd;

			vector<T*>::iterator itor;

			for(itor=items_next.begin();
				  itor != items_next.end();
				  ++itor)
			{
				items_pd.push_back(make_pair(measure(*(*itor)),*itor));
			}

			sort(items_pd.begin(),items_pd.end());

			vector<pair<double,T*> >::iterator itor_vpd;

			for(itor_vpd=items_pd.begin();
				itor_vpd!=items_pd.end();
				++itor_vpd)
			{
				if (goal(*(*itor_vpd)))
				{
					while(!stk.empty())
					{
						delete stk.top();
						stk.pop();
					}
				}
				else
				{
					stk.push(*itor_vpd);
				}
			}

			delete item_cur;
		}

		return null;
	}

};