#include "stdafx.h"
#include "os_2_and_windows_metrics.h"
using namespace std;

// Disable warning C4351: new behavior: elements of array will be default
// initialized.
#pragma warning(disable: 4351)

namespace ttf_dll {

// us_weight_class
enum usWeightClass {
  kFontWeightThin                 = 100,
  kFontWeightExtraLight           = 200,
  kFontWeightLight                = 300,
  kFontWeightNormal               = 400,
  kFontWeightMedium               = 500,
  kFontWeightSemibold             = 600,
  kFontWeightBold                 = 700,
  kFontWeightExtrabold            = 800,
  kFontWeightBlack                = 900
};

// us_width_class
const float kFontWidthUltraCondensed        = 50;
const float kFontWidthExtraCondensed        = 62.5;
const float kFontWidthCondensed             = 75;
const float kFontWidthSemiCondensed         = 87.5;
const float kFontWidthNormal                = 100;
const float kFontWidthSemiExpanded          = 112.5;
const float kFontWidthExpanded              = 125;
const float kFontWidthExtraExpanded         = 150;
const float kFontWidthUltraExpanded         = 200;

Os2AndWindowsMetrics::Os2AndWindowsMetrics(const TrueTypeFont &ttf)
    : TtfSubtable(ttf),
      version_(0),
      x_avg_char_width_(0),
      us_weight_class_(0),
      us_width_class_(0),
      fs_type_(0),
      y_subscript_x_size_(0),
      y_subscript_y_size_(0),
      y_subscript_x_offset_(0),
      y_subscript_y_offset_(0),
      y_superscript_x_size_(0),
      y_superscript_y_size_(0),
      y_superscript_x_offset_(0),
      y_superscript_y_offset_(0),
      y_strike_out_size_(0),
      y_strike_out_position_(0),
      s_family_class_(0),
      // Default initialization. In this case, fields are set to zero.
      panose_(),
      ul_unicode_range_1_(0),
      ul_unicode_range_2_(0),
      ul_unicode_range_3_(0),
      ul_unicode_range_4_(0),
      // Default initialization. In this case, each element is set to zero.
      ach_vend_id_(),
      fs_selection_(0),
      us_first_char_index_(0),
      us_last_char_index_(0),
      s_typo_ascender_(0),
      s_typo_descender_(0),
      s_typo_line_cap_(0),
      us_win_ascent_(0),
      us_win_descent_(0),
      ul_code_page_range_1_(0),
      ul_code_page_range_2_(0),
      sx_height_(0),
      s_cap_height_(0),
      us_default_char_(0),
      us_break_char_(0),
      us_max_coutext_(0) {}

void Os2AndWindowsMetrics::Init(const TableRecordEntry *entry,
                                ifstream &fin) {
  fin.seekg(entry->offset(), ios::beg);
  FREAD(fin, &version_);
  FREAD(fin, &x_avg_char_width_);
  FREAD(fin, &us_weight_class_);
  FREAD(fin, &us_width_class_);
  FREAD(fin, &fs_type_);
  FREAD(fin, &y_subscript_x_size_);
  FREAD(fin, &y_subscript_y_size_);
  FREAD(fin, &y_subscript_x_offset_);
  FREAD(fin, &y_subscript_y_offset_);
  FREAD(fin, &y_superscript_x_size_);
  FREAD(fin, &y_superscript_y_size_);
  FREAD(fin, &y_superscript_x_offset_);
  FREAD(fin, &y_superscript_y_offset_);
  FREAD(fin, &y_strike_out_size_);
  FREAD(fin, &y_strike_out_position_);
  FREAD(fin, &s_family_class_);
  fin.read((char*)&panose_, sizeof(PANOSE));
  FREAD(fin, &ul_unicode_range_1_);
  FREAD(fin, &ul_unicode_range_2_);
  FREAD(fin, &ul_unicode_range_3_);
  FREAD(fin, &ul_unicode_range_4_);
  FREAD_N(fin, ach_vend_id_, 4);
  FREAD(fin, &fs_selection_);
  FREAD(fin, &us_first_char_index_);
  FREAD(fin, &us_last_char_index_);
  FREAD(fin, &s_typo_ascender_);
  FREAD(fin, &s_typo_descender_);
  FREAD(fin, &s_typo_line_cap_);
  FREAD(fin, &us_win_ascent_);
  FREAD(fin, &us_win_descent_);
  FREAD(fin, &ul_code_page_range_1_);
  FREAD(fin, &ul_code_page_range_2_);
  FREAD(fin, &sx_height_);
  FREAD(fin, &s_cap_height_);
  FREAD(fin, &us_default_char_);
  FREAD(fin, &us_break_char_);
  FREAD(fin, &us_max_coutext_);
}

void Os2AndWindowsMetrics::DumpInfo(XmlLogger &logger) const {
  logger.Println("<OS_2>");
  logger.IncreaseIndent();
  logger.Println("<version value=\"%u\"/>", version_);
  logger.Println("<xAvgCharWidth value=\"%d\"/>", x_avg_char_width_);
  logger.Println("<usWeightClass value=\"%u\"/>", us_weight_class_);
  logger.Println("<usWidthClass value=\"%u\"/>", us_width_class_);
  logger.Println("<fsType value=\"%u\"/>", fs_type_);
  logger.Println("<ySubscriptXSize value=\"%d\"/>", y_subscript_x_size_);
  logger.Println("<ySubscriptYSize value=\"%d\"/>", y_subscript_y_size_);
  logger.Println("<ySubscriptXOffset value=\"%d\"/>", y_subscript_x_offset_);
  logger.Println("<ySubscriptYOffset value=\"%d\"/>", y_subscript_y_offset_);
  logger.Println("<ySuperscriptXSize value=\"%d\"/>", y_superscript_x_size_);
  logger.Println("<ySuperscriptYSize value=\"%d\"/>", y_superscript_y_size_);
  logger.Println("<ySuperscriptXOffset value=\"%d\"/>", y_superscript_x_offset_);
  logger.Println("<ySuperscriptYOffset value=\"%d\"/>", y_superscript_y_offset_);
  logger.Println("<yStrikeoutSize value=\"%d\"/>", y_strike_out_size_);
  logger.Println("<yStrikeoutPosition value=\"%d\"/>", y_strike_out_position_);
  logger.Println("<sFamilyClass value=\"%d\"/>", s_family_class_);
  logger.Println("<panose>");
  logger.IncreaseIndent();
  logger.Println("<bFamilyType value=\"%d\"/>", panose_.b_family_type);
  logger.Println("<bSerifStyle value=\"%d\"/>", panose_.b_serif_style);
  logger.Println("<bWeight value=\"%d\"/>", panose_.b_weight);
  logger.Println("<bProportion value=\"%d\"/>", panose_.b_proportion);
  logger.Println("<bContrast value=\"%d\"/>", panose_.b_contrast);
  logger.Println("<bStrokeVariation value=\"%d\"/>", panose_.b_stroke_variation);
  logger.Println("<bArmStyle value=\"%d\"/>", panose_.b_arm_style);
  logger.Println("<bLetterForm value=\"%d\"/>", panose_.b_letter_form);
  logger.Println("<bMidline value=\"%d\"/>", panose_.b_mid_line);
  logger.Println("<bXHeight value=\"%d\"/>", panose_.b_x_height);
  logger.DecreaseIndent();
  logger.Println("</panose>");
  logger.Println("<ulUnicodeRange1 value=\"%lu\"/>", ul_unicode_range_1_);
  logger.Println("<ulUnicodeRange2 value=\"%lu\"/>", ul_unicode_range_2_);
  logger.Println("<ulUnicodeRange3 value=\"%lu\"/>", ul_unicode_range_3_);
  logger.Println("<ulUnicodeRange4 value=\"%lu\"/>", ul_unicode_range_4_);
  logger.Println("<achVendID value=\"%.4s\"/>", ach_vend_id_);
  logger.Println("<fsSelection value=\"%u\"/>", fs_selection_);
  logger.Println("<usFirstCharIndex value=\"%u\"/>", us_first_char_index_);
  logger.Println("<usLastCharIndex value=\"%u\"/>", us_last_char_index_);
  logger.Println("<sTypoAscender value=\"%d\"/>", s_typo_ascender_);
  logger.Println("<sTypoDescender value=\"%d\"/>", s_typo_descender_);
  logger.Println("<sTypoLineGap value=\"%d\"/>", s_typo_line_cap_);
  logger.Println("<usWinAscent value=\"%d\"/>", us_win_ascent_);
  logger.Println("<usWinDescent value=\"%d\"/>", us_win_descent_);
  logger.Println("<ulCodePageRange1 value=\"%lu\"/>", ul_code_page_range_1_);
  logger.Println("<ulCodePageRange2 value=\"%lu\"/>", ul_code_page_range_2_);
  logger.Println("<sxHeight value=\"%d\"/>", sx_height_);
  logger.Println("<sCapHeight value=\"%d\"/>", s_cap_height_);
  logger.Println("<usDefaultChar value=\"%u\"/>", us_default_char_);
  logger.Println("<usBreakChar value=\"%u\"/>", us_break_char_);
  logger.Println("<usMaxContex value=\"%u\"/>", us_max_coutext_);
  logger.DecreaseIndent();
  logger.Println("</OS_2>");
}

} // namespace ttf_dll