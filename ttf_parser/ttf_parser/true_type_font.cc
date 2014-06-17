#include "stdafx.h"
#include "true_type_font.h"
using namespace std; // For `ifstream`
#include "xml_logger.h"

// Disable warning C4355: 'this' : used in base member initializer list.
#pragma warning(disable: 4355)

namespace ttf_dll {
// FIXME : Calculate the checksums!
//ULONG calc_table_checksum(ULONG *table, ULONG num_of_bytes){
//  ULONG sum = 0L;
//  ULONG *end = table + ((num_of_bytes + 3) & ~3) / sizeof(ULONG);
//  while(table < end){
//    sum += *table++;
//  }
//  return sum;
//}

//bool True_Type_Font::valid_checksum(char* tag){
//  Table_Directory_Entry *entry = offset_table.get_table_entry(tag);
//  return entry->checksum == calc_table_checksum(get_table(entry), entry->length);
//}

TrueTypeFont::TrueTypeFont() :
  cmap_(*this),
  head_(*this),
  maxp_(*this),
  loca_(*this),
  hhea_(*this),
  hmtx_(*this),
  name_(*this),
  os_2_(*this),
  glyf_(*this) {}

bool TrueTypeFont::Open(const TCHAR *path) {
  ifstream fin(path, ios::in | ios::binary);
  if (!fin.is_open()) {
    cout << "Error reading file " << path << "!" << endl;
    return false;
  }

  offset_table_.LoadTable(fin);
  cmap_.Init(offset_table_.GetTableEntry("cmap"), fin);
  head_.Init(offset_table_.GetTableEntry("head"), fin);
  maxp_.Init(offset_table_.GetTableEntry("maxp"), fin);
  loca_.Init(offset_table_.GetTableEntry("loca"), fin);
  hhea_.Init(offset_table_.GetTableEntry("hhea"), fin);
  hmtx_.Init(offset_table_.GetTableEntry("hmtx"), fin);
  name_.Init(offset_table_.GetTableEntry("name"), fin);
  os_2_.Init(offset_table_.GetTableEntry("OS/2"), fin);
  glyf_.Init(offset_table_.GetTableEntry("glyf"), fin);

  fin.close();
  return true;
}

void TrueTypeFont::Close() {
  offset_table_.Destroy();
  cmap_.Destroy();
  head_.Destroy();
  maxp_.Destroy();
  loca_.Destroy();
  hhea_.Destroy();
  hmtx_.Destroy();
  name_.Destroy();
  os_2_.Destroy();
  glyf_.Destroy();
}

//void TrueTypeFont::GetGlyphOutline(const UShort ch) {
  //USHORT glyph_index = cmap.get_glyph_index(Windows, Unicode_BMP, ch);
  ////ULONG location = loca.find_location(glyph_index);
  //wstring name = L"C:/msyh/";
  //name += ch;
  //name += L".svg";
  //// FIXME: the following codes should be more generic, i.e. use _t instead of _w.
  //FILE *fp = NULL;
  //if(_tfopen_s(&fp, name.c_str(), _T("w")) == 0){ // Return 0 if successful.
  //  glyf.dump_svg_outline(fp);
  //  fclose(fp);
  //}
//}

bool TrueTypeFont::DumpTtf(const TCHAR *path) const {
  XmlLogger logger(path);
  if (logger.Error()) return false;
  logger.Println("<ttFont>");
  logger.IncreaseIndent();
  offset_table_.DumpInfo(logger);
  cmap_.DumpInfo(logger);
  head_.DumpInfo(logger);
  hhea_.DumpInfo(logger);
  maxp_.DumpInfo(logger);
  os_2_.DumpInfo(logger);
  name_.DumpInfo(logger);
  loca_.DumpInfo(logger);
  glyf_.DumpInfo(logger);
  hmtx_.DumpInfo(logger);
  logger.DecreaseIndent();
  logger.Println("</ttFont>");
  return true;
}

#define SNTPRINTFS(buf, buf_len, len, format_, ...)\
  (len) += _sntprintf_s((buf) + (len), (buf_len) - (len), \
                        _TRUNCATE, (format_), __VA_ARGS__)
void TrueTypeFont::GlyphInfo(const Glyph &glyph, TCHAR *buf,
                             size_t buf_len) const {
  if (!buf || buf_len == 0) return;
  int len = 0;
  SNTPRINTFS(buf, buf_len, len, _T("xMin: %d\n"), glyph.x_min());
  SNTPRINTFS(buf, buf_len, len, _T("xMax: %d\n"), glyph.x_max());
  SNTPRINTFS(buf, buf_len, len, _T("yMin: %d\n"), glyph.y_min());
  SNTPRINTFS(buf, buf_len, len, _T("yMax: %d\n"), glyph.y_max());
  SNTPRINTFS(buf, buf_len, len, _T("numberOfContours: %d\n"),
             glyph.num_contours());
  SNTPRINTFS(buf, buf_len, len, _T("leftSideBearing: %d\n"),
             hmtx_.GetLeftSideBearing(glyph.glyph_index()));
  SNTPRINTFS(buf, buf_len, len, _T("advanceWidth: %d\n"),
             hmtx_.GetAdvanceWidth(glyph.glyph_index()));
}

} // namespace ttf_dll