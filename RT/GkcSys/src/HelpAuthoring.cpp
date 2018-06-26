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
This file contains Help Authoring functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "HelpAuthoring.h"

////////////////////////////////////////////////////////////////////////////////

//functions

//##############################################################################
namespace GKC {
//##############################################################################

// g_charset_cp_map

BEGIN_STATIC_CONST_ARRAY(g_charset_cp_map)
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("US-ASCII")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1252)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Arabic")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1256)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("GB2312")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_936)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Big5")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_950)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-2")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1250)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Greek")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1253)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Hebrew")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1255)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("Shift-JIS")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_932)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-2022-KR")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_949)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-5")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1251)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ISO-8859-9")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, CODEPAGE_1254)
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")),
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S(""))
	END_STATIC_CONST_ARRAY_GROUP_LAST()
END_STATIC_CONST_ARRAY(g_charset_cp_map)

// _Find_CodePage_From_Charset

inline ConstStringS _Find_CodePage_From_Charset(const ConstStringS& strCharset) throw()
{
	ConstArray<_Charset_CP> c_arr(g_charset_cp_map::GetAddress(), g_charset_cp_map::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _Charset_CP& cc = iter.get_Value();
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(strCharset, ConstStringS(cc.m_charset.m_first, cc.m_charset.m_size)) ) {
			return ConstStringS(cc.m_codepage.m_first, cc.m_codepage.m_size);
		}
	}
	return ConstStringS();
}

// g_lcid_short_string_map

BEGIN_STATIC_CONST_ARRAY(g_lcid_short_string_map)
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("en-US")),
		1033,
		0
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ar-SA")),
		1025,
		1
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("zh-CN")),
		2052,
		2
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("zh-TW")),
		1028,
		3
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("cs-CZ")),
		1029,
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("el-GR")),
		1032,
		5
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("he-IL")),
		1037,
		6
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("hu-HU")),
		1038,
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ja-JP")),
		1041,
		7
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ko-KR")),
		1042,
		8
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("pl-PL")),
		1045,
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("ru-RU")),
		1049,
		9
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("sk-SK")),
		1051,
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("sl-SI")),
		1060,
		4
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("tr-TR")),
		1055,
		10
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("bg-BG")),
		1026,
		9
	END_STATIC_CONST_ARRAY_GROUP()
	BEGIN_STATIC_CONST_ARRAY_GROUP()
		IMPLEMENT_CONST_STRING_ENTRY(CharS, _S("")),
		0,
		(uint)-1
	END_STATIC_CONST_ARRAY_GROUP_LAST()
END_STATIC_CONST_ARRAY(g_lcid_short_string_map)

// _Find_LCID_From_ShortString

inline uint _Find_LCID_From_ShortString(const ConstStringS& strShortString, uint& uIndex) throw()
{
	ConstArray<_LCID_ShortString> c_arr(g_lcid_short_string_map::GetAddress(), g_lcid_short_string_map::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _LCID_ShortString& ls = iter.get_Value();
		if( ConstStringCompareTrait<ConstStringS>::IsEQ(strShortString, ConstStringS(ls.m_short_string.m_first, ls.m_short_string.m_size)) ) {
			uIndex = ls.m_uIndex;
			return ls.m_lcid;
		}
	}
	return 0;
}

// _Find_ShortString_From_LCID

inline ConstStringS _Find_ShortString_From_LCID(uint uLCID, uint& uIndex) throw()
{
	ConstArray<_LCID_ShortString> c_arr(g_lcid_short_string_map::GetAddress(), g_lcid_short_string_map::GetCount());
	auto iter(c_arr.GetBegin());
	for( ; iter != c_arr.GetEnd(); iter.MoveNext() ) {
		const _LCID_ShortString& ls = iter.get_Value();
		if( ls.m_lcid == uLCID ) {
			uIndex = ls.m_uIndex;
			return ConstStringS(ls.m_short_string.m_first, ls.m_short_string.m_size);
		}
	}
	return ConstStringS();
}

//##############################################################################
}
//##############################################################################

bool _HelpAuthoring_FindCodePageFromCharset(const GKC::ConstStringS& strCharset, GKC::ConstStringS& strCodePage) throw()
{
	strCodePage = GKC::_Find_CodePage_From_Charset(strCharset);
	return strCodePage.GetCount() != 0;
}

bool _HelpAuthoring_FindLCID(uint uLCID, _HelpLanguageInfo& info) throw()
{
	uint uIndex;
	info.strShortString = GKC::_Find_ShortString_From_LCID(uLCID, uIndex);
	if( info.strShortString.GetCount() == 0 )
		return false;
	info.uLCID = uLCID;
	const GKC::_Charset_CP& cc = (GKC::g_charset_cp_map::GetAddress())[uIndex];
	info.strCharset = GKC::ConstStringS(cc.m_charset.m_first, cc.m_charset.m_size);
	info.strCodePage = GKC::ConstStringS(cc.m_codepage.m_first, cc.m_codepage.m_size);
	return true;
}

bool _HelpAuthoring_FindShortString(const GKC::ConstStringS& strShortString, _HelpLanguageInfo& info) throw()
{
	uint uIndex;
	info.uLCID = GKC::_Find_LCID_From_ShortString(strShortString, uIndex);
	if( info.uLCID == 0 )
		return false;
	info.strShortString = strShortString;
	const GKC::_Charset_CP& cc = (GKC::g_charset_cp_map::GetAddress())[uIndex];
	info.strCharset = GKC::ConstStringS(cc.m_charset.m_first, cc.m_charset.m_size);
	info.strCodePage = GKC::ConstStringS(cc.m_codepage.m_first, cc.m_codepage.m_size);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
