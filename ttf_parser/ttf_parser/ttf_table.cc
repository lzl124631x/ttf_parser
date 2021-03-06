#include "stdafx.h"
#include "ttf_table.h"
using namespace std;

namespace ttf_dll {

TableRecordEntry::TableRecordEntry()
  : tag_(0UL), checksum_(0UL), offset_(0UL), length_(0UL) {}

void TableRecordEntry::LoadEntry(ifstream &fin) {
  // The type of `tag` is ULONG, but it's actually a 4-character string.
  // So don't use FREAD(fin, &tag) because it will reverse the character
  //sequence.
  fin.read(reinterpret_cast<char*>(&tag_), sizeof(tag_));
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

const TableRecordEntry* OffsetTable::GetTableEntry(
    const char *tag_str) const {
  ULong tag = TableRecordTagToULong(tag_str);
  for(int i = 0; i < num_tables_; ++i) {
    TableRecordEntry *entry = &table_record_entries_[i];
    if(entry->tag() == tag) {
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
    ULong tag = entry->tag();
    logger.Println(
        "<entry tag=\"%.4s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>",
        (char*)&tag, entry->checksum(), entry->offset(), entry->length());
  }
  logger.DecreaseIndent();
  logger.Println("</offsetTable>");
}

} // namespace ttf_dll