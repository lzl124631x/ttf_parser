#ifndef TTF_TABLE_H
#define TTF_TABLE_H

#include "type.h"

namespace ttf_dll {

// The Offset Table is followed immediately by the Table Record entries.
// Entries in the Table Record must be sorted in ascending order by tag.
// Offset values in the Table Record are measured from the start of the font
// file.
class DLL_API TableRecordEntry {
 public:
  // Reads the Table Record from the file stream.
  void LoadEntry(std::ifstream &fin);
  // Accessors
  ULong tag() const { return tag_; }
  ULong checksum() const { return checksum_; }
  ULong offset() const { return offset_; }
  ULong length() const { return length_; }

 private:
  // 4 -byte identifier.
  ULong  tag_;
  // CheckSum for this table.
  ULong  checksum_;
  // Offset from beginning of TrueType font file
  ULong  offset_;
  // Length of this table.
  ULong  length_;
};

// The TrueType font starts with Offset Table.
class DLL_API OffsetTable {
 public:
  // Reads the Offset Table from the file stream.
  void LoadTable(std::ifstream &fin);
  // Deallocates the Table Record entries.
  void Destroy() {
    DEL_A(table_record_entries_);
  }
  // Gets the Table Record entry whose tag is specified by `tag_str`.
  TableRecordEntry *GetTableEntry(const char *tag_str) const;
  // Dumps out the information of this Offset Table to an XML file.
  void DumpInfo(XmlLogger &logger) const;

 private:
  // 0x00010000 for version 1.0.
  Fixed   sfnt_version_;
  // Number of tables.
  UShort  num_tables_;
  // (Maximum power of 2 <= numTables) x 16.
  UShort  search_range_;
  // Log2(maximum power of 2 <= numTables).
  UShort  entry_selector_;
  // NumTables x 16-searchRange.
  UShort  range_shift_;

  // An array of Table Record entries.
  TableRecordEntry *table_record_entries_;
};

class TrueTypeFont;
// The base class of subtables of TrueType font.
class DLL_API TtfSubtable {
 public:
  // Binds this subtable with the TrueType font on construction.
  explicit TtfSubtable(const TrueTypeFont &ttf) : ttf_(ttf) {};
  // Reads this subtable from the file stream. The `entry` provides some
  // information needed for loading.
  virtual void Init(const TableRecordEntry *entry,
                    std::ifstream &fin) = 0;
  // Deallocates the memory allocated in `Init`, if any.
  virtual void Destroy() {};
  // Dumps the information of this subtable to an XML file.
  virtual void DumpInfo(XmlLogger &logger) const = 0;

 protected:
  const TrueTypeFont &ttf_;
};
/************************************************************************/
/*                           OPTIONAL TABLES                            */
/************************************************************************/
/******************************* cvt ***********************************/
class ControlValueTable {
  FWord  list_of_values_/*[n]*/;
};

/******************************* EBDT ***********************************/
class EmbeddedBitmapDataTable {
  Fixed  version_;
};

/******************************* EBLC ***********************************/
class EmbeddedBitmapLocationTable {
  Fixed  version_;
  ULong  num_sizes_;
};

/******************************* EBSC ***********************************/

/******************************* fpgm ***********************************/
class FontProgram {
  Byte  instructions_/*[n]*/;
};
/******************************* gasp ***********************************/
//class Grid_Fitting_And_Scan_Conversion_Procedure{
//  USHORT  version;
//  USHORT  num_ranges;
//  struct  GASRANGE{
//    USHORT  range_max_ppem;
//    USHORT  range_gasp_behavior;
//  };
//  GASPRANGE  gasp_range[num_ranges];
//};

/******************************* hdmx ***********************************/

/******************************* kern ***********************************/
class Kerning {
  UShort  version_;
  UShort  n_tables_;
};

class KerningSubtable {
  UShort  version_;
  UShort  length_;
  UShort  coverage_;
};

/******************************* LTSH ***********************************/

/******************************* prep ***********************************/
class ControlValueProgram {
  Byte  set_of_instructions_/*[n]*/;
};

/******************************* PCLT ***********************************/

/******************************* VDMX ***********************************/

/******************************* vhea ***********************************/

/******************************* vmtx ***********************************/
} // namespace ttf_dll

#endif