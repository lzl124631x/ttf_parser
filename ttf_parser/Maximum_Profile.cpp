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
    INDENT(fp, indent); fprintf(fp, "<maxp>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
    INDENT(fp, indent); fprintf(fp, "<numGlyphs value=\"%u\"/>\n", num_glyphs);
    INDENT(fp, indent); fprintf(fp, "<maxPoints value=\"%u\"/>\n", max_points);
    INDENT(fp, indent); fprintf(fp, "<maxContours value=\"%u\"/>\n", max_contours);
    INDENT(fp, indent); fprintf(fp, "<maxCompositePoints value=\"%u\"/>\n", max_composite_points);
    INDENT(fp, indent); fprintf(fp, "<maxCompositeContours value=\"%u\"/>\n", max_composite_contours);
    INDENT(fp, indent); fprintf(fp, "<maxZones value=\"%u\"/>\n", max_zones);
    INDENT(fp, indent); fprintf(fp, "<maxTwilightPoints value=\"%u\"/>\n", max_twilight_points);
    INDENT(fp, indent); fprintf(fp, "<maxStorage value=\"%u\"/>\n", max_storage);
    INDENT(fp, indent); fprintf(fp, "<maxFunctionDefs value=\"%u\"/>\n", max_function_defs);
    INDENT(fp, indent); fprintf(fp, "<maxInstructionDefs value=\"%u\"/>\n", max_instruction_defs);
    INDENT(fp, indent); fprintf(fp, "<maxStackElements value=\"%u\"/>\n", max_stack_elements);
    INDENT(fp, indent); fprintf(fp, "<maxSizeOfInstructions value=\"%u\"/>\n", max_size_of_instructions);
    INDENT(fp, indent); fprintf(fp, "<maxComponentElements value=\"%u\"/>\n", max_component_elements);
    INDENT(fp, indent); fprintf(fp, "<maxComponentDepth value=\"%u\"/>\n", max_component_depth);
    --indent;
    INDENT(fp, indent); fprintf(fp, "</maxp>\n");
  }
}