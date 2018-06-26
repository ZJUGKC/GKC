/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
This file contains global variables for Help Authoring.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __HELP_AUTHORING_H__
#define __HELP_AUTHORING_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Charset_CP

struct _Charset_CP
{
DECLARE_CONST_STRING_STRUCT_MEMBER(m_charset, CharS)
DECLARE_CONST_STRING_STRUCT_MEMBER(m_codepage, CharS)
};

// g_charset_cp_map

DECLARE_STATIC_CONST_ARRAY(g_charset_cp_map, _Charset_CP)

// _LCID_ShortString

struct _LCID_ShortString
{
DECLARE_CONST_STRING_STRUCT_MEMBER(m_short_string, CharS)
uint m_lcid;
uint m_uIndex;
};

// g_lcid_short_string_map

DECLARE_STATIC_CONST_ARRAY(g_lcid_short_string_map, _LCID_ShortString)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
