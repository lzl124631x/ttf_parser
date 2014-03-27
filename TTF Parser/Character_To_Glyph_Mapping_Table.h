#ifndef CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#define CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#include "TTF_Table.h"

/******************************* cmap ***********************************/
class Encoding_Table{
public:
	USHORT	format;
	USHORT	length;
	USHORT	version;
	Encoding_Table(ifstream &fin);
	Encoding_Table(Encoding_Table &et): format(et.format), length(et.length), version(et.version){}
	virtual USHORT get_glyph_index(USHORT ch) { return 0; } // FIXME: shouldn't this function be pure virtual?
};

class Encoding_Table_Entry{
public:
	USHORT	platform_id;
	USHORT	platform_specific_encoding_id;
	ULONG	byte_offset;	// from beginning of table to the subtable for this encoding.
	Encoding_Table* encoding_table;
};

class Character_To_Glyph_Index_Mapping_Table{
private:
	USHORT	table_version_number;
	USHORT	number_of_encoding_tables;
	Encoding_Table_Entry *encoding_table_entries;
	Encoding_Table **encoding_tables;
	Encoding_Table *get_encoding_table(USHORT platform_id, USHORT encoding_id);
public:
	void load_table(Table_Directory_Entry *entry, ifstream &fin);
	void load_encoding_tables(ifstream &fin, ULONG base);
	USHORT get_glyph_index(USHORT platform_id, USHORT encoding_id, USHORT ch);
};

#define FORMAT_BYTE_ENCODING_TABLE 0
class Byte_Encoding_Table: public Encoding_Table{
public:
	BYTE	glyph_id_array[256];
	Byte_Encoding_Table(Encoding_Table &et, ifstream &fin);
};

#define FORMAT_HIGH_BYTE_MAPPING_THROUGH_TABLE 2
class High_Byte_Mapping_Through_Table: public Encoding_Table{
public:
	USHORT	subheader_keys[256];
	struct Sub_Header{
		USHORT	first_code;
		USHORT	entry_count;
		SHORT	id_delta;
		USHORT	id_range_offset;
	};
	High_Byte_Mapping_Through_Table(Encoding_Table &et, ifstream &fin);
};

#define FORMAT_SEGMENT_MAPPING_TO_DELTA_VALUES 4
class Segment_Mapping_To_Delta_Values: public Encoding_Table{
private:
	void dump_info();
public:
	USHORT	seg_countx2;
	USHORT	search_range;
	USHORT	entry_selector;
	USHORT	range_shift;
	USHORT	*end_count;/*[seg_count]*/
	USHORT	reserved_pad;
	USHORT	*start_count;/*[seg_count]*/
	USHORT	*id_delta;/*[seg_count]*/
	USHORT	*id_range_offset;/*[seg_count]*/
	USHORT	*glyph_id_array;/*[]*/
	Segment_Mapping_To_Delta_Values(Encoding_Table &et, ifstream &fin);
	USHORT get_glyph_index(USHORT ch);
};

#define FORMAT_TRIMMED_TABLE_MAPPING 6
class Trimmed_Table_Mapping: public Encoding_Table{
public:
	USHORT	first_code;
	USHORT	entry_count;
	USHORT	*glyph_array;/*[entry_count]*/
	Trimmed_Table_Mapping(Encoding_Table &et, ifstream &fin);
};

#endif