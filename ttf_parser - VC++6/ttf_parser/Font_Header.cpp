#include "stdafx.h"
#include "Font_Header.h"
#include <ctime>

using namespace std;
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

  static void long_date_time_to_string(char *buf, size_t buf_size, LONG_DATE_TIME time){
    // Seconds between 00:00 1904-01-01 (Mac Time) and 00:00 1970-01-01 (UTC Time)
    static const unsigned long secs_until_1970 = 2082844800;
    if(time < secs_until_1970){
        sprintf(buf, "Invalid Time Stamp");
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
    IND_FP(fp, indent, "<head>\n");
    ++indent;
    IND_FP(fp, indent, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    IND_FP(fp, indent, "<fontRevision value=\"0x%08x\"/>\n", font_revision);
    IND_FP(fp, indent, "<checkSumAdjustment value=\"0x%08x\"/>\n", checksum_adjustment);
    IND_FP(fp, indent, "<magicNumber value=\"0x%08x\"/>\n", magic_number);
    IND_FP(fp, indent, "<flags value=\"0x%04x\"/>\n", flags);
    IND_FP(fp, indent, "<unitsPerEm value=\"%d\"/>\n", units_per_em);
    char time[50] = {0};
    long_date_time_to_string(time, 50, created);
    IND_FP(fp, indent, "<created value=\"%s\"/>\n", time);
    long_date_time_to_string(time, 50, modified);
    IND_FP(fp, indent, "<modified value=\"%s\"/>\n", time);
    IND_FP(fp, indent, "<xMin value=\"%d\"/>\n", x_min);
    IND_FP(fp, indent, "<yMin value=\"%d\"/>\n", y_min);
    IND_FP(fp, indent, "<xMax value=\"%d\"/>\n", x_max);
    IND_FP(fp, indent, "<yMax value=\"%d\"/>\n", y_max);
    IND_FP(fp, indent, "<macStyle value=\"0x%04x\"/>\n", mac_style);
    IND_FP(fp, indent, "<lowestRecPPEM value=\"%u\"/>\n", lowest_rec_ppem);
    IND_FP(fp, indent, "<fontDirectionHint value=\"%d\"/>\n", font_direction_hint);
    IND_FP(fp, indent, "<indexToLocFormat value=\"%d\"/>\n", index_to_loc_format);
    IND_FP(fp, indent, "<glyphDataFormat value=\"%d\"/>\n", glygh_data_format);
    --indent;
    IND_FP(fp, indent, "</head>\n");
  }
}