#include "stdafx.h"
#include "maximum_profile.h"
using namespace std;

namespace ttf_dll {

void MaximumProfile::LoadTable(const TableRecordEntry *entry, ifstream &fin) {
  fin.seekg(entry->offset(), ios::beg);
  FREAD(fin, &table_version_number_);
  FREAD(fin, &num_glyphs_);
  FREAD(fin, &max_points_);
  FREAD(fin, &max_contours_);
  FREAD(fin, &max_composite_points_);
  FREAD(fin, &max_composite_contours_);
  FREAD(fin, &max_zones_);
  FREAD(fin, &max_twilight_points_);
  FREAD(fin, &max_storage_);
  FREAD(fin, &max_function_defs_);
  FREAD(fin, &max_instruction_defs_);
  FREAD(fin, &max_stack_elements_);
  FREAD(fin, &max_size_of_instructions_);
  FREAD(fin, &max_component_elements_);
  FREAD(fin, &max_component_depth_);
}

void MaximumProfile::DumpInfo(XmlLogger &logger) const {
  logger.Println("<maxp>");
  logger.IncreaseIndent();
  logger.Println("<tableVersion value=\"0x%08x\"/>", table_version_number_);
  logger.Println("<numGlyphs value=\"%u\"/>", num_glyphs_);
  logger.Println("<maxPoints value=\"%u\"/>", max_points_);
  logger.Println("<maxContours value=\"%u\"/>", max_contours_);
  logger.Println("<maxCompositePoints value=\"%u\"/>", max_composite_points_);
  logger.Println("<maxCompositeContours value=\"%u\"/>", max_composite_contours_);
  logger.Println("<maxZones value=\"%u\"/>", max_zones_);
  logger.Println("<maxTwilightPoints value=\"%u\"/>", max_twilight_points_);
  logger.Println("<maxStorage value=\"%u\"/>", max_storage_);
  logger.Println("<maxFunctionDefs value=\"%u\"/>", max_function_defs_);
  logger.Println("<maxInstructionDefs value=\"%u\"/>", max_instruction_defs_);
  logger.Println("<maxStackElements value=\"%u\"/>", max_stack_elements_);
  logger.Println("<maxSizeOfInstructions value=\"%u\"/>", max_size_of_instructions_);
  logger.Println("<maxComponentElements value=\"%u\"/>", max_component_elements_);
  logger.Println("<maxComponentDepth value=\"%u\"/>", max_component_depth_);
  logger.DecreaseIndent();
  logger.Println("</maxp>");
}

} // namespace ttf_dll