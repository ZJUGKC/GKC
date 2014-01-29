////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//basic types

//bool
//char
//byte
typedef unsigned char   byte;
//short
//ushort
typedef unsigned short  ushort;
//int
//uint
typedef unsigned int    uint;
//float
//double

#if defined(OS_WINDOWS)

#include "Windows/base_type.h"

#elif defined(OS_LINUX)

#include "Linux/base_type.h"

#else
	#error error OS type!
#endif

#if defined(CM_X86)

//intptr
typedef int      intptr;
//uintptr
typedef uint     uintptr;

#elif defined(CM_X64)

//intptr
typedef int64    intptr;
//uintptr
typedef uint64   uintptr;

#else
	#error error machine type!
#endif


////////////////////////////////////////////////////////////////////////////////
