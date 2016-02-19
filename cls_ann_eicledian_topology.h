#include "ann_import.h"

class cls_ann_topology : public cls_serialized_object
{
protected:
	unsigned total_count;
	double max_dim;

public:
	virtual double get_neighbor_weight(unsigned src,unsigned des, double size) const = 0;
	virtual bool check_grid_neighbor(unsigned src,unsigned des) const  = 0;

	unsigned get_total_count() const
	{
		return total_count;
	}

	double get_max_dim() const
	{
		return max_dim;
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_topology::save() - the file is bad for output.\n");

		fout.write((char*)&total_count,sizeof(total_count));
		fout.write((char*)&max_dim,sizeof(max_dim));
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_ann_topology::load() - the file is bad for input.\n");

		fin.read((char*)&total_count,sizeof(total_count));
		fin.read((char*)&max_dim,sizeof(max_dim));
	}
};

template <unsigned t_dim>
class cls_ann_eucledian_topology : public cls_ann_topology
{
public:
	typedef unsigned axes[t_dim];

public:
	axes sizes;

public:
	cls_ann_eucledian_topology(const axes & sizes)
	{
		total_count = 1;
		double mxd_2 = 0;

		for(int i=0;i<t_dim;i++)
		{
			this->sizes[i] = sizes[i];
			total_count *= sizes[i];
			mxd_2 += sizes[i] *sizes[i];
		}

		max_dim = sqrt(mxd_2);
	}

	virtual double get_neighbor_weight(unsigned src,unsigned des,double size) const
	{
		axes a;
		id_to_axes(src,a);
		vector<double> sa = axes_to_vector(a);
		id_to_axes(des,a);
		vector<double> da = axes_to_vector(a);

		double sum = 0;
		for(int cnt=0;cnt<t_dim;cnt++)
		{
			sum += (sa[cnt] - da[cnt])*(sa[cnt] - da[cnt]);
		}

		if ( sqrt(sum) <= size)
			return 1.0;
		else
			return 0.0;
	}

	virtual bool check_grid_neighbor(unsigned src,unsigned des) const
	{
		return get_neighbor_weight(src,des,1.0) == 1.0;
	}

	unsigned get_size(unsigned axis) const
	{
		return (this->axes)[axis];
	}

	void id_to_axes(unsigned id,axes &out) const
	{
		for(unsigned cnt=0; cnt<t_dim;cnt++)
		{
			out[cnt] = id % sizes[cnt];
			id /= sizes[cnt];
		}
	}

	vector<double> axes_to_vector(const axes & a) const
	{
		vector<double> out(t_dim);
		for(unsigned cnt=0;cnt<t_dim;cnt++)
		{
			out[cnt] = a[cnt];
		}

		return out;
	}

	virtual void save( ofstream& fout ) const
	{
		try
		{
			cls_ann_topology::save(fout);

			int num = t_dim;
			fout.write((char*)&num,sizeof(t_dim));
			for(int cnt=0;cnt<t_dim;cnt++)
			{
				fout.write((char*)&sizes[cnt],sizeof(unsigned));
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_eucledian_topology::save() - the output file is bad.\n");
		}
	}

	virtual void load( ifstream& fin )
	{
		try
		{
			cls_ann_topology::load(fin);

			int num;
			fin.read((char*)&num,sizeof(int));

			if (num != t_dim)
				throw cls_exception_not_identical("cls_ann_eucledian_topology::load() - the dim is not the same.");

			for(int cnt=0;cnt<t_dim;cnt++)
			{
				fin.read((char*)&sizes[cnt],sizeof(unsigned));
			}
		}
		catch(cls_exception_io exc)
		{
			throw cls_exception_io(exc.what() + "cls_ann_eucledian_topology::load() - the input file is bad.\n");
		}
	}
};
