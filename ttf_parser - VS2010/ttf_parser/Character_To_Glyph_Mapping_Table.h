#ifndef CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#define CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                cmap - Character To Glyph Index Mapping                   */
/* Spec: https://www.microsoft.com/typography/otspec/cmap.htm               */
/****************************************************************************/
namespace ttf_dll {
class DLL_API EncodingTable {
 public:
  EncodingTable(ifstream &fin);
  virtual ~EncodingTable() {}
  virtual GlyphID GetGlyphIndex(UShort ch) = 0;
  void DumpTableHeader(XmlLogger &logger) const;
  virtual void DumpInfo(XmlLogger &logger) const = 0;

  UShort  format_;
  UShort  length_;
  UShort  language_;
};

class DLL_API EncodingRecord {
 public:
  ~EncodingRecord() {
    DEL(encoding_table_);
  }
  void LoadEntry(ifstream &fin);
  void LoadEncodingTable(ifstream &fin, streampos base);

  UShort              platform_id_;
  UShort              encoding_id_;
  // Byte offset from beginning of table to the subtable for this encoding.
  ULong               byte_offset_;
  // Just a pointer to the corresponding encoding table; NOT an array.
  EncodingTable       *encoding_table_;
};

class DLL_API CharacterToGlyphIndexMappingTable {
 public:
  ~CharacterToGlyphIndexMappingTable() {
    DEL_A(encoding_records_);
  }
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(TableRecordEntry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  GlyphID GetGlyphIndex(UShort platform_id, UShort encoding_id, UShort ch);

 private:
  EncodingTable *GetEncodingTable(UShort platform_id, UShort encoding_id);

  UShort              table_version_number_;
  UShort              num_encoding_tables_;
  EncodingRecord      *encoding_records_;
};

/**************************** Encoding Tables *******************************/
enum EncodingTableFormat {
  kByteEncodingTable                        = 0,
  kHighByteMappingThroughTable              = 2,
  kSegmentMappingToDeltaValues              = 4,
  kTrimmedTableMapping                      = 6
};

class DLL_API ByteEncodingTable: public EncodingTable {
  // This is the Apple standard character to glyph index mapping table, a
  // simple 1 to 1 mapping of character codes to glyph indices. The glyph set
  // is limited to 256. Note that if this format is used to index into a
  // larger glyph set, only the first 256 glyphs will be accessible.
 public:
  ByteEncodingTable(ifstream &fin);
  ~ByteEncodingTable() { }
  GlyphID GetGlyphIndex(UShort ch);
  void DumpInfo(XmlLogger &logger) const;

  Byte  glyph_id_array_[256];
};

class DLL_API HighByteMappingThroughTable: public EncodingTable {
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
  HighByteMappingThroughTable(ifstream &fin);
  ~HighByteMappingThroughTable() {
    DEL_A(subheaders_);
  }
  GlyphID GetGlyphIndex(UShort ch);
  void DumpInfo(XmlLogger &logger) const;

  UShort    subheader_keys_[256];
  struct Subheader {
    UShort  first_code;
    UShort  entry_count;
    Short   id_delta;
    UShort  id_range_offset;
  };
  Subheader  *subheaders_;
  UShort    glyph_id_array_;
};

class DLL_API SegmentMappingToDeltaValues: public EncodingTable {
 public:
  SegmentMappingToDeltaValues(ifstream &fin);
  ~SegmentMappingToDeltaValues() {
    DEL_A(end_count_);
    DEL_A(start_count_);
    DEL_A(id_delta_);
    DEL_A(id_range_offset_);
    DEL_A(glyph_id_array_);
  }
  GlyphID GetGlyphIndex(UShort ch);
  void DumpInfo(XmlLogger &logger) const;

  UShort  seg_countx2_;
  UShort  search_range_;
  UShort  entry_selector_;
  UShort  range_shift_;
  UShort  *end_count_/*[seg_count]*/;
  UShort  reserved_pad_;
  UShort  *start_count_/*[seg_count]*/;
  Short   *id_delta_/*[seg_count]*/;
  UShort  *id_range_offset_/*[seg_count]*/;
  UShort  *glyph_id_array_/*[var_len]*/;
};

class DLL_API TrimmedTableMapping: public EncodingTable {
  // The firstCode and entryCount values specify a subrange (beginning at
  // firstCode, length = entryCount) within the range of possible character
  // codes. Codes outside of this subrange are mapped to glyph index 0. The
  // offset of the code (from the first code) within this subrange is used as
  // index to the glyphIdArray, which provides the glyph index value.
 public:
  TrimmedTableMapping(ifstream &fin);
  ~TrimmedTableMapping() {
    DEL_A(glyph_id_array_);
  }
  GlyphID GetGlyphIndex(UShort ch);
  void DumpInfo(XmlLogger &logger) const;

  UShort  first_code_;
  UShort  entry_count_;
  UShort  *glyph_id_array_/*[entry_count]*/;
};

} // namespace ttf_dll

#endif