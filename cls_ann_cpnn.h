#include "lib_import.h"

template<typename T>
class cls_ann_cpnn
{
private:
	struct tag_layer
	{
		int units;
		vector<T> output;
		cls_matrix<T> weight;
		vector<bool> winner;

		tag_layer()
		{
			units = 0;
		}

		tag_layer(int a_units)
			:output(a_units),
			winner(a_units)
		{
			units = a_units;
		}

		void save(ofstream & fout)
		{
			fout<<units<<" ";
			for(int cnt=0;cnt<units;cnt++)
			{
				fout<<weight[cnt]<<" "<<winner[cnt]<<" ";
			}
		}

		void load(ifstream & fin)
		{
			fin>>units;
			for(int cnt=0;cnt<units;cnt++)
			{
				fin>>weight[cnt]>>winner[cnt];
			}
		}
	};

	tag_layer layer_input;
	tag_layer layer_instar;
	tag_layer layer_outstar;
	int winners;

	int n_feature;
	int n_in;
	int n_hidden;
	int n_output;

	void normalize_input(cls_matrix<T> & src)
	{
		int n_data = src.get_width();

		for(int cnt=0;cnt<n_data;cnt++)
		{
			T length = 0;

			for(int i=0;i<n_feature;i++)
			{
				length += src[cnt][i] * src[cnt][i];
			}

			length = sqrt(length);

			for(int i=0;i<n_feature;i++)
			{
				src(cnt,i) /= length;
			}
		}
	}

	void train_outstarts()
	{
		for(int i=0;i<layer_outstar.units;i++)
		{
			T sum = 0;
			for(int j=0;j<layer_instar.units;j++)
			{
				sum += layer_outstar.weight[j][i] * layer_instar[j];
			}

			layer_outstar.output[i] = sum;
		}
	}

	void train_instars()
	{
		int winner = 0;

		for(int i=0;i<layer_instar.units;i++)
		{
			T sum = 0;
			for(int j=0;j<layer_input.units;j++)
			{
				sum += layer_instar.weight[j][i] * layer_input.output[j];
			}

			layer_instar.output[i] = sum;
			layer_instar.winner[i] = false;
		}

		T sum_winners = 0;
		for(int w=0;w<winners;w++)
		{
			T max_out = - HUGE_VAL;
			for(int i=0;i<layer_instar.units;i++)
			{
				if (!layer_instar.winner[i] && layer_instar.output[i] > max_out)
				{
					max_out = layer_instar.output[i];
					winner = i;
				}
			}

			layer_instar.winner[winner] = true;
			sum_winners += layer_instar.output[winner];
		}

		for(int i=0;i<layer_instar.units;i++)
		{
			if (layer_instar.winner[i] == true)
			{
				layer_instar.output[i] = layer_instar.output[i] /sum_winners;
			}
			else
			{
				layer_instar.output[i] = 0;
			}
		}
	}

public:
	double d_study_factor_for_hidden;
	double d_study_factor_for_output;


	void save(string str_filename)
	{
		ofstream fout(str_filename.c_str());

		layer_input.save(fout);
		layer_instar.save(fout);
		layer_outstar.save(fout);

		fout.close();
	}


	void load(string str_filename)
	{
		ifstream fin(str_filename.c_str());

		layer_input.load(fin);
		layer_instar.load(fin);
		layer_outstar.load(fin);

		fin.close();
	}



	cls_ann_cpnn()
	{
		srand(time(NULL));

		n_feature = 0;
		winners = 0;
		d_study_factor_for_output = 0;
		d_study_factor_for_hidden = 0;
	}


	cls_ann_cpnn(int a_feautre,int a_in,int a_data_num,int a_output,
		double a_study_factor_for_hidden = 0.1,double a_study_factor_for_output = 0.1)
		:layer_input(a_in),
		 layer_instar(a_in*a_data_num),
		 layer_outstar(a_output)
	{
		n_feature = a_feautre;
		n_in = a_in;
		n_hidden = a_in * a_data_num;
		n_output = a_output;

		layer_instar.weight = cls_matrix<T>(n_in,n_hidden);
		layer_outstar.weight = cls_matrix<T>(n_hidden,n_output);

		winners = 1;
		d_study_factor_for_hidden = a_study_factor_for_hidden;
		d_study_factor_for_output = a_study_factor_for_output;
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des)
	{
		train(v_src,v_des,1000);
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times)
	{
		int n_data = v_src.get_width();

		for(int i=0;i<layer_instar.units;i++)
		{
			for(int j=0;j<layer_input.units;j++)
			{
				layer_instar.weight[j][i] = v_src(i,j);
			}
		}

		winners = 1;
		for(int cnt=0;cnt<n_data*n_times;cnt++)
		{
			int n = rand()%(n_data);
			for(int i=0;i<layer_input.units;i++)
			{
				layer_input.output[i] = v_src(n,i);
			}

			train_instars();
			int index_winner = 0;
			for(int i=0;i<layer_instar.units;i++)
			{
				if (layer_instar.winner[i] == true)
				{
					index_winner = i;
					break;
				}
			}

			for(int j=0;j<layer_input.units;j++)
			{
				layer_instar.weight[index_winner][j] += d_study_factor_for_hidden * (v_src(n,j) - layer_instar.weight[index_winner][j]);
			}


		}

		for(int i=0;i<layer_outstar.units;i++)
		{
			for(int j=0;j<layer_instar.units;j++)
			{
				layer_outstar.weight[j][i] = v_des(j,i);
			}
		}

		winners = 1;
		for(int cnt=0;cnt<n_data*n_times;cnt++)
		{
			int n = rand()%(n_data);
			for(int i=0;i<layer_input.units;i++)
			{
				layer_input.output[i] = v_src(n,i);
			}

			train_instars();

			int index_winner = 0;
			for(int i=0;i<layer_instar.units;i++)
			{
				if (layer_instar.winner[i] == true)
				{
					index_winner = i;
					break;
				}
			}

			for(int i=0;i<layer_outstar.units;i++)
			{
				layer_outstar.weight[i][index_winner] +=
					d_study_factor_for_output * (v_des(n,i) - layer_outstar.weight(i,index_winner));
			}
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e )
	{
		train(v_src,v_des,1000);
	}

	virtual void simulate( vector<T> & v_src,vector<T> & v_des )
	{
		for(int cnt=0;cnt<layer_input.units;cnt++)
		{
			layer_input.output[cnt] = v_src[cnt];
		}

		train_instars();
		train_outstarts();

		v_des.resize(layer_outstar.units);

		for(int cnt=0;cnt<layer_outstar.units;cnt++)
		{
			v_des[cnt] = layer_outstar.output[cnt];
		}
	}
};
