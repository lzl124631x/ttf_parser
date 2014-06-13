#ifndef POST_SCRIPT_H
#define POST_SCRIPT_H

#include "type.h"
#include "ttf_table.h"
// FIXME: why is ttf_table.h included here?
// Answer: because load_table function needs the definition of
// `TTF_Table_Entry`.
// Is there any better solution that can combine the two #include?
/****************************************************************************/
/*                        post - Post Script                                */
/* Spec: https://www.microsoft.com/typography/otspec/post.htm               */
/****************************************************************************/
namespace ttf_dll {

class DLL_API Post_Script {
 private:
  Fixed  format_type_;
  Fixed  italic_angle_;
  FWord  underline_position_;
  FWord  underline_thickness_;
  ULong  is_fixed_pitch_;
  ULong  min_mem_type42_;
  ULong  max_mem_type42_;
  ULong  min_mem_type1_;
  ULong  max_mem_type1_;
};

} // namespace ttf_dll
#endif