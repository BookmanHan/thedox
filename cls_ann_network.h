#include "ann_import.h"

class cls_ann_network
	:public cls_classifier
{
protected:
	unsigned n_input;
	unsigned n_output;
	string meta_data;

public:
	cls_ann_network(unsigned a_input,unsigned a_output)
		:cls_classifier(en_clser_ann)
	{
		n_input = a_input;
		n_output = a_output;
		meta_data = "";
	}

	cls_ann_network(const cls_ann_network & that)
		:cls_classifier(en_clser_ann)
	{
		n_input = that.n_input;
		n_output = that.n_output;
	}

	virtual ~cls_ann_network()
	{

	}

	virtual vector<double> get_output(const vector<double> & input) = 0;

	virtual vector<double> get_output(double *input)
	{
		vector<double> re;
		for(unsigned cnt =0;cnt<n_input;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output(re);
	}

	virtual void set_meta_data(string meta_data)
	{
		this->meta_data = meta_data;
	}

	virtual string get_meta_data()
	{
		return meta_data;
	}

	virtual unsigned get_input_count() const
	{
		return n_input;
	}

	virtual unsigned get_output_count() const
	{
		return n_output;
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_network::save() - the file is bad for output.\n");

		fout.write((char*)&n_input,sizeof(n_input));
		fout.write((char*)&n_output,sizeof(n_output));
	}

	virtual void load( ifstream& fin ) 
	{
		if (!fin)
			throw cls_exception_io("cls_ann_network::load() - the file is bad for output.\n");

		fin.read((char*)&n_input,sizeof(n_input));
		fin.read((char*)&n_output,sizeof(n_output));
	}

	virtual vector<double> get_output_data( const vector<double> & input ) 
	{
		return get_output(input);
	}

	virtual int get_output_index(const vector<double> & input ) 
	{
		throw cls_exception_not_sense("cls_ann_network::get_output_index() - there is non-sence to call this.\n");
	}

	virtual double get_output_double(const cls_training_set& ts ) 
	{
		throw cls_exception_not_sense("cls_ann_network::get_output_double() - there is non-sence to call this.\n");
	}
};