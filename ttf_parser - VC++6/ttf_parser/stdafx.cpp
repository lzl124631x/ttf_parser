#include "stdafx.h"
#include <stdarg.h> // for `va_list`
using namespace std;
namespace ttf_dll{

  void IND_FP(FILE *fp, size_t indent, char *format, ...){
    INDENT(fp, indent);
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vfprintf(fp, format, arg_ptr);
    va_end(arg_ptr);
  }
  
  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned size){
    for(int i = size - 1; i >= 0; --i){
      fin.read((char*)buf + i, sizeof(char));
    }
  }

  void ifstream_read_big_endian(ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num){
    for(unsigned i = 0; i < unit_num; ++i){
      for(int j = unit_size - 1; j >= 0; --j){
        fin.read((char*)buf + j, sizeof(char));
      }
      buf = (char*)buf + unit_size;
    }
  }
}