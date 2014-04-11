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

	void Maximum_Profile::dump_info(FILE *fp){
		fprintf(fp, "<maxp>\n");
		fprintf(fp, "<tableVersion value=\"0x%08x\"/>\n", table_version_number);
		fprintf(fp, "<numGlyphs value=\"%u\"/>\n", num_glyphs);
		fprintf(fp, "<maxPoints value=\"%u\"/>\n", max_points);
		fprintf(fp, "<maxContours value=\"%u\"/>\n", max_contours);
		fprintf(fp, "<maxCompositePoints value=\"%u\"/>\n", max_composite_points);
		fprintf(fp, "<maxCompositeContours value=\"%u\"/>\n", max_composite_contours);
		fprintf(fp, "<maxZones value=\"%u\"/>\n", max_zones);
		fprintf(fp, "<maxTwilightPoints value=\"%u\"/>\n", max_twilight_points);
		fprintf(fp, "<maxStorage value=\"%u\"/>\n", max_storage);
		fprintf(fp, "<maxFunctionDefs value=\"%u\"/>\n", max_function_defs);
		fprintf(fp, "<maxInstructionDefs value=\"%u\"/>\n", max_instruction_defs);
		fprintf(fp, "<maxStackElements value=\"%u\"/>\n", max_stack_elements);
		fprintf(fp, "<maxSizeOfInstructions value=\"%u\"/>\n", max_size_of_instructions);
		fprintf(fp, "<maxComponentElements value=\"%u\"/>\n", max_component_elements);
		fprintf(fp, "<maxComponentDepth value=\"%u\"/>\n", max_component_depth);
		fprintf(fp, "</maxp>\n");
	}
}