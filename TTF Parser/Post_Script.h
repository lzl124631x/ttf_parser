#ifndef POST_SCRIPT_H
#define POST_SCRIPT_H

#include "TTF_Table.h"
/******************************* post ***********************************/
class Post_Script{
	FIXED	format_type;
	FIXED	italic_angle;
	FWORD	underline_position;
	FWORD	underline_thickness;
	ULONG	is_fixed_pitch;
	ULONG	min_mem_type42;
	ULONG	max_mem_type42;
	ULONG	min_mem_type1;
	ULONG	max_mem_type1;
};

#endif