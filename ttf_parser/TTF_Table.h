#ifndef TTF_TABLE_H
#define TTF_TABLE_H

#include "Type.h"
#include <hash_map>
#include <vector>
using namespace std;

namespace ttf_dll{
	class Table_Directory_Entry{
	public:
		ULONG	tag;
		ULONG	checksum;
		ULONG	offset;		// Offset from beginning of TrueType font file
		ULONG	length;
		static ULONG tag_string_to_ULONG(char *str);
		static void tag_ULONG_to_string(ULONG ul, char *str);
	};

	class Offset_Table{
	public:
		FIXED	sfnt_version;
		USHORT	num_tables;
		USHORT	search_range;
		USHORT	entry_selector;
		USHORT	range_shift;
		hash_map<ULONG, Table_Directory_Entry*> table_directory_entries;
		inline Table_Directory_Entry* Offset_Table::get_table_entry(char* tag){
			return table_directory_entries[Table_Directory_Entry::tag_string_to_ULONG(tag)];
		}
		void dump_info(FILE *fp);
	};

	/************************************************************************/
	/*                           OPTIONAL TABLES                            */
	/************************************************************************/
	/******************************* cvt ***********************************/
	class Control_Value_Table{
		FWORD	list_of_values/*[n]*/;
	};

	/******************************* EBDT ***********************************/
	class Embedded_Bitmap_Data_Table{
		FIXED	version;
	};

	/******************************* EBLC ***********************************/
	class Embedded_Bitmap_Location_Table{
		FIXED	version;
		ULONG	num_sizes;
	};

	/******************************* EBSC ***********************************/

	/******************************* fpgm ***********************************/
	class Font_Program{
		BYTE	instructions/*[n]*/;
	};
	/******************************* gasp ***********************************/
	//class Grid_Fitting_And_Scan_Conversion_Procedure{
	//	USHORT	version;
	//	USHORT	num_ranges;
	//	typedef struct _GASRANGE{
	//		USHORT	range_max_ppem;
	//		USHORT	range_gasp_behavior;
	//	}GASRANGE;
	//	GASPRANGE	gasp_range[num_ranges];
	//};

	/******************************* hdmx ***********************************/

	/******************************* kern ***********************************/
	class Kerning{
		USHORT	version;
		USHORT	n_tables;
	};
	class Kerning_Subtable{
		USHORT	version;
		USHORT	length;
		USHORT	coverage;
	};

	/******************************* LTSH ***********************************/

	/******************************* prep ***********************************/
	class Control_Value_Program{
		BYTE	set_of_instructions/*[n]*/;
	};

	/******************************* PCLT ***********************************/

	/******************************* VDMX ***********************************/

	/******************************* vhea ***********************************/

	/******************************* vmtx ***********************************/
}

#endif