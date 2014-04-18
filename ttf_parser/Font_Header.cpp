#include "stdafx.h"
#include "Font_Header.h"

namespace ttf_dll{
  void Font_Header::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    FREAD(fin, &table_version_number);
    FREAD(fin, &font_revision);
    FREAD(fin, &checksum_adjustment);
    FREAD(fin, &magic_number);
    FREAD(fin, &flags);
    FREAD(fin, &units_per_em);
    FREAD(fin, &created);
    FREAD(fin, &modified);
    FREAD(fin, &x_min);
    FREAD(fin, &y_min);
    FREAD(fin, &x_max);
    FREAD(fin, &y_max);
    FREAD(fin, &mac_style);
    FREAD(fin, &lowest_rec_ppem);
    FREAD(fin, &font_direction_hint);
    FREAD(fin, &index_to_loc_format);
    FREAD(fin, &glygh_data_format);
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