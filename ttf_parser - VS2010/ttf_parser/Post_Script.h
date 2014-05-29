#ifndef POST_SCRIPT_H
#define POST_SCRIPT_H

#include "Type.h"
#include "TTF_Table.h"
// FIXME: why is TTF_Table.h included here? answer: because load_table function needs the definition of TTF_Table_Entry.
// Is there any better solution that can combine the two #include?
/******************************* post ***********************************/
// https://www.microsoft.com/typography/otspec/post.htm
namespace ttf_dll{
  class DLL_API Post_Script{
    FIXED  format_type;
    FIXED  italic_angle;
    FWORD  underline_position;
    FWORD  underline_thickness;
    ULONG  is_fixed_pitch;
    ULONG  min_mem_type42;
    ULONG  max_mem_type42;
    ULONG  min_mem_type1;
    ULONG  max_mem_type1;
  };
}
#endif