// TTF Parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Type.h"
#include "True_Type_Font.h"

int _tmain(int argc, _TCHAR* argv[])
{
	True_Type_Font ttf;
	ttf.load_path("D:/TTF/msyh.TTF");
	ttf.head.dump_info();
	USHORT ch = 0;
	for(int i = 0, ch = L'Œ“'; i < 100; ++i){
		ttf.get_glyph_outline(ch++);
	}
	//system("pause");
	return 0;
}