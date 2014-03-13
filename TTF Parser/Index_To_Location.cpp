#include "stdafx.h"
#include "Index_To_Location.h"

void Index_To_Location::load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_glyphs){
	fin.seekg(entry->offset, ios::beg);
	this->offsets = new ULONG[num_glyphs];
	ifstream_read_big_endian(fin, this->offsets, sizeof(ULONG), num_glyphs);
}