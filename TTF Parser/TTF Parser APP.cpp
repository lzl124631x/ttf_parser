// TTF Parser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "True_Type_Font.h"


int _tmain(int argc, _TCHAR* argv[])
{
	True_Type_Font ttf;
	ttf.load_path("D:/TTF/msyh.ttf");
	ttf.get_glyph_outline('Œ“');
	system("pause");
	return 0;
}

