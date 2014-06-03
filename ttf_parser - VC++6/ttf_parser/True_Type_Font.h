#ifndef TTF_PARSER_H
#define TTF_PARSER_H

#include <fstream>
#include "TTF_Table.h"
#include "Maximum_Profile.h"
#include "Index_To_Location.h"
#include "Glyph_Data.h"
#include "Font_Header.h"
#include "Character_To_Glyph_Mapping_Table.h"
#include "Horizontal_Header.h"
#include "Horizontal_Metrics.h"
#include "Naming_Table.h"
#include "OS_2_And_Windows_Metrics.h"
#include "Post_Script.h"

namespace ttf_dll{
  class DLL_API True_Type_Font{
  public:
    Offset_Table offset_table;
    Character_To_Glyph_Index_Mapping_Table cmap;
    Font_Header head;
    Maximum_Profile maxp;
    Index_To_Location loca;
    Horizontal_Header hhea;
    Horizontal_Metrics hmtx;
    Naming_Table name;
    OS_2_And_Windows_Metrics os_2;
    Glyph_Data glyf;
    bool load_path(std::string &path);
    ~True_Type_Font();
    bool valid_checksum(char* tag);
    void get_glyph_outline(USHORT ch);
    bool dump_ttf(char *path);
    Glyph *get_glyph(GLYPH_ID glyph_index){
      return glyf.load_glyph(glyph_index);
    }
    ULONG glyph_index_to_offset(GLYPH_ID glyph_index){
      return loca.glyph_index_to_offset(glyph_index);
    }
    void True_Type_Font::glyph_info(Glyph *glyph, TCHAR *buf, size_t buf_len);
  };

  extern True_Type_Font *g_ttf;
  // A global pointer to the ttf.
  // Members of ttf use this pointer to access other members of ttf.
}

#endif
