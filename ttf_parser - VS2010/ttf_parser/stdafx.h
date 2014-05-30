#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "Type.h"
#include <iostream>
#include <fstream>

namespace ttf_dll {

#define BIT(b)  (1 << b)

// All OpenType/TrueType fonts use Motorola-style byte ordering (Big Endian).
void ifstream_read_big_endian(std::ifstream &fin, void *buf, unsigned size);
void ifstream_read_big_endian(std::ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num);
#define FREAD(f, b) ifstream_read_big_endian((f), (b), sizeof(*(b)))
#define FREAD_N(f, b, n) ifstream_read_big_endian((f), (b), sizeof(*(b)), (n))

} // namespace ttf_dll