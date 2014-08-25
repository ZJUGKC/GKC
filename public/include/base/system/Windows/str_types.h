/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
// internal header
////////////////////////////////////////////////////////////////////////////////

//Windows

//------------------------------------------------------------------------------
//character

typedef CHAR           CharA;  //ANSI or UTF8
typedef WCHAR          CharH;  //word or UTF16
typedef unsigned long  CharL;  //long or UTF32

typedef CharH  CharS;  //system type
//for const string
#define _S(x)  L##x

typedef CharH  CharW;  //for wide type, L"..."


////////////////////////////////////////////////////////////////////////////////
