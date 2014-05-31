#include "stdafx.h"
#include "true_type_font.h"
#include <sstream>
#include "Xml_Logger.h"

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
TrueTypeFont *g_ttf;

bool TrueTypeFont::LoadPath(const TCHAR *path) {
  ifstream fin(path, ios::in | ios::binary); // FIXME: think twice about the parameter!
  if(!fin.is_open()) {
    cout << "Error reading file " << path << "!" << endl;
    return false;
  }

  g_ttf = this;
  offset_table_.LoadTable(fin);
  cmap_.LoadTable(offset_table_.GetTableEntry("cmap"), fin);
  head_.LoadTable(offset_table_.GetTableEntry("head"), fin);
  maxp_.LoadTable(offset_table_.GetTableEntry("maxp"), fin);
  loca_.LoadTable(offset_table_.GetTableEntry("loca"), fin);
  hhea_.LoadTable(offset_table_.GetTableEntry("hhea"), fin);
  hmtx_.LoadTable(offset_table_.GetTableEntry("hmtx"), fin);
  name_.LoadTable(offset_table_.GetTableEntry("name"), fin);
  os_2_.LoadTable(offset_table_.GetTableEntry("OS/2"), fin);
  glyf_.LoadTable(offset_table_.GetTableEntry("glyf"), fin);

  fin.close();
  return true;
}

TrueTypeFont::~TrueTypeFont() {}

void TrueTypeFont::GetGlyphOutline(const UShort ch) {
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
}

bool TrueTypeFont::DumpTTF(const char *path) const {
  XmlLogger logger(path);
  if(logger.Error()) return false;
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
  logger.Println("</ttFont>");
  return true;
}

#define SNTPRINTFS(buf, buf_len, len, format_, ...)\
  (len) += _sntprintf_s((buf) + (len), (buf_len) - (len), _TRUNCATE, (format_), __VA_ARGS__)
void TrueTypeFont::GlyphInfo(Glyph *glyph, TCHAR *buf, size_t buf_len) {
  if(!buf || buf_len == 0) return;
  if(!glyph) {
    buf[0] = _T('\0');
    return;
  }
  int len = 0;
  SNTPRINTFS(buf, buf_len, len, _T("xMin: %d\n"), glyph->header_.x_min_);
  SNTPRINTFS(buf, buf_len, len, _T("xMax: %d\n"), glyph->header_.x_max_);
  SNTPRINTFS(buf, buf_len, len, _T("yMin: %d\n"), glyph->header_.y_min_);
  SNTPRINTFS(buf, buf_len, len, _T("yMax: %d\n"), glyph->header_.y_max_);
  SNTPRINTFS(buf, buf_len, len, _T("numberOfContours: %d\n"), glyph->header_.num_contours_);
  SNTPRINTFS(buf, buf_len, len, _T("leftSideBearing: %d\n"), g_ttf->hmtx_.GetLeftSideBearing(glyph->glyph_index_));
  SNTPRINTFS(buf, buf_len, len, _T("advanceWidth: %d\n"), g_ttf->hmtx_.GetAdvanceWidth(glyph->glyph_index_));
}

} // namespace ttf_dll