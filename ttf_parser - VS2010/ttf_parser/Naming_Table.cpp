#include "stdafx.h"
#include "Naming_Table.h"

namespace ttf_dll {

void Name_Record::load_record(ifstream &fin) {
  FREAD(fin, &platform_id);
  FREAD(fin, &encoding_id);
  FREAD(fin, &language_id);
  FREAD(fin, &name_id);
  FREAD(fin, &length);
  FREAD(fin, &offset);
}

void Name_Record::read_string(ifstream &fin) {
  // Each time this function is invoked, the file pointer is pointing to the start of string storage area.
  fin.seekg(offset, ios::cur);
  if(double_byte_string()) {
    wchar_t *str = new wchar_t[(length >> 1) + 1];
    ifstream_read_big_endian(fin, str, sizeof(wchar_t), (length >> 1));
    str[length >> 1] = 0;
    string = str;
  } else {
    char *str = new char[length + 1];
    fin.read(str, length);
    str[length] = 0;
    string = str;
  }
}

void Naming_Table::load_table(Table_Record_Entry *entry, ifstream &fin) {
  fin.seekg(entry->offset, ios::beg);
  FREAD(fin, &format);
  FREAD(fin, &count);
  FREAD(fin, &offset);
  name_records = new Name_Record[count];
  for(int i = 0; i < count; ++i) {  // Load each name record sequentially.
    name_records[i].load_record(fin);
  }
  streampos mark = fin.tellg();     // start of string storage area
  for(int i = 0; i < count; ++i) {  // Read each corresponding strings from storage area;
    name_records[i].read_string(fin);
    fin.seekg(mark);
  }
}

void Naming_Table::dump_info(Xml_Logger &logger) const {
  logger.println("<name>");
  logger.inc_indent();
  logger.println("<format value=\"%u\"/>", format);
  logger.println("<count value=\"%u\"/>", count);
  logger.println("<offset value=\"%u\"/>", offset);
  for(int i = 0; i < count; ++i) {
    name_records[i].dump_info(logger);
  }
  logger.dec_indent();
  logger.println("</name>");
}

void Name_Record::dump_info(Xml_Logger &logger) const {
  logger.println(
      "<nameRecord platformID=\"%u\" encodingID=\"%u\" languageID=\"%u\" "
      "nameID=\"%u\" length=\"%u\" offset=\"%u\">",
      platform_id, encoding_id, language_id,
      name_id, length, offset);
  logger.inc_indent();
  if(double_byte_string()) {
    logger.wprintln(L"%s", (wchar_t*)string);
  } else {
    logger.println("%s", (char*)string);
  }
  logger.dec_indent();
  logger.println("</nameRecord>");
}

} // namespace ttf_dll