#include "stdafx.h"
#include "Horizontal_Metrics.h"
#include "True_Type_Font.h"

namespace ttf_dll{
	void Horizontal_Metrics::load_table(Table_Directory_Entry *entry, ifstream &fin, True_Type_Font *ttf){
		// FIXME: Is it proper to use TTF as a parameter?
		fin.seekg(entry->offset, ios::beg);
		USHORT number_of_hmetrics = ttf->hhea.number_of_hmetrics;
		hmetrics = new Long_Hor_Metric[number_of_hmetrics];
		ifstream_read_big_endian(fin, hmetrics, sizeof(UFWORD), number_of_hmetrics << 1);
		USHORT num_of_lsb = ttf->maxp.num_glyphs - number_of_hmetrics;
		left_side_bearing = new FWORD[num_of_lsb];// FIXME: consider to combine this line and next line into one function
		ifstream_read_big_endian(fin, left_side_bearing, sizeof(FWORD), num_of_lsb);
	}
}