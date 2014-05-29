#include "stdafx.h"
#include "TTF_Table.h"
using namespace std;
namespace ttf_dll{
  void Table_Directory_Entry::load_entry(ifstream &fin){
    // The type of `tag` is ULONG, but it's actually a 4-character string.
    // So don't use FREAD(fin, &tag) because it will reverse the character sequence.
    fin.read((char*)&tag, sizeof(tag));
    FREAD(fin, &checksum);
    FREAD(fin, &offset);
    FREAD(fin, &length);
  }

  void Offset_Table::load_table(ifstream &fin){
    fin.seekg(0, ios::beg);
    FREAD(fin, &sfnt_version);
    FREAD(fin, &num_tables);
    FREAD(fin, &search_range);
    FREAD(fin, &entry_selector);
    FREAD(fin, &range_shift);
    table_directory_entries = new Table_Directory_Entry[num_tables];
    for(int i = 0; i < num_tables; ++i){
      table_directory_entries[i].load_entry(fin);
    };
  }

  Table_Directory_Entry* Offset_Table::get_table_entry(char *tag_str){
    ULONG tag = *(ULONG*)tag_str;
    for(int i = 0; i < num_tables; ++i){
      Table_Directory_Entry *entry = &table_directory_entries[i];
      if(entry->tag == tag){
        return entry;
      }
    }
    return NULL;
  }

  void Offset_Table::dump_info(FILE *fp, size_t indent){
    IND_FP(fp, indent, "<offsetTable sfntVersion=\"%08x\" numTables=\"%d\" "
      "searchRange=\"%d\" entrySelector=\"%d\" rangeShift=\"%d\">\n",
      sfnt_version, num_tables, search_range, entry_selector, range_shift);
    ++indent;
    for(int i = 0; i < num_tables; ++i){
      Table_Directory_Entry *entry = &table_directory_entries[i];
      IND_FP(fp, indent, "<entry tag=\"%.4s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>\n",
        (char*)&entry->tag, entry->checksum, entry->offset, entry->length);
    }
    --indent;
    IND_FP(fp, indent, "</offsetTable>\n");
  }
}