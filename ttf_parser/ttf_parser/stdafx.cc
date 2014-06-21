#include "stdafx.h"

namespace ttf_dll {

void IFStreamReadBigEndian(std::ifstream &fin, void *buf, unsigned unit_size,
                           unsigned unit_num) {
  char *ptr = reinterpret_cast<char*>(buf);
  for (unsigned i = 0; i < unit_num; ++i) {
    for (int j = unit_size - 1; j >= 0; --j) {
      fin.read(ptr + j, sizeof(char));
    }
    ptr += unit_size;
  }
}

} // namespace ttf_dll