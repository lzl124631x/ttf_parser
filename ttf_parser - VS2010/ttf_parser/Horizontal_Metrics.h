#ifndef HORIZONTAL_METRICS_H
#define HORIZONTAL_METRICS_H

#include "Type.h"
#include "TTF_Table.h"
/****************************************************************************/
/*                    hmtx - Horizontal Metrics                             */
/* Spec: https://www.microsoft.com/typography/otspec/hmtx.htm               */
/****************************************************************************/
namespace ttf_dll {

// The fields of longHorMetric are in font design units.
struct DLL_API Long_Hor_Metric {
  void load_metric(ifstream &fin);

  USHORT    advance_width;
  // left side bearing
  SHORT     lsb;
};

class DLL_API Horizontal_Metrics {
 public:
  ~Horizontal_Metrics() {
    DEL_A(hmetrics);
    DEL_A(left_side_bearing);
  }
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  // Gets the lsb of the glyph labeled by `glyph_index`.
  FWORD get_lsb(const GLYPH_ID glyph_index) const;
  // Gets the advance width of the glyph labeled by `glyph_index`.
  UFWORD get_aw(const GLYPH_ID glyph_index) const;

  // Paired advance width and left side bearing values for each glyph. The
  // value numOfHMetrics comes from the 'hhea' table. If the font is
  // monospaced, only one entry need be in the array, but that entry is
  // required. The last entry applies to all subsequent glyphs.
  Long_Hor_Metric   *hmetrics/*[numOfHMetrics]*/;
  // Here the advanceWidth is assumed to be the same as the advanceWidth for
  // the last entry above. The number of entries in this array is derived from
  // numGlyphs (from 'maxp' table) minus numberOfHMetrics. This generally is
  // used with a run of monospaced glyphs (e.g., Kanji fonts or Courier fonts).
  // Only one run is allowed and it must be at the end. This allows a
  // monospaced font to vary the left side bearing values for each glyph.
  FWORD             *left_side_bearing/*[numGlyphs - numOfHMetrics]*/;
  
  // A copy of the `num_hmtx` field of `hhea` table.
  USHORT            num_hmtx;
  // A copy of the `num_glyphs` field of `maxp` table.
  USHORT            num_glyphs;
};

} // namespace ttf_dll
#endif