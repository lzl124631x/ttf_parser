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
    INDENT(fp, indent); fprintf(fp, "<hhea>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    INDENT(fp, indent); fprintf(fp, "<ascender value=\"%d\"/>\n", ascender);
    INDENT(fp, indent); fprintf(fp, "<descender value=\"%d\"/>\n", descender);
    INDENT(fp, indent); fprintf(fp, "<lineGap value=\"%d\"/>\n", line_gap);
    INDENT(fp, indent); fprintf(fp, "<advanceWidthMax value=\"%u\"/>\n", advance_width_max);
    INDENT(fp, indent); fprintf(fp, "<minLeftSideBearing value=\"%d\"/>\n", min_left_side_bearing);
    INDENT(fp, indent); fprintf(fp, "<minRightSideBearing value=\"%d\"/>\n", min_right_side_bearing);
    INDENT(fp, indent); fprintf(fp, "<xMaxExtent value=\"%d\"/>\n", x_max_extent);
    INDENT(fp, indent); fprintf(fp, "<caretSlopeRise value=\"%d\"/>\n", caret_slope_rise);
    INDENT(fp, indent); fprintf(fp, "<caretSlopeRun value=\"%d\"/>\n", caret_slope_run);
    INDENT(fp, indent); fprintf(fp, "<caretOffset value=\"%d\"/>\n", caret_offset);
    INDENT(fp, indent); fprintf(fp, "<reserved0 value=\"0\"/>\n");
    INDENT(fp, indent); fprintf(fp, "<reserved1 value=\"0\"/>\n");
    INDENT(fp, indent); fprintf(fp, "<reserved2 value=\"0\"/>\n");
    INDENT(fp, indent); fprintf(fp, "<reserved3 value=\"0\"/>\n");
    INDENT(fp, indent); fprintf(fp, "<metricDataFormat value=\"%d\"/>\n", metric_data_format);
    INDENT(fp, indent); fprintf(fp, "<numberOfHMetrics value=\"%u\"/>\n", number_of_hmetrics);
    --indent;
    INDENT(fp, indent); fprintf(fp, "</hhea>\n");
  }
}