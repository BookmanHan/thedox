#include "ann_import.h"
#define get_next_pair_for_ts(ts,a,b) pair<vector<vector<double> >::iterator,vector<vector<double> >::iterator> ______re___ = ts.get_itor();vector<double>& a = *(______re___.first);vector<double>& b = *(______re___.second);
#define get_next_pair_for_ts_reverse(ts,a,b) pair<vector<vector<double> >::reverse_iterator,vector<vector<double> >::reverse_iterator> ______re___ = ts.get_itor_reverse();vector<double>& a = *(______re___.first);vector<double>& b = *(______re___.second);

class cls_training_set
	:cls_serialized_object
{
protected:
	vector<vector<double> > inputs;
	vector<vector<double> > outputs;
	vector<vector<double> >::iterator itor_input;
	vector<vector<double> >::iterator itor_output;
	vector<vector<double> >::reverse_iterator ritor_input;
	vector<vector<double> >::reverse_iterator ritor_output;

	vector<int> map_des_num;
	map<vector<double>,int> map_num;
	int max_map;

	unsigned n_inputs;
	unsigned n_outputs;
	unsigned cnt;

public:

	cls_training_set& operator=(const cls_training_set& that)
	{
		inputs = that.inputs;
		outputs = that.outputs;
		itor_input = that.itor_input;
		itor_output = that.itor_output;
		ritor_input = that.ritor_input;
		ritor_output = that.ritor_output;

		map_des_num = that.map_des_num;
		map_num = that.map_num;
		max_map = that.max_map;
		
		n_inputs = that.n_inputs;
		n_outputs = that.n_outputs;

		cnt = that.cnt;

		return *this;
	}

	const vector<vector<double> >& get_inputs()
	{
		return inputs;
	}

	cls_training_set()
	{

	}

	void set_n_inputs(int a_inputs)
	{
		n_inputs = a_inputs;
	}

	void set_n_outputs(int a_output)
	{
		n_outputs = a_output;
	}

	virtual int get_output_classes_count()
	{
		return max_map;
	}

	virtual void save(ofstream& fout) const
	{
		if (!fout)
			throw cls_exception_io("cls_ann_training_set::save() - the file is bad or maybe you open it as text.\n");

		fout.write((char*)&n_inputs,sizeof(n_inputs));
		fout.write((char*)&n_outputs,sizeof(n_outputs));

		int num = get_size();
		fout.write((char*)&num,sizeof(num));

		for(int cnt=0;cnt<num;cnt++)
		{
			for(int i=0;i<n_inputs;i++)
			{
				fout.write((char*)&inputs[cnt][i],sizeof(double));
			}

			for(int i=0;i<n_outputs;i++)
			{
				fout.write((char*)&outputs[cnt][i],sizeof(double));
			}
		}
	}

	virtual void initialize_reverse()
	{
		ritor_input = inputs.rbegin();
		ritor_output = outputs.rbegin();

		cnt = get_size()-1;
	}

	virtual void load(ifstream& fin)
	{
		if (!fin)
			throw cls_exception_io("cls_ann_training_set::load() - the file is bad or maybe you open it as text.\n");

		fin.read((char*)&n_inputs,sizeof(n_inputs));
		fin.read((char*)&n_outputs,sizeof(n_outputs));

		int num = get_size();
		fin.read((char*)&num,sizeof(num));

		inputs.resize(num);
		outputs.resize(num);
		for(int cnt=0;cnt<num;cnt++)
		{
			inputs[cnt].resize(n_inputs);
			for(int i=0;i<n_inputs;i++)
			{
				fin.read((char*)&inputs[cnt][i],sizeof(double));
			}

			outputs[cnt].resize(n_outputs);
			for(int i=0;i<n_outputs;i++)
			{
				fin.read((char*)&outputs[cnt][i],sizeof(double));
			}
		}
	}

	//vector<double> get_output_for_num(int num)
	//{
	//	map<int,int>::iterator itor = map_des_num.begin();
	//	for(itor = map_des_num.begin();itor != map_des_num.end(); itor++)
	//	{
	//		if (num == itor->second)
	//			return outputs[itor->first];
	//	}

	//	throw cls_exception_not_implemented("cls_training_set::get_output_for_num() - there is no mapping num.\n");
	//}

	vector<double> get_output_for_id(int num)
	{
		int cnt=0;
		vector<int>::iterator itor = map_des_num.begin();
		for(itor = map_des_num.begin();itor != map_des_num.end(); itor++,cnt++)
		{
			if (num == *itor)
				return outputs[cnt];
		}

		throw cls_exception_not_implemented("cls_training_set::get_output_for_num() - there is no mapping num.\n");
	}

	int get_example_id(int itor)
	{
		return map_des_num[itor];
	}

	cls_training_set(unsigned in,unsigned out)
	{
		cnt = 0;
		max_map = 0;
		n_outputs = out;
		n_inputs = in;
	}

	cls_training_set(const cls_training_set& that)
	{
		this->inputs = that.inputs;
		this->outputs = that.outputs;
		this->n_inputs = that.n_inputs;
		this->n_outputs = that.n_outputs;
	}

	virtual ~cls_training_set()
	{

	}

	void add_io_pair(double * input,double * output)
	{
		vector<double> in(get_input_size());
		for(int cnt=0;cnt<get_input_size();cnt++)
		{
			in[cnt] = (input[cnt]);
		}

		vector<double> out(get_output_size());
		for(int cnt=0;cnt<get_output_size();cnt++)
		{
			out[cnt] = (output[cnt]);
		}

		add_io_pair(in,out);
	}

	void add_io_pair(const vector<double> & input,int un)
	{
		vector<double> d_o(1);
		d_o[0] = un;
		inputs.push_back(input);
		outputs.push_back(d_o);

		map_des_num.push_back(un);
	}

	pair<vector<vector<double> >::iterator,vector<vector<double> >::iterator> get_itor()
	{
		pair<vector<vector<double> >::iterator,vector<vector<double> >::iterator> re;
		re = make_pair(itor_input,itor_output);
		itor_input++;
		itor_output++;
		cnt++;

		return re;
	}

	pair<vector<vector<double> >::reverse_iterator,vector<vector<double> >::reverse_iterator> get_itor_reverse()
	{
		pair<vector<vector<double> >::reverse_iterator,vector<vector<double> >::reverse_iterator> re;
		re = make_pair(ritor_input,ritor_output);
		ritor_input++;
		ritor_output++;
		cnt--;

		return re;
	}

	void add_io_pair(const vector<double> & input,const vector<double> & output)
	{
		inputs.push_back(input);
		outputs.push_back(output);
	}

	void add_io_pair_with_map(double *input,double *output)
	{
		vector<double> in,out;
		for(int cnt=0;cnt<n_inputs;cnt++)
		{
			in.push_back(input[cnt]);
		}

		for(int cnt=0;cnt<n_outputs;cnt++)
		{
			out.push_back(output[cnt]);
		}

		add_io_pair_with_map(in,out);
	}

	void add_io_pair_with_map(double *input,double *output,int num)
	{
		vector<double> in,out;
		for(int cnt=0;cnt<n_inputs;cnt++)
		{
			in.push_back(input[cnt]);
		}

		for(int cnt=0;cnt<n_outputs;cnt++)
		{
			out.push_back(output[cnt]);
		}

		add_io_pair_with_map(in,out,num);
	}

	void add_io_pair_with_map(const vector<double> & input,const vector<double> & output,int num)
	{
		inputs.push_back(input);
		outputs.push_back(output);

		map_des_num.push_back(num);
		max_map = max(max_map,num);
	}

	void add_io_pair_with_map(const vector<double> & input,const vector<double> & output)
	{
		assert(input.size() == n_inputs);

		inputs.push_back(input);
		outputs.push_back(output);

		map<vector<double>,int>::iterator itor;
		itor = map_num.find(output);
		if(itor != map_num.end())
		{
			map_des_num.push_back(itor->second);
			return;
		}

		map_num.insert(make_pair(output,max_map));
		map_des_num.push_back(max_map);
		max_map++;
	}

	void add_io_pair(const vector<double> & input)
	{
		throw cls_exception_not_implemented("cls_training_set::add_io_pair() - not implemented.");
	}

	virtual int get_count()
	{
		return cnt;
	}

	virtual void initialize()
	{
		itor_input = inputs.begin();
		itor_output = outputs.begin();
		cnt = 0;
	}

	virtual bool epoch_over() const
	{
		if (itor_input == inputs.end() && itor_output == outputs.end())
			return true;
		else
			return false;
	}

	virtual bool epoch_over_reverse() const
	{
		if (ritor_input == inputs.rend() && ritor_output == outputs.rend())
			return true;
		else
			return false;
	}

	virtual unsigned get_size() const
	{
		return inputs.size();
	}

	virtual unsigned get_input_size() const
	{
		return n_inputs;
	}

	virtual unsigned get_output_size() const
	{
		return n_outputs;
	}

	virtual void get_next_pair(vector<double> &input,vector<double> & desired)
	{
		if (itor_input == inputs.end())
		{
			throw cls_exception_overflow("cls_training_set::itor_output() - no more left.");
		}

		input = *itor_input;
		desired = *itor_output;
		itor_input++;
		itor_output++;
		cnt++;
	}

	virtual void get_next_pair_reverse(vector<double> &input,vector<double> & desired)
	{
		if (ritor_input == inputs.rend())
		{
			throw cls_exception_overflow("cls_training_set::itor_output() - no more left.");
		}

		input = *ritor_input;
		desired = *ritor_output;
		ritor_input++;
		ritor_output++;
		cnt--;
	}

	void random_change()
	{
		int size = get_size()-1;
		vector< vector<double> >::iterator inIt,outIt;

		int chosen;
		while(size>=0)
		{
			chosen = (int)(fabs(a_random())*size);

			inIt = inputs.begin() + chosen;
			outIt = outputs.begin() + chosen;

			inputs.push_back(*inIt);
			outputs.push_back(*outIt);

			inIt = inputs.erase(inIt);
			outIt = outputs.erase(outIt);
			size--;
		}
	}

	cls_training_set operator+(const cls_training_set & ts) const
	{
		cls_training_set ret(*this);
		ret += ts;

		return ret;
	}

	cls_training_set & operator += (const cls_training_set & ts)
	{
		if (get_input_size() != ts.get_input_size())
			throw cls_exception_io("out of scripts.");

		if (get_output_size() != ts.get_output_size())
			throw cls_exception_io("out of scripts.");

		cls_training_set b(ts);
		b.initialize();
		vector<double> in(get_input_size());
		vector<double> out(get_output_size());

		while(!b.epoch_over())
		{
			b.get_next_pair(in,out);
			add_io_pair(in,out);
		}

		return *this;
	}
};
