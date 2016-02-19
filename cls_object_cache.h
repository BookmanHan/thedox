#include "lib_import.h"

/*
	the class for cache memory mamangement;
*/
class cls_cache
	: cls_object
{
public:
	class cls_cache_item
	{
	public:
		//the data of cache.
		void *data;
		bool b_start;

	public:
		cls_cache_item()
		{
			b_start = false;
			data = null;
		}

		void set_cache(void *data,bool b_start)
		{
			this->data = data;
			this->b_start = b_start;
		}

		~cls_cache_item()
		{
			if (b_start)
				delete data;
		}
	};

protected:
	//cache pool.
	vector<cls_cache_item*> v_cache;

	//the first is the true addr,the second is the linear addr.
	map<void*,pair<int,int> > map_cache;

	//the first is size,the second is the start place;
	set<pair<int,int> > set_unalloc;

public:
	//init the cache management;
	cls_cache()
	{
		set_unalloc.insert(make_pair(0,0));
		add_cache();
	}

	//add n blocks of cache item;
	void add_cache(int n = 1)
	{
		int num_org = v_cache.size();

		void *new_block = new unsigned char[n*4096];

		for(int cnt=0;cnt<n;cnt++)
		{
			cls_cache_item* item = new cls_cache_item();
			item->set_cache((char*)new_block + cnt*4096,cnt==0);
			v_cache.push_back(item);
		}

		set_unalloc.insert(make_pair(n*4096,num_org*4096));
	}

	//remove the index's cache item;
	void remove_cache(int indx)
	{
		v_cache.erase(v_cache.begin() + indx);
	}

	//get the item num for linar space n;
	int get_item_num(int n)
	{
		return n/4096;
	}

	//get the innner item position for linar space n;
	int get_item_inner(int n)
	{
		return n%4096;
	}

	//find a space for allocing;
	virtual int find_space(int size) = 0;

	//shrink the total size of cache pool;
	virtual void shrink_block(int from,int end) = 0;

	//return the space for allocing;
	virtual void return_space( void * p )
	{
		map<void*,pair<int,int> >::iterator itor = map_cache.find(p);
		if (itor == map_cache.end())
			throw cls_exception_cache_error("cls_object_cache_first_fit::return_space() - the space is not own.\n");

		set_unalloc.insert(make_pair(itor->second.second,itor->second.first));
	}

	//Check if is the alloc memory;
	virtual bool check_alloc_space(void* p)
	{
		if (map_cache.find(p) == map_cache.end())
			return false;
		else
			return true;
	}

	//union the adjusting blocks.
	void union_block()
	{
		set<pair<int,int> >::iterator itor;
		set<pair<int,int> > set_tmp;

		for(itor=set_unalloc.begin();itor != set_unalloc.end();itor++)
		{
			set_tmp.insert(make_pair(itor->second,itor->first));
		}

		set_unalloc.clear();

		int pos_beg = -1;
		int pos_end = -1;
		for(itor=set_tmp.begin();itor != set_tmp.end();itor++)
		{
			if (pos_end == -1 || pos_beg == -1)
			{
				pos_beg = itor->first;
				pos_end = itor->first + itor->second;
			}
			else if (pos_end == itor->first)
			{
				pos_end += itor->second;
			}
			else
			{
				set_unalloc.insert(make_pair(pos_beg,pos_end));
				pos_beg = -1;
				pos_end = -1;
			}
		}
	}

	//get space from cache;
	void* get_space(int size)
	{
		int pos = find_space(size);
		int page = get_item_num(pos);
		int inner = get_item_inner(pos);

		map_cache.insert(make_pair((char*)v_cache[page]->data + inner,make_pair(pos,size)));

		union_block();

		return (char*)v_cache[page]->data + inner;
	}

	virtual ~cls_cache()
	{
		vector<cls_cache_item*>::iterator itor;
		for(itor=v_cache.begin();itor != v_cache.end();itor++)
		{
			delete (*itor);
		}
	}
};

/*
	the best-fit management for cache;
*/
class cls_cache_best_fit
	:public cls_cache
{
protected:
	int cnt_union_block;

public:
	cls_cache_best_fit()
	{
		cnt_union_block = 10;
	}

	virtual int find_space( int size )
	{
		set<pair<int,int> >::iterator itor;

		itor = set_unalloc.upper_bound(make_pair(size,0));

		if (itor == set_unalloc.end() || itor->first < size)
		{
			add_cache(max((size/4096)+1,10));
		}

		itor = set_unalloc.upper_bound(make_pair(size,0));

		int ipos = itor->second;
		int isize = itor->first;

		set_unalloc.erase(itor);
		set_unalloc.insert(make_pair(isize-size,ipos+size));

		return ipos;
	}

	virtual void shrink_block( int from,int end )
	{
		throw cls_exception_not_implemented("cls_object_cache_first_fit::shrink_block() - not implemenent.\n");
	}

	virtual void return_space( void * p )
	{
		cls_cache::return_space(p);

		if (--cnt_union_block == 0)
		{
			cnt_union_block = 10;
			union_block();
		}
	}
};

/*
	the more-fit management for cache;
*/
class cls_cache_more_fit
	:public cls_cache
{
protected:
	int cnt_union_block;

public:
	cls_cache_more_fit()
	{
		cnt_union_block = 10;
	}

	virtual int find_space( int size )
	{
		throw cls_exception_not_implemented("cls_cache_more_fit::find_space() - under working.\n");

		//set<pair<int,int> >::iterator itor = set_unalloc.end() - 1;

		//if (itor->first < size)
		//{
		//	add_cache(max((size/4096)+1,10));
		//}

		//itor = set_unalloc.rend();

		//int ipos = itor->second;
		//int isize = itor->first;

		//set_unalloc.erase(itor);
		//set_unalloc.insert(make_pair(isize-size,ipos+size));

		return -1;
	}

	virtual void shrink_block( int from,int end )
	{
		throw cls_exception_not_implemented("cls_object_cache_first_fit::shrink_block() - not implemenent.\n");
	}
};
