#ifndef INDEX_TO_LOCATION_H
#define INDEX_TO_LOCATION_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* loca ***********************************/
// https://www.microsoft.com/typography/otspec/loca.htm
// This table must be loaded after 'maxp' and 'head' are loaded.
namespace ttf_dll{
  class DLL_API Index_To_Location{
    // The indexToLoc table stores the offsets to the locations of the glyphs in the font,
    // relative to the beginning of the glyphData table.
    // In order to compute the length of the last glyph element, there is an extra entry after the last valid index.
    // By definition, index zero points to the "missing character," which is the character that appears if a character is not found in the font.
    // The missing character is commonly represented by a blank box or a space.
    // If the font does not contain an outline for the missing character, then the first and second offsets should have the same value.
    // This also applies to any other characters without an outline, such as the space character.
    // If a glyph has no outline, then loca[n] = loca [n+1].
    // In the particular case of the last glyph(s), loca[n] will be equal the length of the glyph data ('glyf') table.
    // The offsets must be in ascending order with loca[n] <= loca[n+1].
  public:
    USHORT  num_glyphs;
    SHORT   loca_format;
    void    *offsets; /* [num_glyphs] */ // FIXME: This table should allocate 'num_glyphs + 1' unites!
    // Short Version: USHORT.   The actual local offset divided by 2 is stored.
    // Long Version: ULONG.     The actual local offset is stored.
    void load_table(Table_Directory_Entry *entry, ifstream &fin);
    ~Index_To_Location(){
      if(loca_format){  // 1 for ULONG
        DEL_T(offsets, ULONG);
      }else{            // 0 for USHORT
        DEL_T(offsets, USHORT);
      }
    }
    void dump_info(FILE *fp, size_t indent);
  };
}
#endif