#include "stdafx.h"
#include "Horizontal_Metrics.h"
#include "True_Type_Font.h"

namespace ttf_dll {

void Long_Hor_Metric::load_metric(ifstream &fin) {
  FREAD(fin, &advance_width);
  FREAD(fin, &lsb);
}

FWORD Horizontal_Metrics::get_lsb(const GLYPH_ID glyph_index) const {
  if(glyph_index < num_hmtx) {
    return hmetrics[glyph_index].lsb;
  } else if(glyph_index < num_glyphs) {
    return left_side_bearing[glyph_index - num_hmtx];
  } else {
    // ERROR: Invalid glyph index!
    return 0;
  }
}

UFWORD Horizontal_Metrics::get_aw(const GLYPH_ID glyph_index) const {
  if(glyph_index < num_hmtx) {
    return hmetrics[glyph_index].advance_width;
  } else if(glyph_index < num_glyphs) {
    return hmetrics[num_hmtx - 1].advance_width;
  } else {
    // ERROR: Invalid glyph index!
    return 0;
  }
}

void Horizontal_Metrics::load_table(Table_Record_Entry *entry, ifstream &fin) {
  num_hmtx = g_ttf->hhea.number_of_hmetrics;
  num_glyphs = g_ttf->maxp.num_glyphs;
  fin.seekg(entry->offset, ios::beg);
  hmetrics = new Long_Hor_Metric[num_hmtx];
  for(int i = 0; i < num_hmtx; ++i) {
    hmetrics[i].load_metric(fin);
  }
  USHORT num_lsb = num_glyphs - num_hmtx;
  left_side_bearing = new FWORD[num_lsb];// FIXME: consider to combine this line and next line into one function
  FREAD_N(fin, left_side_bearing, num_lsb);
}

void Horizontal_Metrics::dump_info(Xml_Logger &logger) const {
  logger.println("<hmtx>");
  logger.inc_indent();
  int i = 0;
  for(; i < num_hmtx; ++i) {
    logger.println(
        "<mtx glyphIndex=\"%u\" width=\"%u\" lsb=\"%d\"/>",
        i, hmetrics[i].advance_width, hmetrics[i].lsb);
  }
  USHORT width = hmetrics[num_hmtx - 1].advance_width;
  for(; i < num_glyphs; ++i) {
    logger.println(
        "<mtx glyphIndex=\"%u\" width=\"%u\" lsb=\"%d\"/>",
        i, width, left_side_bearing[i - num_hmtx]);
  }
  logger.dec_indent();
  logger.println("</hmtx>");
}

} // namespace ttf_dll