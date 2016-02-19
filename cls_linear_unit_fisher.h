#include "lib_import.h"

class cls_linear_unit_fisher
	:public cls_linear_unit_abstract
{
protected:
	cls_matrix<double> mat_means;
	cls_multi_array<double> mat_s;
	cls_matrix<double> mat_sw;
	cls_matrix<double> mat_isw;
	cls_matrix<double> mat_w;
	double d_thd;

	vector<vector<double>*> *input_1,*input_2;
	int n_per;

	bool cmp;

public:

	virtual void save(ofstream& fout) const
	{
		cls_linear_unit_abstract::save(fout);

		mat_means.save(fout);
		mat_s.save(fout);
		mat_sw.save(fout);
		mat_isw.save(fout);
		mat_w.save(fout);
		
		fout.write((char*)&d_thd,sizeof(double));
		fout.write((char*)&n_per,sizeof(n_per));
		fout.write((char*)&cmp,sizeof(cmp));

	}

	virtual void load(ifstream& fin)
	{
		cls_linear_unit_abstract::load(fin);

		mat_means.load(fin);
		mat_s.load(fin);
		mat_sw.load(fin);
		mat_isw.load(fin);
		mat_w.load(fin);

		fin.read((char*)&d_thd,sizeof(double));
		fin.read((char*)&n_per,sizeof(n_per));
		fin.read((char*)&cmp,sizeof(cmp));
	}

	cls_linear_unit_fisher(int num)
		:cls_linear_unit_abstract(num),
		mat_means(2,num),
		mat_s(2,num,num),
		mat_sw(num,num),
		mat_isw(num,num),
		mat_w(num,1),
		n_per(num)
	{
		cmp = false;
	}

	virtual void set_input(vector<vector<double>*>* i1,vector<vector<double>*>* i2)
	{
		input_1 = i1;
		input_2 = i2;
	}

	virtual bool update(double d_study_factor) 
	{
		mat_means.fill(0);

		int ar_num[2];
		ar_num[0] = input_1->size();
		ar_num[1] = input_2->size();

		for(int cnt=0;cnt<n_per;cnt++)
		{
			int size;
			size = input_1->size();
			for(int i=0;i<size;i++)
			{
				mat_means(0,cnt) += (*(*input_1)[i])[cnt];
			}

			size = input_2->size();
			for(int i=0;i<size;i++)
			{
				mat_means(1,cnt) += (*(*input_2)[i])[cnt];
			}

			//throw cls_exception;

			mat_means(0,cnt) /= ar_num[0];
			mat_means(1,cnt) /= ar_num[1];
		}

		for(int i=0;i<n_per;i++)
		{
			for(int j=0;j<n_per;j++)
			{
				double s[2];
				s[0] = s[1] = 0;

				int size;
				size = input_1->size();
				for(int cnt=0;cnt<size;cnt++)
				{
					s[0] += ((*(*input_1)[cnt])[i] - mat_means(0,i)) * 
						((*(*input_1)[cnt])[j] - mat_means(0,j));
				}

				size = input_2->size();
				for(int cnt=0;cnt<size;cnt++)
				{
					s[1] += ((*(*input_2)[cnt])[i] - mat_means(1,i)) * 
						((*(*input_2)[cnt])[j] - mat_means(1,j));
				}

				s[0] /= ar_num[0];
				s[1] /= ar_num[1];

				mat_s(0,i,j) = s[0];
				mat_s(1,i,j) = s[1];
			}
		}

		for(int i=0;i<n_per;i++)
		{
			for(int j=0;j<n_per;j++)
			{
				mat_sw(i,j) = mat_s(0,i,j) + mat_s(1,i,j);  
			}
		}
		
		mat_isw = mat_sw.get_inverse();

		for(int cnt=0;cnt<n_per;cnt++)
		{
			mat_w(cnt,0) = mat_means(0,cnt) - mat_means(1,cnt);
		}

		mat_w = ((mat_isw) * mat_w);

		double m[2];
		m[0] = m[1] = 0;

		for(int cnt=0;cnt<input_1->size();cnt++)
		{
			cls_matrix<double> mat_mode(n_per,1);
			for(int i=0;i<n_per;i++)
			{
				mat_mode(i,0) = (*(*input_1)[cnt])[i];
			}

			m[0] += (mat_mode.transpose() * mat_w)(0,0);
		}

		for(int cnt=0;cnt<input_2->size();cnt++)
		{
			cls_matrix<double> mat_mode(n_per,1);
			for(int i=0;i<n_per;i++)
			{
				mat_mode(i,0) = (*(*input_2)[cnt])[i];
			}

			m[1] += (mat_mode.transpose() * mat_w)(0,0);
		}

		m[0] /= ar_num[0];
		m[1] /= ar_num[1];
		d_thd = (m[0]*ar_num[0] + m[1]*ar_num[1]) / (ar_num[0] + ar_num[1]);

		if (m[0] > m[1])
			cmp = true;
		else
			cmp = false;

		return cmp;
	}

	virtual void re_cache_error() 
	{
		throw cls_exception_not_implemented("cls_linear_unit_fisher::re_cache_error() - it mustn't be called.");
	}

	virtual void re_cache_output() 
	{
		if (output_cache_validate)
			return;

		cls_matrix<double> mat_tmp(n_per,1);
		for(int cnt=0;cnt<n_per;cnt++)
		{
			mat_tmp(cnt,0) = (*inputs)[cnt];
		}

		double y = (mat_tmp.get_transpose() * mat_w)(0,0);

		if (cmp == true)
		{
			if (y >= d_thd)
				output_cache = +1;
			else 
				output_cache = -1;
		}
		else
		{
			if (y >= d_thd)
				output_cache = -1;
			else 
				output_cache = +1;
		}

		output_cache_validate = true;
	}
};