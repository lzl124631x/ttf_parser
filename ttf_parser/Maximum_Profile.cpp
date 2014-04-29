#include "stdafx.h"
#include "Maximum_Profile.h"

namespace ttf_dll{
  void Maximum_Profile::load_table(Table_Directory_Entry *entry, ifstream &fin){
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

  void Maximum_Profile::dump_info(FILE *fp, size_t indent){
    IND_FP("<maxp>\n");
    ++indent;
    IND_FP("<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    IND_FP("<numGlyphs value=\"%u\"/>\n", num_glyphs);
    IND_FP("<maxPoints value=\"%u\"/>\n", max_points);
    IND_FP("<maxContours value=\"%u\"/>\n", max_contours);
    IND_FP("<maxCompositePoints value=\"%u\"/>\n", max_composite_points);
    IND_FP("<maxCompositeContours value=\"%u\"/>\n", max_composite_contours);
    IND_FP("<maxZones value=\"%u\"/>\n", max_zones);
    IND_FP("<maxTwilightPoints value=\"%u\"/>\n", max_twilight_points);
    IND_FP("<maxStorage value=\"%u\"/>\n", max_storage);
    IND_FP("<maxFunctionDefs value=\"%u\"/>\n", max_function_defs);
    IND_FP("<maxInstructionDefs value=\"%u\"/>\n", max_instruction_defs);
    IND_FP("<maxStackElements value=\"%u\"/>\n", max_stack_elements);
    IND_FP("<maxSizeOfInstructions value=\"%u\"/>\n", max_size_of_instructions);
    IND_FP("<maxComponentElements value=\"%u\"/>\n", max_component_elements);
    IND_FP("<maxComponentDepth value=\"%u\"/>\n", max_component_depth);
    --indent;
    IND_FP("</maxp>\n");
  }
}