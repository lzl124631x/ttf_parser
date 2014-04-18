#ifndef INDEX_TO_LOCATION_H
#define INDEX_TO_LOCATION_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* loca ***********************************/
// https://www.microsoft.com/typography/otspec/loca.htm
namespace ttf_dll{
  class DLL_API Index_To_Location{
  public:
    SHORT loca_format; // FIXME: this is a copy of head.index_to_loc_format. Consider remove it.
    void *offsets; /* [num_glyphs] */
    // Short Version: USHORT
    // Long Version: ULONG
    void load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_glyphs, SHORT loca_format);
    ~Index_To_Location(){
      if(loca_format){  // 1 for ULONG
        delete[] (ULONG*)offsets;
      }else{            // 0 for USHORT
        delete[] (USHORT*)offsets;
      }
    }
    ULONG find_location(USHORT index){
      return ((ULONG*)offsets)[index]; // FIXME: consider short/long version, dude.
    }
  };
}
#endif