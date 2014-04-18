#include "stdafx.h"
#include "Horizontal_Metrics.h"
#include "True_Type_Font.h"

namespace ttf_dll{
  void Long_Hor_Metric::load_metric(ifstream &fin){
    FREAD(fin, &advance_width);
    FREAD(fin, &lsb);
  }
  void Horizontal_Metrics::load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_hmtx, USHORT num_glyphs){
    // FIXME: Is it proper to use TTF as a parameter?
    fin.seekg(entry->offset, ios::beg);
    hmetrics = new Long_Hor_Metric[num_hmtx];
    for(int i = 0; i < num_hmtx; ++i){
      hmetrics[i].load_metric(fin);
    }
    USHORT num_of_lsb = num_glyphs - num_hmtx;
    left_side_bearing = new FWORD[num_of_lsb];// FIXME: consider to combine this line and next line into one function
    FREAD_N(fin, left_side_bearing, num_of_lsb);
  }
}