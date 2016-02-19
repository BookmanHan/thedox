#include "lib_import.h"

class cls_log
	:virtual public cls_object
{
private:
	ofstream fout;

public:
	~cls_log()
	{
		fout.close();
	}

	cls_log()
		:fout("std.log")
	{
		;
	}

	cls_log(string str_filename)
		:fout(str_filename.c_str())
	{
		;
	}

	template<typename T>
	cls_log & operator << ( T src )
	{
		time_t now;
		struct tm * time_now;

		time(&now);
		time_now = localtime(&now);

		fout<<"["
			<<time_now->tm_year + 1900<<":"
			<<setw(2)<<time_now->tm_mon<<":"
			<<setw(2)<<time_now->tm_mday<<" "
			<<setw(2)<<time_now->tm_hour<<":"
			<<setw(2)<<time_now->tm_min<<":"
			<<setw(2)<<time_now->tm_sec
			<<"]  "<<src<<endl;

		return *this;
	}
};