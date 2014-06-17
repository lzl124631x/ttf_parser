#include "stdafx.h"
#include "horizontal_metrics.h"
#include "true_type_font.h"
using namespace std; // For `ifstream`

namespace ttf_dll {

void LongHorMetric::LoadMetric(ifstream &fin) {
  FREAD(fin, &advance_width_);
  FREAD(fin, &left_side_bearing_);
}

HorizontalMetrics::HorizontalMetrics(const TrueTypeFont &ttf) 
    : TtfSubtable(ttf),
      hmetrics(NULL),
      left_side_bearings_(NULL),
      num_hmtx_(0),
      num_glyphs_(0) {}

void HorizontalMetrics::Init(const TableRecordEntry *entry,
                                  ifstream &fin) {
  num_hmtx_ = ttf_.hhea().num_hmetrics();
  num_glyphs_ = ttf_.maxp().num_glyphs();
  fin.seekg(entry->offset(), ios::beg);
  hmetrics = new LongHorMetric[num_hmtx_];
  for(int i = 0; i < num_hmtx_; ++i) {
    hmetrics[i].LoadMetric(fin);
  }
  UShort num_lsb = num_glyphs_ - num_hmtx_;
  left_side_bearings_ = new FWord[num_lsb];// FIXME: consider to combine this line and next line into one function
  FREAD_N(fin, left_side_bearings_, num_lsb);
}

void HorizontalMetrics::DumpInfo(XmlLogger &logger) const {
  logger.Println("<hmtx>");
  logger.IncreaseIndent();
  int i = 0;
  for(; i < num_hmtx_; ++i) {
    logger.Println(
        "<mtx glyphIndex=\"%u\" width=\"%u\" lsb=\"%d\"/>",
        i, hmetrics[i].advance_width_, hmetrics[i].left_side_bearing_);
  }
  UShort width = hmetrics[num_hmtx_ - 1].advance_width_;
  for(; i < num_glyphs_; ++i) {
    logger.Println(
        "<mtx glyphIndex=\"%u\" width=\"%u\" lsb=\"%d\"/>",
        i, width, left_side_bearings_[i - num_hmtx_]);
  }
  logger.DecreaseIndent();
  logger.Println("</hmtx>");
}

FWord HorizontalMetrics::GetLeftSideBearing(GlyphId glyph_index) const {
  if(glyph_index < num_hmtx_) {
    return hmetrics[glyph_index].left_side_bearing_;
  } else if(glyph_index < num_glyphs_) {
    return left_side_bearings_[glyph_index - num_hmtx_];
  } else {
    // ERROR: Invalid glyph index!
    return 0;
  }
}

UFword HorizontalMetrics::GetAdvanceWidth(GlyphId glyph_index) const {
  if(glyph_index < num_hmtx_) {
    return hmetrics[glyph_index].advance_width_;
  } else if(glyph_index < num_glyphs_) {
    return hmetrics[num_hmtx_ - 1].advance_width_;
  } else {
    // ERROR: Invalid glyph index!
    return 0;
  }
}

} // namespace ttf_dll