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

/*
This file contains charset classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_CHARSET_H__
#define __GKC_CHARSET_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcCharset.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

//------------------------------------------------------------------------------

// CodePage

class CodePage
{
public:
	CodePage() throw()
	{
		m_cp.szName[0] = 0;
		m_cp.uLength = 0;
	}
	CodePage(const CodePage& src) throw() : m_cp(src.m_cp)
	{
	}
	~CodePage() throw()
	{
	}

	CodePage& operator=(const CodePage& src) throw()
	{
		if( this != &src ) {
			m_cp = src.m_cp;
		}
		return *this;
	}

	ConstStringS GetDescription() const throw()
	{
		return ConstStringS(m_cp.szName, m_cp.uLength);
	}

private:
	code_page m_cp;

	friend class LocaleHelper;
};

// LocaleInfo

class LocaleInfo
{
public:
	LocaleInfo() throw()
	{
		m_info.szLanguageName[0] = 0;
		m_info.uLanguageLength = 0;
		m_info.szRegionName[0] = 0;
		m_info.uRegionLength = 0;
	}
	LocaleInfo(const LocaleInfo& src) throw() : m_info(src.m_info)
	{
	}
	~LocaleInfo() throw()
	{
	}

	LocaleInfo& operator=(const LocaleInfo& src) throw()
	{
		if( this != &src ) {
			m_info = src.m_info;
		}
		return *this;
	}

	ConstStringS GetLanguage() const throw()
	{
		return ConstStringS(m_info.szLanguageName, m_info.uLanguageLength);
	}
	ConstStringS GetRegion() const throw()
	{
		return ConstStringS(m_info.szRegionName, m_info.uRegionLength);
	}

private:
	locale_info m_info;

	friend class LocaleHelper;
};

// LocaleHelper

class LocaleHelper
{
public:
	static void GetCurrentLocale(OUT LocaleInfo& info, OUT CodePage& cp) throw()
	{
		get_current_locale(info.m_info, cp.m_cp);
	}
};

//------------------------------------------------------------------------------
//Conversion Classes

// internal

template <typename Tchar1, typename Tchar2, class TInit>
class _CS_CVT
{
private:
	typedef _CS_CVT<Tchar1, Tchar2, TInit>  thisClass;

public:
	_CS_CVT() throw()
	{
	}
	template <typename... Args>
	explicit _CS_CVT(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		Init(str, rv_forward<Args>(args)...);
	}
	_CS_CVT(const thisClass& src) throw() : m_str(src.m_str)
	{
	}
	_CS_CVT(thisClass&& src) throw() : m_str(rv_forward(src.m_str))
	{
	}
	~_CS_CVT() throw()
	{
	}

	thisClass& operator=(const thisClass& src) throw()
	{
		m_str = src.m_str;
		return *this;
	}
	thisClass& operator=(thisClass&& src) throw()
	{
		m_str = rv_forward(src.m_str);
		return *this;
	}

	StringT<Tchar2> GetV() throw()
	{
		return StringT<Tchar2>(m_str);
	}
	ConstStringT<Tchar2> GetC() const throw()
	{
		return StringUtilHelper::To_ConstString(m_str);
	}

	template <typename... Args>
	void Init(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		if( !TInit::DoInit(cc, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		cvt_string(str, cc, m_str);
	}

protected:
	static void cvt_string(const ConstStringT<Tchar1>& str, charset_converter& cc, StringT<Tchar2>& strDest)  //may throw
	{
		uintptr uSrcSize = str.GetCount();
		uintptr uSrcTotalBytes = SafeOperators::MultiplyThrow(uSrcSize + 1, (uintptr)sizeof(Tchar1));  //may throw
		if( uSrcTotalBytes > (uintptr)(Limits<int>::Max) )
			throw OverflowException();
		uintptr uDestTotalBytes = SafeOperators::MultiplyThrow(uSrcSize + 1, (uintptr)sizeof(Tchar2));  //may throw
		if( uDestTotalBytes > (uintptr)(Limits<int>::Max) )
			throw OverflowException();
		//try
		if( strDest.IsBlockNull() )
			strDest = StringHelper::MakeEmptyString<Tchar2>(MemoryHelper::GetCrtMemoryManager());  //may throw
		strDest.SetLength(uDestTotalBytes / sizeof(Tchar2) - 1);  //may throw
		mem_zero(ShareArrayHelper::GetInternalPointer(strDest), uDestTotalBytes);
		while( true ) {
			int ret = cc.Convert(ConstArrayHelper::GetInternalPointer(str), (int)(uSrcTotalBytes - sizeof(Tchar1)),
								ShareArrayHelper::GetInternalPointer(strDest), (int)(uDestTotalBytes - sizeof(Tchar2)));
			if( ret < 0 )
				throw Exception(CallResult(SystemCallResults::Fail));
			if( ret > 0 ) {
				strDest.RecalcLength();
				return ;
			}
			//realloc
			uDestTotalBytes = SafeOperators::MultiplyThrow((uintptr)uDestTotalBytes - (uintptr)sizeof(Tchar2), (uintptr)2);  //may throw
			uDestTotalBytes = SafeOperators::AddThrow(uDestTotalBytes, (uintptr)sizeof(Tchar2));  //may throw
			if( uDestTotalBytes > (uintptr)(Limits<int>::Max) )
				throw OverflowException();
			strDest.SetLength(uDestTotalBytes / sizeof(Tchar2) - 1);  //may throw
			mem_zero(ShareArrayHelper::GetInternalPointer(strDest), uDestTotalBytes);
		} // end while
	}

protected:
	StringT<Tchar2> m_str;
};

class _Init_A2A
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP1, const ConstStringS& strCP2) throw()
	{
		return cc.InitializeAnsiToAnsi(ConstArrayHelper::GetInternalPointer(strCP1), ConstArrayHelper::GetInternalPointer(strCP2));
	}
};
class _Init_A2U
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeAnsiToUTF8(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_A2H
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeAnsiToUTF16(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_A2L
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeAnsiToUTF32(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_U2A
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeUTF8ToAnsi(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_U2H
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF8ToUTF16();
	}
};
class _Init_U2L
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF8ToUTF32();
	}
};
class _Init_H2A
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeUTF16ToAnsi(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_H2U
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF16ToUTF8();
	}
};
class _Init_H2L
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF16ToUTF32();
	}
};
class _Init_L2A
{
public:
	static bool DoInit(charset_converter& cc, const ConstStringS& strCP) throw()
	{
		return cc.InitializeUTF32ToAnsi(ConstArrayHelper::GetInternalPointer(strCP));
	}
};
class _Init_L2U
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF32ToUTF8();
	}
};
class _Init_L2H
{
public:
	static bool DoInit(charset_converter& cc) throw()
	{
		return cc.InitializeUTF32ToUTF16();
	}
};

// CS_A2A
typedef _CS_CVT<CharA, CharA, _Init_A2A>  CS_A2A;
// CS_A2U
typedef _CS_CVT<CharA, CharA, _Init_A2U>  CS_A2U;
// CS_A2H
typedef _CS_CVT<CharA, CharH, _Init_A2H>  CS_A2H;
// CS_A2L
typedef _CS_CVT<CharA, CharL, _Init_A2L>  CS_A2L;
// CS_U2A
typedef _CS_CVT<CharA, CharA, _Init_U2A>  CS_U2A;
// CS_U2H
typedef _CS_CVT<CharA, CharH, _Init_U2H>  CS_U2H;
// CS_U2L
typedef _CS_CVT<CharA, CharL, _Init_U2L>  CS_U2L;
// CS_H2A
typedef _CS_CVT<CharH, CharA, _Init_H2A>  CS_H2A;
// CS_H2U
typedef _CS_CVT<CharH, CharA, _Init_H2U>  CS_H2U;
// CS_H2L
typedef _CS_CVT<CharH, CharL, _Init_H2L>  CS_H2L;
// CS_L2A
typedef _CS_CVT<CharL, CharA, _Init_L2A>  CS_L2A;
// CS_L2U
typedef _CS_CVT<CharL, CharA, _Init_L2U>  CS_L2U;
// CS_L2H
typedef _CS_CVT<CharL, CharH, _Init_L2H>  CS_L2H;

//system

template <typename Tchar1, typename Tchar2, class TInit>
class _CS_CVT_S : public _CS_CVT<Tchar1, Tchar2, TInit>
{
private:
	typedef _CS_CVT<Tchar1, Tchar2, TInit>    baseClass;
	typedef _CS_CVT_S<Tchar1, Tchar2, TInit>  thisClass;

public:
	_CS_CVT_S() throw()
	{
	}
	template <typename... Args>
	explicit _CS_CVT_S(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		Init(str, rv_forward<Args>(args)...);
	}
	template <typename... Args>
	explicit _CS_CVT_S(const StringT<Tchar1>& str, Args&&... args)  //may throw
	{
		Init(str, rv_forward<Args>(args)...);
	}
	_CS_CVT_S(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src)),
											m_strC(src.m_strC)
	{
	}
	_CS_CVT_S(thisClass&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src))),
										m_strC(rv_forward(src.m_strC))
	{
	}
	~_CS_CVT_S() throw()
	{
	}

	thisClass& operator=(const thisClass& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		m_strC = src.m_strC;
		return *this;
	}
	thisClass& operator=(thisClass&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		m_strC = rv_forward(src.m_strC);
		return *this;
	}

	StringT<Tchar2> GetV()  //may throw
	{
		if( m_strC.IsNull() )
			return baseClass::GetV();
		if( (baseClass::m_str).IsBlockNull() ) {
			baseClass::m_str = StringHelper::MakeEmptyString<Tchar2>(MemoryHelper::GetCrtMemoryManager());  //may throw
			StringUtilHelper::MakeString(m_strC, baseClass::m_str);  //may throw
		}
		return StringT<Tchar2>(baseClass::m_str);
	}
	ConstStringT<Tchar2> GetC() const throw()
	{
		return m_strC.IsNull() ? baseClass::GetC() : m_strC;
	}

	template <typename... Args>
	void Init(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		bool bSame;
		if( !TInit::DoInit(cc, bSame, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		if( bSame ) {
			baseClass::m_str.Release();
			ConstArrayHelper::SetInternalPointer<Tchar2>((const Tchar2*)ConstArrayHelper::GetInternalPointer<Tchar1>(str), str.GetCount(), m_strC);
			return ;
		}
		baseClass::cvt_string(str, cc, baseClass::m_str);
		ConstArrayHelper::SetInternalPointer<Tchar2>(NULL, 0, m_strC);
	}
	template <typename... Args>
	void Init(const StringT<Tchar1>& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		bool bSame;
		if( !TInit::DoInit(cc, bSame, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		if( bSame )
			baseClass::m_str = *((const StringT<Tchar2>*)(&str));
		else
			baseClass::cvt_string(StringUtilHelper::To_ConstString(str), cc, baseClass::m_str);
		ConstArrayHelper::SetInternalPointer<Tchar2>(NULL, 0, m_strC);
	}

private:
	ConstStringT<Tchar2> m_strC;
};

class _Init_A2S
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame, const ConstStringS& strCP) throw()
	{
		return cc.InitializeAnsiToSystem(ConstArrayHelper::GetInternalPointer(strCP), bSame);
	}
};
class _Init_U2S
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeUTF8ToSystem(bSame);
	}
};
class _Init_H2S
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeUTF16ToSystem(bSame);
	}
};
class _Init_L2S
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeUTF32ToSystem(bSame);
	}
};
class _Init_S2A
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame, const ConstStringS& strCP) throw()
	{
		return cc.InitializeSystemToAnsi(ConstArrayHelper::GetInternalPointer(strCP), bSame);
	}
};
class _Init_S2U
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeSystemToUTF8(bSame);
	}
};
class _Init_S2H
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeSystemToUTF16(bSame);
	}
};
class _Init_S2L
{
public:
	static bool DoInit(charset_converter& cc, bool& bSame) throw()
	{
		return cc.InitializeSystemToUTF32(bSame);
	}
};

// CS_A2S
typedef _CS_CVT_S<CharA, CharS, _Init_A2S>  CS_A2S;
// CS_U2S
typedef _CS_CVT_S<CharA, CharS, _Init_U2S>  CS_U2S;
// CS_H2S
typedef _CS_CVT_S<CharH, CharS, _Init_H2S>  CS_H2S;
// CS_L2S
typedef _CS_CVT_S<CharL, CharS, _Init_L2S>  CS_L2S;
// CS_S2A
typedef _CS_CVT_S<CharS, CharA, _Init_S2A>  CS_S2A;
// CS_S2U
typedef _CS_CVT_S<CharS, CharA, _Init_S2U>  CS_S2U;
// CS_S2H
typedef _CS_CVT_S<CharS, CharH, _Init_S2H>  CS_S2H;
// CS_S2L
typedef _CS_CVT_S<CharS, CharL, _Init_S2L>  CS_S2L;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CHARSET_H__
////////////////////////////////////////////////////////////////////////////////
