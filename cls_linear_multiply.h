#include "lib_import.h"

class cls_linear_mutiply
	:public cls_classifier
{
protected:
	vector<cls_linear*> linears;
	int num;
	int per_num;

public:
	 virtual void save(ofstream& fout) const
	 {
		 if (!fout)
			 throw cls_exception_io("cls_linear_mutiply::save() - the file is bad for output.\n");

		 fout.write((char*)&num,sizeof(num));
		 fout.write((char*)&per_num,sizeof(per_num));

		 vector<cls_linear*>::const_iterator itor;
		 for(itor = linears.begin();itor != linears.end();itor++)
		 {
			 int num = 1;
			 (*itor)->save(fout);
			 fout.write((char*)&num,4);
		 }
	 }

	 virtual void load(ifstream& fin)
	 {
		 if (!fin)
			 throw cls_exception_io("cls_linear_mutiply::load() - the file is bad for input.\n");

		 vector<cls_linear*>::iterator itor;
		 for(itor=linears.begin();itor != linears.end();itor++)
		 {
			 delete (*itor);
		 }

		 linears.clear();

		 fin.read((char*)&num,sizeof(num));
		 fin.read((char*)&per_num,sizeof(per_num));

		 for(int cnt=0;cnt<num*num - num;cnt++)
		 {
			 add_linear();
		 }

		 for(itor = linears.begin();itor != linears.end();itor++)
		 {
			 assert(*itor);

			 (*itor)->load(fin);

			 int num;
			 fin.read((char*)&num,4);
			 cout<<num<<endl;
		 }
	 }

	 virtual ~cls_linear_mutiply()
	 {
		 vector<cls_linear*>::iterator itor;
		 for(itor=linears.begin();itor != linears.end();itor++)
		 {
			 delete (*itor);
		 }
	 }

	 cls_linear_mutiply(int num,int per,int output=0)
		 :cls_classifier(en_clser_linear)
	 {
		 this->num = num;
		 this->per_num = per;
	 }

	 virtual void train(cls_training_set & ts,int n_times) = 0;

	 virtual void add_linear() = 0;

	 virtual int get_output(vector<double>& input) = 0;

	 int get_num()
	 {
		 return num;
	 }

	 int get_size()
	 {
		 return linears.size();
	 }

	 int get_num_per_size()
	 {
		 return per_num;
	 }

	 virtual int get_output(double * input)
	 {
		 vector<double> in;
		 for(int cnt=0;cnt<per_num;cnt++)
		 {
			 in.push_back(input[cnt]);
		 }

		 return get_output(in);
	 }

	 virtual vector<double> get_output_data( const vector<double> & input ) 
	 {
		 throw cls_exception_not_sense("cls_linear_mutiply::get_output_data() - call get_index.\n");
	 }

	 virtual int get_output_index(const vector<double> & input ) 
	 {
		 return get_output((vector<double>&)input);
	 }

	 virtual double get_output_double(const cls_training_set& ts ) 
	 {
		 throw cls_exception_not_sense("cls_linear_mutiply::get_output_double() - call get_index.\n");
	 }
};