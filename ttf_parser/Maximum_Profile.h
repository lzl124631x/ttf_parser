#ifndef MAXIMUM_PROFILE_H
#define MAXIMUM_PROFILE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* maxp ***********************************/
namespace ttf_dll{
	class Maximum_Profile{
	public:
		FIXED	table_version_number;
		USHORT	num_glyphs;
		USHORT	max_points;
		USHORT	max_contours;
		USHORT	max_composite_points;
		USHORT	max_composite_contours;
		USHORT	max_zones;
		USHORT	max_twilight_points;
		USHORT	max_storage;
		USHORT	max_function_defs;
		USHORT	max_instruction_defs;
		USHORT	max_stack_elements;
		USHORT	max_size_of_instructions;
		USHORT	max_component_elements;
		USHORT	max_component_depth;
		void load_table(Table_Directory_Entry *entry, ifstream &fin);
	};
}

#endif