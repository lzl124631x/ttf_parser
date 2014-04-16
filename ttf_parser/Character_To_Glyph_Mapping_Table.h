#ifndef CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#define CHARACTER_TO_GLYPH_MAPPING_TABLE_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* cmap ***********************************/
namespace ttf_dll{
	class Encoding_Table{
	public:
		USHORT	format;
		USHORT	length;
		USHORT	language;
		/*
		Note on the language field in 'cmap' subtables:
		The language field must be set to zero for all cmap subtables whose platform IDs are other than Macintosh (platform ID 1).
		For cmap subtables whose platform IDs are Macintosh, set this field to the Macintosh language ID of the cmap subtable plus one,
		or to zero if the cmap subtable is not language-specific.
		For example, a Mac OS Turkish cmap subtable must set this field to 18, since the Macintosh language ID for Turkish is 17.
		A Mac OS Roman cmap subtable must set this field to 0, since Mac OS Roman is not a language-specific encoding.
		*/
		Encoding_Table(ifstream &fin);
		Encoding_Table(Encoding_Table &et): format(et.format), length(et.length), language(et.language){}
		virtual USHORT get_glyph_index(USHORT ch) { return 0; } // FIXME: shouldn't this function be pure virtual?
		virtual void dump_info(FILE *fp, size_t indent){}; // FIXME: pure virtual?
	};

	class Encoding_Table_Entry{
	public:
		USHORT	platform_id;
		USHORT	encoding_id;
		ULONG	byte_offset;	// from beginning of table to the subtable for this encoding.
		Encoding_Table *encoding_table;
	};

	class DLL_API Character_To_Glyph_Index_Mapping_Table{
	private:
		USHORT	table_version_number;
		USHORT	number_of_encoding_tables;
		Encoding_Table_Entry *encoding_table_entries;
		Encoding_Table **encoding_tables;
		Encoding_Table *get_encoding_table(USHORT platform_id, USHORT encoding_id);
	public:
		void load_table(Table_Directory_Entry *entry, ifstream &fin);
		void load_encoding_tables(ifstream &fin, ULONG base);
		USHORT get_glyph_index(USHORT platform_id, USHORT encoding_id, USHORT ch);
		void dump_info(FILE *fp, size_t indent);
	};

#define FORMAT_BYTE_ENCODING_TABLE 0
	// This is the Apple standard character to glyph index mapping table.
	class Byte_Encoding_Table: public Encoding_Table{
	public:
		BYTE	glyph_id_array[256];
		Byte_Encoding_Table(Encoding_Table &et, ifstream &fin);
		USHORT get_glyph_index(USHORT ch);
		void dump_info(FILE *fp, size_t indent);
	};
	// This is a simple 1 to 1 mapping of character codes to glyph indices.
	// The glyph set is limited to 256. Note that if this format is used to index into a larger glyph set,
	// only the first 256 glyphs will be accessible.

#define FORMAT_HIGH_BYTE_MAPPING_THROUGH_TABLE 2
	class High_Byte_Mapping_Through_Table: public Encoding_Table{
	public:
		USHORT		subheader_keys[256];
		struct Subheader{
			USHORT	first_code;
			USHORT	entry_count;
			SHORT	id_delta;
			USHORT	id_range_offset;
		};
		Subheader	*subheaders;
		USHORT		glyph_id_array;
		High_Byte_Mapping_Through_Table(Encoding_Table &et, ifstream &fin);
		void dump_info(FILE *fp, size_t indent);
	};
	// This subtable is useful for the national character code standards used for Japanese, Chinese, and Korean characters.
	// These code standards use a mixed 8/16-bit encoding, in which certain byte values signal the first byte of a 2-byte character
	// (but these values are also legal as the second byte of a 2-byte character).
	// In addition, even for the 2-byte characters, the mapping of character codes to glyph index values depends heavily on the first byte.
	// Consequently, the table begins with an array that maps the first byte to a 4-word subHeader.
	// For 2-byte character codes, the subHeader is used to map the second byte's value through a subArray, as described below.
	// When processing mixed 8/16-bit text, subHeader 0 is special: it is used for single-byte character codes.
	// When subHeader zero is used, a second byte is not needed; the single byte value is mapped through the subArray.
	

#define FORMAT_SEGMENT_MAPPING_TO_DELTA_VALUES 4
	class Segment_Mapping_To_Delta_Values: public Encoding_Table{
	public:
		USHORT	seg_countx2;
		USHORT	search_range;
		USHORT	entry_selector;
		USHORT	range_shift;
		USHORT	*end_count;/*[seg_count]*/
		USHORT	reserved_pad;
		USHORT	*start_count;/*[seg_count]*/
		SHORT	*id_delta;/*[seg_count]*/
		USHORT	*id_range_offset;/*[seg_count]*/
		USHORT	*glyph_id_array;/*[var_len]*/
		Segment_Mapping_To_Delta_Values(Encoding_Table &et, ifstream &fin);
		~Segment_Mapping_To_Delta_Values(){
			delete[] end_count, start_count, id_delta, id_range_offset, glyph_id_array;
		}
		USHORT get_glyph_index(USHORT ch);
		void dump_info(FILE *fp, size_t indent);
	};

#define FORMAT_TRIMMED_TABLE_MAPPING 6
	class Trimmed_Table_Mapping: public Encoding_Table{
	public:
		USHORT	first_code;
		USHORT	entry_count;
		USHORT	*glyph_id_array;/*[entry_count]*/
		Trimmed_Table_Mapping(Encoding_Table &et, ifstream &fin);
		~Trimmed_Table_Mapping(){
			delete[] glyph_id_array;
		}
		void dump_info(FILE *fp, size_t indent);
	};
	// The firstCode and entryCount values specify a subrange (beginning at firstCode,length = entryCount) within the range of possible character codes.
	// Codes outside of this subrange are mapped to glyph index 0.
	// The offset of the code (from the first code) within this subrange is used as index to the glyphIdArray, which provides the glyph index value.
}

#endif