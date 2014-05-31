#include "stdafx.h"
#include "ttf_table.h"

namespace ttf_dll {

void TableRecordEntry::LoadEntry(ifstream &fin) {
  // The type of `tag` is ULONG, but it's actually a 4-character string.
  // So don't use FREAD(fin, &tag) because it will reverse the character sequence.
  fin.read((char*)&tag_, sizeof(tag_));
  FREAD(fin, &checksum_);
  FREAD(fin, &offset_);
  FREAD(fin, &length_);
}

void OffsetTable::LoadTable(ifstream &fin) {
  fin.seekg(0, ios::beg);
  FREAD(fin, &sfnt_version_);
  FREAD(fin, &num_tables_);
  FREAD(fin, &search_range_);
  FREAD(fin, &entry_selector_);
  FREAD(fin, &range_shift_);
  table_record_entries_ = new TableRecordEntry[num_tables_];
  for(int i = 0; i < num_tables_; ++i) {
    table_record_entries_[i].LoadEntry(fin);
  };
}

TableRecordEntry* OffsetTable::GetTableEntry(const char *tag_str) const {
  ULong tag = *(ULong*)tag_str;
  for(int i = 0; i < num_tables_; ++i) {
    TableRecordEntry *entry = &table_record_entries_[i];
    if(entry->tag_ == tag) {
      return entry;
    }
  }
  return NULL;
}

void OffsetTable::DumpInfo(XmlLogger &logger) const {
  logger.Println(
      "<offsetTable sfntVersion=\"%08x\" numTables=\"%d\" "
      "searchRange=\"%d\" entrySelector=\"%d\" rangeShift=\"%d\">",
      sfnt_version_, num_tables_,
      search_range_, entry_selector_, range_shift_);
  logger.IncreaseIndent();
  for(int i = 0; i < num_tables_; ++i) {
    TableRecordEntry *entry = &table_record_entries_[i];
    logger.Println(
        "<entry tag=\"%.4s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>",
        (char*)&entry->tag_, entry->checksum_, entry->offset_, entry->length_);
  }
  logger.DecreaseIndent();
  logger.Println("</offsetTable>");
}

} // namespace ttf_dll