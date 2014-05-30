#ifndef FONT_HEADER_H
#define FONT_HEADER_H

#include "Type.h"
#include "TTF_Table.h"
/******************************* head ***********************************/
// https://www.microsoft.com/typography/otspec/head.htm
namespace ttf_dll {
  
class DLL_API Font_Header {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  
  FIXED             table_version_number;
  // Set by font manufacturer.
  FIXED             font_revision;
  // To compute: set it to 0, sum the entire font as ULONG, then store
  // 0xB1B0AFBA - sum.
  ULONG             checksum_adjustment;
  // Set to 0x5F0F3CF5.
  ULONG             magic_number;
  USHORT            flags;
  // Valid range is from 16 to 16384. This value should be a power of 2 for
  // fonts that have TrueType outlines.
  USHORT            units_per_em;
  // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
  LONG_DATE_TIME    created;
  // As above.
  LONG_DATE_TIME    modified;
  // For all glyph bounding boxes.
  FWORD             x_min;
  FWORD             y_min;
  FWORD             x_max;
  FWORD             y_max;

  USHORT            mac_style;
  USHORT            lowest_rec_ppem;
  SHORT             font_direction_hint;
  // This field specifies the version of the 'loca' table. 0 for short
  // offsets, 1 for long.
  SHORT             loca_format;
  SHORT             glygh_data_format;
};

} // namespace ttf_dll

#endif