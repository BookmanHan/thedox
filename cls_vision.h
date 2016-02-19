#include "lib_import.h"

class cls_vision
	:public virtual cls_vision_geometry,
	 public virtual cls_vision_mixed,
	 public virtual cls_vision_enhance_space,
	 public virtual cls_vision_enhance_frequence,
	 public virtual cls_vision_restoration
{
public:
	;
};