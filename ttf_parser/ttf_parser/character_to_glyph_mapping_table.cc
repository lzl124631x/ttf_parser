#include "stdafx.h"
#include "character_to_glyph_mapping_table.h"
using namespace std;

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
CharacterToGlyphIndexMappingTable::CharacterToGlyphIndexMappingTable(
    const TrueTypeFont& ttf)
    : TtfSubtable(ttf),
      version_(0),
      num_tables_(0),
      encoding_records_(NULL) {}
  
void CharacterToGlyphIndexMappingTable::Init(
    const TableRecordEntry *entry, ifstream &fin) {
  fin.seekg(entry->offset(), ios::beg);
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

// FIXME: This function uses sequential search. Maybe it can be more efficient.
EncodingTable* CharacterToGlyphIndexMappingTable::GetEncodingTable(
    UShort platform_id,
    UShort encoding_id) const {
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
    UShort platform_id,
    UShort encoding_id,
    UShort ch) const {
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

GlyphId ByteEncodingTable::GetGlyphIndex(UShort ch) const {
  if (ch >= 256) return 0; // ERROR: `ch` should be less than 256
  return glyph_id_array_[ch];
}

/****************************************************************************/
/*                       HighByteMappingThroughTable                        */
/****************************************************************************/

HighByteMappingThroughTable::HighByteMappingThroughTable(ifstream &fin)
  : EncodingTable(fin) {
  FREAD_N(fin, subheader_keys_, 256);
  // The 259 UShort data are composed of:
  // * 3 UShort data in the general cmap header.
  // * 256 UShort data in the `subHeaderKeys` array.
  UShort var_len = (length_ >> 1) - 259;
  subheaders_and_glyph_id_array = new UShort[var_len];
  FREAD_N(fin, subheaders_and_glyph_id_array, var_len);
}

#define HIBYTE(w) ((Byte)((w) >> 8))
#define LOBYTE(w) ((Byte)(w))
GlyphId HighByteMappingThroughTable::GetGlyphIndex(UShort ch) const {
  Byte high_byte = HIBYTE(ch), low_byte = LOBYTE(ch);
  // FIXME: not finished. This subtable is designed for parsing mixed 8/16-bit
  // string, hence the signature of this function is not proper.
  Subheader *subheader_ptr = (Subheader*)(subheaders_and_glyph_id_array
                                          + subheader_keys_[high_byte]);
  // If the subheader key is 0, the character is single-byte or invalid.
  bool single_byte_char = (subheader_keys_[high_byte] == 0);
  // If the character is single-byte, the single byte is used for indexing;
  // otherwise the second byte is used.
  UShort index = (single_byte_char ? high_byte : low_byte)
                   - subheader_ptr->first_code;
  if (index >= 0 && index < subheader_ptr->entry_count) {
    UShort glyph_index = *(&subheader_ptr->id_range_offset
                           + (subheader_ptr->id_range_offset >> 1) + index);
    // If the value obtained from the subarray is not 0, `idDelta` should be
    // added to it in order to get the `glyphIndex`. The idDelta arithmetic
    // is modulo 65536.
    return glyph_index ? (glyph_index + subheader_ptr->id_delta) % 65536 : 0;
  }
  // Return 0 for out-of-range character.
  return 0;
}
#undef HIBYTE
#undef LOBYTE

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
  // The 8 UShort data are composed of:
  // * 3 UShort data in the general cmap header.
  // * 4 UShort data in this format 4 cmap header.
  // * 1 Ushort for reserved pad.
  UShort var_len = seg_count + ((length_ >> 1) - (8 + (seg_countx2_ << 1)));
  id_range_offset_and_glyph_id_array_ = new UShort[var_len];
  FREAD_N(fin, id_range_offset_and_glyph_id_array_, var_len);
}

GlyphId SegmentMappingToDeltaValues::GetGlyphIndex(UShort ch) const {
  int i = 0;
  while (end_count_[i] != 0xFFFF && end_count_[i] < ch) {
    ++i;
  }
  GlyphId glyph_index = 0;
  if (start_count_[i] <= ch) {
    if (id_range_offset_and_glyph_id_array_[i]) {
      // If the `idRangeOffset` value for the segment is not 0, the mapping of
      // character codes relies on `glyphIdArray`. The character code offset 
      // from `startCode` is added to the `idRangeOffset` value. This sum is
      // used as an offset from the current location within `idRangeOffset`
      // itself to index out the correct `glyphIdArray` value. This obscure
      // indexing trick works because `glyphIdArray` immediately follows
      // `idRangeOffset` in the font file. The C expression that yields the
      // glyph index is:
      //   *(idRangeOffset[i]/2
      //     + (c - startCount[i])
      //     + &idRangeOffset[i])
      // The value `c` is the character code in question, and `i` is the
      // segment index in which `c` appears. If the value obtained from the
      // indexing operation is not 0 (which indicates missingGlyph),
      // `idDelta[i]` is added to it to get the glyph index. The `idDelta`
      // arithmetic is modulo 65536.
      glyph_index = *((id_range_offset_and_glyph_id_array_[i] >> 1)
                      + (ch - start_count_[i])
                      + &id_range_offset_and_glyph_id_array_[i]);
    } else {
      // If the `idRangeOffset` is 0, the `idDelta` value is added directly to
      // the character code offset (i.e. `idDelta[i] + c`) to get the
      // corresponding glyph index. Again, the `idDelta` arithmetic is modulo
      // 65536.
      glyph_index = (id_delta_[i] + ch) % 65536;
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

GlyphId TrimmedTableMapping::GetGlyphIndex(UShort ch) const {
  UShort index = ch - first_code_;
  return (index >= 0 && index < entry_count_) ? glyph_id_array_[index] : 0;
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

void ByteEncodingTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_0>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.Println("<glyphIdArray>");
  logger.PrintArray<Byte>(glyph_id_array_, 256, "%8u");
  logger.Println("</glyphIdArray>");
  logger.DecreaseIndent();
  logger.Println("</cmap_format_0>");
}

void HighByteMappingThroughTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_2>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.PrintArray<UShort>(subheader_keys_, 256, "%8u");
  logger.DecreaseIndent();
  logger.Println("</cmap_format_2>");
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
  logger.PrintArray<UShort>(id_range_offset_and_glyph_id_array_,
                            (seg_countx2_ >> 1), "%8u");
  logger.Println("</idRangeOffset>");

  logger.Println("<glyphIdArray>");
  //FIXME: the glyph_id_array is not dumped out.
  logger.Println("</glyphIdArray>");

  logger.DecreaseIndent();
  logger.Println("</cmap_format_4>");
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