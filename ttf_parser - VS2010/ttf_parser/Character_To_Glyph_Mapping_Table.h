#ifndef CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#define CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                cmap - Character To Glyph Index Mapping                   */
/* Spec: https://www.microsoft.com/typography/otspec/cmap.htm               */
/****************************************************************************/
namespace ttf_dll {

// Header of 'cmap' subtables.
class DLL_API EncodingTable {
 public:
  explicit EncodingTable(ifstream &fin);
  virtual ~EncodingTable() { }
  // Gets the glyph index of the character specified by `ch`.
  virtual GlyphId GetGlyphIndex(const UShort ch) const = 0;
  // Dumps the information of this table header to an XML file.
  void DumpTableHeader(XmlLogger &logger) const;
  // Dumps the information of a specific format 'cmap' subtable to an XML file.
  virtual void DumpInfo(XmlLogger &logger) const = 0;

 protected:
  // Format number.
  UShort  format_;
  // This is the length in bytes of the subtable.
  UShort  length_;
  // Note on the language field in 'cmap' subtables: The language field must
  // be set to zero for all cmap subtables whose platform IDs are other than
  // Macintosh (platform ID 1). For cmap subtables whose platform IDs are
  // Macintosh, set this field to the Macintosh language ID of the cmap
  // subtable plus one, or to zero if the cmap subtable is not language-
  // specific. For example, a Mac OS Turkish cmap subtable must set this field
  // to 18, since the Macintosh language ID for Turkish is 17. A Mac OS Roman
  // cmap subtable must set this field to 0, since Mac OS Roman is not a
  // language-specific encoding.
  UShort  language_;
};

// The 'cmap' table header is followed by an array of encoding records that
// specify the particular encoding and the offset to the subtable for that
// encoding. The number of encoding records is `numTables`.
class DLL_API EncodingRecord {
 public:
  ~EncodingRecord() {
    DEL(encoding_table_);
  }
  // Reads the Encoding Record from file stream.
  void LoadRecord(ifstream &fin);
  // Dumps the information of this record to an XML file.
  void DumpRecord(XmlLogger &logger) const;
  // Reads the Encoding Table corresponding to this Encoding Record from file
  // stream. The location of Encoding Table is determined by the argument
  // `base`, which specifies the beginning of 'cmap' table, and the `offset_`
  // field of Encoding Record.
  void LoadEncodingTable(ifstream &fin, const streampos base);
  // Accessors
  UShort platform_id() const { return platform_id_; };
  UShort encoding_id() const { return encoding_id_; };
  EncodingTable *encoding_table() const { return encoding_table_; };

 private:
  // Look up 'name' table for details of Platform IDs, Platform-specific
  // encoding IDs and Language IDs.

  // Platform ID.
  UShort              platform_id_;
  // Platform-specific encoding ID.
  UShort              encoding_id_;
  // Byte offset from beginning of 'cmap' table to the subtable for this
  // encoding.
  ULong               offset_;

  // A pointer to the corresponding encoding table; NOT an array.
  EncodingTable       *encoding_table_;
};

// This table defines the mapping of character codes to the glyph index
// values used in the font.
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
  // Gets the glyph index of the character specified by `ch`. The arguments
  // `platform_id` and `encoding_id` specify which Encoding Table should be
  // used.
  GlyphId GetGlyphIndex(const UShort platform_id,
                        const UShort encoding_id,
                        const UShort ch) const;

 private:
  // Gets the Encoding Table indexed by `platform_id` and `encoding_id`.
  EncodingTable *GetEncodingTable(const UShort platform_id,
                                  const UShort encoding_id) const;

  // Table version number (0).
  UShort              version_;
  // Number of encoding tables that follow.
  UShort              num_tables_;
  // An array of Encoding Records.
  EncodingRecord      *encoding_records_;
};

/****************************************************************************/
/*                             Encoding Tables                              */
/****************************************************************************/
enum EncodingTableFormat {
  kByteEncodingTable                        = 0,
  kHighByteMappingThroughTable              = 2,
  kSegmentMappingToDeltaValues              = 4,
  kTrimmedTableMapping                      = 6,
  // Supporting 4-byte character codes
  // FIXME: The following formats are not supported yet.
  kMixed16BitAnd32BitCoverage               = 8,
  kTrimmedArray                             = 10,
  kSegmentedCoverage                        = 12,
  kManyToOneRangeMappings                   = 13,
  kUnicodeVariationSequences                = 14
};

/****************************************************************************/
/*                     Format 0: Byte Encoding Table                        */
/* ------------------------------------------------------------------------ */
/* This is the Apple standard character to glyph index mapping table, a     */
/* simple 1 to 1 mapping of character codes to glyph indices. The glyph set */
/* is limited to 256. Note that if this format is used to index into a      */
/* larger glyph set, only the first 256 glyphs will be accessible.          */
/****************************************************************************/
class DLL_API ByteEncodingTable: public EncodingTable {
 public:
  explicit ByteEncodingTable(ifstream &fin);
  ~ByteEncodingTable() { }
  // Gets the glyph index of the character specified by `ch`. Glyph ID 0 will
  // be returned if `ch` is greater than or equal to 256.
  GlyphId GetGlyphIndex(const UShort ch) const;
  // Dumps the information of this subtable to an XML file.
  void DumpInfo(XmlLogger &logger) const;

 private:
  // An array that maps character codes to glyph index values.
  Byte  glyph_id_array_[256];
};

/****************************************************************************/
/*              Format 2: High-byte Mapping Through Table                   */
/* ------------------------------------------------------------------------ */
/* This subtable is useful for the national character code standards used   */
/* for Japanese, Chinese, and Korean characters. These code standards use a */
/* mixed 8/16-bit encoding, in which certain byte values signal the first   */
/* byte of a 2-byte character (but these values are also legal as the       */
/* second byte of a 2-byte character). In addition, even for the 2-byte     */
/* characters, the mapping of character codes to glyph index values depends */
/* heavily on the first byte. Consequently, the table begins with an array  */
/* that maps the first byte to a 4-word subHeader. For 2-byte character     */
/* codes, the subHeader is used to map the second byte's value through a    */
/* subArray, as described below. When processing mixed 8/16-bit text,       */
/* subHeader 0 is special: it is used for single-byte character codes. When */
/* subHeader zero is used, a second byte is not needed; the single byte     */
/* value is mapped through the subArray.                                    */
/****************************************************************************/
class DLL_API HighByteMappingThroughTable: public EncodingTable {
 public:
  explicit HighByteMappingThroughTable(ifstream &fin);
  ~HighByteMappingThroughTable() {
    DEL_A(subheaders_and_glyph_id_array);
  }
  // Gets the glyph index of the character specified by `ch`.
  GlyphId GetGlyphIndex(const UShort ch) const;
  // Dumps the information of this subtable to an XML file.
  void DumpInfo(XmlLogger &logger) const;

 private:
  // Array that maps high bytes to subHeaders: value is subHeader index * 8.
  UShort    subheader_keys_[256];
  struct Subheader {
    // First valid low byte for this subHeader.
    UShort  first_code;
    // Number of valid low bytes for this subHeader.
    UShort  entry_count;
    Short   id_delta;
    UShort  id_range_offset;
    // The `firstCode` and `entryCount` values specify a subrange that begins
    // at `firstCode` and has a length equal to the value of `entryCount`.
    // This subrange stays within the 0-255 range of the byte being mapped.
    // Bytes outside of this subrange are mapped to glyph index 0 (missing
    // glyph). The offset of the byte within this subrange is then used as
    // index into a corresponding subarray of glyphIndexArray. This subarray
    // is also of length `entryCount`. The value of the `idRangeOffset` is the
    // number of bytes past the actual location of the `idRangeOffset` word
    // where the glyphIndexArray element corresponding to `firstCode` appears.
    //
    // Finally, if the value obtained from the subarray is not 0 (which
    // indicates the missing glyph), you should add `idDelta` to it in order
    // to get the glyphIndex. The value `idDelta` permits the same subarray
    // to be used for several different subheaders. The `idDelta` arithmetic
    // is modulo 65536.
  };
  // subHeaders[]: Variable-length array of subHeader structures.
  // glyphIndexArray[]: Variable-length array containing subarrays used for
  // mapping the low byte of 2-byte characters.
  UShort    *subheaders_and_glyph_id_array;
};

/****************************************************************************/
/*              Format 4: Segment Mapping To Delta Values                   */
/* ------------------------------------------------------------------------ */
/* This is the Microsoft standard character to glyph index mapping table    */
/* for fonts that support Unicode ranges other than the range [U+D800 -     */
/* U+DFFF] (defined as Surrogates Area, in Unicode v 3.0) which is used for */
/* UCS-4 characters. If a font supports this character range (i.e. in turn  */
/* supports the UCS-4 characters) a subtable in this format with a platform */
/* specific encoding ID 1 is yet needed, in addition to a subtable in       */
/* format 12 with a platform specific encoding ID 10.                       */
/*                                                                          */
/* This format is used when the character codes for the characters          */
/* represented by a font fall into several contiguous ranges, possibly with */
/* holes in some or all of the ranges (that is, some of the codes in a      */
/* range may not have a representation in the font). The format-dependent   */
/* data is divided into three parts, which must occur in the following      */
/* order:                                                                   */
/* 1. A four-word header gives parameters for an optimized search of the    */
/* segment list;                                                            */
/* 2. Four parallel arrays describe the segments (one segment for each      */
/* contiguous range of codes);                                              */
/* 3. A variable-length array of glyph IDs (unsigned words).                */
/****************************************************************************/
class DLL_API SegmentMappingToDeltaValues: public EncodingTable {
 public:
  explicit SegmentMappingToDeltaValues(ifstream &fin);
  ~SegmentMappingToDeltaValues() {
    DEL_A(end_count_);
    DEL_A(start_count_);
    DEL_A(id_delta_);
    DEL_A(id_range_offset_and_glyph_id_array_);
  }
  // Gets the glyph index of the character specified by `ch`.
  GlyphId GetGlyphIndex(const UShort ch) const;
  // Dumps the information of this subtable to an XML file.
  void DumpInfo(XmlLogger &logger) const;

 private:
  // 2 x segCount.
  UShort  seg_countx2_;
  // 2 x (2**floor(log2(segCount))).
  UShort  search_range_;
  // log2(searchRange/2).
  UShort  entry_selector_;
  // 2 x segCount - searchRange.
  UShort  range_shift_;
  // End characterCode for each segment, last=0xFFFF.
  UShort  *end_count_/*[segCount]*/;
  // Set to 0.
  UShort  reserved_pad_;
  // Start character code for each segment.
  UShort  *start_count_/*[segCount]*/;
  // Delta for all character codes in segment.
  Short   *id_delta_/*[segCount]*/;
  // idRangeOffset[segCount]: Offsets (in byte) into `glyphIdArray` or 0.
  // glyphIdArray[]: Glyph index array (arbitrary length).
  UShort  *id_range_offset_and_glyph_id_array_/*[segCount + varLength]*/;

  // NOTE: It's easier to calculate the glyph index if the `glyphIndexArray`
  // directly follows `idRangeOffset`.

  // The length of glyphIdArray.
  UShort  glyph_id_array_len;
};

/****************************************************************************/
/*                    Format 6: Trimmed Table Mapping                       */
/* ------------------------------------------------------------------------ */
/* The firstCode and entryCount values specify a subrange (beginning at     */
/* firstCode, length = entryCount) within the range of possible character   */
/* codes. Codes outside of this subrange are mapped to glyph index 0. The   */
/* offset of the code (from the first code) within this subrange is used as */
/* index to the glyphIdArray, which provides the glyph index value.         */
/****************************************************************************/
class DLL_API TrimmedTableMapping: public EncodingTable {
 public:
  explicit TrimmedTableMapping(ifstream &fin);
  ~TrimmedTableMapping() {
    DEL_A(glyph_id_array_);
  }
  // Gets the glyph index of the character specified by `ch`.
  GlyphId GetGlyphIndex(const UShort ch) const;
  // Dumps the information of this subtable to an XML file.
  void DumpInfo(XmlLogger &logger) const;

 private:
  // First character code of subrange.
  UShort  first_code_;
  // Number of character codes in subrange.
  UShort  entry_count_;
  // Array of glyph index values for character codes in the range.
  UShort  *glyph_id_array_/*[entryCount]*/;
};

} // namespace ttf_dll

#endif