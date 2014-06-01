#include "stdafx.h"
#include "index_to_location.h"
#include "true_type_font.h"

namespace ttf_dll {

void IndexToLocation::LoadTable(TableRecordEntry *entry, ifstream &fin) {
  num_glyphs_ = g_ttf->maxp_.num_glyphs_;
  loca_format_ = g_ttf->head_.loca_format_;
  fin.seekg(entry->offset_, ios::beg);
  if(loca_format_) {
    offsets_ = new ULong[num_glyphs_];      // 1 for ULONG
    FREAD_N(fin, (ULong*)offsets_, num_glyphs_);
  } else {
    offsets_ = new UShort[num_glyphs_];     // 0 for USHORT
    FREAD_N(fin, (UShort*)offsets_, num_glyphs_);
  }
}

void IndexToLocation::DumpInfo(XmlLogger &logger) const {
  logger.Println("<loca format=\"%s\">", loca_format_ ? "ULONG" : "USHORT");
  logger.IncreaseIndent();
  logger.Println("<offsets>");
  logger.IncreaseIndent();
  if(loca_format_) {
    logger.PrintArray<ULong>(offsets_, num_glyphs_, "%8u");
  } else {
    logger.Println(
        "<!-- ATTENTION: The SHORT version 'loca' stores the actual local "
        "offset divided by 2! The divided offsets are shown below. -->");
    logger.PrintArray<UShort>(offsets_, num_glyphs_, "%8u");
  }
  logger.DecreaseIndent();
  logger.Println("</offsets>");
  logger.DecreaseIndent();
  logger.Println("</loca>");
}

ULong IndexToLocation::GlyphIndexToOffset(GlyphId glyph_index) const {
  if(glyph_index >= num_glyphs_) {
    // ERROR: Invalid parameter!
    return 0;
  }
  ULong offset = 0;
  if(loca_format_) {
    offset = *((ULong*)offsets_ + glyph_index);      // 1 for ULONG
  } else {
    offset = *((UShort*)offsets_ + glyph_index);     // 0 for USHORT
    // ATTENTION: The SHORT version 'loca' stores the actual local offset divided by 2!
    offset <<= 1;
  }
  return offset;
}

} // namespace ttf_dll