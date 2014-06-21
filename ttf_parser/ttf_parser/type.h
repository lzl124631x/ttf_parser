#ifndef TYPE_H
#define TYPE_H

#include <fstream>
#include "xml_logger.h"

namespace ttf_dll {

#define DEL(p)  do { delete (p); (p) = NULL; } while(0)
#define DEL_A(p) do { delete[] (p); (p) = NULL; } while(0)
#define DEL_T(p, t) do { delete[] ((t*)p); (p) = NULL; } while(0)

// FIXME: the following export definition might not be suitable to be here.
#ifdef TTF_PARSER_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

typedef signed char         Int8;
typedef signed short        Int16;
typedef signed int          Int32;
typedef signed __int64      Int64;
typedef unsigned char       UInt8;
typedef unsigned short      UInt16;
typedef unsigned int        UInt32;
typedef unsigned __int64    UInt64;

typedef UInt8     Byte;           // 8-bit unsigned integer
typedef Int8      Char;           // 8-bit signed integer
typedef UInt16    UShort;         // 16-bit unsigned integer
typedef Int16     Short;          // 16-bit signed integer
typedef UInt32    ULong;          // 32-bit unsigned integer
typedef Int32     Long;           // 32-bit signed integer
typedef UInt32    Fixed;          // 32-bit signed fixed-point number (16.16)
//typedef FUINT ?                 // Smallest measurable distance in the em space
typedef Short     FWord;          // 16-bit signed integer (SHORT) that describes a quantity in FUnits
typedef UShort    UFword;         // Unsigned 16-bit integer (USHORT) that describes a quantity in FUnits
typedef UInt16    F2Dot14;        // 16-bit signed fixed number with the low 14 bits of fraction (2.14).
typedef Int64     LongDateTime;   // Date represented in number of seconds since 12:00 midnight, January 1, 1904.
// The value is represented as a signed 64-bit integer.
typedef UInt16    GlyphId;	      // Glyph index number, same as uint16 (length = 16 bits)
typedef UInt16    Offset;         // Offset to a table, same as uint16 (length = 16 bits), NULL offset = 0x0000

inline float F2Dot14ToFloat(F2Dot14 val) {
  Short mantissa = static_cast<Short>(val) >> 14;
  float fraction = static_cast<float>(val & 16383) / 16384;
  return mantissa + fraction;
}

inline ULong TableRecordTagToULong(const char *tag) {
  return *reinterpret_cast<const ULong*>(tag);
}

} // namespace ttf_dll

#endif