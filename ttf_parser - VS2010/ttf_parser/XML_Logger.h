#ifndef XML_LOGGER_H
#define XML_LOGGER_H

#include <stdio.h>

namespace ttf_dll {

// The `XmlLogger` is bind with an output file and prints formatted data to
// it. All the print functions will print indentation in front of the
// formatted data.
class XmlLogger {
 public:
  // Opens an output file whose path is specified by `out_path`.
  XmlLogger(const char *out_path);
  // Closes the output file.
  ~XmlLogger();
  // Returns true if failed to open output file.
  bool Error() const { return error_; }
  // Increases the indentation level.
  void IncreaseIndent() { indent_ += 2; }
  // Decreases the indentation level.
  void DecreaseIndent() { indent_ -= 2; }
  // Prints to file the formatted data.
  void Print(const char *format, ...) const;
  // Prints to file the formatted data followed by line feed character '\n'.
  void Println(const char *format, ...) const;
  // Prints to file the formatted data as a double-byte string.
  void WPrint(const wchar_t *format, ...) const;
  // Prints to file the formatted data as a double-byte string followed by
  // line feed character '\n'.
  void WPrintln(const wchar_t *format, ...) const;
  // Prints to file the `array` whose length is specified by `len`. The
  // argument `format` specifies how each element in `array` is printed.
  template<typename T>
  void PrintArray(void *array, size_t len, char *format) const {
    PrintIndent();
    T *ptr = (T*)array;
    for(size_t i = 0; i < len; ++i) {
      fprintf(fp_, format, *ptr++);
      if(i == len - 1) {            // end of array
        fprintf(fp_, "\n");
      } else if((i + 1) % 10) {     // in the middle of line
        fprintf(fp_, "  ");
      } else {                      // end of line
        fprintf(fp_, "\n");
        PrintIndent();
      }
    }
  }
 private:
  // Prints spaces as indentation.
  void PrintIndent() const;

  // Indentation level.
  unsigned  indent_;
  // Pointer to output file.
  FILE      *fp_;
  // True if failed to open output file.
  bool      error_;
};

} // namespace ttf_dll

#endif