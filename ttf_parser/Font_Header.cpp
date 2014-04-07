#include "stdafx.h"
#include "Font_Header.h"

namespace ttf_dll{
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

	void Font_Header::dump_info(){
		cout << "Version:\t\t" << table_version_number << endl;
		cout << "Revision:\t\t" << font_revision << endl;
		cout << "CheckSumAdj:\t\t" << checksum_adjustment << endl;
		cout << "UnitsPerEm:\t\t" << units_per_em << endl;
		cout << "Created:\t\t" << created << endl;
		cout << "Modified:\t\t" << modified << endl;
		cout << "xMin:\t\t" << x_min << endl;
		cout << "yMin:\t\t" << y_min << endl;
		cout << "xMax:\t\t" << x_max << endl;
		cout << "xMax:\t\t" << x_max << endl;
	}
}