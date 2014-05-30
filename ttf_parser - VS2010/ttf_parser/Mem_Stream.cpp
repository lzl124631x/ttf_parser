#include "stdafx.h"
#include "Mem_Stream.h"

namespace ttf_dll {

void Mem_Stream::read_big_endian(void *buf, unsigned size) {
  // FIXME: need invalid parameter handling
  for(int i = size - 1; i >= 0; --i) {
    read((char*)buf + i, sizeof(char));
  }
}

void Mem_Stream::read_big_endian(void *buf, unsigned unit_size, unsigned unit_num) {
  // FIXME: need invalid parameter handling
  for(unsigned i = 0; i < unit_num; ++i) {
    for(int j = unit_size - 1; j >= 0; --j) {
      read((char*)buf + j, sizeof(char));
    }
    buf = (char*)buf + unit_size;
  }
}

void Mem_Stream::read(void *buf, size_t size) {
  char *tmp = (char*)buf;             // FIXME: need invalid parameter handling
  for(unsigned i = 0; i < size; ++i) {
    *tmp++ = *cursor++;
  }
}

void Mem_Stream::seek(int offset) {
  cursor += offset;
  if(cursor < start) {
    cursor = start;
  } else if(cursor > end) {
    cursor = end;
  }
}

int Mem_Stream::tell() {
  return cursor - start;
}

void *Mem_Stream::get_cursor() {
  return cursor;
}

} // namespace ttf_dll