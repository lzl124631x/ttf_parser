#ifndef NAMING_TABLE_H
#define NAMING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/****************************************************************************/
/*                         name - Naming Table                              */
/* Spec: https://www.microsoft.com/typography/otspec/name.htm               */
/****************************************************************************/
namespace ttf_dll {

enum Platform_ID {
  Unicode         = 0,
  Macintosh       = 1,
  ISO             = 2,
  Windows         = 3,
  Custom          = 4
};

enum Windows_Encoding_ID {
  Symbol          = 0,
  Unicode_BMP     = 1,
  ShiftJIS        = 2,
  PRC             = 3,
  Big5            = 4,
  Wansung         = 5,
  Johab           = 6,
  // 7-9 are reserved.
  Unicode_UCS_4   = 10
};

// Each string in the string storage is referenced by a name record.
struct DLL_API Name_Record {
  ~Name_Record() {
    if(double_byte_string()) {
      DEL_T(string, wchar_t);
    } else {
      DEL_T(string, char);
    }
  }
  // Reads the Name Record from the file stream.
  void load_record(ifstream &fin);
  // Returns true if this is a double-byte string.
  bool double_byte_string() const {
    return
      platform_id == Unicode ||
      platform_id == Windows &&
      encoding_id == Unicode_BMP;
  }
  // Dumps out the information of this Name Record to an XML file.
  void dump_info(Xml_Logger &logger) const;
  // Reads the string data to the `string` field.
  void read_string(ifstream &fin);

  // Platform ID.
  USHORT  platform_id;
  // Platform-specific encoding ID.
  USHORT  encoding_id;
  // Language ID.
  USHORT  language_id;
  // Name ID.
  USHORT  name_id;
  // String length (in bytes).
  USHORT  length;
  // String offset from start of storage area (in bytes).
  USHORT  offset;

  // Custom field: the corresponding string.
  void    *string;
};

// FIXME: There are two versions of naming table! The format 1 is not yet implemented.
// The naming table allows multilingual strings to be associated with the
// OpenType font file. 
class DLL_API Naming_Table {
 public:
  // Deallocates the Name Records on destruction.
  ~Naming_Table() {
    DEL_A(name_records);
  }
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;

  // Format Selector.
  USHORT        format;
  // Number of name records.
  USHORT        count;
  // Offset to start of string storage (from start of table).
  USHORT        offset;
  // The name records where `count` is the number of records.
  Name_Record   *name_records/*[count]*/;
};


// If a Name Record has a language ID equal to or greater than 0x8000, it is
// associated with a language-tag record that references a language-tag string.
// In this way, the language ID is associated with a language-tag string that
// specifies the language for name records using that language ID, regardless
// of the platform. These can be used for any platform that supports this
// language-tag mechanism.
struct Language_Tag_Record {
  // Language-tag string length (in bytes)
  USHORT  length;
  // Language-tag string offset from start of storage area (in bytes).
  USHORT  offset;
};

} // namespace ttf_dll

#endif