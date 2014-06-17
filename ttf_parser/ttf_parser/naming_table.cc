#include "stdafx.h"
#include "naming_table.h"
using namespace std;

namespace ttf_dll {

Naming_Table::Naming_Table(const TrueTypeFont &ttf)
    : TtfSubtable(ttf),
      format_(0),
      count_(0),
      offset_(0),
      name_records_(NULL) {}

void NameRecord::LoadRecord(ifstream &fin) {
  FREAD(fin, &platform_id_);
  FREAD(fin, &encoding_id_);
  FREAD(fin, &language_id_);
  FREAD(fin, &name_id_);
  FREAD(fin, &length_);
  FREAD(fin, &offset_);
}

void NameRecord::ReadString(ifstream &fin) {
  // Each time this function is invoked, the file pointer is pointing to the start of string storage area.
  fin.seekg(offset_, ios::cur);
  if(DoubleByteString()) {
    wchar_t *str = new wchar_t[(length_ >> 1) + 1];
    IFStreamReadBigEndian(fin, str, sizeof(wchar_t), (length_ >> 1));
    str[length_ >> 1] = 0;
    string_ = str;
  } else {
    char *str = new char[length_ + 1];
    fin.read(str, length_);
    str[length_] = 0;
    string_ = str;
  }
}

void Naming_Table::Init(const TableRecordEntry *entry, ifstream &fin) {
  fin.seekg(entry->offset(), ios::beg);
  FREAD(fin, &format_);
  FREAD(fin, &count_);
  FREAD(fin, &offset_);
  name_records_ = new NameRecord[count_];
  for(int i = 0; i < count_; ++i) {  // Load each name record sequentially.
    name_records_[i].LoadRecord(fin);
  }
  streampos mark = fin.tellg();     // start of string storage area
  for(int i = 0; i < count_; ++i) {  // Read each corresponding strings from storage area
    name_records_[i].ReadString(fin);
    fin.seekg(mark);
  }
}

void Naming_Table::DumpInfo(XmlLogger &logger) const {
  logger.Println("<name>");
  logger.IncreaseIndent();
  logger.Println("<format value=\"%u\"/>", format_);
  logger.Println("<count value=\"%u\"/>", count_);
  logger.Println("<offset value=\"%u\"/>", offset_);
  for(int i = 0; i < count_; ++i) {
    name_records_[i].DumpInfo(logger);
  }
  logger.DecreaseIndent();
  logger.Println("</name>");
}

void NameRecord::DumpInfo(XmlLogger &logger) const {
  logger.Println(
      "<nameRecord platformID=\"%u\" encodingID=\"%u\" languageID=\"%u\" "
      "nameID=\"%u\" length=\"%u\" offset=\"%u\">",
      platform_id_, encoding_id_, language_id_,
      name_id_, length_, offset_);
  logger.IncreaseIndent();
  if(DoubleByteString()) {
    logger.WPrintln(L"%s", (wchar_t*)string_);
  } else {
    logger.Println("%s", (char*)string_);
  }
  logger.DecreaseIndent();
  logger.Println("</nameRecord>");
}

} // namespace ttf_dll