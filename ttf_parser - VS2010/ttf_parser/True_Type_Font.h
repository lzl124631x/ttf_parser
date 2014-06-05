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
  // Loads the TrueType font from the file specified by `path`. Memory
  // allocation will occur in this function. Hence, you must invoke
  // `TrueTypeFont::Destroy` to deallocate the memory.
  bool LoadPath(const TCHAR *path);
  // Deallocates memory allocated in `TrueTypeFont::LoadPath`.
  void Destroy();
  // bool ValidChecksum(const char *tag);
  // void GetGlyphOutline(const UShort ch);
  // Dumps this TrueType font to an XML file specified by `path`.
  bool DumpTtf(const char *path) const;
  // Gets the pointer to glyph labeled by `glyph_index`.
  Glyph *GetGlyph(const GlyphId glyph_index) {
    return glyf_.LoadGlyph(glyph_index);
  }
  // Outputs some data of the `glyph` to a sized buffer. The buffer is
  // pointed by `buf` and its size is given by `buf_len`.
  void GlyphInfo(const Glyph *glyph, TCHAR *buf, const size_t buf_len) const;

  OffsetTable offset_table_;
  CharacterToGlyphIndexMappingTable cmap_;
  FontHeader head_;
  MaximumProfile maxp_;
  IndexToLocation loca_;
  HorizontalHeader hhea_;
  HorizontalMetrics hmtx_;
  Naming_Table name_;
  Os2AndWindowsMetrics os_2_;
  GlyphData glyf_;
};

// A global pointer to the ttf. Subtables of ttf use this pointer to access
// other subtables.
extern TrueTypeFont *g_ttf;
} // namespace ttf_dll

#endif