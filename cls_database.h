#include "dlib_import.h"

class cls_database
{
public:

	virtual void create_table(cls_database_create& param) = 0;
	virtual void insert_row(cls_database_insert& param) = 0;
	virtual void select_rows() = 0;
	virtual void update_row() = 0;
	virtual void delete_row() = 0;

	virtual ~cls_database()
	{
		;
	}
};