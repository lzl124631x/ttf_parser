#ifndef MAXIMUM_PROFILE_H
#define MAXIMUM_PROFILE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* maxp ***********************************/
// https://www.microsoft.com/typography/otspec/maxp.htm
namespace ttf_dll{
  class DLL_API Maximum_Profile{
  public:
    FIXED   table_version_number;               // 0x00010000 for version 1.0.
    USHORT  num_glyphs;                         // The number of glyphs in the font.
    USHORT  max_points;                         // Maximum points in a non-composite glyph.
    USHORT  max_contours;                       // Maximum contours in a non-composite glyph.
    USHORT  max_composite_points;               // Maximum points in a composite glyph.
    USHORT  max_composite_contours;             // Maximum contours in a composite glyph.
    USHORT  max_zones;
    // 1 if instructions do not use the twilight zone (Z0), or 2 if instructions do use Z0;
    // should be set to 2 in most cases.
    USHORT  max_twilight_points;                // Maximum points used in Z0.
    USHORT  max_storage;                        // Number of Storage Area locations.
    USHORT  max_function_defs;                  // Number of FDEFs.
    USHORT  max_instruction_defs;               // Number of IDEFs.
    USHORT  max_stack_elements;                 // Maximum stack depth2.
    USHORT  max_size_of_instructions;           // Maximum byte count for glyph instructions.
    USHORT  max_component_elements;             // Maximum number of components referenced at "top level" for any composite glyph.
    USHORT  max_component_depth;                // Maximum levels of recursion; 1 for simple components.
    void load_table(Table_Directory_Entry *entry, ifstream &fin);
    void dump_info(FILE *fp, size_t indent);
  };
}

#endif