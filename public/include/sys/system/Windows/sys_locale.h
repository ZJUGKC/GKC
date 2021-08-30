/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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
// charset converter

namespace _os_charset_conversion {

/*
if iDestLen != 0,
and the return value of ::MultiByteToWideChar or ::WideCharToMultiByte is zero,
::GetLastError() may return ERROR_INSUFFICIENT_BUFFER.
*/

// ansi -> wide string
template <class Tstring>
inline bool _ansi_to_wide(UINT uCP, const char_a* sz, int len, Tstring& str)
{
	assert( len > 0 );
	int ret = ::MultiByteToWideChar(uCP, 0, sz, len, NULL, 0);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	str.SetLength(ret);  //may throw
	ret = ::MultiByteToWideChar(uCP, 0, sz, len, &str.GetBegin().get_Value(), ret);
	assert( ret > 0 );
	return true;
}
// wide string -> ansi
template <class Tstring>
inline bool _wide_to_ansi(UINT uCP, const WCHAR* wsz, int len, Tstring& str)
{
	assert( len > 0 );
	int ret = ::WideCharToMultiByte(uCP, 0, wsz, len, NULL, 0, NULL, NULL);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	str.SetLength(ret);  //may throw
	ret = ::WideCharToMultiByte(uCP, 0, wsz, len, &str.GetBegin().get_Value(), ret, NULL, NULL);
	assert( ret > 0 );
	return true;
}

// ansi -> ansi (MBCS)
template <class Tstring>
inline bool _ansi_to_ansi(UINT uCP1, UINT uCP2, const char_a* sz, int len, Tstring& str)
{
	assert( len > 0 );
	//ansi -> wide
	int ret = ::MultiByteToWideChar(uCP1, 0, sz, len, NULL, 0);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	int w_len = ret;
	uintptr uSize = safe_operators::MultiplyThrow((uintptr)w_len + 1, sizeof(char_h));  //may throw
	_auto_mem spTemp;
	spTemp.Allocate(uSize);  //may throw
	char_h* pTemp = (char_h*)spTemp.GetAddress();
	ret = ::MultiByteToWideChar(uCP1, 0, sz, len, pTemp, w_len);
	assert( ret > 0 );
	//wide -> ansi
	ret = ::WideCharToMultiByte(uCP2, 0, pTemp, w_len, NULL, 0, NULL, NULL);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	str.SetLength(ret);  //may throw
	ret = ::WideCharToMultiByte(uCP2, 0, pTemp, w_len, &str.GetBegin().get_Value(), ret, NULL, NULL);
	assert( ret > 0 );
	return true;
}

// h <--> l
inline bool _is_high_surrogate(char_h ch) throw()
{
	return IS_HIGH_SURROGATE((ushort)ch);
}
inline bool _is_low_surrogate(char_h ch) throw()
{
	return IS_LOW_SURROGATE((ushort)ch);
}
inline char_l _surrogate_to_l(char_h hs, char_h ls) throw()
{
	return (char_l)((((uint)((ushort)hs - HIGH_SURROGATE_START)) << 10)
			+ (uint)((ushort)ls - LOW_SURROGATE_START) + 0x10000UL);
}
inline void _l_to_surrogate(char_l ch, char_h& hs, char_h& ls) throw()
{
	ch -= 0x10000UL;
	hs = (char_h)(HIGH_SURROGATE_START + ((uint)ch >> 10));
	ls = (char_h)(LOW_SURROGATE_START + ((uint)ch & 0x3FFUL));
}

// len>=act
inline bool _calc_utf16_char_num(const char_h* p, int len, int& act) throw()
{
	act = 0;
	const char_h* pe = p + len;
	while( p != pe ) {
		if( _is_low_surrogate(*p) )
			return false;
		if( _is_high_surrogate(*p) ) {
			p ++;
			if( p == pe || !_is_low_surrogate(*p) )
				return false;
		}
		p ++;
		act ++;
	}
	return true;
}
// req: [len, 2*len]
inline bool _calc_utf16_req_num(const char_l* p, int len, int& req) throw()
{
	req = 0;
	const char_l* pe = p + len;
	while( p != pe ) {
		if( (uint)(*p) > 0x10FFFFUL || ((uint)(*p) >= 0xD800 && (uint)(*p) <= 0xDFFF) ) //unicode scalar value
			return false;
		req ++;
		if( req < 0 ) //overflow
			return false;
		if( (uint)(*p) >= 0x10000UL ) {
			req ++;
			if( req < 0 ) //overflow
				return false;
		}
		p ++;
	}
	return true;
}
// ps==pd or ps!=pd
inline void _fill_h_to_l(const char_h* ps, int len, int act_len, char_l* pd) throw()
{
	const char_h* ps1 = ps + len - 1;
	ps --;
	char_l* pd1 = pd + act_len - 1;
	while( ps1 != ps ) {
		if( _is_low_surrogate(*ps1) ) {
			ps1 --;
			*pd1 = _surrogate_to_l(*ps1, *(ps1 + 1));
		}
		else {
			char_h ch = *ps1;
			*pd1 = (char_l)(ushort)ch;
		}
		ps1 --;
		pd1 --;
	}
}
// ps!=pd
inline void _fill_l_to_h(const char_l* ps, int len, char_h* pd) throw()
{
	const char_l* pe = ps + len;
	while( ps != pe ) {
		if( (uint)(*ps) >= 0x10000UL ) {
			_l_to_surrogate(*ps, *pd, *(pd + 1));
			pd ++;
		}
		else {
			*pd = (char_h)(uint)(*ps);
		}
		ps ++;
		pd ++;
	}
}

// h --> l
template <class Tstring>
inline bool _h_to_l(const char_h* sz, int len, Tstring& str)
{
	assert( len > 0 );
	int act_len;
	if( !_calc_utf16_char_num(sz, len, act_len) )
		return false;
	str.SetLength(act_len);  //may throw
	_fill_h_to_l(sz, len, act_len, &str.GetBegin().get_Value());
	return true;
}
// l --> h
template <class Tstring>
inline bool _l_to_h(const char_l* sz, int len, Tstring& str)
{
	assert( len > 0 );
	int req;
	if( !_calc_utf16_req_num(sz, len, req) )
		return false;
	str.SetLength(req);  //may throw
	_fill_l_to_h(sz, len, &str.GetBegin().get_Value());
	return true;
}

// ansi --> l
template <class Tstring>
inline bool _ansi_to_l(UINT uCP, const char_a* sz, int len, Tstring& str)
{
	assert( len > 0 );
	int ret = ::MultiByteToWideChar(uCP, 0, sz, len, NULL, 0);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	int w_len = ret;
	str.SetLength(w_len);  //may throw
	char_l* szDest = &str.GetBegin().get_Value();
	ret = ::MultiByteToWideChar(uCP, 0, sz, len, (char_h*)(void*)szDest, w_len);
	assert( ret > 0 );
	int act_len;
	if( !_calc_utf16_char_num((const char_h*)(const void*)szDest, w_len, act_len) )
		return false;
	_fill_h_to_l((const char_h*)(const void*)szDest, w_len, act_len, szDest);
	str.SetLength(act_len);  //may throw
	return true;
}
// l --> ansi
template <class Tstring>
inline bool _l_to_ansi(UINT uCP, const char_l* sz, int len, Tstring& str)
{
	assert( len > 0 );
	int req;
	if( !_calc_utf16_req_num(sz, len, req) )
		return false;
	uintptr uSize = safe_operators::MultiplyThrow((uintptr)req + 1, sizeof(char_h));  //may throw
	_auto_mem spTemp;
	spTemp.Allocate(uSize);  //may throw
	char_h* pTemp = (char_h*)spTemp.GetAddress();
	_fill_l_to_h(sz, len, pTemp);
	int ret = ::WideCharToMultiByte(uCP, 0, pTemp, req, NULL, 0, NULL, NULL);
	if ( ret <= 0 )
		return false;  //::GetLastError()
	str.SetLength(ret);  //may throw
	ret = ::WideCharToMultiByte(uCP, 0, pTemp, req, &str.GetBegin().get_Value(), ret, NULL, NULL);
	assert( ret > 0 );
	return true;
}

inline bool _to_uint(const char_s* sz, uint& v) throw()
{
	bool bOK;
	char_s* szRet = string_to_value(sz, 10, v, bOK);
	if( !bOK || szRet == sz )
		return false;
	return true;
}

} //namespace

// A2A
template <class Tstring>
class charset_converter_trait_A2A
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest, const const_string_s& strFrom, const const_string_s& strTo)
	{
		const char_s* szFrom = const_array_helper::GetInternalPointer(strFrom);
		const char_s* szTo = const_array_helper::GetInternalPointer(strTo);
		assert( szFrom != NULL && szTo != NULL );
		uint uFrom, uTo;
		if( !_os_charset_conversion::_to_uint(szFrom, uFrom) )
			return false;
		if( !_os_charset_conversion::_to_uint(szTo, uTo) )
			return false;
		if( uFrom == uTo || uFrom == 0 || uTo == 0 )
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_ansi(uFrom, uTo, szSrc, len, strDest);  //may throw
	}
};

// A2U
template <class Tstring>
class charset_converter_trait_A2U
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest, const const_string_s& strFrom)
	{
		const char_s* szFrom = const_array_helper::GetInternalPointer(strFrom);
		assert( szFrom != NULL );
		uint uFrom;
		if( !_os_charset_conversion::_to_uint(szFrom, uFrom) )
			return false;
		if( uFrom == 0 || uFrom == 65001 ) // must not be UTF8
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_ansi(uFrom, CP_UTF8, szSrc, len, strDest);  //may throw
	}
};

// A2H
template <class Tstring>
class charset_converter_trait_A2H
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest, const const_string_s& strFrom)
	{
		const char_s* szFrom = const_array_helper::GetInternalPointer(strFrom);
		assert( szFrom != NULL );
		uint uFrom;
		if( !_os_charset_conversion::_to_uint(szFrom, uFrom) )
			return false;
		if( uFrom == 0 || uFrom == 1200 ) // must not be UTF16
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_wide(uFrom, szSrc, len, strDest);  //may throw
	}
};

// A2L
template <class Tstring>
class charset_converter_trait_A2L
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest, const const_string_s& strFrom)
	{
		const char_s* szFrom = const_array_helper::GetInternalPointer(strFrom);
		assert( szFrom != NULL );
		uint uFrom;
		if( !_os_charset_conversion::_to_uint(szFrom, uFrom) )
			return false;
		if( uFrom == 0 || uFrom == 12000 ) // must not be UTF32
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_l(uFrom, szSrc, len, strDest);  //may throw
	}
};

// U2A
template <class Tstring>
class charset_converter_trait_U2A
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest, const const_string_s& strTo)
	{
		const char_s* szTo = const_array_helper::GetInternalPointer(strTo);
		assert( szTo != NULL );
		uint uTo;
		if( !_os_charset_conversion::_to_uint(szTo, uTo) )
			return false;
		if( uTo == 0 || uTo == 65001 ) // must not be UTF8
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_ansi(CP_UTF8, uTo, szSrc, len, strDest);  //may throw
	}
};

// U2H
template <class Tstring>
class charset_converter_trait_U2H
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_wide(CP_UTF8, szSrc, len, strDest);  //may throw
	}
};

// U2L
template <class Tstring>
class charset_converter_trait_U2L
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_ansi_to_l(CP_UTF8, szSrc, len, strDest);  //may throw
	}
};

// H2A
template <class Tstring>
class charset_converter_trait_H2A
{
public:
	typedef char_h   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_h* szSrc, int len, Tstring& strDest, const const_string_s& strTo)
	{
		const char_s* szTo = const_array_helper::GetInternalPointer(strTo);
		assert( szTo != NULL );
		uint uTo;
		if( !_os_charset_conversion::_to_uint(szTo, uTo) )
			return false;
		if( uTo == 0 || uTo == 1200 ) // must not be UTF16
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_wide_to_ansi(uTo, szSrc, len, strDest);  //may throw
	}
};

// H2U
template <class Tstring>
class charset_converter_trait_H2U
{
public:
	typedef char_h   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_h* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_wide_to_ansi(CP_UTF8, szSrc, len, strDest);  //may throw
	}
};

// H2L
template <class Tstring>
class charset_converter_trait_H2L
{
public:
	typedef char_h   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_h* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_h_to_l(szSrc, len, strDest);  //may throw
	}
};

// L2A
template <class Tstring>
class charset_converter_trait_L2A
{
public:
	typedef char_l   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_l* szSrc, int len, Tstring& strDest, const const_string_s& strTo)
	{
		const char_s* szTo = const_array_helper::GetInternalPointer(strTo);
		assert( szTo != NULL );
		uint uTo;
		if( !_os_charset_conversion::_to_uint(szTo, uTo) )
			return false;
		if( uTo == 0 || uTo == 12000 ) // must not be UTF32
			return false;
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_l_to_ansi(uTo, szSrc, len, strDest);  //may throw
	}
};

// L2U
template <class Tstring>
class charset_converter_trait_L2U
{
public:
	typedef char_l   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_l* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_l_to_ansi(CP_UTF8, szSrc, len, strDest);  //may throw
	}
};

// L2H
template <class Tstring>
class charset_converter_trait_L2H
{
public:
	typedef char_l   Echar1;
	typedef Tstring  Estring2;

	static bool Convert(const char_l* szSrc, int len, Tstring& strDest)
	{
		if( len == 0 ) {
			strDest.Clear();
			return true;
		}
		return _os_charset_conversion::_l_to_h(szSrc, len, strDest);  //may throw
	}
};

//operating system related

// A2S
template <class Tstring>
class charset_converter_trait_A2S : public charset_converter_trait_A2H<Tstring>
{
public:
	static const bool c_bSame = false;
};

// U2S
template <class Tstring>
class charset_converter_trait_U2S : public charset_converter_trait_U2H<Tstring>
{
public:
	static const bool c_bSame = false;
};

// H2S
template <class Tstring>
class charset_converter_trait_H2S
{
public:
	typedef char_h   Echar1;
	typedef Tstring  Estring2;

	static const bool c_bSame = true;

	static bool Convert(const char_h* szSrc, int len, Tstring& strDest)
	{
		return true;
	}
};

// L2S
template <class Tstring>
class charset_converter_trait_L2S : public charset_converter_trait_L2H<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2A
template <class Tstring>
class charset_converter_trait_S2A : public charset_converter_trait_H2A<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2U
template <class Tstring>
class charset_converter_trait_S2U : public charset_converter_trait_H2U<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2H
template <class Tstring>
class charset_converter_trait_S2H
{
public:
	typedef char_s   Echar1;
	typedef Tstring  Estring2;

	static const bool c_bSame = true;

	static bool Convert(const char_s* szSrc, int len, Tstring& strDest)
	{
		return true;
	}
};

// S2L
template <class Tstring>
class charset_converter_trait_S2L : public charset_converter_trait_H2L<Tstring>
{
public:
	static const bool c_bSame = false;
};

////////////////////////////////////////////////////////////////////////////////
