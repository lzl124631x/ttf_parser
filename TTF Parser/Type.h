#ifndef TYPE_H
#define TYPE_H

typedef signed char         INT8;
typedef signed short        INT16;
typedef signed int          INT32;
typedef signed __int64      INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned __int64    UINT64;

typedef UINT8	BYTE;		// 8-bit unsigned integer
typedef INT8	CHAR;		// 8-bit signed integer
typedef UINT16	USHORT;		// 16-bit unsigned integer
typedef INT16	SHORT;		// 16-bit signed integer
typedef UINT32	ULONG;		// 32-bit unsigned integer
typedef INT32	LONG;		// 32-bit signed integer
typedef UINT32	FIXED;		// 32-bit signed fixed-point number
//typedef FUINT ?			// Smallest measurable distance in the em space
typedef SHORT	FWORD;		// 16-bit signed integer (SHORT) that describes a quantity in FUnits
typedef USHORT	UFWORD;		// Unsigned 16-bit integer (USHORT) that describes a quantity in FUnits
typedef UINT16	F2DOT14;	// 16-bit signed fixed number with the low 14 bits of fraction
typedef INT64	longDateTime;

#endif