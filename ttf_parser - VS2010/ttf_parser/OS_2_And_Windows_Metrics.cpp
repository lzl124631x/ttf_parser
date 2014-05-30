#include "stdafx.h"
#include "OS_2_And_Windows_Metrics.h"

namespace ttf_dll {

// us_weight_class
enum usWeightClass {
  FW_THIN                 = 100,
  FW_EXTRA_LIGHT          = 200,
  FW_LIGHT                = 300,
  FW_NORMAL               = 400,
  FW_MEDIUM               = 500,
  FW_SEMIBOLD             = 600,
  FW_BOLD                 = 700,
  FW_EXTRABOLD            = 800,
  FW_BLACK                = 900
};
// us_width_class
const float FWIDTH_ULTRA_CONDENSED      = 50;
const float FWIDTH_EXTRA_CONDENSED      = 62.5;
const float FWIDTH_CONDENSED            = 75;
const float FWIDTH_SEMI_CONDENSED       = 87.5;
const float FWIDTH_NORMAL               = 100;
const float FWIDTH_SEMI_EXPANDED        = 112.5;
const float FWIDTH_EXPANDED             = 125;
const float FWIDTH_EXTRA_EXPANDED       = 150;
const float FWIDTH_ULTRA_EXPANDED       = 200;

void OS_2_And_Windows_Metrics::load_table(Table_Record_Entry *entry, ifstream &fin) {
  fin.seekg(entry->offset, ios::beg);
  FREAD(fin, &version);
  FREAD(fin, &x_avg_char_width);
  FREAD(fin, &us_weight_class);
  FREAD(fin, &us_width_class);
  FREAD(fin, &fs_type);
  FREAD(fin, &y_subscript_x_size);
  FREAD(fin, &y_subscript_y_size);
  FREAD(fin, &y_subscript_x_offset);
  FREAD(fin, &y_subscript_y_offset);
  FREAD(fin, &y_superscript_x_size);
  FREAD(fin, &y_superscript_y_size);
  FREAD(fin, &y_superscript_x_offset);
  FREAD(fin, &y_superscript_y_offset);
  FREAD(fin, &y_strike_out_size);
  FREAD(fin, &y_strike_out_position);
  FREAD(fin, &s_family_class);
  fin.read((char*)&panose, sizeof(PANOSE));
  FREAD(fin, &ul_unicode_range_1);
  FREAD(fin, &ul_unicode_range_2);
  FREAD(fin, &ul_unicode_range_3);
  FREAD(fin, &ul_unicode_range_4);
  FREAD_N(fin, ach_vend_id, 4);
  FREAD(fin, &fs_selection);
  FREAD(fin, &us_first_char_index);
  FREAD(fin, &us_last_char_index);
  FREAD(fin, &s_typo_ascender);
  FREAD(fin, &s_typo_descender);
  FREAD(fin, &s_typo_line_cap);
  FREAD(fin, &us_win_ascent);
  FREAD(fin, &us_win_descent);
  FREAD(fin, &ul_code_page_range_1);
  FREAD(fin, &ul_code_page_range_2);
  FREAD(fin, &sx_height);
  FREAD(fin, &s_cap_height);
  FREAD(fin, &us_default_char);
  FREAD(fin, &us_break_char);
  FREAD(fin, &us_max_coutext);
}

void OS_2_And_Windows_Metrics::dump_info(Xml_Logger &logger) const {
  logger.println("<OS_2>");
  logger.inc_indent();
  logger.println("<version value=\"%u\"/>", version);
  logger.println("<xAvgCharWidth value=\"%d\"/>", x_avg_char_width);
  logger.println("<usWeightClass value=\"%u\"/>", us_weight_class);
  logger.println("<usWidthClass value=\"%u\"/>", us_width_class);
  logger.println("<fsType value=\"%u\"/>", fs_type);
  logger.println("<ySubscriptXSize value=\"%d\"/>", y_subscript_x_size);
  logger.println("<ySubscriptYSize value=\"%d\"/>", y_subscript_y_size);
  logger.println("<ySubscriptXOffset value=\"%d\"/>", y_subscript_x_offset);
  logger.println("<ySubscriptYOffset value=\"%d\"/>", y_subscript_y_offset);
  logger.println("<ySuperscriptXSize value=\"%d\"/>", y_superscript_x_size);
  logger.println("<ySuperscriptYSize value=\"%d\"/>", y_superscript_y_size);
  logger.println("<ySuperscriptXOffset value=\"%d\"/>", y_superscript_x_offset);
  logger.println("<ySuperscriptYOffset value=\"%d\"/>", y_superscript_y_offset);
  logger.println("<yStrikeoutSize value=\"%d\"/>", y_strike_out_size);
  logger.println("<yStrikeoutPosition value=\"%d\"/>", y_strike_out_position);
  logger.println("<sFamilyClass value=\"%d\"/>", s_family_class);
  logger.println("<panose>");
  logger.inc_indent();
  logger.println("<bFamilyType value=\"%d\"/>", panose.b_family_type);
  logger.println("<bSerifStyle value=\"%d\"/>", panose.b_serif_style);
  logger.println("<bWeight value=\"%d\"/>", panose.b_weight);
  logger.println("<bProportion value=\"%d\"/>", panose.b_proportion);
  logger.println("<bContrast value=\"%d\"/>", panose.b_contrast);
  logger.println("<bStrokeVariation value=\"%d\"/>", panose.b_stroke_variation);
  logger.println("<bArmStyle value=\"%d\"/>", panose.b_arm_style);
  logger.println("<bLetterForm value=\"%d\"/>", panose.b_letter_form);
  logger.println("<bMidline value=\"%d\"/>", panose.b_mid_line);
  logger.println("<bXHeight value=\"%d\"/>", panose.b_x_height);
  logger.dec_indent();
  logger.println("</panose>");
  logger.println("<ulUnicodeRange1 value=\"%lu\"/>", ul_unicode_range_1);
  logger.println("<ulUnicodeRange2 value=\"%lu\"/>", ul_unicode_range_2);
  logger.println("<ulUnicodeRange3 value=\"%lu\"/>", ul_unicode_range_3);
  logger.println("<ulUnicodeRange4 value=\"%lu\"/>", ul_unicode_range_4);
  logger.println("<achVendID value=\"%.4s\"/>", ach_vend_id);
  logger.println("<fsSelection value=\"%u\"/>", fs_selection);
  logger.println("<usFirstCharIndex value=\"%u\"/>", us_first_char_index);
  logger.println("<usLastCharIndex value=\"%u\"/>", us_last_char_index);
  logger.println("<sTypoAscender value=\"%d\"/>", s_typo_ascender);
  logger.println("<sTypoDescender value=\"%d\"/>", s_typo_descender);
  logger.println("<sTypoLineGap value=\"%d\"/>", s_typo_line_cap);
  logger.println("<usWinAscent value=\"%d\"/>", us_win_ascent);
  logger.println("<usWinDescent value=\"%d\"/>", us_win_descent);
  logger.println("<ulCodePageRange1 value=\"%lu\"/>", ul_code_page_range_1);
  logger.println("<ulCodePageRange2 value=\"%lu\"/>", ul_code_page_range_2);
  logger.println("<sxHeight value=\"%d\"/>", sx_height);
  logger.println("<sCapHeight value=\"%d\"/>", s_cap_height);
  logger.println("<usDefaultChar value=\"%u\"/>", us_default_char);
  logger.println("<usBreakChar value=\"%u\"/>", us_break_char);
  logger.println("<usMaxContex value=\"%u\"/>", us_max_coutext);
  logger.dec_indent();
  logger.println("</OS_2>");
}

} // namespace ttf_dll