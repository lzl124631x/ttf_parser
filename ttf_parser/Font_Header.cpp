#include "stdafx.h"
#include "Font_Header.h"
#include <ctime>

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

  static void longDateTime_to_string(char *buf, size_t buf_size, longDateTime time){
    // Seconds between 00:00 1904-01-01 (Mac Time) and 00:00 1970-01-01 (UTC Time)
    static const unsigned long secs_until_1970 = 2082844800;
    if(time < secs_until_1970){
        sprintf_s(buf, buf_size, "Invalid Time Stamp");
        return;
    }
    tm mac_time = {0};
    mac_time.tm_sec = (int)(time - secs_until_1970);
    mac_time.tm_hour = 0;   // Adjust this for your timezone.
    mac_time.tm_mday = 1;   // FIXME: Confused! I have to add this 'day to month' to get the right answer!
                            // No use to modify 'day to year'! Why?
    mac_time.tm_year = 70;
    mktime(&mac_time);      // Adjust members of tm structure.
    strftime(buf, buf_size, "%a %b %d %X %Y", &mac_time);
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
    char time[50] = {0};
    longDateTime_to_string(time, 50, created);
    INDENT(fp, indent); fprintf(fp, "<created value=\"%s\"/>\n", time);
    longDateTime_to_string(time, 50, modified);
    INDENT(fp, indent); fprintf(fp, "<modified value=\"%s\"/>\n", time);
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