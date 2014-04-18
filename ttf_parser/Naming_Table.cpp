#include "stdafx.h"
#include "Naming_Table.h"

namespace ttf_dll{
  void Name_Record::load_record(ifstream &fin){
    FREAD(fin, &platform_id);
    FREAD(fin, &encoding_id);
    FREAD(fin, &language_id);
    FREAD(fin, &name_id);
    FREAD(fin, &length);
    FREAD(fin, &offset);
  }

  void Name_Record::read_string(ifstream &fin){
    // Each time this function is invoked, the file pointer is pointing to the start of string storage area.
    fin.seekg(offset, ios::cur);
    if(double_byte_string()){
      wchar_t *str = new wchar_t[(length >> 1) + 1];
      ifstream_read_big_endian(fin, str, sizeof(wchar_t), (length >> 1));
      str[length >> 1] = 0;
      string = str;
    }else{
      char *str = new char[length + 1];
      fin.read(str, length);
      str[length] = 0;
      string = str;
    }
  }

  void Naming_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    FREAD(fin, &format);
    FREAD(fin, &count);
    FREAD(fin, &offset);
    name_records = new Name_Record[count];
    for(int i = 0; i < count; ++i){   // Load each name record sequentially.
      name_records[i].load_record(fin);      
    }
    streampos mark = fin.tellg();     // start of string storage area
    for(int i = 0; i < count; ++i){   // Read each corresponding strings from storage area;
      name_records[i].read_string(fin);
      fin.seekg(mark);
    }
  }

  void Naming_Table::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<name>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format);
    INDENT(fp, indent); fprintf(fp, "<count value=\"%u\"/>\n", count);
    INDENT(fp, indent); fprintf(fp, "<offset value=\"%u\"/>\n", offset);
    for(int i = 0; i < count; ++i){
      name_records[i].dump_info(fp, indent);
    }
    --indent;
    INDENT(fp, indent); fprintf(fp, "</name>\n");
  }

  void Name_Record::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp,
      "<nameRecord platformID=\"%u\" encodingID=\"%u\" languageID=\"%u\" nameID=\"%u\" length=\"%u\" offset=\"%u\">\n",
      platform_id, encoding_id, language_id, name_id, length, offset);
    INDENT(fp, indent + 1); 
    if(double_byte_string()){
      fwprintf(fp, L"%s\n", (wchar_t*)string);
    }else{
      fprintf(fp, "%s\n", (char*)string);
    }
    INDENT(fp, indent); fprintf(fp, "</nameRecord>\n");
  }
}