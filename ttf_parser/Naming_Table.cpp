#include "stdafx.h"
#include "Naming_Table.h"

namespace ttf_dll{
	void Naming_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg);
		ifstream_read_big_endian(fin, &format_selector, sizeof(USHORT));
		ifstream_read_big_endian(fin, &number_of_name_records, sizeof(USHORT));
		ifstream_read_big_endian(fin, &offset, sizeof(USHORT));
		name_records = new Name_Record[number_of_name_records];
		ifstream_read_big_endian(fin, name_records, sizeof(USHORT), 6 * number_of_name_records);
	}
}