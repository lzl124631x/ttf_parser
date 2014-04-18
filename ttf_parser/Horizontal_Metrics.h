#ifndef HORIZONTAL_METRICS_H
#define HORIZONTAL_METRICS_H

#include "Type.h"
#include "TTF_Table.h"
/******************************* hmtx ***********************************/\
// https://www.microsoft.com/typography/otspec/hmtx.htm
namespace ttf_dll{
  class True_Type_Font; // FIXME: How about remove this declaration?
  struct Long_Hor_Metric{
    UFWORD    advance_width;
    FWORD     lsb;
  };

  class DLL_API Horizontal_Metrics{
  public:
    Long_Hor_Metric  *hmetrics;/* [number_of_hmetrics(from hhea)] */
    FWORD     *left_side_bearing;/* [num_glyphs(from maxp) - number_of_hmetrics] */
    void load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_hmtx, USHORT num_glyphs);
    ~Horizontal_Metrics(){
      delete[] hmetrics;
      delete[] left_side_bearing;
    }
  };
}
#endif