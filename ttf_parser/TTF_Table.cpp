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

	void Offset_Table::load_table(ifstream &fin){
		fin.seekg(0, ios::beg);
		ifstream_read_big_endian(fin, &sfnt_version, sizeof(FIXED));
		ifstream_read_big_endian(fin, &num_tables, sizeof(USHORT));
		ifstream_read_big_endian(fin, &search_range, sizeof(USHORT));
		ifstream_read_big_endian(fin, &entry_selector, sizeof(USHORT));
		ifstream_read_big_endian(fin, &range_shift, sizeof(USHORT));
		table_directory_entries = new Table_Directory_Entry[num_tables];
		for(int i = 0; i < num_tables; ++i){
			Table_Directory_Entry *entry = &table_directory_entries[i];
			ifstream_read_big_endian(fin, &entry->tag, sizeof(ULONG));
			ifstream_read_big_endian(fin, &entry->checksum, sizeof(ULONG));
			ifstream_read_big_endian(fin, &entry->offset, sizeof(ULONG));
			ifstream_read_big_endian(fin, &entry->length, sizeof(ULONG));
		};
	}

	Table_Directory_Entry* Offset_Table::get_table_entry(char *tag_str){
		ULONG tag = Table_Directory_Entry::tag_string_to_ULONG(tag_str);
		for(int i = 0; i < num_tables; ++i){
			Table_Directory_Entry *entry = &table_directory_entries[i];
			if(entry->tag == tag){
				return entry;
			}
		}
		return NULL;
	}

	void Offset_Table::dump_info(FILE *fp, size_t indent){
		INDENT(fp, indent); fprintf(fp, "<offsetTable sfntVersion=\"%08x\" numTables=\"%d\" searchRange=\"%d\" entrySelector=\"%d\" rangeShift=\"%d\">\n",
			sfnt_version, num_tables, search_range, entry_selector, range_shift);
		++indent;
		for(int i = 0; i < num_tables; ++i){
			Table_Directory_Entry *entry = &table_directory_entries[i];
			INDENT(fp, indent);
			fprintf(fp, "<entry tag=\"%.4s\" checksum=\"%08x\" offset=\"%d\" length=\"%d\"/>\n",
				(char*)&entry->tag, entry->checksum, entry->offset, entry->length);
		}
		--indent;
		INDENT(fp, indent); fprintf(fp, "</offsetTable>\n");
	}
}