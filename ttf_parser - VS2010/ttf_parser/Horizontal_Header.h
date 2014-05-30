#ifndef HORIZONTAL_HEADER_H
#define HORIZONTAL_HEADER_H
#include "Type.h"
#include "TTF_Table.h"
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
class DLL_API Horizontal_Header {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  
  // 0x00010000 for version 1.0.
  FIXED     table_version_number;
  // Typographic ascent. (Distance from baseline of highest ascender)
  FWORD     ascender;
  // Typographic descent. (Distance from baseline of lowest descender)
  FWORD     descender;
  // Typographic line gap. Negative LineGap values are treated as zero in
  // Windows 3.1, System 6, and System 7.
  FWORD     line_gap;
  // Maximum advance width value in 'hmtx' table.
  UFWORD    advance_width_max;
  // Minimum left sidebearing value in 'hmtx' table.
  FWORD     min_left_side_bearing;
  // Minimum right sidebearing value; calculated as
  // Min(aw - lsb - (xMax - xMin)).
  FWORD     min_right_side_bearing;
  // Max(lsb + (xMax - xMin)).
  FWORD     x_max_extent;
  // Used to calculate the slope of the cursor (rise/run); 1 for vertical.
  SHORT     caret_slope_rise;
  // 0 for vertical.
  SHORT     caret_slope_run;
  // The amount by which a slanted highlight on a glyph needs to be shifted to
  // produce the best appearance. Set to 0 for non-slanted fonts.
  SHORT     caret_offset;
  // 4 reserved SHORTs set to 0.
  // 0 for current format.
  SHORT     metric_data_format;
  // Number of hMetric entries in 'hmtx' table.
  USHORT    number_of_hmetrics;
};

} // namespace ttf_dll

#endif