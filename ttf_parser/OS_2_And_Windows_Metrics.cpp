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

	void OS_2_And_Windows_Metrics::dump_info(FILE *fp, size_t indent){
		INDENT(fp, indent); fprintf(fp, "<OS_2>\n");
		++indent;
		INDENT(fp, indent); fprintf(fp, "<version value=\"%u\"/>\n", version);
		INDENT(fp, indent); fprintf(fp, "<xAvgCharWidth value=\"%d\"/>\n", x_avg_char_width);
		INDENT(fp, indent); fprintf(fp, "<usWeightClass value=\"%u\"/>\n", us_width_class);
		INDENT(fp, indent); fprintf(fp, "<usWidthClass value=\"%u\"/>\n", us_width_class);
		INDENT(fp, indent); fprintf(fp, "<fsType value=\"%u\"/>\n", fs_type);
		INDENT(fp, indent); fprintf(fp, "<ySubscriptXSize value=\"%d\"/>\n", y_subscript_x_size);
		INDENT(fp, indent); fprintf(fp, "<ySubscriptYSize value=\"%d\"/>\n", y_subscript_y_size);
		INDENT(fp, indent); fprintf(fp, "<ySubscriptXOffset value=\"%d\"/>\n", y_subscript_x_offset);
		INDENT(fp, indent); fprintf(fp, "<ySubscriptYOffset value=\"%d\"/>\n", y_subscript_y_offset);
		INDENT(fp, indent); fprintf(fp, "<ySuperscriptXSize value=\"%d\"/>\n", y_superscript_x_size);
		INDENT(fp, indent); fprintf(fp, "<ySuperscriptYSize value=\"%d\"/>\n", y_superscript_y_size);
		INDENT(fp, indent); fprintf(fp, "<ySuperscriptXOffset value=\"%d\"/>\n", y_superscript_x_offset);
		INDENT(fp, indent); fprintf(fp, "<ySuperscriptYOffset value=\"%d\"/>\n", y_superscript_y_offset);
		INDENT(fp, indent); fprintf(fp, "<yStrikeoutSize value=\"%d\"/>\n", y_strike_out_size);
		INDENT(fp, indent); fprintf(fp, "<yStrikeoutPosition value=\"%d\"/>\n", y_strike_out_position);
		INDENT(fp, indent); fprintf(fp, "<sFamilyClass value=\"%d\"/>\n", s_family_class);
		INDENT(fp, indent); fprintf(fp, "<panose>\n");
		++indent;
		INDENT(fp, indent); fprintf(fp, "<bFamilyType value=\"%d\"/>\n", panose.b_family_type);
		INDENT(fp, indent); fprintf(fp, "<bSerifStyle value=\"%d\"/>\n", panose.b_serif_style);
		INDENT(fp, indent); fprintf(fp, "<bWeight value=\"%d\"/>\n", panose.b_weight);
		INDENT(fp, indent); fprintf(fp, "<bProportion value=\"%d\"/>\n", panose.b_proportion);
		INDENT(fp, indent); fprintf(fp, "<bContrast value=\"%d\"/>\n", panose.b_contrast);
		INDENT(fp, indent); fprintf(fp, "<bStrokeVariation value=\"%d\"/>\n", panose.b_stroke_variation);
		INDENT(fp, indent); fprintf(fp, "<bArmStyle value=\"%d\"/>\n", panose.b_arm_style);
		INDENT(fp, indent); fprintf(fp, "<bLetterForm value=\"%d\"/>\n", panose.b_letter_form);
		INDENT(fp, indent); fprintf(fp, "<bMidline value=\"%d\"/>\n", panose.b_mid_line);
		INDENT(fp, indent); fprintf(fp, "<bXHeight value=\"%d\"/>\n", panose.b_x_height);
		INDENT(fp, indent); fprintf(fp, "</panose>\n");
		--indent;
		INDENT(fp, indent); fprintf(fp, "<ulUnicodeRange1 value=\"%lu\"/>\n", ul_unicode_range_1);
		INDENT(fp, indent); fprintf(fp, "<ulUnicodeRange2 value=\"%lu\"/>\n", ul_unicode_range_2);
		INDENT(fp, indent); fprintf(fp, "<ulUnicodeRange3 value=\"%lu\"/>\n", ul_unicode_range_3);
		INDENT(fp, indent); fprintf(fp, "<ulUnicodeRange4 value=\"%lu\"/>\n", ul_unicode_range_4);
		INDENT(fp, indent); fprintf(fp, "<achVendID value=\"%s\"/>\n", ach_vend_id); // FIXME: ach_vend_id is a string formed by four char, but here the end of string is not set.
		INDENT(fp, indent); fprintf(fp, "<fsSelection value=\"%u\"/>\n", fs_selection);
		INDENT(fp, indent); fprintf(fp, "<usFirstCharIndex value=\"%u\"/>\n", us_first_char_index);
		INDENT(fp, indent); fprintf(fp, "<usLastCharIndex value=\"%u\"/>\n", us_last_char_index);
		INDENT(fp, indent); fprintf(fp, "<sTypoAscender value=\"%d\"/>\n", s_typo_ascender);
		INDENT(fp, indent); fprintf(fp, "<sTypoDescender value=\"%d\"/>\n", s_typo_descender);
		INDENT(fp, indent); fprintf(fp, "<sTypoLineGap value=\"%d\"/>\n", s_typo_line_cap);
		INDENT(fp, indent); fprintf(fp, "<usWinAscent value=\"%d\"/>\n", us_win_ascent);
		INDENT(fp, indent); fprintf(fp, "<usWinDescent value=\"%d\"/>\n", us_win_descent);
		INDENT(fp, indent); fprintf(fp, "<ulCodePageRange1 value=\"%lu\"/>\n", ul_code_page_range_1);
		INDENT(fp, indent); fprintf(fp, "<ulCodePageRange2 value=\"%lu\"/>\n", ul_code_page_range_2);
		INDENT(fp, indent); fprintf(fp, "<sxHeight value=\"%d\"/>\n", sx_height);
		INDENT(fp, indent); fprintf(fp, "<sCapHeight value=\"%d\"/>\n", s_cap_height);
		INDENT(fp, indent); fprintf(fp, "<usDefaultChar value=\"%u\"/>\n", us_default_char);
		INDENT(fp, indent); fprintf(fp, "<usBreakChar value=\"%u\"/>\n", us_break_char);
		INDENT(fp, indent); fprintf(fp, "<usMaxContex value=\"%u\"/>\n", us_max_coutext);
		--indent;
		INDENT(fp, indent); fprintf(fp, "</OS_2>\n");
	}
}