/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////
//internal header
//
//This file can use exceptions
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------

// code_page

struct code_page
{
public:
	enum { NAME_LEN = 28 };

public:
	CharS szName[NAME_LEN];
    uint  uLength;
};

// locale_info

struct locale_info
{
public:
	enum { LANGUAGE_LEN = 12, REGION_LEN = 12 };

public:
	CharS szLanguageName[LANGUAGE_LEN];  //ISO 639
    uint  uLanguageLength;
	CharS szRegionName[REGION_LEN];      //ISO 3166
    uint  uRegionLength;
};

// constant strings for code page

#define CODEPAGE_437     _S("437")    // IBM437       [OEM United States]
#define CODEPAGE_500     _S("500")    // IBM500       [IBM EBCDIC International]
#define CODEPAGE_850     _S("850")    // ibm850       [OEM Multilingual Latin 1; Western European (DOS)]
#define CODEPAGE_852     _S("852")    // ibm852       [OEM Latin 2; Central European (DOS)]
#define CODEPAGE_855     _S("855")    // IBM855       [OEM Cyrillic (primarily Russian)]
#define CODEPAGE_857     _S("857")    // ibm857       [OEM Turkish; Turkish (DOS)]
#define CODEPAGE_860     _S("860")    // IBM860       [OEM Portuguese; Portuguese (DOS)]
#define CODEPAGE_861     _S("861")    // ibm861       [OEM Icelandic; Icelandic (DOS)]
#define CODEPAGE_862     _S("862")    // DOS-862      [OEM Hebrew; Hebrew (DOS)]
#define CODEPAGE_863     _S("863")    // IBM863       [OEM French Canadian; French Canadian (DOS)]
#define CODEPAGE_864     _S("864")    // IBM864       [OEM Arabic; Arabic (864)]
#define CODEPAGE_865     _S("865")    // IBM865       [OEM Nordic; Nordic (DOS)]
#define CODEPAGE_866     _S("866")    // cp866        [OEM Russian; Cyrillic (DOS)]
#define CODEPAGE_869     _S("869")    // ibm869       [OEM Modern Greek; Greek, Modern (DOS)]
#define CODEPAGE_874     _S("874")    // windows-874  [ANSI/OEM Thai (ISO 8859-11); Thai (Windows)]
#define CODEPAGE_1026    _S("1026")   // IBM1026      [IBM EBCDIC Turkish (Latin 5)]
#define CODEPAGE_1047    _S("1047")   // IBM01047     [IBM EBCDIC Latin 1/Open System]

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/sys_locale.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/sys_locale.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
