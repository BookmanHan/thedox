#include "lib_import.h"

template<typename T>
class cls_ann_bpnn
{
private:
	vector<vector<T> > weight_i_to_h;
	vector<vector<T> > weight_h_to_o;

	vector<T> thd_h;
	vector<T> thd_o;

	double factor_study_h;
	double factor_study_o;

	double factor_thd_study_h;
	double factor_thd_study_o;
public:

	double e;
	cls_ann_bpnn()
	{
		;
	}

	cls_ann_bpnn(int num_in,int num_hide,int num_output,double d_1=0.05,double d_2=0.047,double d_3=0.05,double d_4=0.047)
	{
		e = 1.0;

		factor_study_h = d_1;
		factor_study_o = d_2;
		factor_thd_study_h = d_3;
		factor_thd_study_o = d_4;

		weight_i_to_h.resize(num_in);
		for(int cnt=0;cnt<num_in;cnt++)
		{
			weight_i_to_h[cnt].resize(num_hide);
			for(int cnt_cur=0;cnt_cur<num_hide;cnt_cur++)
			{
				weight_i_to_h[cnt][cnt_cur] = rand() % 2000 * 1.0 / 1000.0 - 1.0;
			}
		}

		weight_h_to_o.resize(num_hide);
		for(int cnt=0;cnt<num_hide;cnt++)
		{
			weight_h_to_o[cnt].resize(num_output);
			for(int cnt_cur=0;cnt_cur<num_output;cnt_cur++)
			{
				weight_h_to_o[cnt][cnt_cur] = rand() % 2000 * 1.0 / 1000.0 - 1.0;
			}
		}

		thd_h.resize(num_hide);
		for(int cnt=0;cnt<num_hide;cnt++)
		{
			thd_h[cnt] = rand()%2000 * 1.0/1000.0 - 1.0;
		}

		thd_o.resize(num_output);
		for(int cnt=0;cnt<num_output;cnt++)
		{
			thd_o[cnt] = rand()%2000 * 1.0/1000.0 - 1.0;
		}
	}

	int get_num_node_in()
	{
		return weight_i_to_h.size();
	}

	int get_num_node_hide()
	{
		return weight_h_to_o.size();
	}

	int get_num_node_out()
	{
		return weight_h_to_o[0].size();
	}

	void save(string str_filename)
	{
		ofstream fout(str_filename.c_str());

		fout<<get_num_node_in()<<" "<<get_num_node_hide()<<" "<<get_num_node_out()<<" ";
		fout<<factor_study_h<<" ";
		fout<<factor_study_o<<" ";
		fout<<factor_thd_study_h<<" ";
		fout<<factor_thd_study_o<<" ";

		for(int cnt=0;cnt<thd_h.size();cnt++)
		{
			fout<<thd_h[cnt]<<" ";
		}

		for(int cnt=0;cnt<thd_o.size();cnt++)
		{
			fout<<thd_o[cnt]<<" ";
		}

		for(int i=0;i<get_num_node_in();i++)
		{
			for(int j=0;j<get_num_node_hide();j++)
			{
				fout<<weight_i_to_h[i][j]<<" ";
			}
		}

		for(int i=0;i<get_num_node_hide();i++)
		{
			for(int j=0;j<get_num_node_out();j++)
			{
				fout<<weight_h_to_o[i][j]<<" ";
			}
		}

		fout.close();
	}

	void load(string str_filename)
	{
		ifstream fin(str_filename.c_str());

		int n_in;
		int n_hide;
		int n_out;

		fin>>n_in>>n_hide>>n_out;
		fin>>factor_study_h;
		fin>>factor_study_o;
		fin>>factor_thd_study_h;
		fin>>factor_thd_study_o;

		thd_h.resize(n_hide);
		for(int cnt=0;cnt<n_hide;cnt++)
		{
			fin>>thd_h[cnt];
		}

		thd_o.resize(n_out);
		for(int cnt=0;cnt<n_out;cnt++)
		{
			fin>>thd_o[cnt];
		}

		weight_i_to_h.resize(n_in);
		for(int i=0;i<n_in;i++)
		{
			weight_i_to_h[i].resize(n_hide);
			for(int j=0;j<n_hide;j++)
			{
				fin>>weight_i_to_h[i][j];
			}
		}

		weight_h_to_o.resize(n_hide);
		for(int i=0;i<n_hide;i++)
		{
			weight_h_to_o[i].resize(n_out);
			for(int j=0;j<n_out;j++)
			{
				fin>>weight_h_to_o[i][j];
			}
		}

		fin.close();
	}

	void train(vector<vector<T> > & v_src,vector<vector<double> >  & v_des,int n_times)
	{
		while(n_times--)
		{
			train(v_src,v_des);
		}
	}

	void train(vector<vector<double> > & v_src,vector<vector<double> >  & v_des,double d_e)
	{
		while(e > d_e)
		{
			train(v_src,v_des);
		}
	}

	void train(vector<vector<T> > & v_src,vector<vector<T> >  & v_des)
	{
		vector<T> v_tmp,v_hide,v_tmp_o,v_o,v_diff_o,v_diff_hide;

		v_tmp.resize(weight_h_to_o.size());
		v_hide.resize(weight_h_to_o.size());
		v_tmp_o.resize(weight_h_to_o[0].size());
		v_o.resize(weight_h_to_o[0].size());
		v_diff_o.resize(weight_h_to_o[0].size());
		v_diff_hide.resize(weight_h_to_o.size());

		for(int cnt=0;cnt<v_src.size();cnt++)
		{
			for(int j=0;j<weight_h_to_o.size();j++)
			{
				v_tmp[j] = 0.0;

				for(int i=0;i<weight_i_to_h.size();i++)
				{
					v_tmp[j] += weight_i_to_h[i][j] * v_src[cnt][i];
				}

				v_hide[j] = 1.0 / ( 1 + exp( -v_tmp[j] - thd_h[j] ));
			}

			for(int k=0;k<weight_h_to_o[0].size();k++)
			{
				v_tmp_o[k] = 0.0;

				for(int j=0;j<weight_h_to_o.size();j++)
				{
					v_tmp_o[k] += weight_h_to_o[j][k] * v_hide[j];
				}

				v_o[k] = 1.0 / ( 1 + exp(-v_tmp_o[k] - thd_o[k]));
			}

			e = 0.0;
			for(int k=0;k<weight_h_to_o[0].size();k++)
			{
				v_diff_o[k] = (v_des[cnt][k] - v_o[k]) * v_o[k] * (1.0 - v_o[k]);
				e += fabs(v_des[cnt][k] - v_o[k]) * fabs(v_des[cnt][k] - v_o[k]);

				for(int j=0;j<weight_h_to_o.size();j++)
				{
					weight_h_to_o[j][k] += factor_study_o * v_diff_o[k] * v_hide[j];
				}
			}

			e = sqrt(e);

			for(int j=0;j<weight_h_to_o.size();j++)
			{
				v_diff_hide[j] = 0.0;

				for(int k=0;k<weight_h_to_o[0].size();k++)
				{
					v_diff_hide[j] += v_diff_o[k] * weight_h_to_o[j][k];
				}

				v_diff_hide[j] = v_diff_hide[j] * v_hide[j] * (1 - v_hide[j]);

				for(int i=0;i<weight_i_to_h.size();i++)
				{
					weight_i_to_h[i][j] += factor_study_h * v_diff_hide[j]*v_src[cnt][i];
				}
			}

			for(int k=0;k<weight_h_to_o[0].size();k++)
			{
				thd_o[k] = thd_o[k] + factor_thd_study_o * v_diff_o[k];
			}

			for(int j=0;j<weight_h_to_o.size();j++)
			{
				thd_h[j] = thd_h[j] + factor_thd_study_h * v_diff_hide[j];
			}
		}
	}

	void simulate(vector<T> & v_src,vector<T> & v_des)
	{
		vector<T> v_tmp,v_hide,v_tmp_o;

		v_tmp.resize(weight_h_to_o.size());
		v_hide.resize(weight_h_to_o.size());
		v_tmp_o.resize(weight_h_to_o[0].size());
		v_des.resize( weight_h_to_o[0].size() );

		for(int j=0;j<weight_h_to_o.size();j++)
		{
			v_tmp[j] = 0.0;

			for(int i=0;i<weight_i_to_h.size();i++)
			{
				v_tmp[j] += weight_i_to_h[i][j] * v_src[i];
			}

			v_hide[j] = 1.0 / ( 1 + exp(-v_tmp[j] - thd_h[j]));
		}

		for(int j=0;j<weight_h_to_o[0].size();j++)
		{
			v_tmp_o[j] = 0.0;

			for(int i=0;i<weight_h_to_o.size();i++)
			{
				v_tmp_o[j] += weight_h_to_o[i][j] * v_hide[i];
			}

			v_des[j] = 1.0 / ( 1 + exp(-v_tmp_o[j] -thd_o[j]));
		}
	}
};
