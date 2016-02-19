#include "lib_import.h"

//解决旅行商问题，需要修改。
class cls_ann_boltzman 
{
private:
	int units;
	vector<bool> output;
	vector<int> on;
	vector<int> off;
	vector<double> threshold;
	cls_matrix<double> weight;
	double temperature;
	double gamma;
	cls_matrix<double> distance;
	double d_rate;

private:

	void calc_weights()
	{
		for(int i=0;i<units;i++)
		{
			for(int j=0;j<units;j++)
			{
				int skip_i = i*units + j;

				for(int m=0;m<units;m++)
				{
					for(int n=0;n<units;n++)
					{
						int skip_j = m*units + n;
						double w = 0;

						if (skip_i != skip_j)
						{
							int Pred_n = (m == 0?units-1:m-1);
							int Secc_n = (m == units?0:m + 1);

							if (m == i || j == n)
							{
								w = - gamma;
							}
							else if (i == Pred_n || i == Secc_n)
							{
								w = - distance(j,n);
							}
						}

						weight(skip_i,skip_j) = w;
					}
				}

				threshold[skip_i] = -gamma/2;
			}
		}
	}

	bool valid_tour()
	{
		for(int i=0;i<units;i++)
		{
			int cities = 0;
			int stops = 0;

			for(int j=0;j<units;j++)
			{
				if (output[i*units + j])
				{
					if (++ cities > 1)
						return false;
				}

				if (output[j*units + i])
				{
					if (++ stops > 1)
						return false;
				}
			}

			if (cities != 1 || stops != 1)
				return false;
		}

		return true;
	}

	double length_of_tour()
	{
		double length = 0;
		for(int i=0;i<units;i++)
		{
			int index_1;
			int index_2;
			for(int j=0;j<units;j++)
			{
				if (output[(i%units)*units + j])
				{
					index_1 = j;
					break;
				}
			}

			for(int j=0;j<units;j++)
			{
				if (output[((i+1) % units) * units + j])
				{
					index_2 = j;
					break;
				}
			}

			length += distance(index_1,index_2);
		}

		return length;
	}

	void propagate_unit(int i)
	{
		double sum;
		double prob;

		sum = 0;
		for(int cnt=0;cnt<units*units;cnt++)
		{
			sum += weight(i,cnt) * output[cnt];
		}

		sum -= threshold[i];
		prob = 1.0/(1.0 + exp(-sum/temperature));

		if (double(rand()) / RAND_MAX <= prob)
			output[i] = true;
		else
			output[i] = false;
	}

	void bring_to_thermal_equilibrium()
	{
		int index = 0;
		for(int cnt=0;cnt<units*units;cnt++)
		{
			on[cnt] = 0;
			off[cnt] = 0;
		}

		for(int cnt=0;cnt<1000*units*units;cnt++)
		{
			index = rand()%(units*units);
			propagate_unit(index);
		}

		for(int cnt=0;cnt<100*units*units;cnt++)
		{
			index = rand()%(units*units);
			propagate_unit(index);

			if (output[index])
				on[index]++;
			else
				off[index]++;
		}

		for(int cnt=0;cnt<units*units;cnt++)
		{
			output[cnt] = on[cnt] > off[cnt];
		}
	}

public:
	cls_ann_boltzman()
	{
		srand(time(NULL));
		units = 0;
	}

	cls_ann_boltzman(int a_units,double d_temperature,double a_rate)
		:output(a_units*a_units),
		 on(a_units*a_units),
		 off(a_units*a_units),
		 threshold(a_units*a_units)
	{
		srand(time(NULL));
		units = a_units;
		d_rate = a_rate;
		temperature = d_temperature;
		gamma = 7;

		weight = cls_matrix<double>(a_units*a_units,a_units*a_units);

		for(int i=0;i<a_units;i++)
		{
			for(int j=0;j<a_units;j++)
			{
				double alpha1 = (double(i) / a_units) * 2 * pi;
				double alpha2 = (double(j) / a_units) * 2 * pi;
				
				double x1 = cos(alpha1);
				double y1 = sin(alpha1);
				double x2 = cos(alpha2);
				double y2 = sin(alpha2);

				distance(i,j) = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			}
		}

		for(int cnt=0;cnt<units*units;cnt++)
		{
			output[cnt] = rand()%2;
		}
	}

	virtual void train( cls_matrix<double> & v_src,cls_matrix<double> & v_des) 
	{
		calc_weights();
		do 
		{
			bring_to_thermal_equilibrium();
			temperature *= d_rate;
		} while (!valid_tour());
	}

	virtual void train( cls_matrix<double> & v_src,cls_matrix<double> & v_des,int n_times) 
	{
		train(v_src,v_des);
	}

	virtual void train( cls_matrix<double> & v_src,cls_matrix<double> & v_des,double d_e ) 
	{
		train(v_src,v_des);
	}

	virtual void simulate( vector<double> & v_src,vector<double> & v_des ) 
	{

	}
};