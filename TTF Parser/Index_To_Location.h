#include "TTF_Table.h"
/******************************* loca ***********************************/
class Index_To_Location{
public:
	void *offsets; /* [num_glyphs] */
	// Short Version: USHORT
	// Long Version: ULONG
	void load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT num_glyphs);
	inline ULONG find_location(USHORT index){
		return ((ULONG*)this->offsets)[index];
	}
};