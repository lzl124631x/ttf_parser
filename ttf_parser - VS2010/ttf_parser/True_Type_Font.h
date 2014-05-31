#ifndef TTF_PARSER_H
#define TTF_PARSER_H

#include <fstream>
using namespace std;
#include "ttf_table.h"
#include "maximum_profile.h"
#include "index_to_location.h"
#include "glyph_data.h"
#include "font_header.h"
#include "character_to_glyph_mapping_table.h"
#include "horizontal_header.h"
#include "horizontal_metrics.h"
#include "naming_table.h"
#include "os_2_and_windows_metrics.h"
#include "post_script.h"

namespace ttf_dll {

class DLL_API TrueTypeFont {
 public:
  ~TrueTypeFont();
  bool LoadPath(const TCHAR *path);
  bool ValidChecksum(const char *tag);
  void GetGlyphOutline(const UShort ch);
  bool DumpTTF(const char *path) const;
  Glyph *GetGlyph(const GlyphID glyph_index) {
    return glyf_.LoadGlyph(glyph_index);
  }
  ULong GlyphIndexToOffset(const GlyphID glyph_index) const {
    return loca_.GlyphIndexToOffset(glyph_index);
  }
  void GlyphInfo(Glyph *glyph, TCHAR *buf, size_t buf_len);

  OffsetTable offset_table_;
  CharacterToGlyphIndexMappingTable cmap_;
  FontHeader head_;
  MaximumProfile maxp_;
  IndexToLocation loca_;
  HorizontalHeader hhea_;
  HorizontalMetrics hmtx_;
  Naming_Table name_;
  OS2AndWindowsMetrics os_2_;
  GlyphData glyf_;
};

// A global pointer to the ttf. Subtables of ttf use this pointer to access
// other subtables.
extern TrueTypeFont *g_ttf;
} // namespace ttf_dll

#endif