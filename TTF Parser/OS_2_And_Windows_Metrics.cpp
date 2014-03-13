#include "stdafx.h"
#include "OS_2_And_Windows_Metrics.h"

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
	ifstream_read_big_endian(fin, &this->version, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->x_avg_char_width, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->us_weight_class, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_width_class, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->fs_type, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_subscript_x_size, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_subscript_y_size, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_subscript_x_offset, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_subscript_y_offset, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_superscript_x_size, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_superscript_y_size, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_superscript_x_offset, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_superscript_y_offset, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_strike_out_size, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->y_strike_out_position, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->s_family_class, sizeof(SHORT));
	fin.read((char*)&this->panose, sizeof(PANOSE));
	ifstream_read_big_endian(fin, &this->ul_unicode_range_1, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->ul_unicode_range_2, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->ul_unicode_range_3, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->ul_unicode_range_4, sizeof(ULONG));
	ifstream_read_big_endian(fin, this->ach_vend_id, sizeof(CHAR), 4);
	ifstream_read_big_endian(fin, &this->fs_selection, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_first_char_index, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_last_char_index, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->s_typo_ascender, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->s_typo_descender, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->s_typo_line_cap, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_win_ascent, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_win_descent, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->ul_code_page_range_1, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->ul_code_page_range_2, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->sx_height, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->s_cap_height, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->us_default_char, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_break_char, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->us_max_coutext, sizeof(USHORT));
}