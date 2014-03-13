#include "stdafx.h"
#include "Horizontal_Header.h"

void Horizontal_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
	fin.seekg(entry->offset, ios::beg);
	ifstream_read_big_endian(fin, &this->table_version_number, sizeof(FIXED));
	ifstream_read_big_endian(fin, &this->ascender, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->descender, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->line_gap, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->advance_width_max, sizeof(UFWORD));
	ifstream_read_big_endian(fin, &this->min_left_side_bearing, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->min_right_side_bearing, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->x_max_extent, sizeof(FWORD));
	ifstream_read_big_endian(fin, &this->caret_slope_rise, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->caret_slope_run, sizeof(SHORT));
	fin.seekg(5 * sizeof(SHORT), ios::cur);
	ifstream_read_big_endian(fin, &this->metric_data_format, sizeof(SHORT));
	ifstream_read_big_endian(fin, &this->number_of_hmetrics, sizeof(USHORT));
}