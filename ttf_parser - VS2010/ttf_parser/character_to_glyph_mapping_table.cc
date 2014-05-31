#include "stdafx.h"
#include "character_to_glyph_mapping_table.h"

namespace ttf_dll {

/******************* Character_To_Glyph_Index_Mapping_Table ******************/
void CharacterToGlyphIndexMappingTable::LoadTable(TableRecordEntry *entry, ifstream &fin) {
  fin.seekg(entry->offset_, ios::beg);
  streampos base = fin.tellg();
  FREAD(fin, &table_version_number_);
  FREAD(fin, &num_encoding_tables_);
  if(num_encoding_tables_ == 0) return; // FIXME: futile?
  // Read encoding table entries.
  encoding_records_ = new EncodingRecord[num_encoding_tables_];
  EncodingRecord *record = encoding_records_;
  for(int i = 0; i < num_encoding_tables_; ++i, ++record) {
    record->LoadEntry(fin);
  }
  record = encoding_records_;
  for(int i = 0; i < num_encoding_tables_; ++i, ++record) {
    record->LoadEncodingTable(fin, base);
  }
  // Since all the encoding records are stored sequentially, they are read
  // first. I cannot guarantee the encoding tables are store sequentially, so
  // they are read in a separate loop.
  // FIXME: Maybe the above two loops can be combined without efficiency
  // decline.
}

// FIXME: This Function uses sequential search. Maybe it can be more efficient.
EncodingTable* CharacterToGlyphIndexMappingTable::GetEncodingTable(UShort platform_id, UShort encoding_id) {
  EncodingTable* t = NULL;
  for(int i = 0; i < num_encoding_tables_; ++i) {
    if(encoding_records_[i].platform_id_ == platform_id &&
        encoding_records_[i].encoding_id_ == encoding_id) {
      t = encoding_records_[i].encoding_table_;
    }
  }
  return t;
}

GlyphID CharacterToGlyphIndexMappingTable::GetGlyphIndex(UShort platform_id, UShort encoding_id, UShort ch) {
  GlyphID glyph_index = 0;
  EncodingTable* encoding_table = GetEncodingTable(platform_id, encoding_id);
  if(encoding_table) {
    glyph_index = encoding_table->GetGlyphIndex(ch);
  }
  return glyph_index;
}

/**************************** Encoding_Record ********************************/
void EncodingRecord::LoadEncodingTable(ifstream &fin, streampos base) {
  fin.seekg(base);
  fin.seekg(byte_offset_, ios::cur);
  // FIXME: fstream is random-access stream! Simplify the code!
  UShort format = 0;
  FREAD(fin, &format);
  // Rewind to let 'Encodint_Table' read the format.
  fin.seekg(-(int)sizeof(format), ios::cur);
  switch(format) {
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
  }
}

void EncodingRecord::LoadEntry(ifstream &fin) {
  FREAD(fin, &platform_id_);
  FREAD(fin, &encoding_id_);
  FREAD(fin, &byte_offset_);
}

/**************************** Encoding_Table *********************************/
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

/*************************** Byte_Encoding_Table *****************************/
ByteEncodingTable::ByteEncodingTable(ifstream &fin) : EncodingTable(fin) {
  FREAD_N(fin, glyph_id_array_, 256);
}

GlyphID ByteEncodingTable::GetGlyphIndex(UShort ch) {
  if(ch >= 256) return 0; // ERROR: ch should be no more than 256
  return glyph_id_array_[ch];
}

/********************** High_Byte_Mapping_Through_Table **********************/
HighByteMappingThroughTable::HighByteMappingThroughTable(ifstream &fin) : EncodingTable(fin) {
  FREAD_N(fin, subheader_keys_, 256);
  //FIXME: not finished
}

GlyphID HighByteMappingThroughTable::GetGlyphIndex(UShort ch) {
  return 0; // FIXME: not finished
}

/******************* Segment_Mapping_To_Delta_Values *************************/
SegmentMappingToDeltaValues::SegmentMappingToDeltaValues(ifstream &fin) : EncodingTable(fin) {
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

GlyphID SegmentMappingToDeltaValues::GetGlyphIndex(UShort ch) {
  int i = 0;
  while(end_count_[i] != 0xFFFF && end_count_[i] < ch) {
    ++i;
  }
  GlyphID glyph_index = 0;
  if(start_count_[i] <= ch) {
    if(id_range_offset_[i]) {
      glyph_index = *(glyph_id_array_ + (id_range_offset_[i] >> 1) + (ch - start_count_[i])
                      - (&id_range_offset_[seg_countx2_ >> 1] - &id_range_offset_[i]));
    } else {
      glyph_index = id_delta_[i] + ch;
    }
  }
  return glyph_index;
}

/*********************** Trimmed_Table_Mapping *******************************/
TrimmedTableMapping::TrimmedTableMapping(ifstream &fin) : EncodingTable(fin) {
  FREAD(fin, &first_code_);
  FREAD(fin, &entry_count_);
  glyph_id_array_ = new UShort[entry_count_];
  FREAD_N(fin, glyph_id_array_, entry_count_);
}

GlyphID TrimmedTableMapping::GetGlyphIndex(UShort ch) {
  return 0; // FIXME: not finished
}

void CharacterToGlyphIndexMappingTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap tableVersion=\"0x%08x\" numberOfEncodingTables=\"%d\">",
         table_version_number_, num_encoding_tables_);
  logger.IncreaseIndent();
  for(int i = 0; i < num_encoding_tables_; ++i) {
    EncodingRecord *record = &encoding_records_[i];
    logger.Println("<encodingRecord>");
    logger.IncreaseIndent();
    logger.Println("<platformID value=\"%u\"/>", record->platform_id_);
    logger.Println("<encodingID value=\"%u\"/>", record->encoding_id_);
    logger.Println("<offset value=\"%lu\"/>", record->byte_offset_);
    logger.DecreaseIndent();
    logger.Println("</encodingRecord>");
  }
  for(int i = 0; i < num_encoding_tables_; ++i) {
    encoding_records_[i].encoding_table_->DumpInfo(logger);
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
  logger.PrintArray<UShort>(glyph_id_array_, (seg_countx2_ >> 1), "%8u");//FIXME: the glyph_id_array is not dumped out with the right length.
  logger.Println("</glyphIdArray>");

  logger.DecreaseIndent();
  logger.Println("</cmap_format_4>");
}

void ByteEncodingTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_0>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.Println("<glyphIdArray>");
  logger.PrintArray<Byte>((void*)glyph_id_array_, 256, "%8u"); // FIXME: why have to add (void*)
  logger.Println("</glyphIdArray>");
  logger.DecreaseIndent();
  logger.Println("</cmap_format_0>");
}

void HighByteMappingThroughTable::DumpInfo(XmlLogger &logger) const {
  logger.Println("<cmap_format_2>");
  logger.IncreaseIndent();
  DumpTableHeader(logger);
  logger.PrintArray<UShort>((void*)subheader_keys_, 256, "%8u"); // FIXME: why have to add (void*)
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