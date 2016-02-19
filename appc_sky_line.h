#include "lib_import.h"

class appc_sky_line_conjecture
{
public:
	vector<double> operator()(vector<double>& re)
	{
		vector<double> v_re;
		v_re.resize(4);
		for(int cnt=0;cnt<re.size();cnt++)
		{
			v_re[cnt%4] += v_re[cnt];
		}

		return v_re;
	}

	bool operator()(const vector<double>&src ,const vector<double>&des) const
	{
		bool b_flag = false;
		for(int cnt=0;cnt<min(src.size(),des.size());cnt++)
		{
			if (src[cnt] > des[cnt])
			{
				b_flag = true;
				break;
			}
		}

		if (b_flag)
		{
			return src[0] > des[0];
		}
		else
		{
			return false;
		}
	}
};

class appc_sky_line_compare
{
public:
	double operator()(pair<int,vector<double> >& src,pair<int,vector<double> > & des)
	{
		for(int cnt=0;cnt<min(src.second.size(),des.second.size());cnt++)
		{
			if (src.second[cnt] < des.second[cnt])
				return true;
			else if (src.second[cnt] > des.second[cnt])
				return false;
		}

		return false;
	}
};

inline
double appc_sky_line_distance(vector<double>& src,vector<double>& des)
{
	double d_re = 0;
	for(int cnt=0;cnt<min(src.size(),des.size());cnt++)
	{
		d_re = (src[cnt] - des[cnt])*(src[cnt] - des[cnt]);
	}

	return sqrt(d_re);
}

class appc_sky_line
{
private:
	cls_pso_multi_dim<double,appc_sky_line_conjecture> pso;
	vector<vector<double> > v_best_area;
	vector<vector<double> > v_data;
	appc_sky_line_compare func_cmp;
	int num_top;

public:
	vector<vector<double> >& choose(vector<vector<double> > & re)
	{
		vector<tag_particle_multi_values<double,appc_sky_line_conjecture> > v_part = pso.get_particles();

		return re;
	}

	appc_sky_line(string str_filename,int n_time,int arg_num_particle,int dim,int n_top):pso(n_time,arg_num_particle,dim,n_top)
	{
		num_top = n_top;
		pso.max_bound = 100;
		pso.min_bound = 0;

		ifstream fin(str_filename.c_str());
		while(!fin.eof())
		{
			vector<double> v_tmp;
			v_tmp.resize(4);

			int n;
			double d;
			char c;
			fin>>n>>v_tmp[0]>>c>>v_tmp[1]>>c>>v_tmp[2]>>c>>v_tmp[3];
			v_data.push_back(v_tmp);
		}

		fin.close();
	}

	void train()
	{
		pso.run();
	}

	void get_best_area()
	{
		v_best_area.clear();

		vector<tag_particle_multi_values<double,appc_sky_line_conjecture> >& v_particle = pso.get_particles();
		sort(v_particle.begin(),v_particle.end());

		for(int cnt=0;cnt<v_particle.size();cnt++)
		{
			v_best_area.push_back(v_particle[cnt].v_pos);
		}
	}
};
