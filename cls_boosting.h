#include "lib_import.h"

class cls_boosting
	:public cls_classifier
{
protected:
	int size_weaker;
	vector<pair<cls_classifier*,double> > weakers;
	vector<vector<double> > mem_num_output;

protected:

	virtual int get_index_for_output(vector<double> d)
	{
		if (cls_conjector_euclidean<double>()(d,mem_num_output[0]) > cls_conjector_euclidean<double>()(d,mem_num_output[1]))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void trans_training_set(cls_training_set& ts,cls_training_set&ts_re)
	{
		mem_num_output.resize(2);
		bool b_flag[2];
		b_flag[0] = b_flag[1] = true;

		ts.initialize();
		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			int num = ts.get_example_id(ts.get_count()-1);

			vector<double> d_out;
			d_out.push_back(num);

			if (b_flag[num])
			{
				mem_num_output[num] = b;
				b_flag[num] = false;
			}

			ts_re.add_io_pair_with_map(a,d_out,num);
		}
	}

public:
	cls_boosting()
		:cls_classifier(en_clser_boosting)
	{
		size_weaker = 0;
	}

	virtual ~cls_boosting()
	{
		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor = weakers.begin();itor != weakers.end();itor++)
		{
			delete itor->first;
		}
	}

	virtual void add_weaker(cls_classifier* that)
	{
		weakers.push_back(make_pair(that,0.0));
		size_weaker++;
	}

	virtual void save( ofstream& fout ) const
	{
		if (!fout)
			throw cls_exception_io("cls_boosting::save() - the file is bad.\n");

		fout.write((char*)&size_weaker,sizeof(size_weaker));

		vector<pair<cls_classifier*,double> >::const_iterator itor;
		for(itor=weakers.begin();itor != weakers.end();itor++)
		{
			fout.write((char*)&itor->second,sizeof(itor->second));
			itor->first->save(fout);
		}

		//TODO:Here is the outputnum mapping...
	}

	virtual void train_example( cls_training_set & ts ) = 0;

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_boosting::save() - the file is bad.\n");

		int num = size_weaker;

		fin.read((char*)&size_weaker,sizeof(size_weaker));

		if (num != size_weaker)
		{
			throw cls_exception_not_identical("cls_boosting::load() - you must add the clser for youself.\n");
		}

		weakers.resize(size_weaker);

		vector<pair<cls_classifier*,double> >::iterator itor;
		for(itor=weakers.begin();itor != weakers.end();itor++)
		{
			fin.read((char*)&itor->second,sizeof(itor->second));
			itor->first->load(fin);
		}
	}

	virtual vector<double> get_output_data( const vector<double> & input )
	{
		return mem_num_output[get_output_index(input)];
	}

	virtual int get_output_index(const vector<double> & input ) = 0;

	virtual double get_output_double(const cls_training_set& ts )
	{
		throw cls_exception_not_sense("cls_boosting::get_output_double() - this is of nonsense. call index.\n");
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

	virtual en_clser get_classifier_type()
	{
		return en_clser_boosting;
	}
};
