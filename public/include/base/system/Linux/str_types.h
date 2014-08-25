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

//Linux

//------------------------------------------------------------------------------
//character

typedef char            CharA;  //ANSI or UTF8
typedef unsigned short  CharH;  //word or UTF16
typedef wchar_t         CharL;  //long or UTF32

typedef CharA  CharS;  //system type, UTF8
//for const string
#define _S(x)  x

typedef CharL  CharW;  //for wide type, L"..."

////////////////////////////////////////////////////////////////////////////////
