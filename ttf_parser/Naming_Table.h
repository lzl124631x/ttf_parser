#ifndef NAMING_TABLE_H
#define NAMING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* name ***********************************/
// https://www.microsoft.com/typography/otspec/name.htm
namespace ttf_dll{
  enum Platform_ID{
    Unicode         = 0,
    Macintosh       = 1,
    ISO             = 2,
    Windows         = 3,
    Custom          = 4
  };

  enum Windows_Encoding_ID{
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

  struct Name_Record{
      USHORT  platform_id;  // Platform ID.
      USHORT  encoding_id;  // Platform-specific encoding ID.
      USHORT  language_id;  // Language ID.
      USHORT  name_id;      // Name ID.
      USHORT  length;       // String length (in bytes).
      USHORT  offset;       // String offset from start of storage area (in bytes).
      void    *string;      // Custom field: corresponding string.
      bool double_byte_string(){
        return
        platform_id == Unicode ||
        platform_id == Windows &&
        encoding_id == Unicode_BMP;
      }
      void dump_info(FILE *fp, size_t indent);
      void load_record(ifstream &fin);
      void read_string(ifstream &fin);
      ~Name_Record(){
        if(double_byte_string()){
          DEL_T(string, wchar_t);
        }else{
          DEL_T(string, char);
        }
      }
  };
  
  // FIXME: There are two versions of naming table! The format 1 is not yet implemented.
  class DLL_API Naming_Table{
  public:
    USHORT        format;         // Format Selector.
    USHORT        count;          // Number of name records.
    USHORT        offset;         // Offset to start of string storage (from start of table).
    Name_Record   *name_records;  // The name records where count is the number of records.
    void load_table(Table_Directory_Entry *entry, ifstream &fin);
    ~Naming_Table(){
      DEL_A(name_records);
    }
    void dump_info(FILE *fp, size_t indent);
  };

  struct Language_Tag_Record{
    USHORT  length;        // Language-tag string length (in bytes)
    USHORT  offset;        // Language-tag string offset from start of storage area (in bytes).
  };
}

#endif