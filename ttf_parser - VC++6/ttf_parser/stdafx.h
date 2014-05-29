#pragma once

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include "Type.h"

namespace ttf_dll{
#define BIT(b)  (1 << b)

// All OpenType/TrueType fonts use Motorola-style byte ordering (Big Endian).
  void ifstream_read_big_endian(std::ifstream &fin, void *buf, unsigned size);
  void ifstream_read_big_endian(std::ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num);
#define FREAD(f, b) ifstream_read_big_endian((f), (b), sizeof(*(b)))
#define FREAD_N(f, b, n) ifstream_read_big_endian((f), (b), sizeof(*(b)), (n))

#define INDENT(fp, indent) for(size_t __i = 0; __i < (indent); ++__i) fprintf((fp), "  ")
// IND_FP = indented fprintf.
// Note that this macro uses two IMPLICIT parameters.
// 1. (FILE *)fp is a pointer to the output file.
// 2. (size_t)indent specifies the indentation level.
//#define IND_FP(format, ...) do{ INDENT(fp, indent); fprintf(fp, format, __VA_ARGS__); }while(0)
// VC doesn't support variadic macro. Variadic function is used instead.
  void IND_FP(FILE *fp, size_t indent, char *format, ...);

  // MEMO: Template function/class could not be separated into .h and .cpp files! Just define it in .h file!
  template<typename T>
  void dump_array(FILE *fp, size_t indent, void *array, size_t len, char *format){
    INDENT(fp, indent);
    T *ptr = (T*)array;
    for(size_t i = 0; i < len; ++i){
      fprintf(fp, format, *ptr++);
      if(i == len - 1){           // end of array
        fprintf(fp, "\n");
      }else if((i + 1) % 10){     // in the middle of line
        fprintf(fp, "  ");
      }else{                      // end of line
        fprintf(fp, "\n");
        INDENT(fp, indent);
      }
    }
  }
}