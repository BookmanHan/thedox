#include "lib_import.h"

enum en_clser
{
	en_clser_ann,
	en_clser_linear,
	en_clser_hmm,
	en_clser_neaest,
	en_clser_svm,
	en_clser_bayes,
	en_clser_boosting,
	en_clser_bagging,
};

class cls_classifier
	:public cls_serialized_object 
{
protected:
	en_clser type_clser;

public:
	cls_classifier(en_clser a_type)
	{
		type_clser = a_type;
	}

	virtual void save( ofstream& fout ) const=0;
	virtual void load( ifstream& fin ) =0;

	virtual vector<double> get_output_data(const vector<double> & input) = 0;

	virtual int get_output_index(const vector<double> & input) = 0;

	virtual double get_output_double(const cls_training_set& ts) = 0;

	virtual void train_example(cls_training_set & ts) = 0;

	virtual en_clser get_classifier_type()
	{
		return type_clser;
	}

	virtual vector<double> get_output_data(double* input,int len)
	{
		vector<double> re;
		for(int cnt=0;cnt<len;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output_data(re);
	}

	virtual int get_output_index(double* input,int len)
	{
		vector<double> re;
		for(int cnt=0;cnt<len;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output_index(re);
	}
};