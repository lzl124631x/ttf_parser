#ifndef FONT_HEADER_H
#define FONT_HEADER_H

#include "TTF_Table.h"
/******************************* head ***********************************/
class Font_Header{
private:
	FIXED	table_version_number;
	FIXED	font_revision;
	ULONG	checksum_adjustment;
	ULONG	magic_number;
	USHORT	flags;
	USHORT	units_per_em;
	longDateTime	created;
	longDateTime	modified;
	FWORD	x_min;
	FWORD	y_min;
	FWORD	x_max;
	FWORD	y_max;
	USHORT	mac_style;
	USHORT	lowest_rec_ppem;
	SHORT	font_direction_hint;
	SHORT	index_to_loc_format;
	SHORT	glygh_data_format;
public:
	void load_table(Table_Directory_Entry *entry, ifstream &fin);
};

#endif