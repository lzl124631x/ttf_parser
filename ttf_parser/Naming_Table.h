#ifndef NAMING_TABLE_H
#define NAMING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* name ***********************************/
namespace ttf_dll{
	class Naming_Table{
	public:
		USHORT	format_selector;
		USHORT	number_of_name_records;
		USHORT	offset;				// to start of string storage (from start of table)
		typedef struct _Name_Record{
			USHORT	platform_id;
			USHORT	platform_specific_encoding_id;
			USHORT	language_id;
			USHORT	name_id;
			USHORT	string_length;	// in bytes
			USHORT	string_offset;	// from start of storage area (in bytes)
		}Name_Record;
		Name_Record	*name_records;/* [number_of_name_records] */
		// (Variable) Storage for the actual string data.
		void load_table(Table_Directory_Entry *entry, ifstream &fin);
	};
}

#endif