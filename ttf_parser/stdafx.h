// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "Type.h"
#include <iostream>
#include <fstream>
using namespace std;

namespace ttf_dll{
	void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned size);
	void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num);
}

// TODO: reference additional headers your program requires here
