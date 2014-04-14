#include "stdafx.h"
#include "Naming_Table.h"

namespace ttf_dll{
	void Naming_Table::load_table(Table_Directory_Entry *entry, ifstream &fin){
		fin.seekg(entry->offset, ios::beg); // FIXME: this line could be more efficient.
		ifstream_read_big_endian(fin, &format, sizeof(USHORT));
		ifstream_read_big_endian(fin, &count, sizeof(USHORT));
		ifstream_read_big_endian(fin, &offset, sizeof(USHORT));
		name_records = new Name_Record[count];
		ifstream_read_big_endian(fin, name_records, sizeof(USHORT), 6 * count);
		strings = new char*[count];
		ios::pos_type mark = fin.tellg();
		for(int i = 0; i < count; ++i){
			Name_Record *record = &name_records[i];
			fin.seekg(record->offset, ios::cur);
			if(record->platform_id == Unicode ||
				record->platform_id == Windows &&
				record->encoding_id == Unicode_BMP){
				strings[i] = new char[record->length + 2];
				ifstream_read_big_endian(fin, strings[i], sizeof(wchar_t), (record->length >> 1));
				((wchar_t*)(strings[i]))[record->length >> 1] = 0;
			}else{
				strings[i] = new char[record->length + 1];
				fin.read(strings[i], record->length);
				strings[i][record->length] = 0;
			}
			fin.seekg(mark);
		}
	}

	void Naming_Table::dump_info(FILE *fp, size_t indent){
		INDENT(fp, indent); fprintf(fp, "<name>\n");
		++indent;
		INDENT(fp, indent); fprintf(fp, "<format value=\"%u\"/>\n", format);
		INDENT(fp, indent); fprintf(fp, "<count value=\"%u\"/>\n", count);
		INDENT(fp, indent); fprintf(fp, "<offset value=\"%u\"/>\n", offset);
		for(int i = 0; i < count; ++i){
			char *str = strings[i];
			name_records[i].dump_info(fp, indent, str);
		}
		--indent;
		INDENT(fp, indent); fprintf(fp, "</name>\n");
	}

	void Name_Record::dump_info(FILE *fp, size_t indent, char *str){
		INDENT(fp, indent); fprintf(fp,
			"<nameRecord platformID=\"%u\" encodingID=\"%u\" languageID=\"%u\" nameID=\"%u\" length=\"%u\" offset=\"%u\">\n",
			platform_id, encoding_id, language_id, name_id, length, offset);
		INDENT(fp, indent + 1); 
		if(platform_id == Unicode ||
			platform_id == Windows &&
			encoding_id == Unicode_BMP){
			fwprintf(fp, L"%s\n", str);
		}else{
			fprintf(fp, "%s\n", str);
		}
		INDENT(fp, indent); fprintf(fp, "</nameRecord>\n");
	}
}