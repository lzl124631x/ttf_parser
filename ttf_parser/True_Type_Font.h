#ifndef TTF_PARSER_H
#define TTF_PARSER_H

#include "TTF_Table.h"
#include "Maximum_Profile.h"
#include "Index_To_Location.h"
#include "Glyph_Data.h"
#include "Font_Header.h"
#include "Character_To_Glyph_Mapping_Table.h"
#include "Horizontal_Header.h"
#include "Horizontal_Metrics.h"
#include "Naming_Table.h"
#include "OS_2_And_Windows_Metrics.h"

class True_Type_Font{
private:
	void load_glyph_data_array(ifstream &fin);
public:
	BYTE *file_binary;
	Offset_Table *offset_table;
	Character_To_Glyph_Index_Mapping_Table cmap;
	Font_Header head;
	Maximum_Profile maxp;
	Index_To_Location loca;
	Horizontal_Header hhea;
	Horizontal_Metrics hmtx;
	Naming_Table name;
	OS_2_And_Windows_Metrics os_2;
	vector<Glyph_Data*> glyph_data_array;
	bool load_path(string path);
	bool valid_checksum(char* tag);
	void get_glyph_outline(USHORT ch);
	inline ULONG* get_table(Table_Directory_Entry *entry){
		return (ULONG*)&this->file_binary[entry->offset];
	}
};

#endif
