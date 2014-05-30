#ifndef XML_LOGGER_H
#define XML_LOGGER_H

#include <stdio.h>

namespace ttf_dll {

class Xml_Logger {
 public:
  // Opens an output file whose path is specified by `out_path`.
  Xml_Logger(const char *out_path);
  // Closes the output file.
  ~Xml_Logger();
  // Returns true if failed to open output file.
  bool error() const { return err; }
  // Increases the indentation level.
  void inc_indent() { indent += 2; }
  // Decreases the indentation level.
  void dec_indent() { indent -= 2; }
  // Prints to file the formatted data.
  void print(const char *format, ...) const;
  // Prints to file the formatted data followed by line feed character '\n'.
  void println(const char *format, ...) const;
  // Prints to file the formatted data as a double-byte string.
  void wprint(const wchar_t *format, ...) const;
  // Prints to file the formatted data as a double-byte string followed by
  // line feed character '\n'.
  void wprintln(const wchar_t *format, ...) const;
  // Prints to file the `array` whose length is specified by `len`. The
  // argument `format` specifies how each element in `array` is printed.
  template<typename T>
  void print_array(void *array, size_t len, char *format) const {
    print_indent();
    T *ptr = (T*)array;
    for(size_t i = 0; i < len; ++i) {
      fprintf(fp, format, *ptr++);
      if(i == len - 1) {            // end of array
        fprintf(fp, "\n");
      } else if((i + 1) % 10) {     // in the middle of line
        fprintf(fp, "  ");
      } else {                      // end of line
        fprintf(fp, "\n");
        print_indent();
      }
    }
  }
 private:

  // Prints spaces as indentation.
  void print_indent() const;
  // Indentation level.
  unsigned  indent;
  // Pointer to output file.
  FILE      *fp;
  // True if failed to open output file.
  bool      err;
};

} // namespace ttf_dll

#endif