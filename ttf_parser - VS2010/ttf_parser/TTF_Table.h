#ifndef TTF_TABLE_H
#define TTF_TABLE_H

#include "type.h"
using namespace std;

namespace ttf_dll {

// The Offset Table is followed immediately by the Table Record entries.
// Entries in the Table Record must be sorted in ascending order by tag.
// Offset values in the Table Record are measured from the start of the font
// file.
class DLL_API TableRecordEntry {
 public:
  // Reads the Table Record from the file stream.
  void LoadEntry(ifstream &fin);

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
  // Deallocates the Table Record entries during destruction.
  ~OffsetTable() {
    DEL_A(table_record_entries_);
  }
  // Reads the Offset Table from the file stream.
  void LoadTable(ifstream &fin);
  // Gets the Table Record entry whose tag is specified by `tag_str`.
  TableRecordEntry *GetTableEntry(const char *tag_str) const;
  // Dumps out the information of this Offset Table to an XML file.
  void DumpInfo(XmlLogger &logger) const;

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