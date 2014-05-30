#include "stdafx.h"
#include "Horizontal_Header.h"

namespace ttf_dll {

void Horizontal_Header::load_table(Table_Record_Entry *entry, ifstream &fin) {
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

void Horizontal_Header::dump_info(Xml_Logger &logger) const {
  logger.println("<hhea>");
  logger.inc_indent();
  logger.println("<tableVersion value=\"0x%08x\"/>", table_version_number);
  logger.println("<ascender value=\"%d\"/>", ascender);
  logger.println("<descender value=\"%d\"/>", descender);
  logger.println("<lineGap value=\"%d\"/>", line_gap);
  logger.println("<advanceWidthMax value=\"%u\"/>", advance_width_max);
  logger.println("<minLeftSideBearing value=\"%d\"/>", min_left_side_bearing);
  logger.println("<minRightSideBearing value=\"%d\"/>", min_right_side_bearing);
  logger.println("<xMaxExtent value=\"%d\"/>", x_max_extent);
  logger.println("<caretSlopeRise value=\"%d\"/>", caret_slope_rise);
  logger.println("<caretSlopeRun value=\"%d\"/>", caret_slope_run);
  logger.println("<caretOffset value=\"%d\"/>", caret_offset);
  logger.println("<reserved0 value=\"0\"/>");
  logger.println("<reserved1 value=\"0\"/>");
  logger.println("<reserved2 value=\"0\"/>");
  logger.println("<reserved3 value=\"0\"/>");
  logger.println("<metricDataFormat value=\"%d\"/>", metric_data_format);
  logger.println("<numberOfHMetrics value=\"%u\"/>", number_of_hmetrics);
  logger.dec_indent();
  logger.println("</hhea>");
}

} // namespace ttf_dll