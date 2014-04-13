#include "stdafx.h"
#include "True_Type_Font.h"

namespace ttf_dll{
	ULONG calc_table_checksum(ULONG *table, ULONG num_of_bytes){
		ULONG sum = 0L;
		ULONG *end = table + ((num_of_bytes + 3) & ~3) / sizeof(ULONG);
		while(table < end){
			sum += *table++;
		}
		return sum;
	}

	bool True_Type_Font::valid_checksum(char* tag){
		Table_Directory_Entry *entry = offset_table->get_table_entry(tag);
		return entry->checksum == calc_table_checksum(get_table(entry), entry->length);
	}

	bool True_Type_Font::load_path(string path){
		ifstream fin(path, ios::in | ios::binary | ios::ate);
		if(!fin.is_open()){
			cout << "Error reading file " << path.c_str() << "!" << endl;
			return false;
		}
		streamoff fsize = fin.tellg();
		fin.seekg(0, ios::beg);
		file_binary = new BYTE[(unsigned)fsize];
		fin.read((char*)file_binary, fsize * sizeof(char));

		fin.seekg(0, ios::beg);
		offset_table = new Offset_Table();
		ifstream_read_big_endian(fin, &offset_table->sfnt_version, sizeof(FIXED));
		ifstream_read_big_endian(fin, &offset_table->num_tables, sizeof(USHORT));
		ifstream_read_big_endian(fin, &offset_table->search_range, sizeof(USHORT));
		ifstream_read_big_endian(fin, &offset_table->entry_selector, sizeof(USHORT));
		ifstream_read_big_endian(fin, &offset_table->range_shift, sizeof(USHORT));
		for(int i = 0; i < offset_table->num_tables; ++i){
			Table_Directory_Entry *entry = new Table_Directory_Entry();
			ifstream_read_big_endian(fin, &entry->tag, sizeof(ULONG));
			char str[5] = {0};
			Table_Directory_Entry::tag_ULONG_to_string(entry->tag, str);
			ifstream_read_big_endian(fin, &entry->checksum, sizeof(ULONG));
			ifstream_read_big_endian(fin, &entry->offset, sizeof(ULONG));
			ifstream_read_big_endian(fin, &entry->length, sizeof(ULONG));
			offset_table->table_directory_entries[entry->tag] = entry;
			char tag_str[5] = {0}; // TEST
			Table_Directory_Entry::tag_ULONG_to_string(entry->tag, tag_str); // TEST
			cout << tag_str << "\t" << boolalpha << valid_checksum(tag_str) << endl; // TEST
		};

		cmap.load_table(offset_table->get_table_entry("cmap"), fin);
		head.load_table(offset_table->get_table_entry("head"), fin);
		maxp.load_table(offset_table->get_table_entry("maxp"), fin);
		loca.load_table(offset_table->get_table_entry("loca"), fin, maxp.num_glyphs, head.index_to_loc_format);
		hhea.load_table(offset_table->get_table_entry("hhea"), fin);
		hmtx.load_table(offset_table->get_table_entry("hmtx"), fin, this);
		name.load_table(offset_table->get_table_entry("name"), fin);
		os_2.load_table(offset_table->get_table_entry("OS/2"), fin);

		load_glyph_data_array(fin);// FIXME

		return true;
	}

	void True_Type_Font::load_glyph_data_array(ifstream &fin){
		ULONG glyph_data_offset = offset_table->get_table_entry("glyf")->offset;
		glyph_data_array = new Glyph_Data*[maxp.num_glyphs];
		for(int i = 0; i <= maxp.num_glyphs; ++i){
			ULONG offset = glyph_data_offset;
			if(head.index_to_loc_format){ // The format of 'loca' table is determined by the 'index_to_loc_format' entry in 'head' table.
				offset += *((ULONG*)loca.offsets + i); // 1 for ULONG
		}else{
				offset += *((USHORT*)loca.offsets + i); // 0 for USHORT
			}
			Glyph_Data *glyph_data = Glyph_Data::load_table(fin, offset, maxp.max_contours);
			glyph_data_array[i] = glyph_data;
		}
		int j = 0;
	}

	// FIXME: maybe the following #defines should be moved to name table.
#define Unicode				0
#define Macintosh			1
#define ISO					2
#define Windows				3
#define Custom				4

#define Symbol				0
#define Unicode_BMP			1
#define ShiftJIS			2
#define PRC					3
#define Big5				4
#define Wansung				5
#define Johab				6
#define Unicode_UCS_4		10

	void True_Type_Font::get_glyph_outline(USHORT ch){
		USHORT glyph_index = cmap.get_glyph_index(Windows, Unicode_BMP, ch);
		//ULONG location = loca.find_location(glyph_index);
		wstring name = L"C:/msyh/";
		name += ch;
		name += L".svg";
		// FIXME: the following codes should be more generic, i.e. use _t instead of _w.
		FILE *fp = NULL;
		if(_tfopen_s(&fp, name.c_str(), _T("w")) == 0){ // Return 0 if successful.
			glyph_data_array[glyph_index]->dump_svg_outline(fp);
			fclose(fp);
		}
	}

	void True_Type_Font::dump_ttf(FILE *fp){
		fprintf(fp, "<ttFont>\n");
		offset_table->dump_info(fp, 1);
		head.dump_info(fp, 1);
		hhea.dump_info(fp, 1);
		maxp.dump_info(fp, 1);
		os_2.dump_info(fp, 1);
		cmap.dump_info(fp, 1);
		fprintf(fp, "</ttFont>\n");
	}
}