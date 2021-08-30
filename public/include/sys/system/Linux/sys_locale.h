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

//Linux

//------------------------------------------------------------------------------
// charset converter

namespace _os_charset_converter {

class conv_base
{
public:
	conv_base() noexcept : m_c((iconv_t)-1)
	{
	}
	conv_base(const conv_base&) = delete;
	conv_base& operator=(const conv_base&) = delete;
	~conv_base() noexcept
	{
		Close();
	}

	void Close() noexcept
	{
		if( m_c != (iconv_t)-1 ) {
			int ret = ::iconv_close(m_c);
			(void)ret;
			assert( ret == 0 );
			m_c = (iconv_t)-1;
		}
	}

	bool Open(const char* szFrom, const char* szTo) noexcept
	{
		assert( m_c == (iconv_t)-1 );
		m_c = ::iconv_open(szTo, szFrom);
		return m_c != (iconv_t)-1;
	}

	size_t Convert(char** ppSrc, size_t* pSrcBytes, char** ppDest, size_t* pDestBytes) noexcept
	{
		assert( m_c != (iconv_t)-1 );
		return ::iconv(m_c, ppSrc, pSrcBytes, ppDest, pDestBytes);
	}

private:
	iconv_t  m_c;
};

template <class Tstring>
inline void _realloc_string(Tstring& str, size_t& uTotal, size_t& uRest, char*& pNext)
{
	size_t uOffset = uTotal - uRest;
	uTotal = safe_operators::MultiplyThrow<uintptr, uintptr>(uTotal, (size_t)2);  //may throw
	str.SetLength(uTotal / sizeof(typename Tstring::EType));  //may throw
	uRest = uTotal - uOffset;
	pNext = (char*)&str.GetBegin().get_Value() + uOffset;
}

template <typename Tchar, class Tstring>
inline bool cvt_string(const char* szFrom, const char* szTo, const Tchar* sz, int len, Tstring& str)
{
	assert( len >= 0 );

	size_t uSrcBytes = safe_operators::MultiplyThrow<uintptr, uintptr>((size_t)len, sizeof(Tchar));  // may throw
	size_t uDestBytes = safe_operators::MultiplyThrow<uintptr, uintptr>((size_t)len, sizeof(typename Tstring::EType));  //may throw

	conv_base cvt;
	if ( !cvt.Open(szFrom, szTo) )
		return false;

	str.SetLength(uDestBytes / sizeof(typename Tstring::EType));  //may throw
	//reset state (no check)
	cvt.Convert(NULL, NULL, NULL, NULL);
	char* ps = (char*)sz;
	char* pd = (char*)&str.GetBegin().get_Value();
	size_t uRest = uDestBytes;
	while ( true ) {
		size_t res = cvt.Convert(&ps, &uSrcBytes, &pd, &uRest);
		if ( res != (size_t)-1 ) {
			do {
				res = cvt.Convert(NULL, NULL, &pd, &uRest);
				if ( res != (size_t)-1 )
					break;
				if ( errno != E2BIG )
					return false;
				_realloc_string(str, uDestBytes, uRest, pd);  //may throw
			} while ( true );
			str.SetLength((uDestBytes - uRest) / sizeof(typename Tstring::EType));
			break;
		}
		if ( errno != E2BIG )
			return false;
		_realloc_string(str, uDestBytes, uRest, pd);  //may throw
	}

	//BOM
	if ( sizeof(typename Tstring::EType) > 1 ) {
		if ( str.GetLength() != 0 && str[0] == 0xFEFF )
			str.RemoveAt(0);
	}
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
		if( compare_string(szFrom, szTo) == 0 )
			return false;
		return _os_charset_converter::cvt_string(szFrom, szTo, szSrc, len, strDest);  //may throw
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
		if( compare_string(szFrom, "UTF-8") == 0 )
			return false;
		return _os_charset_converter::cvt_string(szFrom, "UTF-8", szSrc, len, strDest);  //may throw
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
		if( compare_string(szFrom, "UTF-16") == 0 )
			return false;
		return _os_charset_converter::cvt_string(szFrom, "UTF-16", szSrc, len, strDest);  //may throw
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
		if( compare_string(szFrom, "UTF-32") == 0 )
			return false;
		return _os_charset_converter::cvt_string(szFrom, "UTF-32", szSrc, len, strDest);  //may throw
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
		if( compare_string(szTo, "UTF-8") == 0 )
			return false;
		return _os_charset_converter::cvt_string("UTF-8", szTo, szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-8", "UTF-16", szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-8", "UTF-32", szSrc, len, strDest);  //may throw
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
		if( compare_string(szTo, "UTF-16") == 0 )
			return false;
		return _os_charset_converter::cvt_string("UTF-16", szTo, szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-16", "UTF-8", szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-16", "UTF-32", szSrc, len, strDest);  //may throw
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
		if( compare_string(szTo, "UTF-32") == 0 )
			return false;
		return _os_charset_converter::cvt_string("UTF-32", szTo, szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-32", "UTF-8", szSrc, len, strDest);  //may throw
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
		return _os_charset_converter::cvt_string("UTF-32", "UTF-16", szSrc, len, strDest);  //may throw
	}
};

//operating system related

// A2S
template <class Tstring>
class charset_converter_trait_A2S : public charset_converter_trait_A2U<Tstring>
{
public:
	static const bool c_bSame = false;
};

// U2S
template <class Tstring>
class charset_converter_trait_U2S
{
public:
	typedef char_a   Echar1;
	typedef Tstring  Estring2;

	static const bool c_bSame = true;

	static bool Convert(const char_a* szSrc, int len, Tstring& strDest)
	{
		return true;
	}
};

// H2S
template <class Tstring>
class charset_converter_trait_H2S : public charset_converter_trait_H2U<Tstring>
{
public:
	static const bool c_bSame = false;
};

// L2S
template <class Tstring>
class charset_converter_trait_L2S : public charset_converter_trait_L2U<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2A
template <class Tstring>
class charset_converter_trait_S2A : public charset_converter_trait_U2A<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2U
template <class Tstring>
class charset_converter_trait_S2U
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

// S2H
template <class Tstring>
class charset_converter_trait_S2H : public charset_converter_trait_U2H<Tstring>
{
public:
	static const bool c_bSame = false;
};

// S2L
template <class Tstring>
class charset_converter_trait_S2L : public charset_converter_trait_U2L<Tstring>
{
public:
	static const bool c_bSame = false;
};

////////////////////////////////////////////////////////////////////////////////
