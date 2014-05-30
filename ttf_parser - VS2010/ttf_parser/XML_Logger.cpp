#include "stdafx.h"
#include "XML_Logger.h"
#include <stdarg.h>

namespace ttf_dll {

static char spaces[64] = {0};

Xml_Logger::Xml_Logger(const char *out_path) {
  memset(spaces, ' ', sizeof(spaces));
  indent = 0;
  fp = NULL;
  // `fopen_s` returns zero if successful; an error code on failure.
  err = (fopen_s(&fp, out_path, "w") != 0);
}

Xml_Logger::~Xml_Logger() {
  fclose(fp);
  fp = NULL;
}

// FIXME: How to simplify the following four function?
void Xml_Logger::print(const char *format, ...) const {
  print_indent();
  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);
}

void Xml_Logger::println(const char *format, ...) const {
  print_indent();
  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);
  fputc('\n', fp);
}

void Xml_Logger::wprint(const wchar_t *format, ...) const {
  print_indent();
  va_list args;
  va_start(args, format);
  vfwprintf(fp, format, args);
  va_end(args);
}

void Xml_Logger::wprintln(const wchar_t *format, ...) const {
  print_indent();
  va_list args;
  va_start(args, format);
  vfwprintf(fp, format, args);
  va_end(args);
  fputwc(L'\n', fp);
}

void Xml_Logger::print_indent() const {
  spaces[indent] = '\0';
  fprintf(fp, spaces);
  spaces[indent] = ' ';  
}

} // namespace ttf_dll