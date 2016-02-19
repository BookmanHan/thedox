#include "lib_import.h"

class cls_linear
	: public cls_classifier
{
protected:
	vector<cls_linear_unit*> units;
	int _size;
	int _size_per_unit;

public:
	virtual void save(ofstream& fout) const
	{
		if (!fout)
			throw cls_exception_io("cls_linear::save() - the file is bad for output.");

		fout.write((char*)&_size,sizeof(_size));
		fout.write((char*)&_size_per_unit,sizeof(_size_per_unit));

		vector<cls_linear_unit*>::const_iterator itor;
		for(itor = units.begin();itor != units.end();itor++)
		{
			(*itor)->save(fout);
		}
	}

	virtual void load(ifstream& fin) 
	{
		if (!fin)
			throw cls_exception_io("cls_linear::load() - the file is bad for input.");

		vector<cls_linear_unit*>::iterator itor;
		for(itor=units.begin();itor != units.end();itor++)
		{
			delete *itor;
		}

		units.clear();
		fin.read((char*)&_size,sizeof(_size));
		fin.read((char*)&_size_per_unit,sizeof(_size_per_unit));

		for(int cnt=0;cnt<_size;cnt++)
		{
			add_unit();
		}

		for(itor=units.begin();itor != units.end();itor++)
		{
			(*itor)->load(fin);
		}
	}

	virtual ~cls_linear()
	{
		vector<cls_linear_unit*>::iterator itor;
		for(itor=units.begin();itor != units.end();itor++)
		{
			delete *itor;
		}
	}

	cls_linear(int num_unit,int num_per_unit,int output=1)
		:cls_classifier(en_clser_linear)
	{
		_size = num_unit;
		_size_per_unit = num_per_unit;
	}

	virtual void add_unit() = 0;

public:
	virtual void train_example(cls_training_set & ts) = 0;
	virtual int get_output(vector<double>& input) = 0;

	virtual int get_output(double *input)
	{
		vector<double> re;
		for(int cnt=0;cnt<_size_per_unit;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output(re);
	}

	virtual unsigned get_size()
	{
		return _size;
	}

	virtual unsigned get_size_per_unit()
	{
		return _size_per_unit;
	}

	virtual vector<double> get_output_data( const vector<double> & input ) 
	{
		throw cls_exception_not_sense("cls_linear::get_output_data() - there is nonse to call this,use index instead.\n");
	}

	virtual int get_output_index(const vector<double> & input ) 
	{
		return get_output((vector<double>&)input);
	}

	virtual double get_output_double(const cls_training_set& ts ) 
	{
		throw cls_exception_not_sense("cls_linear::get_output_double() - there is nonse to call this,use index instead.\n");
	}
};