#ifndef INDEX_TO_LOCATION_H
#define INDEX_TO_LOCATION_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                     loca - Index To Location                             */
/* Spec: https://www.microsoft.com/typography/otspec/loca.htm               */
/****************************************************************************/
namespace ttf_dll {

// The indexToLoc table stores the offsets to the locations of the glyphs in
// the font, relative to the beginning of the glyphData table.
// NOTE: This table must be loaded after 'maxp' and 'head' are loaded.
class DLL_API IndexToLocation : public TtfSubtable {
  // In order to compute the length of the last glyph element, there is an
  // extra entry after the last valid index. By definition, index zero points
  // to the "missing character," which is the character that appears if a
  // character is not found in the font. The missing character is commonly
  // represented by a blank box or a space. If the font does not contain an
  // outline for the missing character, then the first and second offsets
  // should have the same value. This also applies to any other characters
  // without an outline, such as the space character.

  // If a glyph has no outline, then loca[n] = loca [n+1].
  // In the particular case of the last glyph(s), loca[n] will be equal the
  // length of the glyph data ('glyf') table.
  // The offsets must be in ascending order with loca[n] <= loca[n+1].
 public:
  explicit IndexToLocation(const TrueTypeFont &ttf);
  // Overrides
  void Init(const TableRecordEntry *entry, std::ifstream &fin);
  void Destroy() {
    if (loca_format_) { // 1 for ULONG
      DEL_T(offsets_, ULong);
    } else {           // 0 for USHORT
      DEL_T(offsets_, UShort);
    }
  }
  void DumpInfo(XmlLogger &logger) const;
  // Gets the offset and length of the glyph labeled by `glyph_index`.
  // `offset` and `length` are output parameters.
  void GetGlyphOffsetAndLength(GlyphId glyph_index,
                               ULong *offset, ULong *length) const;

 private:
  // Short Version: USHORT.   The actual local offset divided by 2 is stored.
  // Long Version: ULONG.     The actual local offset is stored.
  void    *offsets_/*[numGlyphs + 1]*/;
  
  // A copy of the `num_glyphs` field of `maxp` table.
  UShort  num_glyphs_;
  // A copy of the `loca_format` field of `head` table.
  Short   loca_format_;
};

} // namespace ttf_dll
#endif