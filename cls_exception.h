#include "lib_import.h"

//the basic class for exception handling.
class cls_exception
	:virtual public cls_object
{
private:
	string str_context;

public:

	//what is the exception.
	cls_exception(string str)
	{
		str_context = str;
	}

	cls_exception(const char * str)
	{
		str_context = str;
	}

	//get the type of error for the exception.
	virtual string get_error_type()=0;

	//show the exception's content.
	string display()
	{
		string re;
		re += string("[") + get_error_type() + string("]") + "\n";
		re += str_context;

		return re;
	}

	//show the exception's content.
	string what()
	{
		return str_context;
	}

	virtual ~cls_exception()
	{
        ;
	}
};

//the exception for io.
class cls_exception_io
	:public cls_exception
{
public:
	cls_exception_io(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_io(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "file io error";
	}
};

//the exception for everything overflowing.
class cls_exception_overflow
	:public cls_exception
{
public:
	cls_exception_overflow(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_overflow(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the size is out of the subscripts.";
	}
};


//the exception for method not implemented.
class cls_exception_not_implemented
	:public cls_exception
{
public:
	cls_exception_not_implemented(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_not_implemented(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the function is not implemented.";
	}
};


//the exception for not_identical.
class cls_exception_not_identical
	:public cls_exception
{
public:
	cls_exception_not_identical(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_not_identical(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the two things doesn't match.";
	}
};

//the exception for special reason,
class cls_exception_special_value
	:public cls_exception
{
public:
	cls_exception_special_value(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_special_value(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "it must be a special value.";
	}
};

//the exception for training set.
class cls_exception_training_set
	:public cls_exception
{
public:
	cls_exception_training_set(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_training_set(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the training set is not right for the classifier.";
	}
};

//the exception for null pointer.
class cls_exception_null_pointer
	:public cls_exception
{
public:
	cls_exception_null_pointer(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_null_pointer(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the pointer can't be null.";
	}
};

//the exception for not found.
class cls_exception_not_found
	:public cls_exception
{
public:
	cls_exception_not_found(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_not_found(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "thedox can't found the rignt one.";
	}
};

//the exception for nonsence
class cls_exception_not_sense
	:public cls_exception
{
public:
	cls_exception_not_sense(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_not_sense(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the method you called is of no sense.";
	}
};

//the exception for low effective.
class cls_exception_low_effective
	:public cls_exception
{
public:
	cls_exception_low_effective(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_low_effective(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "you may get a low effective method.";
	}
};

//the exception for cache.
class cls_exception_cache_error
	:public cls_exception
{
public:
	cls_exception_cache_error(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_cache_error(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "somthing wrong with your cache.";
	}
};

//the exception for connection.
class cls_exception_connection_error
	:public cls_exception
{
public:
	cls_exception_connection_error(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_connection_error(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the connection can't be builded.";
	}
};

//the exception for object management.
class cls_exception_build_error
	:public cls_exception
{
public:
	cls_exception_build_error(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_build_error(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the object can't be fetched or builded.";
	}
};

//the exception for excution.
class cls_exception_execution
	:public cls_exception
{
public:
	cls_exception_execution(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_execution(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "something bad happened in the execution.";
	}
};

//the exception for not supporting.
class cls_exception_not_support
	:public cls_exception
{
public:
	cls_exception_not_support(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_not_support(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "thedox doesn't support this.";
	}
};

//the exception for unstable.
class cls_exception_un_stable
	:public cls_exception
{
public:
	cls_exception_un_stable(string str)
		:cls_exception(str)
	{
		;
	}

	cls_exception_un_stable(const char * str)
		:cls_exception(str)
	{
		;
	}

	virtual string get_error_type()
	{
		return "the system is unstable.";
	}
};
