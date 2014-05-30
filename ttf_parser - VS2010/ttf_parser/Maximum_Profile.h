#ifndef MAXIMUM_PROFILE_H
#define MAXIMUM_PROFILE_H
#include "Type.h"
#include "TTF_Table.h"
/****************************************************************************/
/*                         maxp - Maximum Profile                           */
/* Spec: https://www.microsoft.com/typography/otspec/maxp.htm               */
/****************************************************************************/
namespace ttf_dll {

class DLL_API Maximum_Profile {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;

  // 0x00010000 for version 1.0.
  FIXED   table_version_number;
  // The number of glyphs in the font.
  USHORT  num_glyphs;
  // Maximum points in a non-composite glyph.
  USHORT  max_points;
  // Maximum contours in a non-composite glyph.
  USHORT  max_contours;
  // Maximum points in a composite glyph.
  USHORT  max_composite_points;
  // Maximum contours in a composite glyph.
  USHORT  max_composite_contours;
  // 1 if instructions do not use the twilight zone (Z0), or 2 if instructions
  // do use Z0; should be set to 2 in most cases.
  USHORT  max_zones;
  // Maximum points used in Z0.
  USHORT  max_twilight_points;
  // Number of Storage Area locations.
  USHORT  max_storage;
  // Number of FDEFs.
  USHORT  max_function_defs;
  // Number of IDEFs.
  USHORT  max_instruction_defs;
  // Maximum stack depth2.
  USHORT  max_stack_elements;
  // Maximum byte count for glyph instructions.
  USHORT  max_size_of_instructions;
  // Maximum number of components referenced at "top level" for any composite
  // glyph.
  USHORT  max_component_elements;
  // Maximum levels of recursion; 1 for simple components.
  USHORT  max_component_depth;
};

} // namespace ttf_dll

#endif