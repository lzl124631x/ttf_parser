#include "stdafx.h"
#include "OS_2_And_Windows_Metrics.h"

namespace ttf_dll{
	// us_weight_class
#define FW_THIN				100
#define FW_EXTRA_LIGHT		200
#define FW_LIGHT			300
#define FW_NORMAL			400
#define FW_MEDIUM			500
#define FW_SEMIBOLD			600
#define FW_BOLD				700
#define FW_EXTRABOLD		800
#define FW_BLACK			900
	// us_width_class
#define FWIDTH_ULTRA_CONDENSED		50
#define FWIDTH_EXTRA_CONDENSED		62.5
#define FWIDTH_CONDENSED			75
#define FWIDTH_SEMI_CONDENSED		87.5
#define FWIDTH_NORMAL				100
#define FWIDTH_SEMI_EXPANDED		112.5
#define FWIDTH_EXPANDED				125
#define FWIDTH_EXTRA_EXPANDED		150
#define FWIDTH_ULTRA_EXPANDED		200
	void OS_2_And_Windows_Metrics::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg);
		ifstream_read_big_endian(fin, &version, sizeof(USHORT));
		ifstream_read_big_endian(fin, &x_avg_char_width, sizeof(SHORT));
		ifstream_read_big_endian(fin, &us_weight_class, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_width_class, sizeof(USHORT));
		ifstream_read_big_endian(fin, &fs_type, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_subscript_x_size, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_subscript_y_size, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_subscript_x_offset, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_subscript_y_offset, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_superscript_x_size, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_superscript_y_size, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_superscript_x_offset, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_superscript_y_offset, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_strike_out_size, sizeof(SHORT));
		ifstream_read_big_endian(fin, &y_strike_out_position, sizeof(SHORT));
		ifstream_read_big_endian(fin, &s_family_class, sizeof(SHORT));
		fin.read((char*)&panose, sizeof(PANOSE));
		ifstream_read_big_endian(fin, &ul_unicode_range_1, sizeof(ULONG));
		ifstream_read_big_endian(fin, &ul_unicode_range_2, sizeof(ULONG));
		ifstream_read_big_endian(fin, &ul_unicode_range_3, sizeof(ULONG));
		ifstream_read_big_endian(fin, &ul_unicode_range_4, sizeof(ULONG));
		ifstream_read_big_endian(fin, ach_vend_id, sizeof(CHAR), 4);
		ifstream_read_big_endian(fin, &fs_selection, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_first_char_index, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_last_char_index, sizeof(USHORT));
		ifstream_read_big_endian(fin, &s_typo_ascender, sizeof(USHORT));
		ifstream_read_big_endian(fin, &s_typo_descender, sizeof(USHORT));
		ifstream_read_big_endian(fin, &s_typo_line_cap, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_win_ascent, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_win_descent, sizeof(USHORT));
		ifstream_read_big_endian(fin, &ul_code_page_range_1, sizeof(ULONG));
		ifstream_read_big_endian(fin, &ul_code_page_range_2, sizeof(ULONG));
		ifstream_read_big_endian(fin, &sx_height, sizeof(SHORT));
		ifstream_read_big_endian(fin, &s_cap_height, sizeof(SHORT));
		ifstream_read_big_endian(fin, &us_default_char, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_break_char, sizeof(USHORT));
		ifstream_read_big_endian(fin, &us_max_coutext, sizeof(USHORT));
	}

	void OS_2_And_Windows_Metrics::dump_info(FILE *fp){
		fprintf(fp, "<OS_2>\n");
		fprintf(fp, "<version value=\"%u\"/>\n", version);
		fprintf(fp, "<xAvgCharWidth value=\"%d\"/>\n", x_avg_char_width);
		fprintf(fp, "<usWeightClass value=\"%u\"/>\n", us_width_class);
		fprintf(fp, "<usWidthClass value=\"%u\"/>\n", us_width_class);
		fprintf(fp, "<fsType value=\"%u\"/>\n", fs_type);
		fprintf(fp, "<ySubscriptXSize value=\"%d\"/>\n", y_subscript_x_size);
		fprintf(fp, "<ySubscriptYSize value=\"%d\"/>\n", y_subscript_y_size);
		fprintf(fp, "<ySubscriptXOffset value=\"%d\"/>\n", y_subscript_x_offset);
		fprintf(fp, "<ySubscriptYOffset value=\"%d\"/>\n", y_subscript_y_offset);
		fprintf(fp, "<ySuperscriptXSize value=\"%d\"/>\n", y_superscript_x_size);
		fprintf(fp, "<ySuperscriptYSize value=\"%d\"/>\n", y_superscript_y_size);
		fprintf(fp, "<ySuperscriptXOffset value=\"%d\"/>\n", y_superscript_x_offset);
		fprintf(fp, "<ySuperscriptYOffset value=\"%d\"/>\n", y_superscript_y_offset);
		fprintf(fp, "<yStrikeoutSize value=\"%d\"/>\n", y_strike_out_size);
		fprintf(fp, "<yStrikeoutPosition value=\"%d\"/>\n", y_strike_out_position);
		fprintf(fp, "<sFamilyClass value=\"%d\"/>\n", s_family_class);
		fprintf(fp, "<panose>\n");
		fprintf(fp, "<bFamilyType value=\"%d\"/>\n", panose.b_family_type);
		fprintf(fp, "<bSerifStyle value=\"%d\"/>\n", panose.b_serif_style);
		fprintf(fp, "<bWeight value=\"%d\"/>\n", panose.b_weight);
		fprintf(fp, "<bProportion value=\"%d\"/>\n", panose.b_proportion);
		fprintf(fp, "<bContrast value=\"%d\"/>\n", panose.b_contrast);
		fprintf(fp, "<bStrokeVariation value=\"%d\"/>\n", panose.b_stroke_variation);
		fprintf(fp, "<bArmStyle value=\"%d\"/>\n", panose.b_arm_style);
		fprintf(fp, "<bLetterForm value=\"%d\"/>\n", panose.b_letter_form);
		fprintf(fp, "<bMidline value=\"%d\"/>\n", panose.b_mid_line);
		fprintf(fp, "<bXHeight value=\"%d\"/>\n", panose.b_x_height);
		fprintf(fp, "</panose>\n");
		fprintf(fp, "<ulUnicodeRange1 value=\"%ul\"/>\n", ul_unicode_range_1);
		fprintf(fp, "<ulUnicodeRange2 value=\"%ul\"/>\n", ul_unicode_range_2);
		fprintf(fp, "<ulUnicodeRange3 value=\"%ul\"/>\n", ul_unicode_range_3);
		fprintf(fp, "<ulUnicodeRange4 value=\"%ul\"/>\n", ul_unicode_range_4);
		fprintf(fp, "<achVendID value=\"%s\"/>\n", ach_vend_id); // FIXME: ach_vend_id is a string formed by four char, but here the end of string is not set.
		fprintf(fp, "<fsSelection value=\"%u\"/>\n", fs_selection);
		fprintf(fp, "<usFirstCharIndex value=\"%u\"/>\n", us_first_char_index);
		fprintf(fp, "<usLastCharIndex value=\"%u\"/>\n", us_last_char_index);
		fprintf(fp, "<sTypoAscender value=\"%d\"/>\n", s_typo_ascender);
		fprintf(fp, "<sTypoDescender value=\"%d\"/>\n", s_typo_descender);
		fprintf(fp, "<sTypoLineGap value=\"%d\"/>\n", s_typo_line_cap);
		fprintf(fp, "<usWinAscent value=\"%d\"/>\n", us_win_ascent);
		fprintf(fp, "<usWinDescent value=\"%d\"/>\n", us_win_descent);
		fprintf(fp, "<ulCodePageRange1 value=\"%ul\"/>\n", ul_code_page_range_1);
		fprintf(fp, "<ulCodePageRange2 value=\"%ul\"/>\n", ul_code_page_range_2);
		fprintf(fp, "<sxHeight value=\"%d\"/>\n", sx_height);
		fprintf(fp, "<sCapHeight value=\"%d\"/>\n", s_cap_height);
		fprintf(fp, "<usDefaultChar value=\"%u\"/>\n", us_default_char);
		fprintf(fp, "<usBreakChar value=\"%u\"/>\n", us_break_char);
		fprintf(fp, "<usMaxContex value=\"%u\"/>\n", us_max_coutext);
		fprintf(fp, "</OS_2>\n");
	}
}