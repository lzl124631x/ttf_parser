#include "stdafx.h"
#include "True_Type_Font.h"
#include <sstream>

namespace ttf_dll{
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
  True_Type_Font *g_ttf;

  bool True_Type_Font::load_path(string &path){
    ifstream fin(path, ios::in | ios::binary); // FIXME: think twice about the parameter!
    if(!fin.is_open()){
      cout << "Error reading file " << path.c_str() << "!" << endl;
      return false;
    }

    g_ttf = this;
    offset_table.load_table(fin);
    cmap.load_table(offset_table.get_table_entry("cmap"), fin);
    head.load_table(offset_table.get_table_entry("head"), fin);
    maxp.load_table(offset_table.get_table_entry("maxp"), fin);
    loca.load_table(offset_table.get_table_entry("loca"), fin);
    hhea.load_table(offset_table.get_table_entry("hhea"), fin);
    hmtx.load_table(offset_table.get_table_entry("hmtx"), fin);
    name.load_table(offset_table.get_table_entry("name"), fin);
    os_2.load_table(offset_table.get_table_entry("OS/2"), fin);
    glyf.load_table(offset_table.get_table_entry("glyf"), fin);

    fin.close();
    return true;
  }

  True_Type_Font::~True_Type_Font(){}

  void True_Type_Font::get_glyph_outline(USHORT ch){
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

  bool True_Type_Font::dump_ttf(char *path){
    FILE *fp = NULL;
    if(fopen_s(&fp, path, "w") == 0){ // Zero if successful; an error code on failure.
      fprintf(fp, "<ttFont>\n");
      offset_table.dump_info(fp, 1);
      cmap.dump_info(fp, 1);
      head.dump_info(fp, 1);
      hhea.dump_info(fp, 1);
      maxp.dump_info(fp, 1);
      os_2.dump_info(fp, 1);
      name.dump_info(fp, 1);
      loca.dump_info(fp, 1);
      glyf.dump_info(fp, 1);
      hmtx.dump_info(fp, 1);
      fprintf(fp, "</ttFont>\n");
      fclose(fp);
      return true;
    }
    return false;
  }

#define SNTPRINTFS(buf, buf_len, len, format, ...)\
  (len) += _sntprintf_s((buf) + (len), (buf_len) - (len), _TRUNCATE, (format), __VA_ARGS__)
  void True_Type_Font::glyph_info(Glyph *glyph, TCHAR *buf, size_t buf_len){
    if(!buf || buf_len == 0) return;
    if(!glyph){
      buf[0] = _T('\0');
      return;
    }
    int len = 0;
    SNTPRINTFS(buf, buf_len, len, _T("xMin: %d\n"), glyph->header.x_min);
    SNTPRINTFS(buf, buf_len, len, _T("xMax: %d\n"), glyph->header.x_max);
    SNTPRINTFS(buf, buf_len, len, _T("yMin: %d\n"), glyph->header.y_min);
    SNTPRINTFS(buf, buf_len, len, _T("yMax: %d\n"), glyph->header.y_max);
    SNTPRINTFS(buf, buf_len, len, _T("numberOfContours: %d\n"), glyph->header.num_contours);
    SNTPRINTFS(buf, buf_len, len, _T("leftSideBearing: %d\n"), g_ttf->hmtx.get_lsb(glyph->glyph_index));
    SNTPRINTFS(buf, buf_len, len, _T("advanceWidth: %d\n"), g_ttf->hmtx.get_aw(glyph->glyph_index));
  }
}