#include "stdafx.h"
#include "xml_logger.h"
#include <cstdarg>

namespace ttf_dll {

static char g_spaces[64] = {0};

XmlLogger::XmlLogger(const TCHAR *out_path) {
  memset(g_spaces, ' ', sizeof(g_spaces));
  indent_ = 0;
  fp_ = NULL;
  // `fopen_s` returns zero if successful; an error code on failure.
  error_ = (_tfopen_s(&fp_, out_path, _T("w")) != 0);
}

XmlLogger::~XmlLogger() {
  fclose(fp_);
  fp_ = NULL;
}

// FIXME: How to simplify the following four function?
// http://stackoverflow.com/questions/23956548/how-to-simplify-these-variadic-functions
void XmlLogger::Print(const char *format, ...) const {
  PrintIndent();
  va_list args;
  va_start(args, format);
  vfprintf(fp_, format, args);
  va_end(args);
}

void XmlLogger::Println(const char *format, ...) const {
  PrintIndent();
  va_list args;
  va_start(args, format);
  vfprintf(fp_, format, args);
  va_end(args);
  fputc('\n', fp_);
}

void XmlLogger::WPrint(const wchar_t *format, ...) const {
  PrintIndent();
  va_list args;
  va_start(args, format);
  vfwprintf(fp_, format, args);
  va_end(args);
}

void XmlLogger::WPrintln(const wchar_t *format, ...) const {
  PrintIndent();
  va_list args;
  va_start(args, format);
  vfwprintf(fp_, format, args);
  va_end(args);
  fputwc(L'\n', fp_);
}

void XmlLogger::PrintIndent() const {
  g_spaces[indent_] = '\0';
  fprintf(fp_, g_spaces);
  g_spaces[indent_] = ' ';  
}

} // namespace ttf_dll