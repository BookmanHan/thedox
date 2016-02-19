#include "lib_import.h"

void test_ann_add()
{
	srand(time(null));

	cls_training_set ts(2,1);
	double input[2];
	double output[1];

	for(int i=0;i<100;i++)
	{
		for(int j=0;j<100;j++)
		{
			input[0] = static_cast<double>(i) / 200;
			input[1] = static_cast<double>(j) / 200;
			output[0] = input[0] + input[1];

			ts.add_io_pair(input,output);
		}
	}

	cls_ann_network_bilayer<a_sigmod> net(2,4,1);
	net.connect_all();

	cout<<"training starts..."<<endl;
	net.train(ts,1000,0.5,0.0);
	cout<<"training ends..."<<endl;

	while(true)
	{
		double a,b;
		cin>>a>>b;
		input[0] = a/200;
		input[1] = b/200;

		cout<<net.get_output(input)[0]*200<<endl;
	}
}

void test_ann_xor()
{
	srand(time(null));

	double input1[]={0,0}; double output1[]={0};
	double input2[]={1,0}; double output2[]={1};
	double input3[]={0,1}; double output3[]={1};
	double input4[]={1,1}; double output4[]={0};

	cls_ann_network_bilayer<a_tansig> canb(2,6,1);
	canb.connect_all();
	cls_training_set ts(2,1);
	ts.add_io_pair(input1,output1);
	ts.add_io_pair(input2,output2);
	ts.add_io_pair(input3,output3);
	ts.add_io_pair(input4,output4);

	vector<double> output;
	canb.train(ts,100,0.3,0.6);

	ofstream fout("a.net",ios::binary);
	canb.save(fout);
	fout.close();

	ifstream fin("a.net",ios::binary);
	canb.load(fin);
	fin.close();

	output = canb.get_output(input2);
	cout<<output[0]<<endl;
	cout<<endl;
}

void test_ann_hopfield()
{
	srand(time(null));

	int	pattern_org[400][32];
	int pattern_noi[400][32];
	for(int cnt=0;cnt<400;cnt++)
	{
		for(int i=0;i<32;i++)
		{
			pattern_org[cnt][i] = (rand()%2==0)?-1:1;

			if (rand() % 100 < 0)
				pattern_noi[cnt][i] = -pattern_org[cnt][i];
			else
				pattern_noi[cnt][i] = pattern_org[cnt][i];
		}
	}

	cls_ann_network_hopfield<a_signum> net(32);
	for(int cnt=0;cnt<400;cnt++)
	{
		net.add_pattern(pattern_org[cnt]);
	}

	cout<<"choose：0-"<<400<<":";
	int n_choose;
	cin>>n_choose;

	net.set_input(pattern_noi[n_choose]);
	if (!net.propagate(pattern_noi[n_choose],2000))
		cout<<"not included.";
	else
	{
		vector<int> o = net.get_output_int();
		for(int cnt=0;cnt<32;cnt++)
		{
			cout<<pattern_org[n_choose][cnt]<<"\t"<<o[cnt]<<endl;
		}
	}

	ofstream fout("a.net",ios::binary);
	net.save(fout);
	fout.close();

	ifstream fin("a.net",ios::binary);
	net.load(fin);
	fin.close();

	while(true)
	{
		cout<<"choose：0-"<<400<<":";
		int n_choose;
		cin>>n_choose;

		net.set_input(pattern_noi[n_choose]);
		if (!net.propagate(pattern_noi[n_choose],2000))
			cout<<"not included.";
		else
		{
			vector<int> o = net.get_output_int();
			for(int cnt=0;cnt<32;cnt++)
			{
				cout<<pattern_org[n_choose][cnt]<<"\t"<<o[cnt]<<endl;
			}
		}
	}
}

void test_random(vector<double> &re,int n,double d_a = 1.0)
{
	re.resize(n);
	for(int cnt=0;cnt<n;cnt++)
	{
		re[cnt] = a_unfirom_random(0,1) * d_a;
	}
}

void test_make_one(vector<double> &re,int n,int pos)
{
	re.resize(n,0);
	re[pos] = 1;
}

void test_ann_radial_basis()
{
	int cnt_num = 10;
	srand(time(null));

	cls_ann_network_radial_basis<a_gaussian,a_identity> canb(10,6,1);
	cls_training_set ts(10,1);

	vector<vector<double> > mat;
	for(int cnt=0;cnt<cnt_num;cnt++)
	{
		vector<double> d_i;
		vector<double> d_o;
		d_o.push_back(cnt);
		test_make_one(d_i,10,cnt);
		ts.add_io_pair(d_i,d_o);
		mat.push_back(d_i);
	}

	int n_times = 300;
	while(n_times--)
		canb.train_weights(ts);

	ofstream fout("a.net",ios::binary);
	canb.save(fout);
	fout.close();

	ifstream fin("a.net",ios::binary);
	canb.load(fin);
	fin.close();

	for(int cnt=0;cnt<cnt_num;cnt++)
	{
		cout<<canb.get_output(mat[cnt])[0]<<endl;
	}
}

void test_ann_kohonen()
{
	int num_in = 4;
	int num_feature = 4;

	unsigned dims[2] = {4,4};

	cls_ann_eucledian_topology<2> top(dims);
	cls_ann_param_kohonen_standard param(top.get_max_dim(),1);
	cls_ann_network_kohonen<a_sigmod> net(num_feature,top,param);
	cls_training_set ts(num_feature,num_feature);

	vector<vector<double> > mat;
	for(int cnt=0;cnt<num_in;cnt++)
	{
		vector<double> d;
		test_random(d,num_feature);
		ts.add_io_pair(d,d);
		mat.push_back(d);
	}

	vector<int> index;
	net.train(ts,2);
	for(int cnt=0;cnt<num_in;cnt++)
	{
		index.push_back(net.get_winner_output(mat[cnt]));
		for(int t=0;t<10;t++)
		{
			mat[cnt][rand()%num_feature] += 0.01;
			mat[cnt][rand()%num_feature]  -= 0.01;
		}
	}

	ofstream fout("a.net",ios::binary);
	net.save(fout);
	fout.close();

	ifstream fin("a.net",ios::binary);
	net.load(fin);
	fin.close();

	for(int cnt=0;cnt<num_in;cnt++)
	{
		cout<<index[cnt]<<"\t";
		cout<<net.get_winner_output(mat[cnt])<<endl;
	}

	exit(0);
};

void test_linear_sensor()
{

	double src[4][2] =
	{
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	double des[4][1] =
	{
		{10},
		{11},
		{12},
		{13},
	};

	cls_linear_sensor clser(4,2,1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<4;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	ofstream fout("a.net");
	clser.save(fout);
	fout.close();

	ifstream fin("a.net");
	clser.load(fin);
	fin.close();

	while(true)
	{
		int n;
		cin>>n;
		cout<<"re:"<<ts.get_output_for_id(clser.get_output(src[n]))[0]<<endl;
	}
};

void test_linear_sensor_gnz()
{

	double src[4][2] =
	{
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	double des[4][1] =
	{
		{10},
		{11},
		{12},
		{13},
	};

	cls_linear_sensor_grad_normz clser(4,2,1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<4;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	ofstream fout("a.net");
	clser.save(fout);
	fout.close();

	ifstream fin("a.net");
	clser.load(fin);
	fin.close();

	while(true)
	{
		int n;
		cin>>n;
		cout<<"re:"<<ts.get_output_for_id(clser.get_output(src[n]))[0]<<endl;
	}
};


void test_linear_sensor_inc()
{
	double src[4][2] =
	{
		{0,0},
		{1,0},
		{0,1},
		{1,1},
	};

	double des[4][1] =
	{
		{10},
		{11},
		{12},
		{13},
	};

	cls_linear_sensor_inctreasement clser(4,2,10);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<4;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	//ofstream fout("a.net");
	//clser.save(fout);
	//fout.close();

	//ifstream fin("a.net");
	//clser.load(fin);
	//fin.close();

	while(true)
	{
		int n;
		cin>>n;
		cout<<"re:"<<ts.get_output_for_id(clser.get_output(src[n]))[0]<<endl;
	}
};

void test_linear_sensor_wh()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_linear_wh clser(4,2,1.0);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	ofstream fout("a.net");
	clser.save(fout);
	fout.close();

	ifstream fin("a.net");
	clser.load(fin);
	fin.close();

	while(true)
	{
		int n;
		cin>>n;
		int re = 0;
		re = clser.get_output(src[n]);
		cout<<ts.get_output_for_id(re)[0]<<endl;
	}
};

void test_linear_fisher()
{
	double src[][2] =
	{
		{10,12},
		{11,15},
		{20,23},
		{21,28},
		{30,35},
		{31,32},
		{40,42},
		{41,48},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_linear_fisher clser(4,2);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	//ofstream fout("a.net");
	//clser.save(fout);
	//fout.close();

	//ifstream fin("a.net");
	//clser.load(fin);
	//fin.close();

	while(true)
	{
		int n;
		cin>>n;
		int re = 0;
		re = clser.get_output(src[n]);
		if (re != -1)
			cout<<ts.get_output_for_id(re)[0]<<endl;
	}
}

void test_linear_lmse()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_linear_lmse clser(8,2);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	while(true)
	{
		int n;
		cin>>n;
		int re = 0;
		re = clser.get_output(src[n]);
		cout<<ts.get_output_for_id(re)[0]<<endl;
	}
}


void test_linear_multiply_lmse()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_linear_mutiply_lmse clser(4,2);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts,1000);

	ofstream fout("D:\\trys\\b.net",ios::binary);
	clser.save(fout);
	fout.flush();
	fout.close();

	ifstream fin("D:\\trys\\b.net",ios::binary);
	clser.load(fin);
	fin.close();

	while(true)
	{
		int n;
		cin>>n;
		int re = 0;
		re = clser.get_output(src[n]);
		if (re != -1)
			cout<<ts.get_output_for_id(re)[0]<<endl;
	}
}

void test_svm()
{
	double src[][2] =
	{
		{1,0.01},
		{0.9,0.1},
		{0.01,1},
		{0.1,0.9},
		{0.01,0.01},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{1},
		{1},
		{2},
		{2},
		{3},
		{3},
		{4},
		{4},
	};

	cls_svm svm;
	cls_svm_problem prob;
	cls_svm_param param;

	param.svm_type = en_c_svc;
	param.kernel_type = en_sigmoid;
	param.degree = 3;
	param.gamma = 0.1;	// 1/k
	param.coef = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.c = 1;
	param.eps = 0.01;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = null;
	param.weight = null;

	prob.l = 8;
	prob.y = new double[8];
	cls_svm_node *tmp = new cls_svm_node[120];
	prob.x = new cls_svm_node*[8];

	for(int i=0;i<8;i++)
	{
		prob.y[i] = des[i][0];
		prob.x[i] = &tmp[i*2];

		for(int j=0;j<2;j++)
		{
			(prob.x[i])[j].value = src[i][j];
			(prob.x[i])[j].index = j+1;
		}

		(prob.x[i])[2].index = -1;
	}

	svm.train(&prob,&param);

	cout<<"trained."<<endl;

	for(int cnt=0;cnt<8;cnt++)
		cout<<svm.predict(prob.x[cnt])<<endl;
}


void test_svm_c_svc()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_svm_c_svc clser(en_poly,5,0.1,0.1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output(src[cnt]))[0]<<endl;
	}
};

void test_svm_nu_svc()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{20},
		{20},
		{30},
		{30},
		{40},
		{40},
	};

	cls_svm_nu_svc clser(en_sigmoid,0.1,0.1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output(src[cnt]))[0]<<endl;
	}
};


void test_svm_one_class()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_svm_c_svc clser(en_rbf,0.1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output(src[cnt]))[0]<<endl;
	}
};


void test_svm_ev()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_svm_epsilon_svr clser(en_sigmoid,0.1,0.1);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output(src[cnt]))[0]<<endl;
	}
};

void test_svm_nu_svr()
{
	double src[][2] =
	{
		{1,0},
		{0.9,0.1},
		{0,1},
		{0.1,0.9},
		{0,0},
		{0.1,0.1},
		{1,1},
		{0.9,0.9},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_svm_nu_svr clser(en_sigmoid,0.5,0.5);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train(ts);

	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output(src[cnt]))[0]<<endl;
	}
};

void test_hmm()
{
	double input[][1] =
	{
		{0},
		{1},
		{2},
		{3},
		{4},
		{3},
		{2},
		{1},
		{0},
		{1},
	};

	double output[][1] =
	{
		{0},
		{0},
		{2},
		{2},
		{2},
		{3},
		{2},
		{1},
		{0},
		{4},
	};

	cls_training_set ts(1,1);
	cls_training_set t(1,1);

	for(int cnt=0;cnt<10;cnt++)
	{
		ts.add_io_pair_with_map(input[cnt],input[cnt],input[cnt][0]);
		t.add_io_pair_with_map(output[cnt],output[cnt],output[cnt][0]);
	}

	cls_hmm hmm(10,5,0.00001);

	hmm.train_example(ts);

	cout<<log(hmm.get_output(t))<<endl;
	cout<<log(hmm.get_output(ts))<<endl;
}

void test_boosting_first()
{
	double src[][2] =
	{
		{10,12},
		{11,15},
		{20,23},
		{21,28},
		{30,35},
		{31,32},
		{40,42},
		{41,48},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_boosting_adaptive_first clser;
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	//此处一定要加入new出来的对象，而且这个对象以后不用再管了。
	clser.add_weaker(new cls_linear_sensor(2,2));
	clser.add_weaker(new cls_linear_fisher(2,2));
	clser.add_weaker(new cls_linear_fisher(2,2));
	clser.add_weaker(new cls_linear_sensor(2,2));
	clser.add_weaker(new cls_linear_fisher(2,2));

	int n = 5;
	while(n--)
		clser.train_example(ts);


	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<clser.get_output_data(src[cnt],2)[0]<<endl;
	}
}

void test_boosting_second()
{
	double src[][2] =
	{
		{10,40},
		{11,30},
		{20,30},
		{21,20},
		{30,20},
		{31,10},
		{40,10},
		{41.40},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_boosting_adaptive_second clser;
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	//此处一定要加入new出来的对象，而且这个对象以后不用再管了。
	clser.add_weaker(new cls_linear_fisher(2,2));
	clser.add_weaker(new cls_linear_sensor(2,2));

	int n = 5;
	while(n--)
		clser.train_example(ts);


	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<clser.get_output_data(src[cnt],2)[0]<<endl;
	}
}

void test_bagging()
{
	double src[][2] =
	{
		{10,12},
		{11,15},
		{20,23},
		{21,28},
		{30,35},
		{31,32},
		{40,42},
		{41,48},
	};

	double des[][1] =
	{
		{10},
		{10},
		{10},
		{10},
		{20},
		{20},
		{20},
		{20},
	};

	cls_bagging clser;
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<8;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	//此处一定要加入new出来的对象，而且这个对象以后不用再管了。
	clser.add_weaker(new cls_linear_fisher(2,2));
	clser.add_weaker(new cls_linear_sensor(2,2));

	int n = 5;
	while(n--)
		clser.train_example(ts);


	for(int cnt=0;cnt<8;cnt++)
	{
		cout<<clser.get_output_data(src[cnt],2)[0]<<endl;
	}
}

void test_bayes_basis()
{
	//这里输入值是0-1的对象。
	double src[][2] =
	{
		{0,0},
		{0.1,0},
		{0,0.1},
		{0.1,0.1},
		{1,0},
		{0,1},
		{1,1},
	};

	double des[][1] =
	{
		{1},
		{1},
		{1},
		{1},
		{2},
		{3},
		{4},
	};

	cls_bayes_basis clser(4,2);
	cls_training_set ts(2,1);

	for(int cnt=0;cnt<7;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train_example(ts);

	for(int cnt=0;cnt<7;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output_index(src[cnt]))[0]<<endl;
	}
}

void test_bayes_least_error()
{
	//这里输入值是0-1的对象。
	double src[][3] =
	{
		{0.9,0,0.8},
		{0.8,1.03,0.9},
		{0.1,1,0.2},
		{0.2,0,0.1},
	};

	double des[][1] =
	{
		{1},
		{1},
		{2},
		{2},
	};

	cls_bayes_least_error clser(2,3);
	cls_training_set ts(3,1);

	for(int cnt=0;cnt<4;cnt++)
	{
		ts.add_io_pair_with_map(src[cnt],des[cnt]);
	}

	clser.train_example(ts);

	for(int cnt=0;cnt<4;cnt++)
	{
		cout<<ts.get_output_for_id(clser.get_output_index(src[cnt]))[0]<<endl;
	}
}
