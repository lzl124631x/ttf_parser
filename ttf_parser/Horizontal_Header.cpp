#include "stdafx.h"
#include "Horizontal_Header.h"

namespace ttf_dll{
	void Horizontal_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg);
		ifstream_read_big_endian(fin, &table_version_number, sizeof(FIXED));
		ifstream_read_big_endian(fin, &ascender, sizeof(FWORD));
		ifstream_read_big_endian(fin, &descender, sizeof(FWORD));
		ifstream_read_big_endian(fin, &line_gap, sizeof(FWORD));
		ifstream_read_big_endian(fin, &advance_width_max, sizeof(UFWORD));
		ifstream_read_big_endian(fin, &min_left_side_bearing, sizeof(FWORD));
		ifstream_read_big_endian(fin, &min_right_side_bearing, sizeof(FWORD));
		ifstream_read_big_endian(fin, &x_max_extent, sizeof(FWORD));
		ifstream_read_big_endian(fin, &caret_slope_rise, sizeof(SHORT));
		ifstream_read_big_endian(fin, &caret_slope_run, sizeof(SHORT));
		ifstream_read_big_endian(fin, &caret_offset, sizeof(SHORT));
		fin.seekg(4 * sizeof(SHORT), ios::cur);
		ifstream_read_big_endian(fin, &metric_data_format, sizeof(SHORT));
		ifstream_read_big_endian(fin, &number_of_hmetrics, sizeof(USHORT));
	}

	void Horizontal_Header::dump_info(FILE *fp){
		fprintf(fp, "<hhea>\n");
		fprintf(fp, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
		fprintf(fp, "<ascender value=\"%d\"/>\n", ascender);
		fprintf(fp, "<descender value=\"%d\"/>\n", descender);
		fprintf(fp, "<lineGap value=\"%d\"/>\n", line_gap);
		fprintf(fp, "<advanceWidthMax value=\"%u\"/>\n", advance_width_max);
		fprintf(fp, "<minLeftSideBearing value=\"%d\"/>\n", min_left_side_bearing);
		fprintf(fp, "<minRightSideBearing value=\"%d\"/>\n", min_right_side_bearing);
		fprintf(fp, "<xMaxExtent value=\"%d\"/>\n", x_max_extent);
		fprintf(fp, "<caretSlopeRise value=\"%d\"/>\n", caret_slope_rise);
		fprintf(fp, "<caretSlopeRun value=\"%d\"/>\n", caret_slope_run);
		fprintf(fp, "<caretOffset value=\"%d\"/>\n", caret_offset);
		fprintf(fp, "<reserved0 value=\"0\"/>\n");
		fprintf(fp, "<reserved1 value=\"0\"/>\n");
		fprintf(fp, "<reserved2 value=\"0\"/>\n");
		fprintf(fp, "<reserved3 value=\"0\"/>\n");
		fprintf(fp, "<metricDataFormat value=\"%d\"/>\n", metric_data_format);
		fprintf(fp, "<numberOfHMetrics value=\"%u\"/>\n", number_of_hmetrics);
		fprintf(fp, "</hhea>\n");
	}
}