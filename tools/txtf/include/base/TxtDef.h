/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TXT_DEF_H__
#define __TXT_DEF_H__
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

// _Find_CodePage_From_Charset

ConstStringS _Find_CodePage_From_Charset(const ConstStringS& strCharset) throw();

// _LCID_ShortString

struct _LCID_ShortString
{
uint m_lcid;
DECLARE_CONST_STRING_STRUCT_MEMBER(m_short_string, CharS)
uint m_uIndex;
};

// g_lcid_short_string

DECLARE_STATIC_CONST_ARRAY(g_lcid_short_string, _LCID_ShortString)

// _Find_LCID_From_ShortString

uint _Find_LCID_From_ShortString(const ConstStringS& strShortString, uint& uIndex) throw();

// _Find_ShortString_From_LCID

ConstStringS _Find_ShortString_From_LCID(uint uLCID, uint& uIndex) throw();

// g_html_github_header

DECLARE_STATIC_CONST_STRING(g_html_github_header, CharA)

// g_html_tail

DECLARE_STATIC_CONST_STRING(g_html_tail, CharA)

// g_html_xhtml_header

DECLARE_STATIC_CONST_STRING(g_html_xhtml_header, CharA)

// g_html_xhtml_tail

DECLARE_STATIC_CONST_STRING(g_html_xhtml_tail, CharA)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
