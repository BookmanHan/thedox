#include "lib_import.h"

class cls_svm_base
	:public cls_classifier
{
protected:
	cls_svm svm;
	cls_svm_problem prob;
	cls_svm_param param;

	cls_svm_node* tmp;
	int _input;

	virtual void trans_ts_to_prob(cls_training_set& ts)
	{
		int _size = ts.get_size();
		int _input = ts.get_input_size();

		prob.l = _size;
		prob.y = new double[_size];
		prob.x = new cls_svm_node*[_size];

		tmp = new cls_svm_node[(_input+1) * _size];

		int cnt=0;
		ts.initialize();
		while(! ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			int num = ts.get_example_id(ts.get_count()-1);

			prob.y[cnt] = num;
			prob.x[cnt] = &tmp[(_input+1)*cnt];

			int num_d = 0;
			for(int i=0;i<_input;i++)
			{
				if (a[i] == 0.0)
					continue;

				(prob.x[cnt])[num_d].index = i+1;
				(prob.x[cnt])[num_d].value = a[i];
				num_d++;
			}

			(prob.x[cnt])[num_d].index = -1;
			(prob.x[cnt])[num_d].value = 0.0;

			cnt++;
		}
	}

	cls_svm_base()
		:cls_classifier(en_clser_hmm)
	{
		_input = 0;

		prob.x = null;
		prob.y = null;
		prob.l = 0;

		tmp = null;

		param.svm_type = en_c_svc;
		param.kernel_type = en_sigmoid;
		param.degree = 4;
		param.gamma = 0.2;	// 1/k
		param.coef = 0.2;
		param.nu = 0.5;
		param.cache_size = 500;
		param.c = 0.1;
		param.eps = 0.001;
		param.p = 0.1;
		param.shrinking = 1;
		param.probability = 0.1;
		param.nr_weight = 0;
		param.weight_label = null;
		param.weight = null;
	}

public:
	cls_svm_param& get_param()
	{
		return param;
	}

	virtual ~cls_svm_base()
	{
		if (prob.x != null)
			delete [] prob.x;

		if (prob.y != null)
			delete [] prob.y;

		if (tmp != null)
			delete [] tmp; 
	}

	virtual void save( ofstream& fout ) const
	{
		throw cls_exception_not_implemented("The method or operation is not implemented.");
	}

	virtual void train_example( cls_training_set & ts )
	{
		_input = ts.get_input_size();
		trans_ts_to_prob(ts);
		svm.train(&prob,&param);
	}

	virtual void train(cls_training_set & ts)
	{
		train_example(ts);
	}

	virtual vector<double> output( vector<double> &input ) 
	{
		throw cls_exception_not_sense("cls_svm_base::output() - call get_output instead.\n");
	}

	virtual int get_output(double* input)
	{
		vector<double> re;
		for(int cnt=0;cnt<_input;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output(re);
	}

	virtual int get_output(vector<double> &input)
	{
		cls_svm_node *p = new cls_svm_node[input.size()+1];
		int size = input.size();
		for(int cnt=0;cnt<size;cnt++)
		{
			p[cnt].value = input[cnt];
			p[cnt].index = cnt+1;
		}

		p[size].index = -1;
		p[size].value = 0.0;

		int num = svm.predict(p);

		delete [] p;

		return num;
	}

	virtual void load( ifstream& fin ) 
	{
		throw cls_exception_not_implemented("The method or operation is not implemented.");
	}

	virtual vector<double> get_output_data( const vector<double> & input ) 
	{
		throw cls_exception_not_sense("cls_svm::get_output_data() - use index instread.\n");
	}

	virtual int get_output_index(const vector<double> & input ) 
	{
		return get_output((vector<double>&)input);
	}

	virtual double get_output_double(const cls_training_set& ts ) 
	{
		throw cls_exception_not_sense("cls_svm::get_output_data() - use index instread.\n");
	}
};