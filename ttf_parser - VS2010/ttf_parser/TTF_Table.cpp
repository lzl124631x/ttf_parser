#include "stdafx.h"
#include "TTF_Table.h"

namespace ttf_dll {

void Table_Record_Entry::load_entry(ifstream &fin) {
  // The type of `tag` is ULONG, but it's actually a 4-character string.
  // So don't use FREAD(fin, &tag) because it will reverse the character sequence.
  fin.read((char*)&tag, sizeof(tag));
  FREAD(fin, &checksum);
  FREAD(fin, &offset);
  FREAD(fin, &length);
}

void Offset_Table::load_table(ifstream &fin) {
  fin.seekg(0, ios::beg);
  FREAD(fin, &sfnt_version);
  FREAD(fin, &num_tables);
  FREAD(fin, &search_range);
  FREAD(fin, &entry_selector);
  FREAD(fin, &range_shift);
  table_record_entries = new Table_Record_Entry[num_tables];
  for(int i = 0; i < num_tables; ++i) {
    table_record_entries[i].load_entry(fin);
  };
}

Table_Record_Entry* Offset_Table::get_table_entry(const char *tag_str) const {
  ULONG tag = *(ULONG*)tag_str;
  for(int i = 0; i < num_tables; ++i) {
    Table_Record_Entry *entry = &table_record_entries[i];
    if(entry->tag == tag) {
      return entry;
    }
  }
  return NULL;
}

void Offset_Table::dump_info(Xml_Logger &logger) const {
  logger.println(
      "<offsetTable sfntVersion=\"%08x\" numTables=\"%d\" "
      "searchRange=\"%d\" entrySelector=\"%d\" rangeShift=\"%d\">",
      sfnt_version, num_tables,
      search_range, entry_selector, range_shift);
  logger.inc_indent();
  for(int i = 0; i < num_tables; ++i) {
    Table_Record_Entry *entry = &table_record_entries[i];
    logger.println(
        "<entry tag=\"%.4s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>",
        (char*)&entry->tag, entry->checksum, entry->offset, entry->length);
  }
  logger.dec_indent();
  logger.println("</offsetTable>");
}

} // namespace ttf_dll