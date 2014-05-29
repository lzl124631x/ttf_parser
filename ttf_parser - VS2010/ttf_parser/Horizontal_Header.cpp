#include "stdafx.h"
#include "Horizontal_Header.h"

namespace ttf_dll{
  void Horizontal_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    FREAD(fin, &table_version_number);
    FREAD(fin, &ascender);
    FREAD(fin, &descender);
    FREAD(fin, &line_gap);
    FREAD(fin, &advance_width_max);
    FREAD(fin, &min_left_side_bearing);
    FREAD(fin, &min_right_side_bearing);
    FREAD(fin, &x_max_extent);
    FREAD(fin, &caret_slope_rise);
    FREAD(fin, &caret_slope_run);
    FREAD(fin, &caret_offset);
    fin.seekg(4 * sizeof(SHORT), ios::cur);
    FREAD(fin, &metric_data_format);
    FREAD(fin, &number_of_hmetrics);
  }

  void Horizontal_Header::dump_info(FILE *fp, size_t indent){
    IND_FP("<hhea>\n");
    ++indent;
    IND_FP("<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    IND_FP("<ascender value=\"%d\"/>\n", ascender);
    IND_FP("<descender value=\"%d\"/>\n", descender);
    IND_FP("<lineGap value=\"%d\"/>\n", line_gap);
    IND_FP("<advanceWidthMax value=\"%u\"/>\n", advance_width_max);
    IND_FP("<minLeftSideBearing value=\"%d\"/>\n", min_left_side_bearing);
    IND_FP("<minRightSideBearing value=\"%d\"/>\n", min_right_side_bearing);
    IND_FP("<xMaxExtent value=\"%d\"/>\n", x_max_extent);
    IND_FP("<caretSlopeRise value=\"%d\"/>\n", caret_slope_rise);
    IND_FP("<caretSlopeRun value=\"%d\"/>\n", caret_slope_run);
    IND_FP("<caretOffset value=\"%d\"/>\n", caret_offset);
    IND_FP("<reserved0 value=\"0\"/>\n");
    IND_FP("<reserved1 value=\"0\"/>\n");
    IND_FP("<reserved2 value=\"0\"/>\n");
    IND_FP("<reserved3 value=\"0\"/>\n");
    IND_FP("<metricDataFormat value=\"%d\"/>\n", metric_data_format);
    IND_FP("<numberOfHMetrics value=\"%u\"/>\n", number_of_hmetrics);
    --indent;
    IND_FP("</hhea>\n");
  }
}