#include "lib_import.h"

template<typename T,typename FUNC>
class tag_particle_multi_values
{
public:
	vector<T> v_pos;
	vector<T> v_vel;
	vector<T> d_fit;
	int dim;
	FUNC func;

public:
	tag_particle_multi_values()
	{
	}

	tag_particle_multi_values(int d)
	{
		dim = d;
		v_pos.resize(d);
		v_vel.resize(d);
	}

	void set_dim(int d)
	{
		dim = d;
		v_pos.resize(d);
		v_vel.resize(d);
	}

	bool operator < (const tag_particle_multi_values<T,FUNC> & that) const
	{
		if (func(this->v_pos,that.v_pos))
			return true;
		else
			return false;
	}
};

template<typename T,typename FUNC>
class cls_pso_multi_dim
{
public:
	int cnt_times;
	T up;
	T down;
	T max_bound;
	T min_bound;
	int w_max;
	int w_min;

public:
	cls_pso_multi_dim()
	{
		cnt_times = 0;
		up = 5;
		down = -5;
		max_bound = 100;
		min_bound = 0;
		w_max = 1.1;
		w_min = 0.9;
	}

	cls_pso_multi_dim(int n_time,int arg_num_particle,int dim,int powler)
	{
		cnt_times = 0;
		up = 5;
		down = -5;
		max_bound = 100;
		min_bound = 0;
		w_max = 1.1;
		w_min = 0.9;

		cnt_times = n_time;
		num_particles = arg_num_particle;
		n_dim = dim;

		v_particles.resize(num_particles);
		for(int cnt=0;cnt<v_particles.size();cnt++)
		{
			v_particles[cnt].set_dim(n_dim*(rand()%powler+1));
		}
	}

protected:
	vector<tag_particle_multi_values<T,FUNC> > v_particles;
	int num_particles;
	int n_dim;

protected:
	vector<vector<T> > opt_pos;
	int index_best;
	vector<T> best;
	vector<T> last_best;
	T c1;
	T c2;
	T w;
	FUNC func;

public:
	virtual void init();
	virtual void update(int n_times);
	virtual void run();
	virtual vector<tag_particle_multi_values<T,FUNC> > & get_particles();
};

template<typename T,typename FUNC>
vector<tag_particle_multi_values<T,FUNC> >& cls_pso_multi_dim<T, FUNC>::get_particles()
{
	return v_particles;
}

template<typename T,typename FUNC>
void cls_pso_multi_dim<T, FUNC>::run()
{
	int n = cnt_times;

	init();
	while(n--)
	{
		update(n);
	}
}

template<typename T,typename FUNC>
void cls_pso_multi_dim<T, FUNC>::update( int n_times )
{
	srand((unsigned int)time(NULL));

	for(int cnt=0;cnt<num_particles;cnt++)
	{
		for(int cnt_dim=0;cnt_dim<v_particles[cnt].dim;cnt_dim++)
		{
			T weight = w_min + (w_max - w_min) * n_times / cnt_times;
			v_particles[cnt].v_vel[cnt_dim] = w * v_particles[cnt].v_vel[cnt_dim]
			+ double(rand())/RAND_MAX * c1 * (opt_pos[cnt][cnt_dim] - v_particles[cnt].v_pos[cnt_dim])
				+ double(rand())/RAND_MAX * c2 * (opt_pos[index_best][cnt_dim] - v_particles[cnt].v_pos[cnt_dim]);

			if (v_particles[cnt].v_vel[cnt_dim] > up)
				v_particles[cnt].v_vel[cnt_dim] = up;
			else if (v_particles[cnt].v_vel[cnt_dim] < down)
				v_particles[cnt].v_vel[cnt_dim] = down;
		}
	}

	for(int cnt=0;cnt<num_particles;cnt++)
	{
		for(int cnt_dim=0;cnt_dim<v_particles[cnt].dim;cnt_dim++)
		{
			v_particles[cnt].v_pos[cnt_dim] += v_particles[cnt].v_vel[cnt_dim];

			if (v_particles[cnt].v_pos[cnt_dim] > max_bound)
				v_particles[cnt].v_pos[cnt_dim] = max_bound;
			else if (v_particles[cnt].v_pos[cnt_dim] < min_bound)
				v_particles[cnt].v_pos[cnt_dim] = min_bound;
		}

		vector<T> cur_best = func(v_particles[cnt].v_pos);

		if (func(cur_best,v_particles[cnt].d_fit))
		{
			v_particles[cnt].d_fit = cur_best;
			for(int cnt_dim=0;cnt_dim<v_particles[cnt].dim;cnt_dim++)
			{
				opt_pos[cnt][cnt_dim] = v_particles[cnt_dim].v_pos[cnt_dim];
			}

			if (func(cur_best,best))
			{
				last_best = best;
				best = cur_best;
				index_best = cnt;
			}
		}
	}
}

template<typename T,typename FUNC>
void cls_pso_multi_dim<T, FUNC>::init()
{
	c1 = 0.002;
	c2 = 0.002;

	srand((unsigned int)time(NULL));

	for(int cnt=0;cnt<num_particles;cnt++)
	{
		for(int cnti=0;cnti<v_particles[cnt].dim;cnti++)
		{
			v_particles[cnt].v_pos[cnti] = (rand() % int(max_bound-min_bound)) + min_bound;
			v_particles[cnt].v_vel[cnti] = (rand() % int(up)-int(down)) + down;
		}
	}

	best = func(v_particles[0].v_pos);
	index_best = 0;

	opt_pos.resize(num_particles);
	for(int cnt=0;cnt<num_particles;cnt++)
	{
		opt_pos[cnt].resize(v_particles[cnt].dim);
		for(int cnti=0;cnti<v_particles[cnt].dim;cnti++)
		{
			opt_pos[cnt][cnti] = v_particles[cnt].v_pos[cnti];
		}

		v_particles[cnt].d_fit = func(v_particles[cnt].v_pos);

		if (func(v_particles[cnt].d_fit,best))
		{
			best = v_particles[cnt].d_fit;
			index_best = cnt;
		}
	}
}
