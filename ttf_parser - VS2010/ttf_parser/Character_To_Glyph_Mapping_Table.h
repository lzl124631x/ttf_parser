#ifndef CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#define CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/****************************************************************************/
/*                cmap - Character To Glyph Index Mapping                   */
/* Spec: https://www.microsoft.com/typography/otspec/cmap.htm               */
/****************************************************************************/
namespace ttf_dll {
class DLL_API Encoding_Table {
 public:
  Encoding_Table(ifstream &fin);
  virtual ~Encoding_Table() {}
  virtual GLYPH_ID get_glyph_index(USHORT ch) = 0;
  void dump_table_header(Xml_Logger &logger) const;
  virtual void dump_info(Xml_Logger &logger) const = 0;

  USHORT  format;
  USHORT  length;
  USHORT  language;
};

class DLL_API Encoding_Record {
 public:
  Encoding_Record() { }
  void load_entry(ifstream &fin);
  void load_encoding_table(ifstream &fin, streampos base);
  ~Encoding_Record() {
    DEL(encoding_table);
  }

  USHORT            platform_id;
  USHORT            encoding_id;
  // Byte offset from beginning of table to the subtable for this encoding.
  ULONG             byte_offset;
  // Just a pointer to the corresponding encoding table; NOT an array.
  Encoding_Table    *encoding_table;
};

class DLL_API Character_To_Glyph_Index_Mapping_Table {
 public:
  ~Character_To_Glyph_Index_Mapping_Table() {
    DEL_A(encoding_records);
  }
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  GLYPH_ID get_glyph_index(USHORT platform_id, USHORT encoding_id, USHORT ch);

 private:
  Encoding_Table *get_encoding_table(USHORT platform_id, USHORT encoding_id);

  USHORT  table_version_number;
  USHORT  num_encoding_tables;
  Encoding_Record *encoding_records;
};

/**************************** Encoding Tables *******************************/
enum Encoding_Table_Format {
  BYTE_ENCODING_TABLE                       = 0,
  HIGH_BYTE_MAPPING_THROUGH_TABLE           = 2,
  SEGMENT_MAPPING_TO_DELTA_VALUES           = 4,
  TRIMMED_TABLE_MAPPING                     = 6
};

class DLL_API Byte_Encoding_Table: public Encoding_Table {
  // This is the Apple standard character to glyph index mapping table, a
  // simple 1 to 1 mapping of character codes to glyph indices. The glyph set
  // is limited to 256. Note that if this format is used to index into a
  // larger glyph set, only the first 256 glyphs will be accessible.
 public:
  Byte_Encoding_Table(ifstream &fin);
  ~Byte_Encoding_Table() { }
  GLYPH_ID get_glyph_index(USHORT ch);
  void dump_info(Xml_Logger &logger) const;

  BYTE  glyph_id_array[256];
};

class DLL_API High_Byte_Mapping_Through_Table: public Encoding_Table {
  // This subtable is useful for the national character code standards used
  // for Japanese, Chinese, and Korean characters. These code standards use a
  // mixed 8/16-bit encoding, in which certain byte values signal the first
  // byte of a 2-byte character (but these values are also legal as the second
  // byte of a 2-byte character). In addition, even for the 2-byte characters,
  // the mapping of character codes to glyph index values depends heavily on
  // the first byte. Consequently, the table begins with an array that maps
  // the first byte to a 4-word subHeader. For 2-byte character codes, the
  // subHeader is used to map the second byte's value through a subArray, as
  // described below. When processing mixed 8/16-bit text, subHeader 0 is
  // special: it is used for single-byte character codes. When subHeader zero
  // is used, a second byte is not needed; the single byte value is mapped
  // through the subArray.
 public:
  High_Byte_Mapping_Through_Table(ifstream &fin);
  ~High_Byte_Mapping_Through_Table() {
    DEL_A(subheaders);
  }
  GLYPH_ID get_glyph_index(USHORT ch);
  void dump_info(Xml_Logger &logger) const;

  USHORT    subheader_keys[256];
  struct Subheader {
    USHORT  first_code;
    USHORT  entry_count;
    SHORT   id_delta;
    USHORT  id_range_offset;
  };
  Subheader  *subheaders;
  USHORT    glyph_id_array;
};

class DLL_API Segment_Mapping_To_Delta_Values: public Encoding_Table {
 public:
  Segment_Mapping_To_Delta_Values(ifstream &fin);
  ~Segment_Mapping_To_Delta_Values() {
    DEL_A(end_count);
    DEL_A(start_count);
    DEL_A(id_delta);
    DEL_A(id_range_offset);
    DEL_A(glyph_id_array);
  }
  GLYPH_ID get_glyph_index(USHORT ch);
  void dump_info(Xml_Logger &logger) const;

  USHORT  seg_countx2;
  USHORT  search_range;
  USHORT  entry_selector;
  USHORT  range_shift;
  USHORT  *end_count/*[seg_count]*/;
  USHORT  reserved_pad;
  USHORT  *start_count/*[seg_count]*/;
  SHORT   *id_delta/*[seg_count]*/;
  USHORT  *id_range_offset/*[seg_count]*/;
  USHORT  *glyph_id_array/*[var_len]*/;
};

class DLL_API Trimmed_Table_Mapping: public Encoding_Table {
  // The firstCode and entryCount values specify a subrange (beginning at
  // firstCode, length = entryCount) within the range of possible character
  // codes. Codes outside of this subrange are mapped to glyph index 0. The
  // offset of the code (from the first code) within this subrange is used as
  // index to the glyphIdArray, which provides the glyph index value.
 public:
  Trimmed_Table_Mapping(ifstream &fin);
  ~Trimmed_Table_Mapping() {
    DEL_A(glyph_id_array);
  }
  GLYPH_ID get_glyph_index(USHORT ch);
  void dump_info(Xml_Logger &logger) const;

  USHORT  first_code;
  USHORT  entry_count;
  USHORT  *glyph_id_array/*[entry_count]*/;
};

} // namespace ttf_dll

#endif