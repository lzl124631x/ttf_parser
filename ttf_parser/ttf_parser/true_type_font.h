#ifndef TTF_PARSER_H
#define TTF_PARSER_H

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
  TrueTypeFont();
  // Loads the TrueType font from the file specified by `path`. Memory
  // allocation will occur in this function. Hence, you must invoke
  // `TrueTypeFont::Close` to deallocate the memory.
  void Open(const TCHAR *path);
  // Deallocates memory allocated in `TrueTypeFont::Open`.
  void Close();
  // Dumps this TrueType font to an XML file specified by `path`.
  bool DumpTtf(const TCHAR *path) const;
  // Gets the pointer to glyph labeled by `glyph_index`.
  const Glyph &GetGlyph(const GlyphId glyph_index) {
    return glyf_.LoadGlyph(glyph_index);
  }
  // Outputs some data of the `glyph` to a sized buffer. The buffer is
  // pointed by `buf` and its size is given by `buf_len`.
  void GlyphInfo(const Glyph &glyph, TCHAR *buf, size_t buf_len) const;
  // Accessors
  const CharacterToGlyphIndexMappingTable &cmap() const { return cmap_; }
  const FontHeader &head() const { return head_; }
  const MaximumProfile &maxp() const { return maxp_; }
  const IndexToLocation &loca() const { return loca_; }
  const HorizontalHeader &hhea() const { return hhea_; }
  const HorizontalMetrics &hmtx() const { return hmtx_; }
  const Naming_Table &name() const { return name_; }
  const Os2AndWindowsMetrics &os_2() const { return os_2_; }
  const GlyphData &glyf() const { return glyf_; }
  bool is_open() const { return is_open_; }

 private:
  // Returns true if the procedure of checksumming succeeds.
  bool Checksum(std::ifstream &fin) const;
  // Returns true if the checksumming of the subtable labeled by `tag`
  // succeeds.
  bool TableChecksum(const char *data, const char *tag) const;

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

  bool is_open_;
};

} // namespace ttf_dll

#endif