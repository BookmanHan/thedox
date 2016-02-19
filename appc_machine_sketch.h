#include "lib_import.h"

class appc_machine_sketch
{
protected:
	cls_vision vision;

public:
	appc_machine_sketch(wstring str_filename,int size = 3)
	{
		a_load_image(wstring_to_string(str_filename).c_str(),vision);
		vision.reverse();
		
		cls_vision vision_bk = vision;
		vision_bk.enhance_sequence_rectangle(size,size);

		vision.mixed_linear_dodge(vision_bk,vision);
	}

	cls_vision& result()
	{
		return vision;
	}
};