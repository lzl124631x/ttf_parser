#ifndef HORIZONTAL_METRICS_H
#define HORIZONTAL_METRICS_H

#include "Type.h"
#include "TTF_Table.h"
/************************************************************************/
/*                    hmtx - Horizontal Metrics                         */
/* Spec: https://www.microsoft.com/typography/otspec/hmtx.htm           */
/************************************************************************/
namespace ttf_dll{
  struct DLL_API Long_Hor_Metric{
    UFWORD    advance_width;
    FWORD     lsb;
    void load_metric(std::ifstream &fin);
  };

  class DLL_API Horizontal_Metrics{
  public:
    Long_Hor_Metric   *hmetrics;            // [number_of_hmetrics(from hhea)]
    // Paired advance width and left side bearing values for each glyph.
    // The value numOfHMetrics comes from the 'hhea' table.
    // If the font is monospaced, only one entry need be in the array, but that entry is required.
    // The last entry applies to all subsequent glyphs.
    FWORD             *left_side_bearing;   // [num_glyphs(from maxp) - number_of_hmetrics]
    // Here the advanceWidth is assumed to be the same as the advanceWidth for the last entry above.
    // The number of entries in this array is derived from numGlyphs (from 'maxp' table) minus numberOfHMetrics.
    // This generally is used with a run of monospaced glyphs (e.g., Kanji fonts or Courier fonts).
    // Only one run is allowed and it must be at the end.
    // This allows a monospaced font to vary the left side bearing values for each glyph.
    USHORT            num_hmtx;
    USHORT            num_glyphs;
    void load_table(Table_Directory_Entry *entry, std::ifstream &fin);
    ~Horizontal_Metrics(){
      DEL_A(hmetrics);
      DEL_A(left_side_bearing);
    }
    FWORD get_lsb(GLYPH_ID glyph_index);
    UFWORD get_aw(GLYPH_ID glyph_index);
    void dump_info(FILE *fp, size_t indent);
  };
}
#endif