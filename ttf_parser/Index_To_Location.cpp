#include "stdafx.h"
#include "Index_To_Location.h"

namespace ttf_dll{
  void Index_To_Location::load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_glyphs, SHORT loca_format){
    fin.seekg(entry->offset, ios::beg);
    if(loca_format){
      offsets = new ULONG[num_glyphs]; // 1 for ULONG
      ifstream_read_big_endian(fin, offsets, sizeof(ULONG), num_glyphs);
    }else{
      offsets = new USHORT[num_glyphs]; // 0 for USHORT
      ifstream_read_big_endian(fin, offsets, sizeof(USHORT), num_glyphs);
    }
  }
}