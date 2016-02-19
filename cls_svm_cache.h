#include "lib_import.h"

class cls_svm_cache
{
public:
	cls_svm_cache(int l,long size)
	{
		this->l = l;
		this->size = size;

		head = new head_t[l];

		size /= sizeof(double);
		size -= max((int)size,2*l);
		lru_head.next = lru_head.prev = &lru_head;
	}

	~cls_svm_cache()
	{
		for(head_t* h=lru_head.next;h != &lru_head; h= h->next)
		{
			delete[] h->data;
		}

		delete[] head;
	}

	int get_data(const int index,double **data,int len)
	{
		head_t *h = &head[index];
		if (h->len)
			lru_delete(h);

		int more = len - h->len;
		if (more > 0)
		{
			while(size < more)
			{
				head_t *old = lru_head.next;
				lru_delete(old);
				delete old->data;

				size += old->len;
				old->data = null;
				old->len = 0;
			}

			double *tmp = new double[len];
			if (h->data != null)
			{
				memcpy(tmp,h->data,len*sizeof(double));
				delete[] h->data;
			}

			h->data = tmp;
			size-= more;
			swap(h->len,len);
		}

		lru_insert(h);
		*data = h->data;
		return len;
	}

	void swap_index(int i,int j)
	{
		if (i == j)
			return;

		if (head[i].len)
			lru_delete(&head[i]);

		if (head[j].len)
			lru_delete(&head[j]);

		swap(head[i].data,head[j].data);
		swap(head[i].len,head[j].len);

		if (head[i].len) lru_insert(&head[i]);
		if (head[j].len) lru_insert(&head[j]);

		if (i > j)
			swap(i,j);

		for(head_t* h=lru_head.next; h != &lru_head;h=h->next)
		{
			if (h->len > i)
			{
				if (h->len > j)
				{
					swap(h->data[i],h->data[j]);
				}
				else
				{
					lru_delete(h);
					delete h->data;
					size += h->len;
					h->data = 0;
					h->len = 0;
				}
			}
		}
	}

private:
	int l;
	long int size;

	struct head_t
	{
		head_t *prev,*next;
		double *data;
		int len;

	public:
		head_t()
		{
			prev = next = null;
			data = null;
			len = 0;
		}
	};

	head_t *head;
	head_t lru_head;

	void lru_delete(head_t *h)
	{
		h->prev->next = h->next;
		h->next->prev = h->prev;
	}

	void lru_insert(head_t *h)
	{
		h->next = &lru_head;
		h->prev = lru_head.prev;
		h->prev->next = h;
		h->next->prev = h;
	}
};
