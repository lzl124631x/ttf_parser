#ifndef OS_2_AND_WINDOWS_METRICS_H
#define OS_2_AND_WINDOWS_METRICS_H

#include "TTF_Table.h"

/******************************* OS/2 ***********************************/
// version 0: TrueType rev 1.5
// version 1: TrueType rev 1.66
// version 2: OpenType rev 1.2
// version 3: OpenType rev 1.4
// version 4: OpenType rev 1.6 http://www.microsoft.com/typography/otspec/
class OS_2_And_Windows_Metrics{// version 3
public:
	USHORT	version;
	SHORT	x_avg_char_width;
	USHORT	us_weight_class;
	USHORT	us_width_class;
	SHORT	fs_type;
	SHORT	y_subscript_x_size;
	SHORT	y_subscript_y_size;
	SHORT	y_subscript_x_offset;
	SHORT	y_subscript_y_offset;
	SHORT	y_superscript_x_size;
	SHORT	y_superscript_y_size;
	SHORT	y_superscript_x_offset;
	SHORT	y_superscript_y_offset;
	SHORT	y_strike_out_size;
	SHORT	y_strike_out_position;
	SHORT	s_family_class;
	typedef struct _PANOSE{
		BYTE	b_family_type;
		BYTE	b_serif_style;
		BYTE	b_weight;
		BYTE	b_proportion;
		BYTE	b_contrast;
		BYTE	b_stroke_variation;
		BYTE	b_arm_style;
		BYTE	b_letter_form;
		BYTE	b_mid_line;
		BYTE	b_x_height;
	}PANOSE;
	PANOSE	panose;
	ULONG	ul_unicode_range_1;			// Bits 0-31
	ULONG	ul_unicode_range_2;			// Bits 32-63
	ULONG	ul_unicode_range_3;			// Bits 64-95
	ULONG	ul_unicode_range_4;			// Bits 96-127
	CHAR	ach_vend_id[4];
	USHORT	fs_selection;
	USHORT	us_first_char_index;
	USHORT	us_last_char_index;
	USHORT	s_typo_ascender;
	USHORT	s_typo_descender;
	USHORT	s_typo_line_cap;
	USHORT	us_win_ascent;
	USHORT	us_win_descent;
	ULONG	ul_code_page_range_1;		// Bits 0-31
	ULONG	ul_code_page_range_2;		// Bits 32-63
	SHORT	sx_height;
	SHORT	s_cap_height;
	USHORT	us_default_char;
	USHORT	us_break_char;
	USHORT	us_max_coutext;
	void load_table(Table_Directory_Entry *entry, ifstream &fin);
};

#endif