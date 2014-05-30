#include "stdafx.h"
#include "Index_To_Location.h"
#include "True_Type_Font.h"

namespace ttf_dll {

void Index_To_Location::load_table(Table_Record_Entry *entry, ifstream &fin) {
  num_glyphs = g_ttf->maxp.num_glyphs;
  loca_format = g_ttf->head.loca_format;
  fin.seekg(entry->offset, ios::beg);
  if(loca_format) {
    offsets = new ULONG[num_glyphs];      // 1 for ULONG
    FREAD_N(fin, (ULONG*)offsets, num_glyphs);
  } else {
    offsets = new USHORT[num_glyphs];     // 0 for USHORT
    FREAD_N(fin, (USHORT*)offsets, num_glyphs);
  }
}

void Index_To_Location::dump_info(Xml_Logger &logger) const {
  logger.println("<loca format=\"%s\">", loca_format ? "ULONG" : "USHORT");
  logger.inc_indent();
  logger.println("<offsets>");
  logger.inc_indent();
  if(loca_format) {
    logger.print_array<ULONG>(offsets, num_glyphs, "%8u");
  } else {
    logger.println(
        "<!-- ATTENTION: The SHORT version 'loca' stores the actual local "
        "offset divided by 2! The divided offsets are shown below. -->");
    logger.print_array<USHORT>(offsets, num_glyphs, "%8u");
  }
  logger.dec_indent();
  logger.println("</offsets>");
  logger.dec_indent();
  logger.println("</loca>");
}

ULONG Index_To_Location::glyph_index_to_offset(GLYPH_ID glyph_index) const {
  if(glyph_index >= num_glyphs) {
    // ERROR: Invalid parameter!
    return 0;
  }
  ULONG offset = 0;
  if(loca_format) {
    offset = *((ULONG*)offsets + glyph_index);      // 1 for ULONG
  } else {
    offset = *((USHORT*)offsets + glyph_index);     // 0 for USHORT
    // ATTENTION: The SHORT version 'loca' stores the actual local offset divided by 2!
    offset <<= 1;
  }
  return offset;
}

} // namespace ttf_dll