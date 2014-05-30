#ifndef TYPE_H
#define TYPE_H

#include "Xml_Logger.h"

namespace ttf_dll {

#define DEL(p)  do{ delete (p); (p) = NULL; }while(0)
#define DEL_A(p) do{ delete[] (p); (p) = NULL; }while(0)
#define DEL_T(p, t) do{ delete[] ((t*)p); (p) = NULL; }while(0)

// FIXME: the following export definition might not be suitable to be here.
#ifdef TTF_PARSER_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

typedef signed char         INT8;
typedef signed short        INT16;
typedef signed int          INT32;
typedef signed __int64      INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned __int64    UINT64;

typedef UINT8     BYTE;           // 8-bit unsigned integer
typedef INT8      CHAR;           // 8-bit signed integer
typedef UINT16    USHORT;         // 16-bit unsigned integer
typedef INT16     SHORT;          // 16-bit signed integer
typedef UINT32    ULONG;          // 32-bit unsigned integer
typedef INT32     LONG;           // 32-bit signed integer
typedef UINT32    FIXED;          // 32-bit signed fixed-point number (16.16)
//typedef FUINT ?                 // Smallest measurable distance in the em space
typedef SHORT     FWORD;          // 16-bit signed integer (SHORT) that describes a quantity in FUnits
typedef USHORT    UFWORD;         // Unsigned 16-bit integer (USHORT) that describes a quantity in FUnits
typedef UINT16    F2DOT14;        // 16-bit signed fixed number with the low 14 bits of fraction (2.14).
typedef INT64     LONG_DATE_TIME; // Date represented in number of seconds since 12:00 midnight, January 1, 1904.
// The value is represented as a signed 64-bit integer.
typedef UINT16    GLYPH_ID;	      // Glyph index number, same as uint16 (length = 16 bits)
typedef UINT16    OFFSET;         // Offset to a table, same as uint16 (length = 16 bits), NULL offset = 0x0000

} // namespace ttf_dll

#endif