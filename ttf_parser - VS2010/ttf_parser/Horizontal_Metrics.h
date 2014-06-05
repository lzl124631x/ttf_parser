#ifndef HORIZONTAL_METRICS_H
#define HORIZONTAL_METRICS_H

#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                    hmtx - Horizontal Metrics                             */
/* Spec: https://www.microsoft.com/typography/otspec/hmtx.htm               */
/****************************************************************************/
namespace ttf_dll {

// The fields of longHorMetric are in font design units.
struct DLL_API LongHorMetric {
  void LoadMetric(ifstream &fin);

  UShort    advance_width_;
  Short     left_side_bearing_;
};

class DLL_API HorizontalMetrics {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(TableRecordEntry *entry, ifstream &fin);
  // Deallocates the memory allocated in `LoadTable`, if any.
  void Destroy() {
    DEL_A(hmetrics);
    DEL_A(left_side_bearings_);
  }
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  // Gets the lsb of the glyph labeled by `glyph_index`.
  FWord GetLeftSideBearing(const GlyphId glyph_index) const;
  // Gets the advance width of the glyph labeled by `glyph_index`.
  UFword GetAdvanceWidth(const GlyphId glyph_index) const;

  // Paired advance width and left side bearing values for each glyph. The
  // value numOfHMetrics comes from the 'hhea' table. If the font is
  // monospaced, only one entry need be in the array, but that entry is
  // required. The last entry applies to all subsequent glyphs.
  LongHorMetric   *hmetrics/*[numOfHMetrics]*/;
  // Here the advanceWidth is assumed to be the same as the advanceWidth for
  // the last entry above. The number of entries in this array is derived from
  // numGlyphs (from 'maxp' table) minus numberOfHMetrics. This generally is
  // used with a run of monospaced glyphs (e.g., Kanji fonts or Courier fonts).
  // Only one run is allowed and it must be at the end. This allows a
  // monospaced font to vary the left side bearing values for each glyph.
  FWord             *left_side_bearings_/*[numGlyphs - numOHMetrics]*/;
  
  // A copy of the `num_hmtx` field of `hhea` table.
  UShort            num_hmtx_;
  // A copy of the `num_glyphs` field of `maxp` table.
  UShort            num_glyphs_;
};

} // namespace ttf_dll
#endif