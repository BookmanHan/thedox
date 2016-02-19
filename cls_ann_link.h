#include "ann_import.h"

template<typename FUNC> class cls_ann_neuron;
template<typename FUNC> class cls_ann_link;

template<typename FUNC>
class cls_ann_link : public cls_serialized_object
{
private:
	friend class cls_ann_neuron<FUNC>;

	cls_ann_neuron<FUNC> *from;
	cls_ann_neuron<FUNC> *to;
	double weight;
	double delta_weight;
	double last_delta_weight;

public:
	cls_ann_link(cls_ann_neuron<FUNC> * from,cls_ann_neuron<FUNC> * to)
	{
		this->from = from;
		this->to = to;
		this->delta_weight = 0;
		this->last_delta_weight = 0;
		set_weight(a_uniform_random());
	}

	cls_ann_link(cls_ann_neuron<FUNC> * from,cls_ann_neuron<FUNC> * to,double weight)
	{
		this->from = from;
		this->to = to;
		this->delta_weight = 0;
		this->last_delta_weight = 0;
		set_weight(weight);
	}

	virtual ~cls_ann_link()
	{
#ifdef __USING_GCC__
		typename vector<cls_ann_link<FUNC>*>::iterator itor;
#else
		vector<cls_ann_link<FUNC>*>::iterator itor;
#endif

		for(itor=to->input_links.begin();itor != to->input_links.end();itor++)
		{
			if (check_equal_to(*itor))
			{
				itor = to->input_links.erase(itor);
				break;
			}
		}

		for(itor=from->output_links.begin();itor != from->output_links.end();itor++)
		{
			if (check_equal_to(*itor))
			{
				itor = from->output_links.erase(itor);
				break;
			}
		}
	}

	inline cls_ann_neuron<FUNC> * get_source() const
	{
		return from;
	}

	inline cls_ann_neuron<FUNC> * get_destination() const
	{
		return to;
	}

	inline double get_weight() const
	{
		return weight;
	}

	void set_weight(double weight)
	{
		this->weight = weight;
		if (to != null)
			to->invalidate_output_cache();

		if (from != null)
			from->invalidate_error_cache();
	}

	void set_delta_weight(double delta)
	{
		this->delta_weight = delta;
	}

	void update_weight()
	{
		weight += delta_weight;
		last_delta_weight = delta_weight;
		delta_weight = 0.0;
	}

	bool check_equal_to(cls_ann_link * that)
	{
		return (that->from == this->from && that->to == this->to);
	}

	double get_last_delta_weight()
	{
		return last_delta_weight;
	}

	virtual void save( ofstream& fout ) const
	{
		if(!fout)
			throw cls_exception_io("cls_ann_link::save() - file is bad for output.\n");

		fout.write((char*)&delta_weight,sizeof(delta_weight));
		fout.write((char*)&last_delta_weight,sizeof(last_delta_weight));
		fout.write((char*)&weight,sizeof(weight));
	}

	virtual void load( ifstream& fin )
	{
		if (!fin)
			throw cls_exception_io("cls_ann_link::load() - file is bad for input.\n");

		fin.read((char*)&delta_weight,sizeof(delta_weight));
		fin.read((char*)&last_delta_weight,sizeof(last_delta_weight));
		fin.read((char*)&weight,sizeof(weight));
	}
};
