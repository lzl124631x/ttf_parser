#include "stdafx.h"
#include "Character_To_Glyph_Mapping_Table.h"

namespace ttf_dll{
/******************************* Character_To_Glyph_Index_Mapping_Table ***********************************/
  void Character_To_Glyph_Index_Mapping_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    streampos base = fin.tellg();
    FREAD(fin, &table_version_number);
    FREAD(fin, &number_of_encoding_tables);
    if(number_of_encoding_tables <= 0) return; // FIXME: futile?
    // Read encoding table entries.
    encoding_records = new Encoding_Record[number_of_encoding_tables];
    Encoding_Record *record = encoding_records;
    for(int i = 0; i < number_of_encoding_tables; ++i, ++record){
      record->load_entry(fin);
    }
    record = encoding_records;
    for(int i = 0; i < number_of_encoding_tables; ++i, ++record){
      record->load_encoding_table(fin, base);
    }
    // Since all the encoding records are stored sequentially, they are read first.
    // I cannot guarantee the encoding tables are store sequentially, so they are read in a separate loop.
    // FIXME: Maybe the above two loops can be combined without efficiency decline.
  }

  // FIXME: This Function uses sequential search. Maybe it can be more efficient.
  Encoding_Table* Character_To_Glyph_Index_Mapping_Table::get_encoding_table(USHORT platform_id, USHORT encoding_id){
    Encoding_Table* t = NULL;
    for(int i = 0; i < number_of_encoding_tables; ++i){
      if(encoding_records[i].platform_id == platform_id &&
        encoding_records[i].encoding_id == encoding_id){
          t = encoding_records[i].encoding_table;
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

/******************************* Encoding_Record ***********************************/
  void Encoding_Record::load_encoding_table(ifstream &fin, streampos base){
    fin.seekg(base);
    fin.seekg(byte_offset, ios::cur); // FXIME: fstream is random-access stream! Simplify the code!
    USHORT format = 0;
    FREAD(fin, &format);
    fin.seekg(-(int)sizeof(format), ios::cur); // Rewind to let 'Encodint_Table' read the format.
    switch(format){
    case BYTE_ENCODING_TABLE:{
      encoding_table = new Byte_Encoding_Table(fin);
      break;
                             }
    case HIGH_BYTE_MAPPING_THROUGH_TABLE:{
      encoding_table = new High_Byte_Mapping_Through_Table(fin);
      break;
                                         }
    case SEGMENT_MAPPING_TO_DELTA_VALUES:{
      encoding_table = new Segment_Mapping_To_Delta_Values(fin);
      break;
                                         }
    case TRIMMED_TABLE_MAPPING:{
      encoding_table = new Trimmed_Table_Mapping(fin);
      break;
                               }
    }
  }

  void Encoding_Record::load_entry(ifstream &fin){
    FREAD(fin, &platform_id);
    FREAD(fin, &encoding_id);
    FREAD(fin, &byte_offset);
  }

/******************************* Encoding_Table ***********************************/
  Encoding_Table::Encoding_Table(ifstream &fin){
    FREAD(fin, &format);
    FREAD(fin, &length);
    FREAD(fin, &language);
  }

/******************************* Byte_Encoding_Table ***********************************/
  Byte_Encoding_Table::Byte_Encoding_Table(ifstream &fin) : Encoding_Table(fin) {
      FREAD_N(fin, glyph_id_array, 256);
  }

  USHORT Byte_Encoding_Table::get_glyph_index(USHORT ch){
    if(ch >= 256) return 0; // ERROR: ch should be no more than 256
    return glyph_id_array[ch];
  }

/******************************* High_Byte_Mapping_Through_Table ***********************************/
  High_Byte_Mapping_Through_Table::High_Byte_Mapping_Through_Table(ifstream &fin) : Encoding_Table(fin) {
      FREAD_N(fin, subheader_keys, 256);
      //FIXME: not finished
  }

  USHORT High_Byte_Mapping_Through_Table::get_glyph_index(USHORT ch){
    return 0; // FIXME: not finished
  }

/******************************* Segment_Mapping_To_Delta_Values ***********************************/
  Segment_Mapping_To_Delta_Values::Segment_Mapping_To_Delta_Values(ifstream &fin) : Encoding_Table(fin) {
      FREAD(fin, &seg_countx2);
      FREAD(fin, &search_range);
      FREAD(fin, &entry_selector);
      FREAD(fin, &range_shift);
      USHORT seg_count = seg_countx2 >> 1;

      end_count = new USHORT[seg_count];
      FREAD_N(fin, end_count, seg_count);
      FREAD(fin, &reserved_pad);
      start_count = new USHORT[seg_count];
      FREAD_N(fin, start_count, seg_count);
      id_delta = new SHORT[seg_count];
      FREAD_N(fin, id_delta, seg_count);
      id_range_offset = new USHORT[seg_count];
      FREAD_N(fin, id_range_offset, seg_count);
      USHORT var_len = length - sizeof(USHORT) * (8 + (seg_countx2 << 1));
      glyph_id_array = new USHORT[var_len];
      FREAD_N(fin, glyph_id_array, var_len);
  }

  USHORT Segment_Mapping_To_Delta_Values::get_glyph_index(USHORT ch){
    int i = 0;
    while(end_count[i] != 0xFFFF && end_count[i] < ch){
      ++i;
    }
    USHORT glyph_index = 0;
    if(start_count[i] <= ch){
      if(id_range_offset[i]){
        glyph_index = *(glyph_id_array + (id_range_offset[i] >> 1) + (ch - start_count[i])
          - (&id_range_offset[seg_countx2 >> 1] - &id_range_offset[i]));
      }else{
        glyph_index = id_delta[i] + ch;
      }
    }
    return glyph_index;
  }

/******************************* Trimmed_Table_Mapping ***********************************/
  Trimmed_Table_Mapping::Trimmed_Table_Mapping(ifstream &fin) : Encoding_Table(fin) {
      FREAD(fin, &first_code);
      FREAD(fin, &entry_count);
      glyph_id_array = new USHORT[entry_count];
      FREAD_N(fin, glyph_id_array, entry_count);
  }

  USHORT Trimmed_Table_Mapping::get_glyph_index(USHORT ch){
    return 0; // FIXME: not finished
  }

  void Character_To_Glyph_Index_Mapping_Table::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<cmap tableVersion=\"0x%08x\" numberOfEncodingTables=\"%d\">\n",
      table_version_number, number_of_encoding_tables);
    ++indent;
    for(int i = 0; i < number_of_encoding_tables; ++i){
      Encoding_Record entry = encoding_records[i];
      INDENT(fp, indent); fprintf(fp, "<encodingTable>\n");
      ++indent;
      INDENT(fp, indent); fprintf(fp, "<platformID value=\"%u\"/>\n", entry.platform_id);
      INDENT(fp, indent); fprintf(fp, "<encodingID value=\"%u\"/>\n", entry.encoding_id);
      INDENT(fp, indent); fprintf(fp, "<offset value=\"%lu\"/>\n", entry.byte_offset);
      --indent;
      INDENT(fp, indent); fprintf(fp, "</encodingTable>\n");
    }
    for(int i = 0; i < number_of_encoding_tables; ++i){
      encoding_records[i].encoding_table->dump_info(fp, indent);
    }
    --indent;
    INDENT(fp, indent); fprintf(fp, "</cmap>\n");
  }

  template<typename T>
  void dump_array(FILE *fp, size_t indent, T *array, size_t len, char *format){
    INDENT(fp, indent);
    for(size_t i = 0; i < len; ++i){
      fprintf(fp, format, array[i]);
      if((i + 1) % 10){
        fprintf(fp, "\t");
      }else if(i != len - 1){
        fprintf(fp, "\n");
        INDENT(fp, indent);
      }
    }
    fprintf(fp, "\n");
  }

  void Segment_Mapping_To_Delta_Values::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<cmap_format_4>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format);
    INDENT(fp, indent); fprintf(fp, "<length value=\"%u\"/>\n", length);
    INDENT(fp, indent); fprintf(fp, "<language value=\"%u\"/>\n", language);
    INDENT(fp, indent); fprintf(fp, "<segCountX2 value=\"%u\"/>\n", seg_countx2);
    INDENT(fp, indent); fprintf(fp, "<searchRange value=\"%u\"/>\n", search_range);
    INDENT(fp, indent); fprintf(fp, "<entrySelector value=\"%u\"/>\n", entry_selector);
    INDENT(fp, indent); fprintf(fp, "<rangeShift value=\"%u\"/>\n", range_shift);

    INDENT(fp, indent); fprintf(fp, "<endCount>\n");
    dump_array<USHORT>(fp, indent + 1, end_count, (seg_countx2 >> 1), "%+5u");
    INDENT(fp, indent); fprintf(fp, "</endCount>\n");

    INDENT(fp, indent); fprintf(fp, "<reservedPad value=\"%u\"/>\n", reserved_pad);

    INDENT(fp, indent); fprintf(fp, "<startCount>\n");
    dump_array<USHORT>(fp, indent + 1, start_count, (seg_countx2 >> 1), "%+5u");
    INDENT(fp, indent); fprintf(fp, "</startCount>\n");

    INDENT(fp, indent); fprintf(fp, "<idDelta>\n");
    dump_array<SHORT>(fp, indent + 1, id_delta, (seg_countx2 >> 1), "%+5d");
    INDENT(fp, indent); fprintf(fp, "</idDelta>\n");

    INDENT(fp, indent); fprintf(fp, "<idRangeOffset>\n");
    dump_array<USHORT>(fp, indent + 1, id_range_offset, (seg_countx2 >> 1), "%+5u");
    INDENT(fp, indent); fprintf(fp, "</idRangeOffset>\n");

    INDENT(fp, indent); fprintf(fp, "<glyphIdArray>\n");
    dump_array<USHORT>(fp, indent + 1, glyph_id_array, (seg_countx2 >> 1), "%+5u");//FIXME: the glyph_id_array is not dumped out with the right length.
    INDENT(fp, indent); fprintf(fp, "</glyphIdArray>\n");

    --indent;
    INDENT(fp, indent); fprintf(fp, "</cmap_format_4>\n");
  }

  void Byte_Encoding_Table::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<cmap_format_0>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format); // FIXME: these three lines should be encapsulated into a function for Encoding_Table
    INDENT(fp, indent); fprintf(fp, "<length value=\"%u\"/>\n", length);
    INDENT(fp, indent); fprintf(fp, "<language value=\"%u\"/>\n", language);
    INDENT(fp, indent); fprintf(fp, "<glyphIdArray>\n");
    dump_array<BYTE>(fp, indent + 1, glyph_id_array, 256, "%+5u");
    INDENT(fp, indent); fprintf(fp, "</glyphIdArray>\n");
    --indent;
    INDENT(fp, indent); fprintf(fp, "</cmap_format_0>\n");
  }

  void High_Byte_Mapping_Through_Table::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<cmap_format_2>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format); // FIXME: these three lines should be encapsulated into a function for Encoding_Table
    INDENT(fp, indent); fprintf(fp, "<length value=\"%u\"/>\n", length);
    INDENT(fp, indent); fprintf(fp, "<language value=\"%u\"/>\n", language);
    dump_array<USHORT>(fp, indent + 1, subheader_keys, 256, "%+5u");
    --indent;
    INDENT(fp, indent); fprintf(fp, "</cmap_format_2>\n");
  }

  void Trimmed_Table_Mapping::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<cmap_format_6>\n");
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format); // FIXME: these three lines should be encapsulated into a function for Encoding_Table
    INDENT(fp, indent); fprintf(fp, "<length value=\"%u\"/>\n", length);
    INDENT(fp, indent); fprintf(fp, "<language value=\"%u\"/>\n", language);
    fprintf(fp, "<firstCode value=\"%u\"/>\n", first_code);
    fprintf(fp, "<entryCount value=\"%u\"/>\n", entry_count);
    dump_array<USHORT>(fp, indent + 1, glyph_id_array, entry_count, "%+5u");
    --indent;
    INDENT(fp, indent); fprintf(fp, "</cmap_format_6>\n");
  }
}