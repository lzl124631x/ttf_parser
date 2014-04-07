#include "stdafx.h"
#include "Character_To_Glyph_Mapping_Table.h"

namespace ttf_dll{
	Encoding_Table::Encoding_Table(ifstream &fin){
		ifstream_read_big_endian(fin, &this->format, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->length, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->version, sizeof(USHORT));
	}

	Byte_Encoding_Table::Byte_Encoding_Table(Encoding_Table &et, ifstream &fin)
		:Encoding_Table(et){
			ifstream_read_big_endian(fin, &this->glyph_id_array, sizeof(BYTE), 256);
	}

	High_Byte_Mapping_Through_Table::High_Byte_Mapping_Through_Table(Encoding_Table &et, ifstream &fin)
		:Encoding_Table(et){
			ifstream_read_big_endian(fin, &this->subheader_keys, sizeof(BYTE), 256);
	}

	void Segment_Mapping_To_Delta_Values::dump_info(){
		USHORT seg_count = this->seg_countx2 >> 1;
		printf("Segment_Mapping_To_Delta_Values::dump_info\n");
		printf("Start\tEnd\tDelta\tOffset\n");
		for(int i =0; i < seg_count; ++i){
			printf("(%u,\t%u,\t%u,\t%u)\n",this->start_count[i], this->end_count[i],
				this->id_delta[i], this->id_range_offset[i]);
		}
	}

	Segment_Mapping_To_Delta_Values::Segment_Mapping_To_Delta_Values(Encoding_Table &et, ifstream &fin)
		:Encoding_Table(et){
			ifstream_read_big_endian(fin, &this->seg_countx2, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->search_range, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->entry_selector, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->range_shift, sizeof(USHORT));
			USHORT seg_count = this->seg_countx2 >> 1;

			this->end_count = new USHORT[seg_count];
			ifstream_read_big_endian(fin, this->end_count, sizeof(USHORT), seg_count);
			ifstream_read_big_endian(fin, &this->reserved_pad, sizeof(USHORT));
			this->start_count = new USHORT[seg_count];
			ifstream_read_big_endian(fin, this->start_count, sizeof(USHORT), seg_count);
			this->id_delta = new USHORT[seg_count];
			ifstream_read_big_endian(fin, this->id_delta, sizeof(USHORT), seg_count);
			this->id_range_offset = new USHORT[seg_count];
			ifstream_read_big_endian(fin, this->id_range_offset, sizeof(USHORT), seg_count);
			USHORT var_len = this->length - sizeof(USHORT) * (8 + (this->seg_countx2 << 1));
			this->glyph_id_array = new USHORT[var_len];
			ifstream_read_big_endian(fin, this->glyph_id_array, sizeof(USHORT), var_len);
			//this->dump_info();
	}

	USHORT Segment_Mapping_To_Delta_Values::get_glyph_index(USHORT ch){
		int i = 0;
		while(this->end_count[i] != 0xFFFF && this->end_count[i] < ch){
			++i;
		}
		USHORT glyph_index = 0;
		if(this->start_count[i] <= ch){
			if(this->id_range_offset[i]){
				glyph_index = *(this->glyph_id_array + this->id_range_offset[i] / 2 + (ch - this->start_count[i])
					- (&this->id_range_offset[this->seg_countx2 >> 1] - &this->id_range_offset[i]));
			}else{
				glyph_index = this->id_delta[i] + ch;
			}
		}
		return glyph_index;
	}

	Trimmed_Table_Mapping::Trimmed_Table_Mapping(Encoding_Table &et, ifstream &fin)
		:Encoding_Table(et){
			ifstream_read_big_endian(fin, &this->first_code, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->entry_count, sizeof(USHORT));
	}

	void Character_To_Glyph_Index_Mapping_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg);
		ifstream_read_big_endian(fin, &this->table_version_number, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->number_of_encoding_tables, sizeof(USHORT));
		if(this->number_of_encoding_tables <= 0) return;
		this->encoding_table_entries = new Encoding_Table_Entry[this->number_of_encoding_tables];
		for(int i = 0; i < this->number_of_encoding_tables; ++i){
			ifstream_read_big_endian(fin, &this->encoding_table_entries[i].platform_id, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->encoding_table_entries[i].platform_specific_encoding_id, sizeof(USHORT));
			ifstream_read_big_endian(fin, &this->encoding_table_entries[i].byte_offset, sizeof(ULONG));
		}
		this->encoding_tables = new Encoding_Table*[this->number_of_encoding_tables];
		this->load_encoding_tables(fin, entry->offset);
	}

	void Character_To_Glyph_Index_Mapping_Table::load_encoding_tables(ifstream &fin, ULONG base){
		for(int i = 0; i < this->number_of_encoding_tables; ++i){
			fin.seekg(base + this->encoding_table_entries[i].byte_offset, ios::beg);
			Encoding_Table encoding_table(fin);
			switch(encoding_table.format){
			case FORMAT_BYTE_ENCODING_TABLE:{
				this->encoding_tables[i] = new Byte_Encoding_Table(encoding_table, fin);
				break;
											}
			case FORMAT_HIGH_BYTE_MAPPING_THROUGH_TABLE:{
				this->encoding_tables[i] = new High_Byte_Mapping_Through_Table(encoding_table, fin);
				break;
														}
			case FORMAT_SEGMENT_MAPPING_TO_DELTA_VALUES:{
				this->encoding_tables[i] = new Segment_Mapping_To_Delta_Values(encoding_table, fin);
				break;
														}
			case FORMAT_TRIMMED_TABLE_MAPPING:{
				this->encoding_tables[i] = new Trimmed_Table_Mapping(encoding_table, fin);
				break;
											  }
			}
			this->encoding_table_entries[i].encoding_table = this->encoding_tables[i];
		}
	}

	// FIXME: This Function uses sequential search. Maybe it can be more efficient.
	// FIXME: It's wrong to presume the ith encoding table corresponds to the ith entry.
	Encoding_Table* Character_To_Glyph_Index_Mapping_Table::get_encoding_table(USHORT platform_id, USHORT encoding_id){
		Encoding_Table* t = NULL;
		for(int i = 0; i < this->number_of_encoding_tables; ++i){
			if(this->encoding_table_entries[i].platform_id == platform_id &&
				this->encoding_table_entries[i].platform_specific_encoding_id == encoding_id){
					t = this->encoding_table_entries[i].encoding_table;
			}
		}
		return t;
	}

	USHORT Character_To_Glyph_Index_Mapping_Table::get_glyph_index(USHORT platform_id, USHORT encoding_id, USHORT ch){
		USHORT index = 0;
		Encoding_Table* encoding_table = get_encoding_table(platform_id, encoding_id);
		if(encoding_id){
			index = encoding_table->get_glyph_index(ch);
		}
		return index;
	}
}