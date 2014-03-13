#include "stdafx.h"
#include "Font_Header.h"

void Font_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
	fin.seekg(entry->offset, ios::beg);
	ifstream_read_big_endian(fin, &this->table_version_number, sizeof(FIXED));
	ifstream_read_big_endian(fin, &this->font_revision, sizeof(FIXED));
	ifstream_read_big_endian(fin, &this->checksum_adjustment, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->magic_number, sizeof(ULONG));
	ifstream_read_big_endian(fin, &this->flags, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->units_per_em, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->created, sizeof(longDateTime));
	ifstream_read_big_endian(fin, &this->modified, sizeof(longDateTime));
	ifstream_read_big_endian(fin, &this->x_min, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->y_min, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->x_max, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->y_max, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->mac_style, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->lowest_rec_ppem, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->font_direction_hint, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->index_to_loc_format, sizeof(USHORT));
	ifstream_read_big_endian(fin, &this->glygh_data_format, sizeof(USHORT));
}