#include "stdafx.h"
#include "TTF_Table.h"

namespace ttf_dll{
	ULONG Table_Directory_Entry::tag_string_to_ULONG(char *str){
		ULONG ul = 0;
		for(int i = 0; i < 4; ++i){
			ul <<= 8;
			ul += *str++;
		}
		return ul;
	}

	void Table_Directory_Entry::tag_ULONG_to_string(ULONG ul, char *str){
		for(int i = 3; i >= 0; --i){
			*(str + i)= (ul & 0xFF);
			ul >>= 8;
		}
	}

	void Offset_Table::dump_info(FILE *fp, size_t indent){
		INDENT(fp, indent); fprintf(fp, "<offsetTable sfntVersion=\"%08x\" numTables=\"%d\" searchRange=\"%d\" entrySelector=\"%d\" rangeShift=\"%d\">\n",
			sfnt_version, num_tables, search_range, entry_selector, range_shift);
		++indent;
		for(hash_map<ULONG, Table_Directory_Entry*>::iterator i = table_directory_entries.begin(); i != table_directory_entries.end(); ++i){
			Table_Directory_Entry *entry = i->second;
			char tag_str[5] = {0};
			Table_Directory_Entry::tag_ULONG_to_string(entry->tag, tag_str);
			INDENT(fp, indent); fprintf(fp, "<entry tag=\"%s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>\n",
				tag_str, entry->checksum, entry->offset, entry->length);
		}
		--indent;
		INDENT(fp, indent); fprintf(fp, "</offsetTable>\n");
	}
}