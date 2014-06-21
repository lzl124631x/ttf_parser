#include <cstring>
#include <tchar.h>
#include "resource.h"
#include "true_type_font.h"

int _tmain(int argc, TCHAR *argv[]) {
  ttf_dll::TrueTypeFont ttf;
  TCHAR xml_path[MAX_PATH] = {0};
  // The first argument is the name of this executable. Skip it.
  // The trailing arguments shall be paths of ttf files.
  for (int i = 1; i < argc; ++i) {
    TCHAR *path_name = argv[i];
    ttf.Open(argv[i]);
    if (ttf.is_open()) {
      _tcscpy_s(xml_path, path_name);
      _tcscat_s(xml_path, _T(".xml"));
      ttf.DumpTtf(xml_path);
    }
    ttf.Close();
  }
  return 0;
}
