#include "stdafx.h"

namespace ttf_dll {

void IFStreamReadBigEndian(std::ifstream &fin, void *buf, unsigned unit_size, unsigned unit_num) {
  for(unsigned i = 0; i < unit_num; ++i) {
    for(int j = unit_size - 1; j >= 0; --j) {
      fin.read((char*)buf + j, sizeof(char));
    }
    buf = (char*)buf + unit_size;
  }
}

} // namespace ttf_dll