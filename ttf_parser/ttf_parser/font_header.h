#ifndef FONT_HEADER_H
#define FONT_HEADER_H

#include "type.h"
#include "ttf_table.h"
/******************************* head ***********************************/
// https://www.microsoft.com/typography/otspec/head.htm
namespace ttf_dll {
  
class DLL_API FontHeader {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(const TableRecordEntry *entry, std::ifstream &fin);
  // Deallocates the memory allocated in `LoadTable`, if any.
  void Destroy() {}
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  // Accessors
  FWord x_min() const { return x_min_; }
  FWord y_min() const { return y_min_; }
  FWord x_max() const { return x_max_; }
  FWord y_max() const { return y_max_; }
  Short loca_format() const { return loca_format_; }

 private:
  Fixed             table_version_number_;
  // Set by font manufacturer.
  Fixed             font_revision_;
  // To compute: set it to 0, sum the entire font as ULONG, then store
  // 0xB1B0AFBA - sum.
  ULong             checksum_adjustment_;
  // Set to 0x5F0F3CF5.
  ULong             magic_number_;
  UShort            flags_;
  // Valid range is from 16 to 16384. This value should be a power of 2 for
  // fonts that have TrueType outlines.
  UShort            units_per_em_;
  // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
  LongDateTime      created_;
  // As above.
  LongDateTime      modified_;
  // For all glyph bounding boxes.
  FWord             x_min_;
  FWord             y_min_;
  FWord             x_max_;
  FWord             y_max_;

  UShort            mac_style_;
  UShort            lowest_rec_ppem_;
  Short             font_direction_hint_;
  // This field specifies the version of the 'loca' table. 0 for short
  // offsets, 1 for long.
  Short             loca_format_;
  Short             glygh_data_format_;
};

} // namespace ttf_dll

#endif