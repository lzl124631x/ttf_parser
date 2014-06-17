#ifndef MAXIMUM_PROFILE_H
#define MAXIMUM_PROFILE_H
#include "type.h"
#include "ttf_table.h"
/****************************************************************************/
/*                         maxp - Maximum Profile                           */
/* Spec: https://www.microsoft.com/typography/otspec/maxp.htm               */
/****************************************************************************/
namespace ttf_dll {

class DLL_API MaximumProfile : public TtfSubtable {
 public:
  explicit MaximumProfile(const TrueTypeFont& ttf);
  // Overrides
  void Init(const TableRecordEntry *entry, std::ifstream &fin);
  void DumpInfo(XmlLogger &logger) const;
  // Accessors
  UShort num_glyphs() const { return num_glyphs_; }
  UShort max_points() const { return max_points_; }
  UShort max_contours() const { return max_contours_; }
  UShort max_composite_points() const { return max_composite_points_; }
  UShort max_composite_contours() const { return max_composite_contours_; }
  UShort max_size_of_instructions() const {
    return max_size_of_instructions_;
  }
  UShort max_component_depth() const { return max_component_depth_; }

 private:
  // 0x00010000 for version 1.0.
  Fixed   table_version_number_;
  // The number of glyphs in the font.
  UShort  num_glyphs_;
  // Maximum points in a non-composite glyph.
  UShort  max_points_;
  // Maximum contours in a non-composite glyph.
  UShort  max_contours_;
  // Maximum points in a composite glyph.
  UShort  max_composite_points_;
  // Maximum contours in a composite glyph.
  UShort  max_composite_contours_;
  // 1 if instructions do not use the twilight zone (Z0), or 2 if instructions
  // do use Z0; should be set to 2 in most cases.
  UShort  max_zones_;
  // Maximum points used in Z0.
  UShort  max_twilight_points_;
  // Number of Storage Area locations.
  UShort  max_storage_;
  // Number of FDEFs.
  UShort  max_function_defs_;
  // Number of IDEFs.
  UShort  max_instruction_defs_;
  // Maximum stack depth2.
  UShort  max_stack_elements_;
  // Maximum byte count for glyph instructions.
  UShort  max_size_of_instructions_;
  // Maximum number of components referenced at "top level" for any composite
  // glyph.
  UShort  max_component_elements_;
  // Maximum levels of recursion; 1 for simple components.
  // TIP: If the font only has simple glyphs, this field equals 0.
  UShort  max_component_depth_;
};

} // namespace ttf_dll

#endif