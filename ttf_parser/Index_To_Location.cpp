#include "stdafx.h"
#include "Index_To_Location.h"
#include "True_Type_Font.h"

namespace ttf_dll{
  void Index_To_Location::load_table(Table_Directory_Entry *entry, ifstream &fin){
    num_glyphs = g_ttf->maxp.num_glyphs;
    loca_format = g_ttf->head.index_to_loc_format;
    fin.seekg(entry->offset, ios::beg);
    if(loca_format){
      offsets = new ULONG[num_glyphs]; // 1 for ULONG
      FREAD_N(fin, (ULONG*)offsets, num_glyphs);
    }else{
      offsets = new USHORT[num_glyphs]; // 0 for USHORT
      FREAD_N(fin, (USHORT*)offsets, num_glyphs);
    }
  }

  void Index_To_Location::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<loca format=\"%s\">\n", loca_format ? "ULONG" : "USHORT");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<offsets>\n");
    if(loca_format){
      dump_array<ULONG>(fp, indent + 1, (ULONG*)offsets, num_glyphs, "%5u"); // FIXME: Consider change dump_array to use void* as the type of array.
    }else{
      dump_array<USHORT>(fp, indent + 1, (USHORT*)offsets, num_glyphs, "%5u");
    }
    INDENT(fp, indent); fprintf(fp, "</offsets>\n");
    --indent;
    INDENT(fp, indent); fprintf(fp, "</loca>\n");
  }
}