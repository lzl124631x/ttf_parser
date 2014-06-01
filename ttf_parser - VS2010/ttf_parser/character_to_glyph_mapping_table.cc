#include "stdafx.h"
#include "character_to_glyph_mapping_table.h"

namespace ttf_dll {

/****************************************************************************/
/*                             EncodingTable                                */
/****************************************************************************/
EncodingTable::EncodingTable(ifstream &fin) {
  FREAD(fin, &format_);
  FREAD(fin, &length_);
  FREAD(fin, &language_);
}

void EncodingTable::DumpTableHeader(XmlLogger &logger) const {
  logger.Println("<format value=\"%u\"/>", format_);
  logger.Println("<length value=\"%u\"/>", length_);
  logger.Println("<language value=\"%u\"/>", language_);
}

/****************************************************************************/
/*                             EncodingRecord                               */
/****************************************************************************/
void EncodingRecord::LoadRecord(ifstream &fin) {
  FREAD(fin, &platform_id_);
  FREAD(fin, &encoding_id_);
  FREAD(fin, &offset_);
}

void EncodingRecord::DumpRecord(XmlLogger &logger) const {
  logger.Println("<encodingRecord>");
  logger.IncreaseIndent();
  logger.Println("<platformID value=\"%u\"/>", platform_id_);
  logger.Println("<encodingID value=\"%u\"/>", encoding_id_);
  logger.Println("<offset value=\"%lu\"/>", offset_);
  logger.DecreaseIndent();
  logger.Println("</encodingRecord>");
}

void EncodingRecord::LoadEncodingTable(ifstream &fin, const streampos base) {
  fin.seekg(base);
  fin.seekg(offset_, ios::cur);
  // FIXME: fstream is random-access stream! Simplify the code!
  UShort format = 0;
  FREAD(fin, &format);
  // Rewind to let Encoding Table read the format.
  fin.seekg(-(int)sizeof(format), ios::cur);
  switch (format) {
    case kByteEncodingTable: {
      encoding_table_ = new ByteEncodingTable(fin);
      break;
    }
    case kHighByteMappingThroughTable: {
      encoding_table_ = new HighByteMappingThroughTable(fin);
      break;
    }
    case kSegmentMappingToDeltaValues: {
      encoding_table_ = new SegmentMappingToDeltaValues(fin);
      break;
    }
    case kTrimmedTableMapping: {
      encoding_table_ = new TrimmedTableMapping(fin);
      break;
    }
    default: {
      // FIXME: Tables supporting 4-byte character codes are not supported yet.
      encoding_table_ = NULL;
    }
  }
}

/****************************************************************************/
/*                   CharacterToGlyphIndexMappingTable                      */
/****************************************************************************/
void CharacterToGlyphIndexMappingTable::LoadTable(TableRecordEntry *entry,
                                                  ifstream &fin) {
  fin.seekg(entry->offset_, ios::beg);
  streampos base = fin.tellg();
  FREAD(fin, &version_);
  FREAD(fin, &num_tables_);
  if (num_tables_ == 0) return; // FIXME: futile?
  // Read encoding table entries.
  encoding_records_ = new EncodingRecord[num_tables_];
  EncodingRecord *record = encoding_records_;
  for (int i = 0; i < num_tables_; ++i, ++record) {
    record->LoadRecord(fin);
  }
  record = encoding_records_;
  for (int i = 0; i < num_tables_; ++i, ++record) {
    record->LoadEncodingTable(fin, base);
  }
  // Since all the encoding records are stored sequentially, they are read
  // first. I cannot guarantee the encoding tables are store sequentially, so
  // they are read in a separate loop.
  // FIXME: Maybe the above two loops can be combined without efficiency
  // decline.
}

// FIXME: This Function uses sequential search. Maybe it can be more efficient.
EncodingTable* CharacterToGlyphIndexMappingTable::GetEncodingTable(
    const UShort platform_id,
    const UShort encoding_id) const {
  EncodingTable* t = NULL;
  for (int i = 0; i < num_tables_; ++i) {
    if (encoding_records_[i].platform_id() == platform_id &&
        encoding_records_[i].encoding_id() == encoding_id) {
      t = encoding_records_[i].encoding_table();
    }
  }
  return t;
}

GlyphId CharacterToGlyphIndexMappingTable::GetGlyphIndex(
    const UShort platform_id,
    const UShort encoding_id,
    const UShort ch) const {
  GlyphId glyph_index = 0;
  EncodingTable* encoding_table = GetEncodingTable(platform_id, encoding_id);
  if (encoding_table) {
    glyph_index = encoding_table->GetGlyphIndex(ch);
  }
  return glyph_index;
}

/****************************************************************************/
/*                           ByteEncodingTable                              */
/****************************************************************************/
ByteEncodingTable::ByteEncodingTable(ifstream &fin) : EncodingTable(fin) {
  FREAD_N(fin, glyph_id_array_, 256);
}

GlyphId ByteEncodingTable::GetGlyphIndex(const UShort ch) const {
  if (ch >= 256) return 0; // ERROR: `ch` should be less than 256
  return glyph_id_array_[ch];
}

/****************************************************************************/
/*                       HighByteMappingThroughTable                        */
/****************************************************************************/
HighByteMappingThroughTable::HighByteMappingThroughTable(ifstream &fin)
  : EncodingTable(fin) {
  FREAD_N(fin, subheader_keys_, 256);
  //FIXME: not finished
}

GlyphId HighByteMappingThroughTable::GetGlyphIndex(const UShort ch) const {
  return 0; // FIXME: not finished
}

/****************************************************************************/
/*                       SegmentMappingToDeltaValues                        */
/****************************************************************************/
SegmentMappingToDeltaValues::SegmentMappingToDeltaValues(ifstream &fin)
  : EncodingTable(fin) {
  FREAD(fin, &seg_countx2_);
  FREAD(fin, &search_range_);
  FREAD(fin, &entry_selector_);
  FREAD(fin, &range_shift_);
  UShort seg_count = seg_countx2_ >> 1;

  end_count_ = new UShort[seg_count];
  FREAD_N(fin, end_count_, seg_count);
  FREAD(fin, &reserved_pad_);
  start_count_ = new UShort[seg_count];
  FREAD_N(fin, start_count_, seg_count);
  id_delta_ = new Short[seg_count];
  FREAD_N(fin, id_delta_, seg_count);
  id_range_offset_ = new UShort[seg_count];
  FREAD_N(fin, id_range_offset_, seg_count);
  UShort var_len = length_ - sizeof(UShort) * (8 + (seg_countx2_ << 1));
  glyph_id_array_ = new UShort[var_len];
  FREAD_N(fin, glyph_id_array_, var_len);
}

GlyphId SegmentMappingToDeltaValues::GetGlyphIndex(const UShort ch) const {
  int i = 0;
  while (end_count_[i] != 0xFFFF && end_count_[i] < ch) {
    ++i;
  }
  GlyphId glyph_index = 0;
  if (start_count_[i] <= ch) {
    if (id_range_offset_[i]) {
      glyph_index = *(glyph_id_array_ + (id_range_offset_[i] >> 1)
                    + (ch - start_count_[i])
                    - (&id_range_offset_[seg_countx2_ >> 1]
                    - &id_range_offset_[i]));
    } else {
      glyph_index = id_delta_[i] + ch;
    }
  }
  return glyph_index;
}

/****************************************************************************/
/*                           TrimmedTableMapping                            */
/****************************************************************************/
TrimmedTableMapping::TrimmedTableMapping(ifstream &fin) : EncodingTable(fin) {
  FREAD(fin, &first_code_);
  FREAD(fin, &entry_count_);
  glyph_id_array_ = new UShort[entry_count_];
  FREAD_N(fin, glyph_id_array_, entry_count_);
}

GlyphId TrimmedTableMapping::GetGlyphIndex(const UShort ch) const {
  return 0; // FIXME: not finished
}

/****************************************************************************/
/*                               DumpInfo                                   */
/****************************************************************************/
void CharacterToGlyphIndexMappingTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap tableVersion=\"0x%08x\" "
                 "numberOfEncodingTables=\"%d\">",
                 version_, num_tables_);
  logger.IncreaseIndent();
  for (int i = 0; i < num_tables_; ++i) {
    encoding_records_[i].DumpRecord(logger);
  }
  for (int i = 0; i < num_tables_; ++i) {
    encoding_records_[i].encoding_table()->DumpInfo(logger);
  }
  logger.DecreaseIndent();
  logger.Println("</cmap>");
}

void SegmentMappingToDeltaValues::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_4>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.Println("<segCountX2 value=\"%u\"/>", seg_countx2_);
  logger.Println("<searchRange value=\"%u\"/>", search_range_);
  logger.Println("<entrySelector value=\"%u\"/>", entry_selector_);
  logger.Println("<rangeShift value=\"%u\"/>", range_shift_);

  logger.Println("<endCount>");
  logger.PrintArray<UShort>(end_count_, (seg_countx2_ >> 1), "%8u");
  logger.Println("</endCount>");

  logger.Println("<reservedPad value=\"%u\"/>", reserved_pad_);

  logger.Println("<startCount>");
  logger.PrintArray<UShort>(start_count_, (seg_countx2_ >> 1), "%8u");
  logger.Println("</startCount>");

  logger.Println("<idDelta>");
  logger.PrintArray<Short>(id_delta_, (seg_countx2_ >> 1), "%8d");
  logger.Println("</idDelta>");

  logger.Println("<idRangeOffset>");
  logger.PrintArray<UShort>(id_range_offset_, (seg_countx2_ >> 1), "%8u");
  logger.Println("</idRangeOffset>");

  logger.Println("<glyphIdArray>");
  logger.PrintArray<UShort>(glyph_id_array_, (seg_countx2_ >> 1), "%8u");
  //FIXME: the glyph_id_array is not dumped out with the right length.
  logger.Println("</glyphIdArray>");

  logger.DecreaseIndent();
  logger.Println("</cmap_format_4>");
}

void ByteEncodingTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_0>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.Println("<glyphIdArray>");
  logger.PrintArray<Byte>((void*)glyph_id_array_, 256, "%8u");
  // FIXME: why have to add (void*)
  logger.Println("</glyphIdArray>");
  logger.DecreaseIndent();
  logger.Println("</cmap_format_0>");
}

void HighByteMappingThroughTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_2>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.PrintArray<UShort>((void*)subheader_keys_, 256, "%8u");
  // FIXME: why have to add (void*)
  logger.DecreaseIndent();
  logger.Println("</cmap_format_2>");
}

void TrimmedTableMapping::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_6>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.Println("<firstCode value=\"%u\"/>", first_code_);
  logger.Println("<entryCount value=\"%u\"/>", entry_count_);
  logger.PrintArray<UShort>(glyph_id_array_, entry_count_, "%8u");
  logger.DecreaseIndent();
  logger.Println("</cmap_format_6>");
}

} // namespace ttf_dll