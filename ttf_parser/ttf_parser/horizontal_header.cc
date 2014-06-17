#include "stdafx.h"
#include "horizontal_header.h"
using namespace std;

namespace ttf_dll {

HorizontalHeader::HorizontalHeader(const TrueTypeFont &ttf)
    : TtfSubtable(ttf),
      table_version_number_(0),
      ascender_(0),
      descender_(0),
      line_gap_(0),
      advance_width_max_(0),
      min_left_side_bearing_(0),
      min_right_side_bearing_(0),
      x_max_extent_(0),
      caret_slope_rise_(0),
      caret_slope_run_(0),
      caret_offset_(0),
      metric_data_format_(0),
      num_hmetrics_(0) {}

void HorizontalHeader::Init(const TableRecordEntry *entry, ifstream &fin) {
  fin.seekg(entry->offset(), ios::beg);
  FREAD(fin, &table_version_number_);
  FREAD(fin, &ascender_);
  FREAD(fin, &descender_);
  FREAD(fin, &line_gap_);
  FREAD(fin, &advance_width_max_);
  FREAD(fin, &min_left_side_bearing_);
  FREAD(fin, &min_right_side_bearing_);
  FREAD(fin, &x_max_extent_);
  FREAD(fin, &caret_slope_rise_);
  FREAD(fin, &caret_slope_run_);
  FREAD(fin, &caret_offset_);
  fin.seekg(4 * sizeof(Short), ios::cur);
  FREAD(fin, &metric_data_format_);
  FREAD(fin, &num_hmetrics_);
}

void HorizontalHeader::DumpInfo(XmlLogger &logger) const {
  logger.Println("<hhea>");
  logger.IncreaseIndent();
  logger.Println("<tableVersion value=\"0x%08x\"/>", table_version_number_);
  logger.Println("<ascender value=\"%d\"/>", ascender_);
  logger.Println("<descender value=\"%d\"/>", descender_);
  logger.Println("<lineGap value=\"%d\"/>", line_gap_);
  logger.Println("<advanceWidthMax value=\"%u\"/>", advance_width_max_);
  logger.Println("<minLeftSideBearing value=\"%d\"/>", min_left_side_bearing_);
  logger.Println("<minRightSideBearing value=\"%d\"/>", min_right_side_bearing_);
  logger.Println("<xMaxExtent value=\"%d\"/>", x_max_extent_);
  logger.Println("<caretSlopeRise value=\"%d\"/>", caret_slope_rise_);
  logger.Println("<caretSlopeRun value=\"%d\"/>", caret_slope_run_);
  logger.Println("<caretOffset value=\"%d\"/>", caret_offset_);
  logger.Println("<reserved0 value=\"0\"/>");
  logger.Println("<reserved1 value=\"0\"/>");
  logger.Println("<reserved2 value=\"0\"/>");
  logger.Println("<reserved3 value=\"0\"/>");
  logger.Println("<metricDataFormat value=\"%d\"/>", metric_data_format_);
  logger.Println("<numberOfHMetrics value=\"%u\"/>", num_hmetrics_);
  logger.DecreaseIndent();
  logger.Println("</hhea>");
}

} // namespace ttf_dll