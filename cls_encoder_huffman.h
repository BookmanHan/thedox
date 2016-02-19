#include "lib_import.h"

template<class T>
class cls_encoder_huffman
{
private:
	T * data;
	unsigned char *output_data;

	int num;

	map<T,int> map_context;
	map<T,string> codes;

	struct itag_huff_tree_node
	{
		T context;
		itag_huff_tree_node* left;
		itag_huff_tree_node* right;
		double pro;

		bool operator<(const itag_huff_tree_node&that) const
		{
			if (this->pro < that.pro)
				return true;
			else
				return false;
		}

		bool operator>(const itag_huff_tree_node&that) const
		{
			if (this->pro > that.pro)
				return true;
			else
				return false;
		}
	};

	priority_queue<itag_huff_tree_node,vector<itag_huff_tree_node>,greater<itag_huff_tree_node> > pq_tree;

	void make_map_context()
	{
		for(int cnt=0;cnt<num;cnt++)
		{
			if (map_context.end() == map_context.find(data[cnt]))
			{
				map_context[data[cnt]] = 1;
			}
			else
			{
				map_context[data[cnt]] ++;
			}
		}
	}

	void make_huff_tree()
	{
	    #ifdef __USING_GCC__
		typename map<T,int>::iterator itor = map_context.begin();
		#else
		map<T,int>::iterator itor = map_context.begin();
		#endif

		while(itor != map_context.end())
		{
			itag_huff_tree_node node;
			node.left = NULL;
			node.right = NULL;
			node.pro = itor->second;
			node.context = itor->first;

			pq_tree.push(node);
			itor++;
		}

		while(pq_tree.size() > 1)
		{
			itag_huff_tree_node *n1 = new itag_huff_tree_node;
			*n1 = pq_tree.top();
			pq_tree.pop();
			itag_huff_tree_node *n2 = new itag_huff_tree_node;
			*n2 = pq_tree.top();
			pq_tree.pop();

			itag_huff_tree_node n_re;
			n_re.left = n1;
			n_re.right = n2;
			n_re.pro = n1->pro + n2->pro;

			pq_tree.push(n_re);
		}
	}

	void set_huff_code()
	{
		make_map_context();
		make_huff_tree();
		i_set_huff_code_for_pq_tree(&(pq_tree.top()),"");
	}

	void i_set_huff_code_for_pq_tree(itag_huff_tree_node *node,string re)
	{
		if (node->right == NULL && node->left == NULL)
		{
			codes.insert(make_pair(node->context,re));
		}

		if (node->right != NULL)
		{
			string re_p = re;
			re_p +="1";
			i_set_huff_code_for_pq_tree(node->right,re_p);
		}

		if (node->left != NULL)
		{
			string re_p = re;
			re_p +="0";
			i_set_huff_code_for_pq_tree(node->left,re_p);
		}
	}

	int i_encode()
	{
		unsigned char*pout = output_data;
		int nout = 0;
		int re = 0;

		for(int cnt=0;cnt<num;cnt++)
		{
			string code = codes[data[cnt]];

			for(int cnt_c = 0;cnt_c<code.size();cnt_c++)
			{
				if (nout == 8)
				{
					re++;
					pout++;
					nout = 0;
				}

				if(code[cnt_c] == '0')
				{
					*pout &= ~(1<<nout);
					nout++;
				}
				else
				{
					*pout |= (1<<nout);
					nout++;
				}
			}
		}

		return re;
	}

public:
	cls_encoder_huffman(T *a_data,int a_num)
	{
		data = a_data;
		num = a_num;
		output_data = new unsigned char[sizeof(T)*num];
	}

	map<T,string>& get_codes()
	{
		return codes;
	}

	pair<unsigned char*,int> encode()
	{
		set_huff_code();
		return make_pair(output_data,i_encode());
	}

	~cls_encoder_huffman()
	{
		delete[] output_data;
	}
};
