#include "lib_import.h"

void test_cluster_k_means()
{
	double src [][4] = 
	{
		{0,0,0,0},
		{0.1,0.1,0.1,0.1},
		{10,10,10,10},
		{10,10,10,10},
		{100,100,100,100},
		{100,100,100,100},
		{1000,1000,1000,1000},
		{1000,1000,1000,1000},
	};

	cls_cluster_k_means<cls_conject_linear_euclid> cluster(4,4);
	cls_training_set ts(4,0);
	
	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair(src[cnt],src[cnt]);
	}

	vector<int> re = cluster.train_example(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<re[cnt]<<endl;
	}
}

void test_cluster_hierarchical()
{
	double src [][4] = 
	{
		{0,0,0,0},
		{0.1,0.1,0.1,0.1},
		{10,10,10,10},
		{10.1,10.1,10.1,10.1},
		{100,100,100,100},
		{100.1,100.1,100.1,100.1},
		{1000,1000,1000,1000},
		{1000.1,1000.1,1000.1,1000.1},
	};

	cls_cluster_hierarchical<cls_conject_linear_euclid> cluster(4);
	cls_training_set ts(4,0);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair(src[cnt],src[cnt]);
	}

	vector<int> re = cluster.train_example(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<re[cnt]<<endl;
	}
}

void test_cluster_max_min_distance()
{
	double src [][4] = 
	{
		{0,0,0,0},
		{0.1,0.1,0.1,0.1},
		{10,10,10,10},
		{10,10,10,10},
		{100,100,100,100},
		{100,100,100,100},
		{1000,1000,1000,1000},
		{1000,1000,1000,1000},
	};

	cls_cluster_max_min_distance<cls_conject_linear_euclid> cluster(0.00001);
	cls_training_set ts(4,0);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair(src[cnt],src[cnt]);
	}

	vector<int> re = cluster.train_example(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<re[cnt]<<endl;
	}
}

void test_cluster_isoda()
{
	double src [][4] = 
	{
		{0,0,0,0},
		{2,2,2,2},
		{4,4,4,4},
		{8,8,8,8},
	};

	cls_cluster_isoda<cls_conject_linear_euclid> cluster(6,4,0,1,0.1,0.1,0.5,100);
	cls_training_set ts(4,0);

	for(int cnt=0;cnt<4;cnt++)
	{
		ts.add_io_pair(src[cnt],src[cnt]);
	}

	vector<int> re = cluster.train_example(ts);

	for(int cnt=0;cnt<4;cnt++)
	{
		cout<<re[cnt]<<endl;
	}
}