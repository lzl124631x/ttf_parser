#include "stdafx.h"
#include "mem_stream.h"

namespace ttf_dll {

void MemStream::ReadBigEndian(void *buf, unsigned size) {
  // FIXME: need invalid parameter handling
  for(int i = size - 1; i >= 0; --i) {
    Read((char*)buf + i, sizeof(char));
  }
}

void MemStream::ReadBigEndian(void *buf, unsigned unit_size, unsigned unit_num) {
  // FIXME: need invalid parameter handling
  for(unsigned i = 0; i < unit_num; ++i) {
    for(int j = unit_size - 1; j >= 0; --j) {
      Read((char*)buf + j, sizeof(char));
    }
    buf = (char*)buf + unit_size;
  }
}

void MemStream::Read(void *buf, size_t size) {
  char *tmp = (char*)buf;             // FIXME: need invalid parameter handling
  for(unsigned i = 0; i < size; ++i) {
    *tmp++ = *cursor_++;
  }
}

void MemStream::Seek(int offset) {
  cursor_ += offset;
  if(cursor_ < start_) {
    cursor_ = start_;
  } else if(cursor_ > end_) {
    cursor_ = end_;
  }
}

int MemStream::Tell() {
  return cursor_ - start_;
}

} // namespace ttf_dll