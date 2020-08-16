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

// cs_string_init<Tstring>
template <class Tstring>
class cs_string_init
{
public:
	static void DoInit(Tstring& str)
	{
		if ( str.IsBlockNull() )
			str = StringHelper::MakeEmptyString<typename Tstring::EType>(MemoryHelper::GetCrtMemoryManager());  //may throw
	}
	static void DoDump(Tstring& str) noexcept
	{
		str.Release();
	}
};
// cs_unique_string_init<Tstring>
template <class Tstring>
class cs_unique_string_init
{
public:
	static void DoInit(Tstring& str)
	{
	}
	static void DoDump(Tstring& str) noexcept
	{
		str.Clear();
	}
};

template <typename Tchar1, class Tstring2, class Thelper, class TStringInit>
class _CS_CVT_BASE
{
private:
	typedef _CS_CVT_BASE<Tchar1, Tstring2, Thelper, TStringInit>  thisClass;

protected:
	_CS_CVT_BASE() throw()
	{
	}
	_CS_CVT_BASE(thisClass&& src) throw() : m_str(rv_forward(src.m_str))
	{
	}
	~_CS_CVT_BASE() throw()
	{
	}

	thisClass& operator=(thisClass&& src) throw()
	{
		m_str = rv_forward(src.m_str);
		return *this;
	}

public:
	Tstring2&& GetV() throw()
	{
		return rv_forward(m_str);
	}

protected:
	static void cvt_string(const ConstStringT<Tchar1>& str, charset_converter& cc, Tstring2& strDest)
	{
		uintptr uSrcSize = str.GetCount();
		uintptr uSrcTotalBytes = SafeOperators::MultiplyThrow(uSrcSize + 1, (uintptr)sizeof(Tchar1));  //may throw
		if( uSrcTotalBytes > (uintptr)(Limits<int>::Max) )
			throw OverflowException();
		uintptr uDestTotalBytes = SafeOperators::MultiplyThrow(uSrcSize + 1, (uintptr)sizeof(typename Tstring2::EType));  //may throw
		if( uDestTotalBytes > (uintptr)(Limits<int>::Max) )
			throw OverflowException();
		//try
		TStringInit::DoInit(strDest);  //may throw
		strDest.SetLength(uDestTotalBytes / sizeof(typename Tstring2::EType) - 1);  //may throw
		mem_zero(Thelper::GetInternalPointer(strDest), uDestTotalBytes);
		while( true ) {
			int ret = cc.Convert(ConstArrayHelper::GetInternalPointer(str), (int)(uSrcTotalBytes - sizeof(Tchar1)),
								Thelper::GetInternalPointer(strDest), (int)(uDestTotalBytes - sizeof(typename Tstring2::EType)));
			if( ret < 0 )
				throw Exception(CallResult(SystemCallResults::Fail));
			if( ret > 0 ) {
				strDest.RecalcLength();
				return ;
			}
			//realloc
			uDestTotalBytes = SafeOperators::MultiplyThrow((uintptr)uDestTotalBytes - (uintptr)sizeof(typename Tstring2::EType), (uintptr)2);  //may throw
			uDestTotalBytes = SafeOperators::AddThrow(uDestTotalBytes, (uintptr)sizeof(typename Tstring2::EType));  //may throw
			if( uDestTotalBytes > (uintptr)(Limits<int>::Max) )
				throw OverflowException();
			strDest.SetLength(uDestTotalBytes / sizeof(typename Tstring2::EType) - 1);  //may throw
			mem_zero(Thelper::GetInternalPointer(strDest), uDestTotalBytes);
		} // end while
	}

private:
	_CS_CVT_BASE(const thisClass& src) throw();
	thisClass& operator=(const thisClass& src) throw();

protected:
	Tstring2 m_str;
};

template <typename Tchar1, class Tstring2, class Thelper, class TStringInit, class TCCInit>
class _CS_CVT : public _CS_CVT_BASE<Tchar1, Tstring2, Thelper, TStringInit>
{
private:
	typedef _CS_CVT_BASE<Tchar1, Tstring2, Thelper, TStringInit>  baseClass;

public:
	_CS_CVT() throw()
	{
	}
	template <typename... Args>
	explicit _CS_CVT(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		Init(str, rv_forward<Args>(args)...);
	}
	_CS_CVT(_CS_CVT&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_CS_CVT() throw()
	{
	}

	_CS_CVT& operator=(_CS_CVT&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	template <typename... Args>
	void Init(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		if( !TCCInit::DoInit(cc, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		baseClass::cvt_string(str, cc, baseClass::m_str);
	}

private:
	_CS_CVT(const _CS_CVT&) throw();
	_CS_CVT& operator=(const _CS_CVT&) throw();
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
typedef _CS_CVT<CharA, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_A2A>  CS_A2A;
// CS_A2U
typedef _CS_CVT<CharA, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_A2U>  CS_A2U;
// CS_A2H
typedef _CS_CVT<CharA, StringT<CharH>, ShareArrayHelper, cs_string_init<StringT<CharH>>, _Init_A2H>  CS_A2H;
// CS_A2L
typedef _CS_CVT<CharA, StringT<CharL>, ShareArrayHelper, cs_string_init<StringT<CharL>>, _Init_A2L>  CS_A2L;
// CS_U2A
typedef _CS_CVT<CharA, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_U2A>  CS_U2A;
// CS_U2H
typedef _CS_CVT<CharA, StringT<CharH>, ShareArrayHelper, cs_string_init<StringT<CharH>>, _Init_U2H>  CS_U2H;
// CS_U2L
typedef _CS_CVT<CharA, StringT<CharL>, ShareArrayHelper, cs_string_init<StringT<CharL>>, _Init_U2L>  CS_U2L;
// CS_H2A
typedef _CS_CVT<CharH, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_H2A>  CS_H2A;
// CS_H2U
typedef _CS_CVT<CharH, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_H2U>  CS_H2U;
// CS_H2L
typedef _CS_CVT<CharH, StringT<CharL>, ShareArrayHelper, cs_string_init<StringT<CharL>>, _Init_H2L>  CS_H2L;
// CS_L2A
typedef _CS_CVT<CharL, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_L2A>  CS_L2A;
// CS_L2U
typedef _CS_CVT<CharL, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_L2U>  CS_L2U;
// CS_L2H
typedef _CS_CVT<CharL, StringT<CharH>, ShareArrayHelper, cs_string_init<StringT<CharH>>, _Init_L2H>  CS_L2H;

// CSU_A2A
typedef _CS_CVT<CharA, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_A2A>  CSU_A2A;
// CSU_A2U
typedef _CS_CVT<CharA, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_A2U>  CSU_A2U;
// CSU_A2H
typedef _CS_CVT<CharA, UniqueStringT<CharH>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharH>>, _Init_A2H>  CSU_A2H;
// CSU_A2L
typedef _CS_CVT<CharA, UniqueStringT<CharL>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharL>>, _Init_A2L>  CSU_A2L;
// CSU_U2A
typedef _CS_CVT<CharA, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_U2A>  CSU_U2A;
// CSU_U2H
typedef _CS_CVT<CharA, UniqueStringT<CharH>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharH>>, _Init_U2H>  CSU_U2H;
// CSU_U2L
typedef _CS_CVT<CharA, UniqueStringT<CharL>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharL>>, _Init_U2L>  CSU_U2L;
// CSU_H2A
typedef _CS_CVT<CharH, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_H2A>  CSU_H2A;
// CSU_H2U
typedef _CS_CVT<CharH, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_H2U>  CSU_H2U;
// CSU_H2L
typedef _CS_CVT<CharH, UniqueStringT<CharL>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharL>>, _Init_H2L>  CSU_H2L;
// CSU_L2A
typedef _CS_CVT<CharL, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_L2A>  CSU_L2A;
// CSU_L2U
typedef _CS_CVT<CharL, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_L2U>  CSU_L2U;
// CSU_L2H
typedef _CS_CVT<CharL, UniqueStringT<CharH>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharH>>, _Init_L2H>  CSU_L2H;

//system

template <typename Tchar1, class Tstring2, class Thelper, class TStringInit, class TCCInit>
class _CS_CVT_S : public _CS_CVT_BASE<Tchar1, Tstring2, Thelper, TStringInit>
{
private:
	typedef _CS_CVT_BASE<Tchar1, Tstring2, Thelper, TStringInit>  baseClass;
	typedef _CS_CVT_S<Tchar1, Tstring2, Thelper, TStringInit, TCCInit>  thisClass;

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
	explicit _CS_CVT_S(typename Tstring2::template StringType<Tchar1>&& str, Args&&... args)  //may throw
	{
		Init(rv_forward(str), rv_forward<Args>(args)...);
	}
	_CS_CVT_S(thisClass&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src))),
										m_strC(rv_forward(src.m_strC))
	{
	}
	~_CS_CVT_S() throw()
	{
	}

	thisClass& operator=(thisClass&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		m_strC = rv_forward(src.m_strC);
		return *this;
	}

	//the return value may indicate the same object when initialized by StringT or UniqueStringT
	Tstring2&& GetV()  //may throw
	{
		if( m_strC.IsNull() )
			return baseClass::GetV();
		TStringInit::DoInit(baseClass::m_str);  //may throw
		if( (baseClass::m_str).IsEmpty() )
			StringUtilHelper::MakeString(m_strC, baseClass::m_str);  //may throw
		return rv_forward(baseClass::m_str);
	}
	//DO NOT assign the return value to other variables
	//  this method can be called many times when using class object.
	ConstStringT<typename Tstring2::EType> GetC() const throw()
	{
		return m_strC.IsNull() ? StringUtilHelper::To_ConstString(baseClass::m_str) : m_strC;
	}

	template <typename... Args>
	void Init(const ConstStringT<Tchar1>& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		bool bSame;
		if( !TCCInit::DoInit(cc, bSame, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		if( bSame ) {
			TStringInit::DoDump(baseClass::m_str);
			ConstArrayHelper::SetInternalPointer<typename Tstring2::EType>((const typename Tstring2::EType*)ConstArrayHelper::GetInternalPointer<Tchar1>(str), str.GetCount(), m_strC);
			return ;
		}
		baseClass::cvt_string(str, cc, baseClass::m_str);
		ConstArrayHelper::SetInternalPointer<typename Tstring2::EType>(NULL, 0, m_strC);
	}
	template <typename... Args>
	void Init(typename Tstring2::template StringType<Tchar1>&& str, Args&&... args)  //may throw
	{
		charset_converter cc;
		bool bSame;
		if( !TCCInit::DoInit(cc, bSame, rv_forward<Args>(args)...) )
			throw Exception(CallResult(SystemCallResults::Fail));
		if( bSame )
			baseClass::m_str = rv_forward(*((Tstring2*)(&str)));
		else
			baseClass::cvt_string(StringUtilHelper::To_ConstString(str), cc, baseClass::m_str);
		ConstArrayHelper::SetInternalPointer<typename Tstring2::EType>(NULL, 0, m_strC);
	}

private:
	_CS_CVT_S(const thisClass&) throw();
	thisClass& operator=(const thisClass&) throw();

private:
	ConstStringT<typename Tstring2::EType> m_strC;
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
typedef _CS_CVT_S<CharA, StringT<CharS>, ShareArrayHelper, cs_string_init<StringT<CharS>>, _Init_A2S>  CS_A2S;
// CS_U2S
typedef _CS_CVT_S<CharA, StringT<CharS>, ShareArrayHelper, cs_string_init<StringT<CharS>>, _Init_U2S>  CS_U2S;
// CS_H2S
typedef _CS_CVT_S<CharH, StringT<CharS>, ShareArrayHelper, cs_string_init<StringT<CharS>>, _Init_H2S>  CS_H2S;
// CS_L2S
typedef _CS_CVT_S<CharL, StringT<CharS>, ShareArrayHelper, cs_string_init<StringT<CharS>>, _Init_L2S>  CS_L2S;
// CS_S2A
typedef _CS_CVT_S<CharS, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_S2A>  CS_S2A;
// CS_S2U
typedef _CS_CVT_S<CharS, StringT<CharA>, ShareArrayHelper, cs_string_init<StringT<CharA>>, _Init_S2U>  CS_S2U;
// CS_S2H
typedef _CS_CVT_S<CharS, StringT<CharH>, ShareArrayHelper, cs_string_init<StringT<CharH>>, _Init_S2H>  CS_S2H;
// CS_S2L
typedef _CS_CVT_S<CharS, StringT<CharL>, ShareArrayHelper, cs_string_init<StringT<CharL>>, _Init_S2L>  CS_S2L;

// CSU_A2S
typedef _CS_CVT_S<CharA, UniqueStringT<CharS>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharS>>, _Init_A2S>  CSU_A2S;
// CSU_U2S
typedef _CS_CVT_S<CharA, UniqueStringT<CharS>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharS>>, _Init_U2S>  CSU_U2S;
// CSU_H2S
typedef _CS_CVT_S<CharH, UniqueStringT<CharS>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharS>>, _Init_H2S>  CSU_H2S;
// CSU_L2S
typedef _CS_CVT_S<CharL, UniqueStringT<CharS>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharS>>, _Init_L2S>  CSU_L2S;
// CSU_S2A
typedef _CS_CVT_S<CharS, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_S2A>  CSU_S2A;
// CSU_S2U
typedef _CS_CVT_S<CharS, UniqueStringT<CharA>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharA>>, _Init_S2U>  CSU_S2U;
// CSU_S2H
typedef _CS_CVT_S<CharS, UniqueStringT<CharH>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharH>>, _Init_S2H>  CSU_S2H;
// CSU_S2L
typedef _CS_CVT_S<CharS, UniqueStringT<CharL>, UniqueArrayHelper, cs_unique_string_init<UniqueStringT<CharL>>, _Init_S2L>  CSU_S2L;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CHARSET_H__
////////////////////////////////////////////////////////////////////////////////
