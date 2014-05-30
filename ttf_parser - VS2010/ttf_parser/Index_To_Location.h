#ifndef INDEX_TO_LOCATION_H
#define INDEX_TO_LOCATION_H
#include "Type.h"
#include "TTF_Table.h"
/****************************************************************************/
/*                     loca - Index To Location                             */
/* Spec: https://www.microsoft.com/typography/otspec/loca.htm               */
/****************************************************************************/
namespace ttf_dll {

// The indexToLoc table stores the offsets to the locations of the glyphs in
// the font, relative to the beginning of the glyphData table.
// NOTE: This table must be loaded after 'maxp' and 'head' are loaded.
class DLL_API Index_To_Location {
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
  ~Index_To_Location() {
    if(loca_format) { // 1 for ULONG
      DEL_T(offsets, ULONG);
    } else {           // 0 for USHORT
      DEL_T(offsets, USHORT);
    }
  }
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  // Gets the offset of the glyph labeled by `glyph_index`.
  ULONG glyph_index_to_offset(const GLYPH_ID glyph_index) const;

  // Short Version: USHORT.   The actual local offset divided by 2 is stored.
  // Long Version: ULONG.     The actual local offset is stored.
  void    *offsets/*[num_glyphs]*/;
  // FIXME: This table should allocate 'num_glyphs + 1' unites!
  
  // A copy of the `num_glyphs` field of `maxp` table.
  USHORT  num_glyphs;
  // A copy of the `loca_format` field of `head` table.
  SHORT   loca_format;
};

} // namespace ttf_dll
#endif