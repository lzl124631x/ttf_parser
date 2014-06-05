#ifndef OS_2_AND_WINDOWS_METRICS_H
#define OS_2_AND_WINDOWS_METRICS_H

#include "type.h"
#include "ttf_table.h"
/******************************* OS/2 ***********************************/
// https://www.microsoft.com/typography/otspec/os2.htm
namespace ttf_dll {

// version 0: TrueType rev 1.5
// version 1: TrueType rev 1.66
// version 2: OpenType rev 1.2
// version 3: OpenType rev 1.4
// version 4: OpenType rev 1.6
class DLL_API Os2AndWindowsMetrics { // version 4
  // FIXME: This table varies in different version. Always using version 4 is wrong.
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(TableRecordEntry *entry, ifstream &fin);
  // Deallocates the memory allocated in `LoadTable`, if any.
  void Destroy() {}
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;

  UShort    version_;
  Short     x_avg_char_width_;
  UShort    us_weight_class_;
  UShort    us_width_class_;
  UShort    fs_type_;
  Short     y_subscript_x_size_;
  Short     y_subscript_y_size_;
  Short     y_subscript_x_offset_;
  Short     y_subscript_y_offset_;
  Short     y_superscript_x_size_;
  Short     y_superscript_y_size_;
  Short     y_superscript_x_offset_;
  Short     y_superscript_y_offset_;
  Short     y_strike_out_size_;
  Short     y_strike_out_position_;
  Short     s_family_class_;
  struct PANOSE {
    Byte      b_family_type;
    Byte      b_serif_style;
    Byte      b_weight;
    Byte      b_proportion;
    Byte      b_contrast;
    Byte      b_stroke_variation;
    Byte      b_arm_style;
    Byte      b_letter_form;
    Byte      b_mid_line;
    Byte      b_x_height;
  };
  PANOSE    panose_;
  ULong     ul_unicode_range_1_;      // Bits 0-31
  ULong     ul_unicode_range_2_;      // Bits 32-63
  ULong     ul_unicode_range_3_;      // Bits 64-95
  ULong     ul_unicode_range_4_;      // Bits 96-127
  Char      ach_vend_id_[4];
  UShort    fs_selection_;
  UShort    us_first_char_index_;
  UShort    us_last_char_index_;
  Short     s_typo_ascender_;
  Short     s_typo_descender_;
  Short     s_typo_line_cap_;
  UShort    us_win_ascent_;
  UShort    us_win_descent_;
  ULong     ul_code_page_range_1_;    // Bits 0-31
  ULong     ul_code_page_range_2_;    // Bits 32-63
  Short     sx_height_;
  Short     s_cap_height_;
  UShort    us_default_char_;
  UShort    us_break_char_;
  UShort    us_max_coutext_;
};

} // namespace ttf_dll
#endif