#ifndef TTF_TABLE_H
#define TTF_TABLE_H

#include "Type.h"
using namespace std;

namespace ttf_dll {

// The Offset Table is followed immediately by the Table Record entries.
// Entries in the Table Record must be sorted in ascending order by tag.
// Offset values in the Table Record are measured from the start of the font
// file.
class DLL_API Table_Record_Entry {
 public:
  // Reads the Table Record from the file stream.
  void load_entry(ifstream &fin);

  // 4 -byte identifier.
  ULONG  tag;
  // CheckSum for this table.
  ULONG  checksum;
  // Offset from beginning of TrueType font file
  ULONG  offset;
  // Length of this table.
  ULONG  length;
};

// The TrueType font starts with Offset Table.
class DLL_API Offset_Table {
 public:
  // Deallocates the Table Record entries during destruction.
  ~Offset_Table() {
    DEL_A(table_record_entries);
  }
  // Reads the Offset Table from the file stream.
  void load_table(ifstream &fin);
  // Gets the Table Record entry whose tag is specified by `tag_str`.
  Table_Record_Entry *get_table_entry(const char *tag_str) const;
  // Dumps out the information of this Offset Table to an XML file.
  void dump_info(Xml_Logger &logger) const;

  // 0x00010000 for version 1.0.
  FIXED   sfnt_version;
  // Number of tables.
  USHORT  num_tables;
  // (Maximum power of 2 <= numTables) x 16.
  USHORT  search_range;
  // Log2(maximum power of 2 <= numTables).
  USHORT  entry_selector;
  // NumTables x 16-searchRange.
  USHORT  range_shift;

  // An array of Table Record entries.
  Table_Record_Entry *table_record_entries;
};
/************************************************************************/
/*                           OPTIONAL TABLES                            */
/************************************************************************/
/******************************* cvt ***********************************/
class Control_Value_Table {
  FWORD  list_of_values/*[n]*/;
};

/******************************* EBDT ***********************************/
class Embedded_Bitmap_Data_Table {
  FIXED  version;
};

/******************************* EBLC ***********************************/
class Embedded_Bitmap_Location_Table {
  FIXED  version;
  ULONG  num_sizes;
};

/******************************* EBSC ***********************************/

/******************************* fpgm ***********************************/
class Font_Program {
  BYTE  instructions/*[n]*/;
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
  USHORT  version;
  USHORT  n_tables;
};
class Kerning_Subtable {
  USHORT  version;
  USHORT  length;
  USHORT  coverage;
};

/******************************* LTSH ***********************************/

/******************************* prep ***********************************/
class Control_Value_Program {
  BYTE  set_of_instructions/*[n]*/;
};

/******************************* PCLT ***********************************/

/******************************* VDMX ***********************************/

/******************************* vhea ***********************************/

/******************************* vmtx ***********************************/
} // namespace ttf_dll

#endif