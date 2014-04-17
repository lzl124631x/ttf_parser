#ifndef NAMING_TABLE_H
#define NAMING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* name ***********************************/
namespace ttf_dll{
	enum Platform_ID{
		Unicode			= 0,
		Macintosh		= 1,
		ISO				= 2,
		Windows			= 3,
		Custom			= 4
	};

	enum Windows_Encoding_ID{
		Symbol			= 0,
		Unicode_BMP		= 1,
		ShiftJIS		= 2,
		PRC				= 3,
		Big5			= 4,
		Wansung			= 5,
		Johab			= 6,
		// 7-9 are reserved.
		Unicode_UCS_4 = 10
	};

	struct Name_Record{
			USHORT	platform_id;	// Platform ID.
			USHORT	encoding_id;	// Platform-specific encoding ID.
			USHORT	language_id;	// Language ID.
			USHORT	name_id;		// Name ID.
			USHORT	length;			// String length (in bytes).
			USHORT	offset;			// String offset from start of storage area (in bytes).
			void dump_info(FILE *fp, size_t indent, char *str);
	};
	
	// FIXME: There are two versions of naming table! The format 1 is not yet implemented.
	class DLL_API Naming_Table{
	public:
		USHORT	format;				// Format Selector.
		USHORT	count;				// Number of name records.
		USHORT	offset;				// Offset to start of string storage (from start of table).
		Name_Record *name_records;	// The name records where count is the number of records.
		char	**strings;			// (Variable) Storage for the actual string data.
		void load_table(Table_Directory_Entry *entry, ifstream &fin);
		~Naming_Table(){ // FIXME: more proper to move this function to .cpp?
			delete[] name_records;
			for(int i = 0; i < count; ++i){
				delete[] strings[i];
			}
      delete[] strings;
		}
		void dump_info(FILE *fp, size_t indent);
	};

	struct Language_Tag_Record{
		USHORT	length;				// Language-tag string length (in bytes)
		USHORT	offset;				// Language-tag string offset from start of storage area (in bytes).
	};
}

#endif