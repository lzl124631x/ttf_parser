#include "stdafx.h"
#include "Character_To_Glyph_Mapping_Table.h"

namespace ttf_dll{
/******************************* Character_To_Glyph_Index_Mapping_Table ***********************************/
  void Character_To_Glyph_Index_Mapping_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
    fin.seekg(entry->offset, ios::beg);
    streampos base = fin.tellg();
    FREAD(fin, &table_version_number);
    FREAD(fin, &num_encoding_tables);
    if(num_encoding_tables == 0) return; // FIXME: futile?
    // Read encoding table entries.
    encoding_records = new Encoding_Record[num_encoding_tables];
    Encoding_Record *record = encoding_records;
    for(int i = 0; i < num_encoding_tables; ++i, ++record){
      record->load_entry(fin);
    }
    record = encoding_records;
    for(int i = 0; i < num_encoding_tables; ++i, ++record){
      record->load_encoding_table(fin, base);
    }
    // Since all the encoding records are stored sequentially, they are read first.
    // I cannot guarantee the encoding tables are store sequentially, so they are read in a separate loop.
    // FIXME: Maybe the above two loops can be combined without efficiency decline.
  }

  // FIXME: This Function uses sequential search. Maybe it can be more efficient.
  Encoding_Table* Character_To_Glyph_Index_Mapping_Table::get_encoding_table(USHORT platform_id, USHORT encoding_id){
    Encoding_Table* t = NULL;
    for(int i = 0; i < num_encoding_tables; ++i){
      if(encoding_records[i].platform_id == platform_id &&
        encoding_records[i].encoding_id == encoding_id){
          t = encoding_records[i].encoding_table;
      }
    }
    return t;
  }

  GLYPH_ID Character_To_Glyph_Index_Mapping_Table::get_glyph_index(USHORT platform_id, USHORT encoding_id, USHORT ch){
    GLYPH_ID glyph_index = 0;
    Encoding_Table* encoding_table = get_encoding_table(platform_id, encoding_id);
    if(encoding_table){
      glyph_index = encoding_table->get_glyph_index(ch);
    }
    return glyph_index;
  }

/******************************* Encoding_Record ***********************************/
  void Encoding_Record::load_encoding_table(ifstream &fin, streampos base){
    fin.seekg(base);
    fin.seekg(byte_offset, ios::cur); // FIXME: fstream is random-access stream! Simplify the code!
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

  void Encoding_Table::dump_table_header(FILE *fp, size_t indent){
    IND_FP("<format value=\"%u\"/>\n", format);
    IND_FP("<length value=\"%u\"/>\n", length);
    IND_FP("<language value=\"%u\"/>\n", language);
  }

/******************************* Byte_Encoding_Table ***********************************/
  Byte_Encoding_Table::Byte_Encoding_Table(ifstream &fin) : Encoding_Table(fin) {
      FREAD_N(fin, glyph_id_array, 256);
  }

  GLYPH_ID Byte_Encoding_Table::get_glyph_index(USHORT ch){
    if(ch >= 256) return 0; // ERROR: ch should be no more than 256
    return glyph_id_array[ch];
  }

/******************************* High_Byte_Mapping_Through_Table ***********************************/
  High_Byte_Mapping_Through_Table::High_Byte_Mapping_Through_Table(ifstream &fin) : Encoding_Table(fin) {
      FREAD_N(fin, subheader_keys, 256);
      //FIXME: not finished
  }

  GLYPH_ID High_Byte_Mapping_Through_Table::get_glyph_index(USHORT ch){
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

  GLYPH_ID Segment_Mapping_To_Delta_Values::get_glyph_index(USHORT ch){
    int i = 0;
    while(end_count[i] != 0xFFFF && end_count[i] < ch){
      ++i;
    }
    GLYPH_ID glyph_index = 0;
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

  GLYPH_ID Trimmed_Table_Mapping::get_glyph_index(USHORT ch){
    return 0; // FIXME: not finished
  }

  void Character_To_Glyph_Index_Mapping_Table::dump_info(FILE *fp, size_t indent){
    IND_FP("<cmap tableVersion=\"0x%08x\" numberOfEncodingTables=\"%d\">\n",
      table_version_number, num_encoding_tables);
    ++indent;
    for(int i = 0; i < num_encoding_tables; ++i){
      Encoding_Record *record = &encoding_records[i];
      IND_FP("<encodingRecord>\n");
      ++indent;
      IND_FP("<platformID value=\"%u\"/>\n", record->platform_id);
      IND_FP("<encodingID value=\"%u\"/>\n", record->encoding_id);
      IND_FP("<offset value=\"%lu\"/>\n", record->byte_offset);
      --indent;
      IND_FP("</encodingRecord>\n");
    }
    for(int i = 0; i < num_encoding_tables; ++i){
      encoding_records[i].encoding_table->dump_info(fp, indent);
    }
    --indent;
    IND_FP("</cmap>\n");
  }

  void Segment_Mapping_To_Delta_Values::dump_info(FILE *fp, size_t indent){
    IND_FP("<cmap_format_4>\n");
    ++indent;
    dump_table_header(fp, indent);
    IND_FP("<segCountX2 value=\"%u\"/>\n", seg_countx2);
    IND_FP("<searchRange value=\"%u\"/>\n", search_range);
    IND_FP("<entrySelector value=\"%u\"/>\n", entry_selector);
    IND_FP("<rangeShift value=\"%u\"/>\n", range_shift);

    IND_FP("<endCount>\n");
    dump_array<USHORT>(fp, indent + 1, end_count, (seg_countx2 >> 1), "%8u");
    IND_FP("</endCount>\n");

    IND_FP("<reservedPad value=\"%u\"/>\n", reserved_pad);

    IND_FP("<startCount>\n");
    dump_array<USHORT>(fp, indent + 1, start_count, (seg_countx2 >> 1), "%8u");
    IND_FP("</startCount>\n");

    IND_FP("<idDelta>\n");
    dump_array<SHORT>(fp, indent + 1, id_delta, (seg_countx2 >> 1), "%8d");
    IND_FP("</idDelta>\n");

    IND_FP("<idRangeOffset>\n");
    dump_array<USHORT>(fp, indent + 1, id_range_offset, (seg_countx2 >> 1), "%8u");
    IND_FP("</idRangeOffset>\n");

    IND_FP("<glyphIdArray>\n");
    dump_array<USHORT>(fp, indent + 1, glyph_id_array, (seg_countx2 >> 1), "%8u");//FIXME: the glyph_id_array is not dumped out with the right length.
    IND_FP("</glyphIdArray>\n");

    --indent;
    IND_FP("</cmap_format_4>\n");
  }

  void Byte_Encoding_Table::dump_info(FILE *fp, size_t indent){
    IND_FP("<cmap_format_0>\n");
    ++indent;
    dump_table_header(fp, indent);
    IND_FP("<glyphIdArray>\n");
    dump_array<BYTE>(fp, indent + 1, glyph_id_array, 256, "%8u");
    IND_FP("</glyphIdArray>\n");
    --indent;
    IND_FP("</cmap_format_0>\n");
  }

  void High_Byte_Mapping_Through_Table::dump_info(FILE *fp, size_t indent){
    IND_FP("<cmap_format_2>\n");
    ++indent;
    dump_table_header(fp, indent);
    dump_array<USHORT>(fp, indent + 1, subheader_keys, 256, "%8u");
    --indent;
    IND_FP("</cmap_format_2>\n");
  }

  void Trimmed_Table_Mapping::dump_info(FILE *fp, size_t indent){
    IND_FP("<cmap_format_6>\n");
    ++indent;
    dump_table_header(fp, indent);
    IND_FP("<firstCode value=\"%u\"/>\n", first_code);
    IND_FP("<entryCount value=\"%u\"/>\n", entry_count);
    dump_array<USHORT>(fp, indent + 1, glyph_id_array, entry_count, "%8u");
    --indent;
    IND_FP("</cmap_format_6>\n");
  }
}