#include "stdafx.h"
#include "True_Type_Font.h"

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

    offset_table.load_table(fin);
    cmap.load_table(offset_table.get_table_entry("cmap"), fin);
    head.load_table(offset_table.get_table_entry("head"), fin);
    maxp.load_table(offset_table.get_table_entry("maxp"), fin);
    loca.load_table(offset_table.get_table_entry("loca"), fin, maxp.num_glyphs, head.index_to_loc_format);
    hhea.load_table(offset_table.get_table_entry("hhea"), fin);
    hmtx.load_table(offset_table.get_table_entry("hmtx"), fin, hhea.number_of_hmetrics, maxp.num_glyphs);
    name.load_table(offset_table.get_table_entry("name"), fin);
    os_2.load_table(offset_table.get_table_entry("OS/2"), fin);
    glyf.load_table(offset_table.get_table_entry("glyf"), fin, maxp.max_points, maxp.max_contours, maxp.max_size_of_instructions);

    fin.close();
    g_ttf = this;
    return true;
  }

  True_Type_Font::~True_Type_Font(){
    glyf.clear();
  }

  ULONG True_Type_Font::glyph_index_to_offset(SHORT glyph_index){
    if(glyph_index >= maxp.num_glyphs){
      // Invalid parameter
      return NULL;
    }
    ULONG offset = 0;
    // The format of 'loca' table is determined by the 'index_to_loc_format' entry in 'head' table.
    if(head.index_to_loc_format){                 // FIXME: write a function 'glyph_index_to_offset' in 'loca'.
      offset += *((ULONG*)loca.offsets + glyph_index);      // 1 for ULONG
    }else{
      offset += *((USHORT*)loca.offsets + glyph_index);     // 0 for USHORT
    }
    return offset;
  }

  Glyph *True_Type_Font::get_glyph(USHORT glyph_index){
    // Each time user fetches the glyph, table 'glyf' will load the corresponding glyph.
    return glyf.load_glyph(glyph_index);
  }

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
      glyf.dump_info(fp, 1);
      fprintf(fp, "</ttFont>\n");
      fclose(fp);
      return true;
    }
    return false;
  }
}