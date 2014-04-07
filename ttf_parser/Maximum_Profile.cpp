#include "stdafx.h"
#include "Maximum_Profile.h"

namespace ttf_dll{
	void Maximum_Profile::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg);
		ifstream_read_big_endian(fin, &this->table_version_number, sizeof(FIXED));
		ifstream_read_big_endian(fin, &this->num_glyphs, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_points, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_contours, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_composite_points, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_composite_contours, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_zones, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_twilight_points, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_storage, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_function_defs, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_instruction_defs, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_stack_elements, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_size_of_instructions, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_component_elements, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->max_component_depth, sizeof(USHORT));
	}
}