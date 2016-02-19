#include "lib_import.h"

template<typename T>
class cls_ann_hopfield
{
private:
	struct tag_layer
	{
		int units;
		vector<T> output;
		vector<T> threshold;
		cls_matrix<T> weight;

		tag_layer()
		{
			units = 0;
		}

		tag_layer(int a_units)
			:output(a_units),
			threshold(a_units)
		{
			units = a_units;
		}

		void save(ofstream & fout)
		{
			fout<<units<<" ";
			for(int cnt=0;cnt<units;cnt++)
			{
				fout<<weight[cnt]<<" "<<threshold[cnt]<<" ";
			}
		}

		void load(ifstream & fin)
		{
			fin>>units;
			for(int cnt=0;cnt<units;cnt++)
			{
				fin>>weight[cnt]>>threshold[cnt];
			}
		}
	};

	tag_layer layer_net;

private:

	void save(string str_filename)
	{
		ofstream fout(str_filename.c_str());
		layer_net.save(fout);
		fout.close();
	}

	void load(string str_filename)
	{
		ifstream fin(str_filename.c_str());
		layer_net.load(fin);
		fin.close();
	}

	void calc_weights(cls_matrix<T> & src)
	{
		int n_data = src.get_width();

		for(int i=0;i<layer_net.units;i++)
		{
			for(int j=0;j<layer_net.units;j++)
			{
				int weight = 0;

				if (i != j)
				{
					for(int n=0;n<n_data;n++)
					{
						weight += src(n,i) * src(n,j);
					}
				}

				layer_net.weight[i][j] = weight;
			}
		}
	}

	bool propagate_unit(int i)
	{
		bool b_changed = false;
		int sum = 0;
		for(int j=0;j<layer_net.units;j++)
		{
			sum += layer_net.weight[i][j] * layer_net.output[j];
		}

		int out;
		if (sum != layer_net.threshold[i])
		{
			if (sum < layer_net.threshold[i])
				out = -1;
			if (sum >= layer_net.threshold[i])
				out = +1;
			if (out != layer_net.output[i])
			{
				b_changed =true;
				layer_net.output[i] = out;
			}
		}

		return b_changed;
	}

	void propagate_net()
	{
		int n_times = 0;
		int n_of_last_change = 0;

		do
		{
			n_times++;
			if (propagate_unit(rand()%layer_net.units))
			{
				n_of_last_change = n_times;
			}

		} while (n_times - n_of_last_change < 10 * layer_net.units);
	}

public:

	cls_ann_hopfield()
		:layer_net(0)
	{
		srand(time(null));
	}

	cls_ann_hopfield(int a_unit)
		:layer_net(a_unit)
	{
		layer_net.weight = cls_matrix<T>(a_unit,a_unit);

		for(int i=0;i<a_unit;i++)
		{
			layer_net.threshold[i] = 0;
		}
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des)
	{
		calc_weights(v_src);
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,int n_times)
	{
		calc_weights(v_src);
	}

	virtual void train( cls_matrix<T> & v_src,cls_matrix<T> & v_des,double d_e )
	{
		calc_weights(v_src);
	}

	virtual void simulate( vector<T> & v_src,vector<T> & v_des )
	{
		for(int cnt=0;cnt<layer_net.units;cnt++)
		{
			layer_net.output[cnt] = v_src[cnt];
		}

		propagate_net();

		v_des.resize(layer_net.units);

		for(int cnt=0;cnt<layer_net.units;cnt++)
		{
			v_des[cnt] = layer_net.output[cnt];
		}
	}
};
