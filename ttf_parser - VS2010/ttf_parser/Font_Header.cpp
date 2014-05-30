#include "stdafx.h"
#include "Font_Header.h"
#include <ctime>

namespace ttf_dll {

void Font_Header::load_table(Table_Record_Entry *entry, ifstream &fin) {
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
  FREAD(fin, &loca_format);
  FREAD(fin, &glygh_data_format);
}

static void long_date_time_to_string(char *buf, size_t buf_size, LONG_DATE_TIME time) {
  // Seconds between 00:00 1904-01-01 (Mac Time) and 00:00 1970-01-01 (UTC Time)
  static const unsigned long secs_until_1970 = 2082844800;
  if(time < secs_until_1970) {
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

void Font_Header::dump_info(Xml_Logger &logger) const {
  logger.println("<head>");
  logger.inc_indent();
  logger.println("<tableVersion value=\"0x%08x\"/>", table_version_number);
  logger.println("<fontRevision value=\"0x%08x\"/>", font_revision);
  logger.println("<checkSumAdjustment value=\"0x%08x\"/>", checksum_adjustment);
  logger.println("<magicNumber value=\"0x%08x\"/>", magic_number);
  logger.println("<flags value=\"0x%04x\"/>", flags);
  logger.println("<unitsPerEm value=\"%d\"/>", units_per_em);
  char time[50] = {0};
  long_date_time_to_string(time, 50, created);
  logger.println("<created value=\"%s\"/>", time);
  long_date_time_to_string(time, 50, modified);
  logger.println("<modified value=\"%s\"/>", time);
  logger.println("<xMin value=\"%d\"/>", x_min);
  logger.println("<yMin value=\"%d\"/>", y_min);
  logger.println("<xMax value=\"%d\"/>", x_max);
  logger.println("<yMax value=\"%d\"/>", y_max);
  logger.println("<macStyle value=\"0x%04x\"/>", mac_style);
  logger.println("<lowestRecPPEM value=\"%u\"/>", lowest_rec_ppem);
  logger.println("<fontDirectionHint value=\"%d\"/>", font_direction_hint);
  logger.println("<indexToLocFormat value=\"%d\"/>", loca_format);
  logger.println("<glyphDataFormat value=\"%d\"/>", glygh_data_format);
  logger.dec_indent();
  logger.println("</head>");
}

} // namespace ttf_dll