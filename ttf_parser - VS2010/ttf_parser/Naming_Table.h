#ifndef NAMING_TABLE_H
#define NAMING_TABLE_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                         name - Naming Table                              */
/* Spec: https://www.microsoft.com/typography/otspec/name.htm               */
/****************************************************************************/
namespace ttf_dll {

enum PlatformID {
  kUnicode          = 0,
  kMacintosh        = 1,
  kISO              = 2,
  kWindows          = 3,
  kCustom           = 4
};

enum WindowsEncodingID {
  kSymbol           = 0,
  kUnicodeBMP       = 1,
  kShiftJIS         = 2,
  kPRC              = 3,
  kBig5             = 4,
  kWansung          = 5,
  kJohab            = 6,
  // Bits 7-9 are reserved.
  kUnicodeUCS4      = 10
};

// Each string in the string storage is referenced by a name record.
struct DLL_API NameRecord {
  ~NameRecord() {
    if(DoubleByteString()) {
      DEL_T(string_, wchar_t);
    } else {
      DEL_T(string_, char);
    }
  }
  // Reads the Name Record from the file stream.
  void LoadRecord(ifstream &fin);
  // Returns true if this is a double-byte string.
  bool DoubleByteString() const {
    return
      platform_id_ == kUnicode ||
      platform_id_ == kWindows &&
      encoding_id_ == kUnicodeBMP;
  }
  // Dumps out the information of this Name Record to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  // Reads the string data to the `string` field.
  void ReadString(ifstream &fin);

  // Platform ID.
  UShort  platform_id_;
  // Platform-specific encoding ID.
  UShort  encoding_id_;
  // Language ID.
  UShort  language_id_;
  // Name ID.
  UShort  name_id_;
  // String length (in bytes).
  UShort  length_;
  // String offset from start of storage area (in bytes).
  UShort  offset_;

  // Custom field: the corresponding string.
  void    *string_;
};

// FIXME: There are two versions of naming table! The format 1 is not yet implemented.
// The naming table allows multilingual strings to be associated with the
// OpenType font file. 
class DLL_API Naming_Table {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(TableRecordEntry *entry, ifstream &fin);
  // Deallocates the memory allocated in `LoadTable`, if any.
  void Destroy() {
    DEL_A(name_records_);
  }
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;

  // Format Selector.
  UShort        format_;
  // Number of name records.
  UShort        count_;
  // Offset to start of string storage (from start of table).
  UShort        offset_;
  // The name records where `count` is the number of records.
  NameRecord   *name_records_/*[count]*/;
};


// If a Name Record has a language ID equal to or greater than 0x8000, it is
// associated with a language-tag record that references a language-tag string.
// In this way, the language ID is associated with a language-tag string that
// specifies the language for name records using that language ID, regardless
// of the platform. These can be used for any platform that supports this
// language-tag mechanism.
struct LanguageTagRecord {
  // Language-tag string length (in bytes)
  UShort  length_;
  // Language-tag string offset from start of storage area (in bytes).
  UShort  offset_;
};

} // namespace ttf_dll

#endif