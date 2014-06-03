#include "stdafx.h"
#include "OS_2_And_Windows_Metrics.h"
using namespace std;
namespace ttf_dll{
  // us_weight_class
  enum usWeightClass{
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
  
  void OS_2_And_Windows_Metrics::load_table(Table_Directory_Entry *entry, ifstream &fin){
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

  void OS_2_And_Windows_Metrics::dump_info(FILE *fp, size_t indent){
    IND_FP(fp, indent, "<OS_2>\n");
    ++indent;
    IND_FP(fp, indent, "<version value=\"%u\"/>\n", version);
    IND_FP(fp, indent, "<xAvgCharWidth value=\"%d\"/>\n", x_avg_char_width);
    IND_FP(fp, indent, "<usWeightClass value=\"%u\"/>\n", us_weight_class);
    IND_FP(fp, indent, "<usWidthClass value=\"%u\"/>\n", us_width_class);
    IND_FP(fp, indent, "<fsType value=\"%u\"/>\n", fs_type);
    IND_FP(fp, indent, "<ySubscriptXSize value=\"%d\"/>\n", y_subscript_x_size);
    IND_FP(fp, indent, "<ySubscriptYSize value=\"%d\"/>\n", y_subscript_y_size);
    IND_FP(fp, indent, "<ySubscriptXOffset value=\"%d\"/>\n", y_subscript_x_offset);
    IND_FP(fp, indent, "<ySubscriptYOffset value=\"%d\"/>\n", y_subscript_y_offset);
    IND_FP(fp, indent, "<ySuperscriptXSize value=\"%d\"/>\n", y_superscript_x_size);
    IND_FP(fp, indent, "<ySuperscriptYSize value=\"%d\"/>\n", y_superscript_y_size);
    IND_FP(fp, indent, "<ySuperscriptXOffset value=\"%d\"/>\n", y_superscript_x_offset);
    IND_FP(fp, indent, "<ySuperscriptYOffset value=\"%d\"/>\n", y_superscript_y_offset);
    IND_FP(fp, indent, "<yStrikeoutSize value=\"%d\"/>\n", y_strike_out_size);
    IND_FP(fp, indent, "<yStrikeoutPosition value=\"%d\"/>\n", y_strike_out_position);
    IND_FP(fp, indent, "<sFamilyClass value=\"%d\"/>\n", s_family_class);
    IND_FP(fp, indent, "<panose>\n");
    ++indent;
    IND_FP(fp, indent, "<bFamilyType value=\"%d\"/>\n", panose.b_family_type);
    IND_FP(fp, indent, "<bSerifStyle value=\"%d\"/>\n", panose.b_serif_style);
    IND_FP(fp, indent, "<bWeight value=\"%d\"/>\n", panose.b_weight);
    IND_FP(fp, indent, "<bProportion value=\"%d\"/>\n", panose.b_proportion);
    IND_FP(fp, indent, "<bContrast value=\"%d\"/>\n", panose.b_contrast);
    IND_FP(fp, indent, "<bStrokeVariation value=\"%d\"/>\n", panose.b_stroke_variation);
    IND_FP(fp, indent, "<bArmStyle value=\"%d\"/>\n", panose.b_arm_style);
    IND_FP(fp, indent, "<bLetterForm value=\"%d\"/>\n", panose.b_letter_form);
    IND_FP(fp, indent, "<bMidline value=\"%d\"/>\n", panose.b_mid_line);
    IND_FP(fp, indent, "<bXHeight value=\"%d\"/>\n", panose.b_x_height);
    --indent;
    IND_FP(fp, indent, "</panose>\n");
    IND_FP(fp, indent, "<ulUnicodeRange1 value=\"%lu\"/>\n", ul_unicode_range_1);
    IND_FP(fp, indent, "<ulUnicodeRange2 value=\"%lu\"/>\n", ul_unicode_range_2);
    IND_FP(fp, indent, "<ulUnicodeRange3 value=\"%lu\"/>\n", ul_unicode_range_3);
    IND_FP(fp, indent, "<ulUnicodeRange4 value=\"%lu\"/>\n", ul_unicode_range_4);
    IND_FP(fp, indent, "<achVendID value=\"%.4s\"/>\n", ach_vend_id);
    IND_FP(fp, indent, "<fsSelection value=\"%u\"/>\n", fs_selection);
    IND_FP(fp, indent, "<usFirstCharIndex value=\"%u\"/>\n", us_first_char_index);
    IND_FP(fp, indent, "<usLastCharIndex value=\"%u\"/>\n", us_last_char_index);
    IND_FP(fp, indent, "<sTypoAscender value=\"%d\"/>\n", s_typo_ascender);
    IND_FP(fp, indent, "<sTypoDescender value=\"%d\"/>\n", s_typo_descender);
    IND_FP(fp, indent, "<sTypoLineGap value=\"%d\"/>\n", s_typo_line_cap);
    IND_FP(fp, indent, "<usWinAscent value=\"%d\"/>\n", us_win_ascent);
    IND_FP(fp, indent, "<usWinDescent value=\"%d\"/>\n", us_win_descent);
    IND_FP(fp, indent, "<ulCodePageRange1 value=\"%lu\"/>\n", ul_code_page_range_1);
    IND_FP(fp, indent, "<ulCodePageRange2 value=\"%lu\"/>\n", ul_code_page_range_2);
    IND_FP(fp, indent, "<sxHeight value=\"%d\"/>\n", sx_height);
    IND_FP(fp, indent, "<sCapHeight value=\"%d\"/>\n", s_cap_height);
    IND_FP(fp, indent, "<usDefaultChar value=\"%u\"/>\n", us_default_char);
    IND_FP(fp, indent, "<usBreakChar value=\"%u\"/>\n", us_break_char);
    IND_FP(fp, indent, "<usMaxContex value=\"%u\"/>\n", us_max_coutext);
    --indent;
    IND_FP(fp, indent, "</OS_2>\n");
  }
}