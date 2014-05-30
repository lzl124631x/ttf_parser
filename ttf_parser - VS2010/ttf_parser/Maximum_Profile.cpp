#include "stdafx.h"
#include "Maximum_Profile.h"

namespace ttf_dll {

void Maximum_Profile::load_table(Table_Record_Entry *entry, ifstream &fin) {
  fin.seekg(entry->offset, ios::beg);
  FREAD(fin, &table_version_number);
  FREAD(fin, &num_glyphs);
  FREAD(fin, &max_points);
  FREAD(fin, &max_contours);
  FREAD(fin, &max_composite_points);
  FREAD(fin, &max_composite_contours);
  FREAD(fin, &max_zones);
  FREAD(fin, &max_twilight_points);
  FREAD(fin, &max_storage);
  FREAD(fin, &max_function_defs);
  FREAD(fin, &max_instruction_defs);
  FREAD(fin, &max_stack_elements);
  FREAD(fin, &max_size_of_instructions);
  FREAD(fin, &max_component_elements);
  FREAD(fin, &max_component_depth);
}

void Maximum_Profile::dump_info(Xml_Logger &logger) const {
  logger.println("<maxp>");
  logger.inc_indent();
  logger.println("<tableVersion value=\"0x%08x\"/>", table_version_number);
  logger.println("<numGlyphs value=\"%u\"/>", num_glyphs);
  logger.println("<maxPoints value=\"%u\"/>", max_points);
  logger.println("<maxContours value=\"%u\"/>", max_contours);
  logger.println("<maxCompositePoints value=\"%u\"/>", max_composite_points);
  logger.println("<maxCompositeContours value=\"%u\"/>", max_composite_contours);
  logger.println("<maxZones value=\"%u\"/>", max_zones);
  logger.println("<maxTwilightPoints value=\"%u\"/>", max_twilight_points);
  logger.println("<maxStorage value=\"%u\"/>", max_storage);
  logger.println("<maxFunctionDefs value=\"%u\"/>", max_function_defs);
  logger.println("<maxInstructionDefs value=\"%u\"/>", max_instruction_defs);
  logger.println("<maxStackElements value=\"%u\"/>", max_stack_elements);
  logger.println("<maxSizeOfInstructions value=\"%u\"/>", max_size_of_instructions);
  logger.println("<maxComponentElements value=\"%u\"/>", max_component_elements);
  logger.println("<maxComponentDepth value=\"%u\"/>", max_component_depth);
  logger.dec_indent();
  logger.println("</maxp>");
}

} // namespace ttf_dll