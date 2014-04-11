#ifndef FONT_HEADER_H
#define FONT_HEADER_H

#include "Type.h"
#include "TTF_Table.h"
/******************************* head ***********************************/
namespace ttf_dll{
	class DLL_API Font_Header{
	public:
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
		SHORT	index_to_loc_format; // This field specifies the version of the 'loca' table. 0 for short offsets, 1 for long.
		SHORT	glygh_data_format;
		void load_table(Table_Directory_Entry *entry, ifstream &fin);
		void dump_info(FILE *fp);
	};
}

#endif