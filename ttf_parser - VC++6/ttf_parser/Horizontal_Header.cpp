#include "stdafx.h"
#include "Horizontal_Header.h"
using namespace std;
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
    IND_FP(fp, indent, "<hhea>\n");
    ++indent;
    IND_FP(fp, indent, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    IND_FP(fp, indent, "<ascender value=\"%d\"/>\n", ascender);
    IND_FP(fp, indent, "<descender value=\"%d\"/>\n", descender);
    IND_FP(fp, indent, "<lineGap value=\"%d\"/>\n", line_gap);
    IND_FP(fp, indent, "<advanceWidthMax value=\"%u\"/>\n", advance_width_max);
    IND_FP(fp, indent, "<minLeftSideBearing value=\"%d\"/>\n", min_left_side_bearing);
    IND_FP(fp, indent, "<minRightSideBearing value=\"%d\"/>\n", min_right_side_bearing);
    IND_FP(fp, indent, "<xMaxExtent value=\"%d\"/>\n", x_max_extent);
    IND_FP(fp, indent, "<caretSlopeRise value=\"%d\"/>\n", caret_slope_rise);
    IND_FP(fp, indent, "<caretSlopeRun value=\"%d\"/>\n", caret_slope_run);
    IND_FP(fp, indent, "<caretOffset value=\"%d\"/>\n", caret_offset);
    IND_FP(fp, indent, "<reserved0 value=\"0\"/>\n");
    IND_FP(fp, indent, "<reserved1 value=\"0\"/>\n");
    IND_FP(fp, indent, "<reserved2 value=\"0\"/>\n");
    IND_FP(fp, indent, "<reserved3 value=\"0\"/>\n");
    IND_FP(fp, indent, "<metricDataFormat value=\"%d\"/>\n", metric_data_format);
    IND_FP(fp, indent, "<numberOfHMetrics value=\"%u\"/>\n", number_of_hmetrics);
    --indent;
    IND_FP(fp, indent, "</hhea>\n");
  }
}