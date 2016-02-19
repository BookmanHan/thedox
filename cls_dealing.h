#include "lib_import.h"

#ifdef	USING_BOOST

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class cls_dealing
{
public:
	vector<string> filenames;

	cls_dealing(const string& path_dir)
	{
		fs::path full_path = fs::system_complete(fs::path(path_dir,fs::native));
		if (! fs::exists(full_path))
			throw cls_exception_not_found("the path is not found!");
		if (! fs::is_directory(full_path))
			throw cls_exception_not_support("this is not a path!");

		fs::directory_iterator itor_end;
		for(fs::directory_iterator itor(full_path); itor != itor_end; itor++)
		{
			if (!fs::is_directory(*itor))
			{
				filenames.push_back((*itor).path().string());
			}
		}
	}

	cls_dealing(const string& path_dir,const string& str_ext)
	{
		fs::path full_path = fs::system_complete(fs::path(path_dir,fs::native));
		if (! fs::exists(full_path))
			throw cls_exception_not_found("the path is not found!");
		if (! fs::is_directory(full_path))
			throw cls_exception_not_support("this is not a path!");

		fs::directory_iterator itor_end;
		for(fs::directory_iterator itor(full_path); itor != itor_end; itor++)
		{
			if (!fs::is_directory(*itor) && fs::extension(*itor) == str_ext)
			{
				filenames.push_back((*itor).path().string());
			}
		}
	}

	void run()
	{
		vector<string>::iterator itor;
		for(itor=filenames.begin();itor!=filenames.end();itor++)
		{
			deal(*itor);
		}
	}

	virtual void deal(string filename) = 0;
};

#endif