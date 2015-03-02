/***************************************************************************
 *  C/C++ header
 *
 *  Common header for Interface of Nature (ION) and ION-related
 *  projects.
 *
 *  Version 4.2.0
 *
 *  Copyright (c) 2014 Mark Karpov
 *
 *  This program is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details.
 **************************************************************************/

#ifndef COMMONIONHEADER
#define COMMONIONHEADER

#ifndef __cplusplus
#warning Some features of this library are not accesible for pure C programs.
#endif

/***************************************************************************
 *  Inclusion of external libraries
 **************************************************************************/

#if defined __cplusplus // C++ variant:
#include <cstdlib>      // standard C library
#include <fstream>      // file streaming
#include <cmath>        // basic math functions
#include <exception>    // we're going to throw something
#include <string>       // std::string
#include <vector>       // std::vector
#include <map>          // std::map
#include <algorithm>    // some algorithms
#include <complex>      // for operations on complex numbers
#include <valarray>     // std::valarray
#else                   // C variant:
#include <stdlib.h>     // standard C library
#include <math.h>       // basic math functions
#endif                  // __cplusplus

/***************************************************************************
 *  DLL Support
 *
 *  When DLLBUILDING is defined, the file is supposed to assist in DLL
 *  building, otherwise this is supposed to help use DLL.
 *
 *  DLL export/import declaration syntax:
 *  DLLFNC is used in declarations of DLL-exported functions,
 *  DLLVRB is used in declarations of DLL-exported variables.
 **************************************************************************/

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#if defined DLLBUILDING
#define DLLFNC DLLEXPORT
#define DLLVAL extern DLLEXPORT
#else
#define DLLFNC DLLIMPORT
#define DLLVAL extern DLLIMPORT
#endif // DLLBUILDING

/***************************************************************************
 *  Numeric Definitions
 **************************************************************************/

// buffer sizes:

#define SMALLBUFFERSIZE  0x0020u
#define MEDIUMBUFFERSIZE 0x0400u
#define HUGEBUFFERSIZE   0x3fffu

// constants: null pointer, upper and lower limits for 32 bit signed int

#define NIL              0x00000000
#define PMAX             0x7fffffff
#define NMAX             (-PMAX - 1)

/***************************************************************************
 *  Namespace Wrapper
 **************************************************************************/

#if defined IONSTDNAMESPACENAME
#define IONSTD IONSTDNAMESPACENAME
#else
#define IONSTD ionstd
#endif // IONSTDNAMESPACENAME

#define BEGINIONSTD namespace IONSTD {
#define ENDIONSTD }

/***************************************************************************
 *  Type Definitions
 **************************************************************************/

#ifdef __cplusplus
BEGINIONSTD
#endif // __cplusplus

typedef char               BYTE;
typedef char               CHAR;
typedef unsigned short int WORD;
typedef unsigned int       DWORD;
typedef signed int         SDWORD;
typedef float              float32;
typedef double             real;
typedef unsigned int       numerator;
typedef char *             nc_str;
typedef const char *       cc_str;

#ifdef __cplusplus
typedef std::string        String;
typedef std::exception     Exception;
typedef std::runtime_error RuntimeError;
typedef std::logic_error   LogicError;
ENDIONSTD
#endif // __cplusplus

/***************************************************************************
 *  Auxiliary Definitions
 **************************************************************************/

#ifdef __cplusplus

// this definition must be placed in MKSTD namespace in cpp file

#define PLACEEXCEPTIONFNCHERE \
String GetRuntimeWhat(String WhatArg, String FileArg, numerator LineArg) \
{ \
    CHAR aLine[SMALLBUFFERSIZE]; \
    std::itoa(LineArg, aLine, 10); \
    WhatArg += " (runtime exception, thrown from "; \
    WhatArg += FileArg + "::" + String(aLine) + ");"; \
    return WhatArg; \
} \
String GetLogicWhat(String WhatArg, String FileArg, numerator LineArg) \
{ \
    CHAR aLine[SMALLBUFFERSIZE]; \
    std::itoa(LineArg, aLine, 10); \
    WhatArg += " (logic exception, thrown from "; \
    WhatArg += FileArg + "::" + String(aLine) + ");"; \
    return WhatArg; \
}

// these are definitions for smart exception throwing

#define THROW_RE(WHAT) \
throw IONSTD::RuntimeError(IONSTD::GetRuntimeWhat(WHAT, __FILE__, __LINE__))
#define THROW_LE(WHAT) \
throw IONSTD::LogicError(IONSTD::GetLogicWhat(WHAT, __FILE__, __LINE__))
#endif // __cplusplus

#endif // COMMONIONHEADER
