#include "stdafx.h"
#include "TTF_Table.h"

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