#ifndef FONT_HEADER_H
#define FONT_HEADER_H

#include "Type.h"
#include "TTF_Table.h"
/******************************* head ***********************************/
// https://www.microsoft.com/typography/otspec/head.htm
namespace ttf_dll{
  class DLL_API Font_Header{
  public:
    FIXED             table_version_number;
    FIXED             font_revision;            // Set by font manufacturer.
    ULONG             checksum_adjustment;      // To compute: set it to 0, sum the entire font as ULONG, then store 0xB1B0AFBA - sum.
    ULONG             magic_number;             // Set to 0x5F0F3CF5.
    USHORT            flags;
    USHORT            units_per_em;             // Valid range is from 16 to 16384. This value should be a power of 2 for fonts that have TrueType outlines.
    LONG_DATE_TIME    created;                  // Number of seconds since 12:00 midnight, January 1, 1904. 64-bit integer
    LONG_DATE_TIME    modified;                 // As above.
    // For all glyph bounding boxes.
    FWORD             x_min;
    FWORD             y_min;
    FWORD             x_max;
    FWORD             y_max;
    
    USHORT            mac_style;
    USHORT            lowest_rec_ppem;
    SHORT             font_direction_hint;
    SHORT             index_to_loc_format;      // This field specifies the version of the 'loca' table. 0 for short offsets, 1 for long.
    SHORT             glygh_data_format;
    void load_table(Table_Directory_Entry *entry, ifstream &fin);
    void dump_info(FILE *fp, size_t indent);
  };
}

#endif