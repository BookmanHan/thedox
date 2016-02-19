#include "lib_import.h"

class cls_bayes
	:public cls_classifier
{
protected:
	vector<cls_bayes_unit*> units;

	int num_pat;
	int num_feature;

public:
	cls_bayes(int n_pat,int n_feature)
		:cls_classifier(en_clser_bayes)
	{
		num_pat = n_pat;
		num_feature = n_feature;
	}

	int get_patterns_num()
	{
		return num_pat;
	}

	int get_feature_num()
	{
		return num_feature;
	}

	virtual void add_unit() = 0;

	virtual ~cls_bayes()
	{
#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::iterator itor;
#else
		vector<cls_bayes_unit*>::iterator itor;
#endif

		for(itor=units.begin();
			itor != units.end();
			itor++)
		{
			delete (*itor);
		}
	}

	virtual void train_example(cls_training_set & ts)
	{
		int size = ts.get_size();

		ts.initialize();

		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			int num = ts.get_example_id(ts.get_count()-1);

			units[num]->update_in(a);
		}

#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::iterator itor;
#else
		vector<cls_bayes_unit*>::iterator itor;
#endif

		for(itor=units.begin();itor!=units.end();itor++)
		{
			(*itor)->set_total(size);
			(*itor)->update();
		}
	}

	virtual void save( ofstream& fout ) const
	{
		if (!fout)
			throw cls_exception_io(string(__FUNCTION__) + "() - the file is bad.\n");

		fout.write((char*)&num_feature,sizeof(num_feature));
		fout.write((char*)&num_pat,sizeof(num_pat));

#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::const_iterator itor;
#else
		vector<cls_bayes_unit*>::const_iterator itor;
#endif

		for(itor = units.begin();
			itor != units.end();
			itor++)
		{
			(*itor)->save(fout);
		}
	}

	virtual vector<double> get_output_data(const vector<double>& input )
	{
		throw cls_exception_not_sense(string(__FUNCTION__) + "() - call index instead.\n");
	}

	virtual int get_output_index( double* input )
	{
		vector<double> re;
		for(int cnt=0;cnt<num_feature;cnt++)
		{
			re.push_back(input[cnt]);
		}

		return get_output_index(re);
	}

	virtual int get_output_index(const vector<double>& input )
	{
		vector<double> d_pw(num_pat);

#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::iterator itor;
		typename vector<double>::iterator itor_pat;
#else
		vector<cls_bayes_unit*>::iterator itor;
		vector<double>::iterator itor_pat;
#endif

		for(itor=units.begin(),itor_pat=d_pw.begin()
			;itor_pat!=d_pw.end() && itor!=units.end();
			itor_pat++,itor++)
		{
			*itor_pat = (*itor)->get_output(input);
		}

		double max_val = -1;
		int max_indx = -1;
		int cnt = 0;
		for(itor_pat=d_pw.begin();itor_pat!=d_pw.end();itor_pat++,cnt++)
		{
			if (max_val < *itor_pat)
			{
				max_val = *itor_pat;
				max_indx = cnt;
			}
		}

		return num_pat-1 - max_indx;
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io(string(__FUNCTION__) + "() - the file is bad.\n");

		fin.read((char*)&num_feature,sizeof(num_feature));
		fin.read((char*)&num_pat,sizeof(num_pat));

#ifdef __USING_GCC__
		typename vector<cls_bayes_unit*>::iterator itor;
#else
		vector<cls_bayes_unit*>::iterator itor;
#endif

		for(itor = units.begin();
			itor != units.end();
			itor++)
		{
			(*itor)->load(fin);
		}
	}

	virtual double get_output_double( const cls_training_set& ts )
	{
		throw cls_exception_not_sense(string(__FUNCTION__) + "() - call index instead.\n");
	}
};
