#include "lib_import.h"

class cls_hmm
	: cls_classifier
{
private:
	int n_status;
	int n_obs;

	cls_matrix<double> mat_move;
	cls_matrix<double> mat_obs;
	cls_matrix<double> prob;

	double c_delta;

public:
	cls_hmm(int a_status,int a_obs,double a_delta)
		:mat_move(a_status,a_status),
		 mat_obs(a_status,a_obs),
		 prob(a_status,1),
		 c_delta(a_delta),
		 cls_classifier(en_clser_hmm)
	{
		srand(time(null));

		this->n_status = a_status;
		this->n_obs = a_obs;

		for(int i=0;i<n_status;i++)
		{
			double sum = 0;
			for(int j=0;j<n_status;j++)
			{
				mat_move(i,j) = a_unfirom_random(0,1);
				sum += mat_move(i,j);
			}

			for(int j=0;j<n_status;j++)
			{
				mat_move(i,j) /= sum;
			}
		}

		for(int i=0;i<n_status;i++)
		{
			double sum = 0;

			for(int j=0;j<n_obs;j++)
			{
				mat_obs(i,j) = a_unfirom_random(0,1);
				sum += mat_obs(i,j);
			}

			for(int j=0;j<n_obs;j++)
			{
				mat_obs(i,j) /= sum;
			}
		}

		double sum = 0;
		for(int i=0;i<n_status;i++)
		{
			prob(i,0) = a_unfirom_random(0,1);
			sum += prob(i,0);
		}

		for(int i=0;i<n_status;i++)
		{
			prob(i,0) /= sum;
		}
	}

	cls_hmm(const cls_hmm& that)
		:cls_classifier(en_clser_hmm)
	{
		this->n_status = that.n_status;
		this->n_obs = that.n_obs;
		this->mat_obs = that.mat_obs;
		this->mat_move = that.mat_move;
		this->prob = that.prob;
	}

	virtual void save(ofstream& fout) const
	{
		if (!fout)
			throw cls_exception_io("cls_hmm::save() - the file is bad for output.\n");

		fout.write((char*)&n_status,sizeof(n_status));
		fout.write((char*)&n_obs,sizeof(n_obs));

		mat_move.save(fout);
		mat_obs.save(fout);
		prob.save(fout);
	}

	virtual void load(ifstream& fin)
	{
		if (!fin)
			throw cls_exception_io("cls_hmm::load() - the file is bad for input.\n");

		fin.read((char*)&n_status,sizeof(n_status));
		fin.read((char*)&n_obs,sizeof(n_obs));

		mat_move.load(fin);
		mat_obs.load(fin);
		prob.load(fin);
	}

	virtual void forward(cls_training_set & ts,cls_matrix<double>& alpha,
		double* pprob)
	{
		int num = ts.get_example_id(0);
	
		for(int i=0;i<n_status;i++)
		{
			alpha(0,i) = prob(i,0) * mat_obs(i,num);
		}

		int size = ts.get_size();

		for(int cnt=0;cnt<size-1;cnt++)
		{
			for(int j=0;j<n_status;j++)
			{
				double d_sum = 0;
				for(int i=0;i<n_status;i++)
				{
					d_sum += alpha(cnt,i) * mat_move(i,j);
				}

				alpha(cnt+1,j) = d_sum * mat_obs(j,ts.get_example_id(cnt+1));
			
			}
		}

		size = ts.get_size();
		*pprob = 0.0;
		for(int i=0;i<size;i++)
		{
			*pprob += alpha(size-1,i);
		}
	}


	virtual void forward_with_scale(cls_training_set & ts,cls_matrix<double>& alpha,cls_matrix<double>& scale,
		double& pprob)
	{
		int num = ts.get_example_id(0);
	
		scale(0,0) = 0;
		for(int i=0;i<n_status;i++)
		{
			alpha(0,i) = prob(i,0) * mat_obs(i,num);
			scale(0,0) += alpha(0,i);
		}

		for(int i=0;i<n_status;i++)
		{
			alpha(0,i) /= scale(0,0);
		}

		int size = ts.get_size();

		for(int cnt=0;cnt<size-1;cnt++)
		{
			scale(cnt+1,0) = 0.0;
			for(int j=0;j<n_status;j++)
			{
				double d_sum = 0;
				for(int i=0;i<n_status;i++)
				{
					d_sum += alpha(cnt,i) * mat_move(i,j);
				}

				alpha(cnt+1,j) = d_sum * mat_obs(j,ts.get_example_id(cnt+1));
				scale(cnt+1,0) += alpha(cnt+1,j);
			}

			for(int j=0;j<n_status;j++)
			{
				alpha(cnt+1,j) /= scale(cnt+1,0);
			}
		}

		size = ts.get_size();
		pprob = 0.0;
		for(int i=0;i<size;i++)
		{
			pprob += log(scale(i,0));
		}
	}

	virtual void backward_with_scale(cls_training_set & ts,cls_matrix<double>& beta,cls_matrix<double>& scale,
					double* pprob)
	{
		int size = ts.get_size();

		for(int i=0;i<n_status;i++)
			beta(size-1,i) = 1.0 / scale(size-1,0);

		for(int cnt=size-2;cnt>=0;cnt--)
		{
			for(int i=0;i<n_status;i++)
			{
				double d_sum = 0;
				for(int j=0;j<n_status;j++)
				{
					d_sum += mat_move(i,j) * mat_obs(j,ts.get_example_id(cnt+1)) * beta(cnt+1,j);
				}

				beta(cnt,i) = d_sum / scale(cnt,0);
			}
		}
	}

	virtual void viterbi_log(cls_training_set& ts,cls_matrix<double>& delta,cls_matrix<int>& psi,cls_matrix<int>& q,double *pprob)
	{
		for(int i=0;i<n_status;i++)
		{
			prob(i,0) = log(prob(i,0));
		}

		for(int i=0;i<n_status;i++)
		{
			for(int j=0;j<n_status;j++)
			{
				mat_move(i,j) = log(mat_move(i,j));
			}
		}

		cls_matrix<double> biot(n_status,ts.get_size());

		int size = ts.get_size();
		for(int i=0;i<n_status;i++)
		{
			ts.initialize();
			get_next_pair_for_ts(ts,a,b);
			int cnt = ts.get_count() - 1;
			int num = ts.get_example_id(cnt);

			biot(i,cnt) = log(mat_obs(i,num));
		}

		for(int i=0;i<n_status;i++)
		{
			delta(0,i) = prob(i,0) + biot(i,0);
			psi(0,i) = 0;
		}

		ts.initialize();
		ts.get_itor();

		while(!ts.epoch_over())
		{
			get_next_pair_for_ts(ts,a,b);
			int cnt = ts.get_count() - 1;
			int num = ts.get_example_id(cnt);

			for(int j=0;j<n_status;j++)
			{
				double max_val = -inf;
				int max_indx = 0;
				for(int i=0;i<n_status;i++)
				{
					double val = delta(cnt-1,i) + mat_move(i,j);
					if (val > max_val)
					{
						max_val = val;
						max_indx = i;
					}
				}

				delta(cnt,j) = max_val + biot(j,cnt);
				psi(cnt,j) = max_indx;
			}
		}

		*pprob = -inf;
		q(size-1,0) = 1;
		for(int i=0;i<n_status;i++)
		{
			if (delta(size-1,i) > *pprob)
			{
				*pprob = delta(size-1,i);
				q(size-1,0) = i;
			}
		}

		for(int cnt=size-2;cnt>=0;cnt--)
		{
			q(cnt,0) = psi(cnt+1,q(cnt+1,0));
		}
	}

	virtual void viterbi(cls_training_set& ts,cls_matrix<double>& delta,cls_matrix<int>& psi,cls_matrix<int>& q,double *pprob) 
	{
		int num = ts.get_example_id(0);
		for(int i=0;i<n_status;i++)
		{
			delta(0,i) = prob(i,0) * mat_obs(i,num);
			psi(0,i) = 0;
		}

		int max_indx;
		double max_val;
		double val;
		int size = ts.get_size();
		for(int cnt=1;cnt<size;cnt++)
		{
			for(int j=0;j<n_status;j++)
			{
				max_val = -inf;
				max_indx = -1;

				for(int i=0;i<n_status;i++)
				{
					val = delta(cnt-1,i) * mat_move(i,j);

					if (val > max_val)
					{
						max_val = val;
						max_indx  = i;
					}
				}

				delta(cnt,j) = max_val * mat_obs(j,ts.get_example_id(cnt));
				psi(cnt,j) = max_indx;
			}
		}

		*pprob = 0;
		q(size-1,0) = 1;
		
		for(int i=0;i<n_status;i++)
		{
			if (delta(size-1,i) > *pprob)
			{
				*pprob = delta(size-1,i);
				q(size-1,0) = i;
			}
		}

		for(int cnt=size-2;cnt>=0;cnt--)
		{
			q(cnt,0) = psi(cnt+1,q(cnt+1,0));
		}
	}

	virtual void backward(cls_training_set & ts,cls_matrix<double>& beta,
		double* pprob)
	{
		int size = ts.get_size();

		for(int i=0;i<n_status;i++)
			beta(size-1,i) = 1.0;

		for(int cnt=size-2;cnt>=0;cnt--)
		{
			for(int i=0;i<n_status;i++)
			{
				double d_sum = 0;
				for(int j=0;j<n_status;j++)
				{
					d_sum += mat_move(i,j) * mat_obs(j,ts.get_example_id(cnt+1)) * beta(cnt+1,j);
				}

				beta(cnt,i) = d_sum;
			}
		}

		*pprob = 0.0;
		for(int i=0;i<n_status;i++)
		{
			*pprob += beta(0,i);
		}
	}

	virtual void train_example(cls_training_set& ts)
	{
		int size = ts.get_size();

		cls_matrix<double> mat_alpha(size,n_status);
		mat_alpha.fill(0);

		cls_matrix<double> mat_beta(size,n_status);
		mat_beta.fill(0);

		cls_matrix<double> mat_gamma(size,n_status);
		mat_gamma.fill(0);

		int n_iter;
		double prob_b,prob_e;

		baum_welch(ts,mat_alpha,mat_beta,mat_gamma,&n_iter,&prob_b,&prob_e);
	}

	virtual double get_output(cls_training_set& ts)
	{
		int size = ts.get_size();
		cls_matrix<int> q(size,1);
		q.fill(0);

		cls_matrix<double> delta(size,n_status);
		delta.fill(0);

		cls_matrix<int> psi(size,n_status);
		psi.fill(0);

		double re;
		viterbi(ts,delta,psi,q,&re);

		return re;
	}

	virtual void baum_welch(cls_training_set& ts,cls_matrix<double> & alpha,cls_matrix<double>& beta,
							cls_matrix<double> & gamma,int* pniter,double* log_prob_beg,double* log_prob_end)
	{
		int l = 0;
		int size = ts.get_size();
		cls_multi_array<double> xi(size,n_status,n_status);
		xi.fill(0);
	
		cls_matrix<double> scale(size,1);
		scale.fill(0);

		double log_prob_f = 0;
		double log_prob_b = 0 ;
		double log_prob_pre = 0;

		forward_with_scale(ts,alpha,scale,log_prob_f);
		*log_prob_beg = log_prob_f;
		log_prob_pre = log_prob_f;

		backward_with_scale(ts,beta,scale,&log_prob_b);
		calc_gamma(ts,alpha,beta,gamma);
		calc_xi(ts,alpha,beta,xi);

		double delta = 0;
		do 
		{
			for(int i=0;i<n_status;i++)
			{
				prob(i,0) = 0.001 + 0.999*gamma(0,i);
			}

			for(int i=0;i<n_status;i++)
			{
				double d_a = 0.0;
				for(int t=0;t<size-1;t++)
				{
					d_a += gamma(t,i);
				}

				for(int j=0;j<n_status;j++)
				{
					double n_a = 0.0;
					for(int t=0;t<size-1;t++)
					{
						n_a += xi(t,i,j);
					}

					mat_move(i,j) = 0.001 + 0.999 * n_a / d_a;
				}

				double d_b = d_a + gamma(size-1,i);

				int size = ts.get_size();
				for(int k=0;k<n_obs;k++)
				{
					double num_b = 0.0;
					for(int t=0;t<size;t++)
					{
						if (ts.get_example_id(t) == k)
							num_b += gamma(t,i);
					}

					mat_obs(i,k) = 0.001 + 0.999 * num_b / d_b;
				}
			}

			forward_with_scale(ts,alpha,scale,log_prob_f);
			backward_with_scale(ts,beta,scale,&log_prob_b);

			calc_gamma(ts,alpha,beta,gamma);
			calc_xi(ts,alpha,beta,xi);

			delta = log_prob_f - log_prob_pre;

			l++;
			log_prob_pre = log_prob_f;
		} while (delta > c_delta);

		*pniter = l;
		*log_prob_end = log_prob_f;
	}

protected:
	virtual void calc_gamma(cls_training_set& ts,cls_matrix<double>& alpha,cls_matrix<double>& beta,
							cls_matrix<double> & gamma)
	{
		int size = ts.get_size();

		for(int t=0;t<size;t++)
		{
			double d = 0.0;
			
			for(int j=0;j<n_status;j++)
			{
				gamma(t,j) = alpha(t,j) * beta(t,j);
				d += gamma(t,j);
			}

			for(int i=0;i<n_status;i++)
			{
				gamma(t,i) = gamma(t,i) / d;
			}
		}
	}

	virtual void calc_xi(cls_training_set& ts,cls_matrix<double>& alpha,cls_matrix<double>& beta,cls_multi_array<double>& xi)
	{
		int size = ts.get_size();

		for(int cnt=0;cnt<size-1;cnt++)
		{
			double sum = 0;

			for(int i=0;i<n_status;i++)
			{
				for(int j=0;j<n_status;j++)
				{
					xi(cnt,i,j) = alpha(cnt,i) * beta(cnt + 1,j)
						* mat_move(i,j) * mat_obs(j,ts.get_example_id(cnt));
					
					sum += xi(cnt,i,j);
				}
			}

			for(int i=0;i<n_status;i++)
			{
				for(int j=0;j<n_status;j++)
				{
					xi(cnt,i,j) /= sum;
				}
			}
		}
	}

	virtual vector<double> get_output_data( const vector<double> & input ) 
	{
		throw cls_exception_not_sense("cls_hmm::get_output_data() - call get_output_index() instead.\n");
	}

	virtual int get_output_index(const vector<double> & input ) 
	{
		throw cls_exception_not_sense("cls_hmm::get_output_data() - call get_output_index() instead.\n");
	}

	virtual double get_output_double(const cls_training_set& ts ) 
	{
		return get_output((cls_training_set&)ts);
	}
};