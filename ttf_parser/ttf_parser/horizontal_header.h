#ifndef HORIZONTAL_HEADER_H
#define HORIZONTAL_HEADER_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                     hhea - Horizontal Header                             */
/* Spec: https://www.microsoft.com/typography/otspec/hhea.htm               */
/****************************************************************************/
namespace ttf_dll {

// This table contains information for horizontal layout. The values in the
// minRightSidebearing, minLeftSideBearing and xMaxExtent should be computed
// using only glyphs that have contours. Glyphs with no contours should be
// ignored for the purposes of these calculations. All reserved areas must be
// set to 0.
class DLL_API HorizontalHeader : public TtfSubtable {
 public:
  explicit HorizontalHeader(const TrueTypeFont &ttf);
  // Overrides
  void Init(const TableRecordEntry *entry, std::ifstream &fin);
  void DumpInfo(XmlLogger &logger) const;
  // Accessors
  UShort num_hmetrics() const { return num_hmetrics_; }

 private:
  // 0x00010000 for version 1.0.
  Fixed     table_version_number_;
  // Typographic ascent. (Distance from baseline of highest ascender)
  FWord     ascender_;
  // Typographic descent. (Distance from baseline of lowest descender)
  FWord     descender_;
  // Typographic line gap. Negative LineGap values are treated as zero in
  // Windows 3.1, System 6, and System 7.
  FWord     line_gap_;
  // Maximum advance width value in 'hmtx' table.
  UFword    advance_width_max_;
  // Minimum left sidebearing value in 'hmtx' table.
  FWord     min_left_side_bearing_;
  // Minimum right sidebearing value; calculated as
  // Min(aw - lsb - (xMax - xMin)).
  FWord     min_right_side_bearing_;
  // Max(lsb + (xMax - xMin)).
  FWord     x_max_extent_;
  // Used to calculate the slope of the cursor (rise/run); 1 for vertical.
  Short     caret_slope_rise_;
  // 0 for vertical.
  Short     caret_slope_run_;
  // The amount by which a slanted highlight on a glyph needs to be shifted to
  // produce the best appearance. Set to 0 for non-slanted fonts.
  Short     caret_offset_;
  // 4 reserved SHORTs set to 0.
  // 0 for current format.
  Short     metric_data_format_;
  // Number of hMetric entries in 'hmtx' table.
  UShort    num_hmetrics_;
};

} // namespace ttf_dll

#endif