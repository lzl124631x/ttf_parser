// stdafx.cpp : source file that includes just the standard includes
// TTF Parser.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

namespace ttf_dll{
  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned size){
    for(int i = size - 1; i >= 0; i--){
      fin.read((char*)buf + i, sizeof(char));
    }
  }

  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num){
    for(unsigned i = 0; i < unit_num; ++i){
      for(int j = unit_size - 1; j >= 0; j--){
        fin.read((char*)buf + j, sizeof(char));
      }
      buf = (char*)buf + unit_size;
    }
  }

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