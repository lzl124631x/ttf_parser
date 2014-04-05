#ifndef HORIZONTAL_HEADER_H
#define HORIZONTAL_HEADER_H
#include "TTF_Table.h"
/******************************* hhea ***********************************/
class Horizontal_Header{
public:
	FIXED	table_version_number;
	FWORD	ascender;
	FWORD	descender;
	FWORD	line_gap;
	UFWORD	advance_width_max;
	FWORD	min_left_side_bearing;
	FWORD	min_right_side_bearing;
	FWORD	x_max_extent;
	SHORT	caret_slope_rise;
	SHORT	caret_slope_run;
	// 5 reserved SHORTs
	SHORT	metric_data_format;
	USHORT	number_of_hmetrics;
	void load_table(Table_Directory_Entry *entry, ifstream &fin);
};

#endif