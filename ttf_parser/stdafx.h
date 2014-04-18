#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "Type.h"
#include <iostream>
#include <fstream>
using namespace std;
#include <assert.h>

namespace ttf_dll{
  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned size);
  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num);
#define FREAD(f, b) ifstream_read_big_endian((f), (b), sizeof(*(b)))
#define FREAD_N(f, b, n) ifstream_read_big_endian((f), (b), sizeof(*(b)), (n))

  #define INDENT(fp, indent) for(size_t i = 0; i < (indent); ++i) fprintf((fp), "\t")

  // MEMO: Template function/class could not be separated into .h and .cpp files! Just define it in .h file!
  template<typename T>
  void dump_array(FILE *fp, size_t indent, T *array, size_t len, char *format){
    INDENT(fp, indent);
    for(size_t i = 0; i < len; ++i){
      fprintf(fp, format, array[i]);
      if((i + 1) % 10){
        fprintf(fp, "\t");
      }else if(i != len - 1){
        fprintf(fp, "\n");
        INDENT(fp, indent);
      }
    }
    fprintf(fp, "\n");
  }
}