#ifndef TTF_TABLE_H
#define TTF_TABLE_H

#include "Type.h"

namespace ttf_dll{
  class DLL_API Table_Directory_Entry{
  public:
    ULONG  tag;
    ULONG  checksum;
    ULONG  offset;    // Offset from beginning of TrueType font file
    ULONG  length;
    void load_entry(std::ifstream &fin);
  };

  class DLL_API Offset_Table{
  public:
    FIXED   sfnt_version;
    USHORT  num_tables;
    USHORT  search_range;
    USHORT  entry_selector;
    USHORT  range_shift;
    Table_Directory_Entry *table_directory_entries;
    void load_table(std::ifstream &fin);
    ~Offset_Table(){
      DEL_A(table_directory_entries);
    }
    Table_Directory_Entry *get_table_entry(char *tag_str);
    void dump_info(FILE *fp, size_t indent);
  };

  /************************************************************************/
  /*                           OPTIONAL TABLES                            */
  /************************************************************************/
  /******************************* cvt ***********************************/
  class Control_Value_Table{
    FWORD  list_of_values/*[n]*/;
  };

  /******************************* EBDT ***********************************/
  class Embedded_Bitmap_Data_Table{
    FIXED  version;
  };

  /******************************* EBLC ***********************************/
  class Embedded_Bitmap_Location_Table{
    FIXED  version;
    ULONG  num_sizes;
  };

  /******************************* EBSC ***********************************/

  /******************************* fpgm ***********************************/
  class Font_Program{
    BYTE  instructions/*[n]*/;
  };
  /******************************* gasp ***********************************/
  //class Grid_Fitting_And_Scan_Conversion_Procedure{
  //  USHORT  version;
  //  USHORT  num_ranges;
  //  struct  GASRANGE{
  //    USHORT  range_max_ppem;
  //    USHORT  range_gasp_behavior;
  //  };
  //  GASPRANGE  gasp_range[num_ranges];
  //};

  /******************************* hdmx ***********************************/

  /******************************* kern ***********************************/
  class Kerning{
    USHORT  version;
    USHORT  n_tables;
  };
  class Kerning_Subtable{
    USHORT  version;
    USHORT  length;
    USHORT  coverage;
  };

  /******************************* LTSH ***********************************/

  /******************************* prep ***********************************/
  class Control_Value_Program{
    BYTE  set_of_instructions/*[n]*/;
  };

  /******************************* PCLT ***********************************/

  /******************************* VDMX ***********************************/

  /******************************* vhea ***********************************/

  /******************************* vmtx ***********************************/
}

#endif