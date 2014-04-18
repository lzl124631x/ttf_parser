#include "stdafx.h"
#include "Horizontal_Metrics.h"
#include "True_Type_Font.h"

namespace ttf_dll{
  void Horizontal_Metrics::load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_hmtx, USHORT num_glyphs){
    // FIXME: Is it proper to use TTF as a parameter?
    fin.seekg(entry->offset, ios::beg);
    hmetrics = new Long_Hor_Metric[num_hmtx];
    FREAD_N(fin, hmetrics, num_hmtx << 1);
    USHORT num_of_lsb = num_glyphs - num_hmtx;
    left_side_bearing = new FWORD[num_of_lsb];// FIXME: consider to combine this line and next line into one function
    FREAD_N(fin, left_side_bearing, num_of_lsb);
  }
}