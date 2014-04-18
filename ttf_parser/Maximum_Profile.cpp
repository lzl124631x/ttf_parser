#include "stdafx.h"
#include "Maximum_Profile.h"

namespace ttf_dll{
  void Maximum_Profile::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    ifstream_read_big_endian(fin, &table_version_number, sizeof(FIXED));
    ifstream_read_big_endian(fin, &num_glyphs, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_points, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_contours, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_composite_points, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_composite_contours, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_zones, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_twilight_points, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_storage, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_function_defs, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_instruction_defs, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_stack_elements, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_size_of_instructions, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_component_elements, sizeof(USHORT));
    ifstream_read_big_endian(fin, &max_component_depth, sizeof(USHORT));
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