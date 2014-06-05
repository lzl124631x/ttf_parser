#include "stdafx.h"
#include "index_to_location.h"
#include "true_type_font.h"

namespace ttf_dll {

void IndexToLocation::LoadTable(const TableRecordEntry *entry,
                                ifstream &fin) {
  num_glyphs_ = g_ttf->maxp_.num_glyphs_;
  loca_format_ = g_ttf->head_.loca_format_;
  fin.seekg(entry->offset_, ios::beg);
  UShort len = num_glyphs_ + 1;
  if (loca_format_) {
    // 1 for ULONG
    offsets_ = new ULong[len];
    FREAD_N(fin, (ULong*)offsets_, len);
  } else {
    // 0 for USHORT
    offsets_ = new UShort[len];
    FREAD_N(fin, (UShort*)offsets_, len);
  }
}

void IndexToLocation::DumpInfo(XmlLogger &logger) const {
  logger.Println("<loca format=\"%s\">", loca_format_ ? "ULONG" : "USHORT");
  logger.IncreaseIndent();
  logger.Println("<offsets>");
  logger.IncreaseIndent();
  if (loca_format_) {
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

void IndexToLocation::GetGlyphOffsetAndLength(
    const GlyphId glyph_index,
    ULong *offset,
    ULong *length) const {
  if (glyph_index >= num_glyphs_ || !offset || !length) {
    // ERROR: Invalid parameter!
    return;
  }
  if (loca_format_) {
    // 1 for ULONG
    ULong *long_offsets = (ULong*)offsets_;
    *offset = long_offsets[glyph_index];
    *length = long_offsets[glyph_index + 1] - *offset;
  } else {
    // 0 for USHORT
    USHORT *short_offsets = (USHORT*)offsets_;
    *offset = short_offsets[glyph_index];
    // ATTENTION: The SHORT version 'loca' stores the actual local offset
    // divided by 2!
    *offset <<= 1;
    *length = short_offsets[glyph_index + 1] - *offset;
  }
}

} // namespace ttf_dll