#include "lib_import.h"

template<typename T>
cls_static_tree
{
protected:
	class cls_tree_node
	{
		T val;
		int parent;
		set<int> child;
	};

	vector<cls_tree_node> tree;
	set<int> root;

public:
	cls_static_tree
	{
		;
	}

	int add_node(T val,int parent)
	{
		cls_tree_node node;
		node.val = val;
		node.parent = parent;
		node.child.clear();

		tree.insert(node);
	}

	int add_root(T val)
	{
		cls_tree_node node;
		node.val = val;
		node.parent = -1;
		node.child.clear();

		tree.push_back(node);
		root.insert(tree.size()-1);
	}

	void connect_child(int node_want_to_connect,int node_be_connected)
	{
		if (tree[node_want_to_connect].parent == -1)
		{
			tree[node_want_to_connect].parent = node_be_connected;
			tree[node_be_connected].child.insert(node_want_to_connect);
			root.erase(node_want_to_connect);
		}
		else
		{
			throw cls_exception_execution("cls_static_tree::connect_child() - it can only connect root node.\n");
		}
	}

	int add_child(T val,int node)
	{
		return add_node(val,node);
	}

	T& get_value(int node)
	{
		return tree[node].val;
	}

	const int get_parent(int node)
	{
		return tree[node].parent;
	}

	const set<int>& get_child(int node)
	{
		return tree[node].child;
	}

	void disconnect(int node)
	{
		int par = tree[node].parent;
		//TODO
	}

	//TO BE CONTINUE//
};