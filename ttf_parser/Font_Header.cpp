#include "stdafx.h"
#include "Font_Header.h"

namespace ttf_dll{
  void Font_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    ifstream_read_big_endian(fin, &table_version_number, sizeof(FIXED));
    ifstream_read_big_endian(fin, &font_revision, sizeof(FIXED));
    ifstream_read_big_endian(fin, &checksum_adjustment, sizeof(ULONG));
    ifstream_read_big_endian(fin, &magic_number, sizeof(ULONG));
    ifstream_read_big_endian(fin, &flags, sizeof(USHORT));
    ifstream_read_big_endian(fin, &units_per_em, sizeof(USHORT));
    ifstream_read_big_endian(fin, &created, sizeof(longDateTime));
    ifstream_read_big_endian(fin, &modified, sizeof(longDateTime));
    ifstream_read_big_endian(fin, &x_min, sizeof(FWORD));
    ifstream_read_big_endian(fin, &y_min, sizeof(FWORD));
    ifstream_read_big_endian(fin, &x_max, sizeof(FWORD));
    ifstream_read_big_endian(fin, &y_max, sizeof(FWORD));
    ifstream_read_big_endian(fin, &mac_style, sizeof(USHORT));
    ifstream_read_big_endian(fin, &lowest_rec_ppem, sizeof(USHORT));
    ifstream_read_big_endian(fin, &font_direction_hint, sizeof(USHORT));
    ifstream_read_big_endian(fin, &index_to_loc_format, sizeof(USHORT));
    ifstream_read_big_endian(fin, &glygh_data_format, sizeof(USHORT));
  }

  void Font_Header::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<head>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    INDENT(fp, indent); fprintf(fp, "<fontRevision value=\"0x%08x\"/>\n", font_revision);
    INDENT(fp, indent); fprintf(fp, "<checkSumAdjustment value=\"0x%08x\"/>\n", checksum_adjustment);
    INDENT(fp, indent); fprintf(fp, "<magicNumber value=\"0x%08x\"/>\n", magic_number);
    INDENT(fp, indent); fprintf(fp, "<flags value=\"0x%04x\"/>\n", flags);
    INDENT(fp, indent); fprintf(fp, "<unitsPerEm value=\"%d\"/>\n", units_per_em);
    INDENT(fp, indent); fprintf(fp, "<created value=\"%d\"/>\n", created);
    INDENT(fp, indent); fprintf(fp, "<modified value=\"%d\"/>\n", modified);
    INDENT(fp, indent); fprintf(fp, "<xMin value=\"%d\"/>\n", x_min);
    INDENT(fp, indent); fprintf(fp, "<yMin value=\"%d\"/>\n", y_min);
    INDENT(fp, indent); fprintf(fp, "<xMax value=\"%d\"/>\n", x_max);
    INDENT(fp, indent); fprintf(fp, "<yMax value=\"%d\"/>\n", y_max);
    INDENT(fp, indent); fprintf(fp, "<macStyle value=\"0x%04x\"/>\n", mac_style);
    INDENT(fp, indent); fprintf(fp, "<lowestRecPPEM value=\"%u\"/>\n", lowest_rec_ppem);
    INDENT(fp, indent); fprintf(fp, "<fontDirectionHint value=\"%d\"/>\n", font_direction_hint);
    INDENT(fp, indent); fprintf(fp, "<indexToLocFormat value=\"%d\"/>\n", index_to_loc_format);
    INDENT(fp, indent); fprintf(fp, "<glyphDataFormat value=\"%d\"/>\n", glygh_data_format);
    --indent;
    INDENT(fp, indent); fprintf(fp, "</head>\n");
  }
}