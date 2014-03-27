// TTF Parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Type.h"
#include "True_Type_Font.h"

int _tmain(int argc, _TCHAR* argv[])
{
	True_Type_Font ttf;
	ttf.load_path("D:/TTF/STXINGKA.TTF");
	//ÎÒ 25105 Ò» 19968 ÄË 20035 ¶ù 20799
	USHORT ch = 0;
	for(int i = 0, ch = L'ÎÒ'; i < 100; ++i){
		ttf.get_glyph_outline(ch++);
	}
	//system("pause");
	return 0;
}