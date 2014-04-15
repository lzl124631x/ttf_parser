#ifndef INDEX_TO_LOCATION_H
#define INDEX_TO_LOCATION_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* loca ***********************************/
namespace ttf_dll{
	class DLL_API Index_To_Location{
	public:
		void *offsets; /* [num_glyphs] */
		// Short Version: USHORT
		// Long Version: ULONG
		void load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_glyphs, SHORT loca_format);
		inline ULONG find_location(USHORT index){
			return ((ULONG*)offsets)[index];
		}
	};
}
#endif