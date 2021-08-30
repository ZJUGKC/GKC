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

template <class TCCTrait>
class _CS_CVT_BASE
{
private:
	typedef _CS_CVT_BASE<TCCTrait>  thisClass;

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
	typename TCCTrait::Estring2&& GetV() throw()
	{
		return rv_forward(m_str);
	}

protected:
	template <typename... Args>
	void cvt_string(const ConstStringT<typename TCCTrait::Echar1>& str, Args&&... args)
	{
		uintptr uLen = str.GetCount();
		if( uLen > (uintptr)(Limits<int>::Max) )
			throw OverflowException();
		StringOpHelper::Initialize(m_str);  //may throw or not
		if( !TCCTrait::Convert(ConstArrayHelper::GetInternalPointer(str), (int)uLen, m_str, rv_forward<Args>(args)...) ) //may throw
			throw Exception(CallResult(SystemCallResults::Fail));
	}

private:
	_CS_CVT_BASE(const thisClass& src) throw();
	thisClass& operator=(const thisClass& src) throw();

protected:
	typename TCCTrait::Estring2 m_str;
};

template <class TCCTrait>
class _CS_CVT : public _CS_CVT_BASE<TCCTrait>
{
private:
	typedef _CS_CVT_BASE<TCCTrait>  baseClass;

public:
	_CS_CVT() throw()
	{
	}
	template <typename... Args>
	explicit _CS_CVT(const ConstStringT<typename TCCTrait::Echar1>& str, Args&&... args)  //may throw
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
	void Init(const ConstStringT<typename TCCTrait::Echar1>& str, Args&&... args)  //may throw
	{
		baseClass::cvt_string(str, rv_forward<Args>(args)...);
	}

private:
	_CS_CVT(const _CS_CVT&) throw();
	_CS_CVT& operator=(const _CS_CVT&) throw();
};

// CS_A2A
typedef _CS_CVT<charset_converter_trait_A2A<StringA>>  CS_A2A;
// CS_A2U
typedef _CS_CVT<charset_converter_trait_A2U<StringA>>  CS_A2U;
// CS_A2H
typedef _CS_CVT<charset_converter_trait_A2H<StringH>>  CS_A2H;
// CS_A2L
typedef _CS_CVT<charset_converter_trait_A2L<StringL>>  CS_A2L;
// CS_U2A
typedef _CS_CVT<charset_converter_trait_U2A<StringA>>  CS_U2A;
// CS_U2H
typedef _CS_CVT<charset_converter_trait_U2H<StringH>>  CS_U2H;
// CS_U2L
typedef _CS_CVT<charset_converter_trait_U2L<StringL>>  CS_U2L;
// CS_H2A
typedef _CS_CVT<charset_converter_trait_H2A<StringA>>  CS_H2A;
// CS_H2U
typedef _CS_CVT<charset_converter_trait_H2U<StringA>>  CS_H2U;
// CS_H2L
typedef _CS_CVT<charset_converter_trait_H2L<StringL>>  CS_H2L;
// CS_L2A
typedef _CS_CVT<charset_converter_trait_L2A<StringA>>  CS_L2A;
// CS_L2U
typedef _CS_CVT<charset_converter_trait_L2U<StringA>>  CS_L2U;
// CS_L2H
typedef _CS_CVT<charset_converter_trait_L2H<StringH>>  CS_L2H;

// CSU_A2A
typedef _CS_CVT<charset_converter_trait_A2A<UniqueStringA>>  CSU_A2A;
// CSU_A2U
typedef _CS_CVT<charset_converter_trait_A2U<UniqueStringA>>  CSU_A2U;
// CSU_A2H
typedef _CS_CVT<charset_converter_trait_A2H<UniqueStringH>>  CSU_A2H;
// CSU_A2L
typedef _CS_CVT<charset_converter_trait_A2L<UniqueStringL>>  CSU_A2L;
// CSU_U2A
typedef _CS_CVT<charset_converter_trait_U2A<UniqueStringA>>  CSU_U2A;
// CSU_U2H
typedef _CS_CVT<charset_converter_trait_U2H<UniqueStringH>>  CSU_U2H;
// CSU_U2L
typedef _CS_CVT<charset_converter_trait_U2L<UniqueStringL>>  CSU_U2L;
// CSU_H2A
typedef _CS_CVT<charset_converter_trait_H2A<UniqueStringA>>  CSU_H2A;
// CSU_H2U
typedef _CS_CVT<charset_converter_trait_H2U<UniqueStringA>>  CSU_H2U;
// CSU_H2L
typedef _CS_CVT<charset_converter_trait_H2L<UniqueStringL>>  CSU_H2L;
// CSU_L2A
typedef _CS_CVT<charset_converter_trait_L2A<UniqueStringA>>  CSU_L2A;
// CSU_L2U
typedef _CS_CVT<charset_converter_trait_L2U<UniqueStringA>>  CSU_L2U;
// CSU_L2H
typedef _CS_CVT<charset_converter_trait_L2H<UniqueStringH>>  CSU_L2H;

//operating system (OS) related

template <class TCCTrait>
class _CS_CVT_O : public _CS_CVT_BASE<TCCTrait>
{
private:
	typedef _CS_CVT_BASE<TCCTrait>  baseClass;
	typedef _CS_CVT_O<TCCTrait>  thisClass;

public:
	_CS_CVT_O() throw()
	{
	}
	template <typename... Args>
	explicit _CS_CVT_O(const ConstStringT<typename TCCTrait::Echar1>& str, Args&&... args)  //may throw
	{
		Init(str, rv_forward<Args>(args)...);
	}
	template <typename... Args>
	explicit _CS_CVT_O(typename TCCTrait::Estring2::template StringType<typename TCCTrait::Echar1>&& str, Args&&... args)  //may throw
	{
		Init(rv_forward(str), rv_forward<Args>(args)...);
	}
	_CS_CVT_O(thisClass&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src))),
										m_strC(rv_forward(src.m_strC))
	{
	}
	~_CS_CVT_O() throw()
	{
	}

	thisClass& operator=(thisClass&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		m_strC = rv_forward(src.m_strC);
		return *this;
	}

	//the return value may indicate the same object when initialized by StringT or UniqueStringT
	//  this method can be called only once.
	typename TCCTrait::Estring2&& GetV()  //may throw
	{
		StringOpHelper::Initialize(baseClass::m_str);  //may throw or not
		if( !m_strC.IsNull() && m_strC.GetCount() != 0 )
			StringUtilHelper::MakeString(m_strC, baseClass::m_str);  //may throw
		return baseClass::GetV();
	}
	//DO NOT assign the return value to other variables
	//  this method can be called many times when using class object.
	ConstStringT<typename TCCTrait::Estring2::EType> GetC() const throw()
	{
		return m_strC.IsNull() ? StringUtilHelper::To_ConstString(baseClass::m_str) : m_strC;
	}

	template <typename... Args>
	void Init(const ConstStringT<typename TCCTrait::Echar1>& str, Args&&... args)  //may throw
	{
		if( TCCTrait::c_bSame ) {
			StringOpHelper::Uninitialize(baseClass::m_str);
			ConstArrayHelper::SetInternalPointer<typename TCCTrait::Estring2::EType>((const typename TCCTrait::Estring2::EType*)ConstArrayHelper::GetInternalPointer<typename TCCTrait::Echar1>(str), str.GetCount(), m_strC);
			return ;
		}
		baseClass::cvt_string(str, rv_forward<Args>(args)...);
		ConstArrayHelper::SetInternalPointer<typename TCCTrait::Estring2::EType>(NULL, 0, m_strC);
	}
	template <typename... Args>
	void Init(typename TCCTrait::Estring2::template StringType<typename TCCTrait::Echar1>&& str, Args&&... args)  //may throw
	{
		if( TCCTrait::c_bSame )
			baseClass::m_str = rv_forward(*((typename TCCTrait::Estring2*)(&str)));
		else
			baseClass::cvt_string(StringUtilHelper::To_ConstString(str), rv_forward<Args>(args)...);
		ConstArrayHelper::SetInternalPointer<typename TCCTrait::Estring2::EType>(NULL, 0, m_strC);
	}

private:
	_CS_CVT_O(const thisClass&) throw();
	thisClass& operator=(const thisClass&) throw();

private:
	ConstStringT<typename TCCTrait::Estring2::EType> m_strC;
};

// CS_A2S
typedef _CS_CVT_O<charset_converter_trait_A2S<StringS>>  CS_A2S;
// CS_U2S
typedef _CS_CVT_O<charset_converter_trait_U2S<StringS>>  CS_U2S;
// CS_H2S
typedef _CS_CVT_O<charset_converter_trait_H2S<StringS>>  CS_H2S;
// CS_L2S
typedef _CS_CVT_O<charset_converter_trait_L2S<StringS>>  CS_L2S;
// CS_S2A
typedef _CS_CVT_O<charset_converter_trait_S2A<StringA>>  CS_S2A;
// CS_S2U
typedef _CS_CVT_O<charset_converter_trait_S2U<StringA>>  CS_S2U;
// CS_S2H
typedef _CS_CVT_O<charset_converter_trait_S2H<StringH>>  CS_S2H;
// CS_S2L
typedef _CS_CVT_O<charset_converter_trait_S2L<StringL>>  CS_S2L;

// CSU_A2S
typedef _CS_CVT_O<charset_converter_trait_A2S<UniqueStringS>>  CSU_A2S;
// CSU_U2S
typedef _CS_CVT_O<charset_converter_trait_U2S<UniqueStringS>>  CSU_U2S;
// CSU_H2S
typedef _CS_CVT_O<charset_converter_trait_H2S<UniqueStringS>>  CSU_H2S;
// CSU_L2S
typedef _CS_CVT_O<charset_converter_trait_L2S<UniqueStringS>>  CSU_L2S;
// CSU_S2A
typedef _CS_CVT_O<charset_converter_trait_S2A<UniqueStringA>>  CSU_S2A;
// CSU_S2U
typedef _CS_CVT_O<charset_converter_trait_S2U<UniqueStringA>>  CSU_S2U;
// CSU_S2H
typedef _CS_CVT_O<charset_converter_trait_S2H<UniqueStringH>>  CSU_S2H;
// CSU_S2L
typedef _CS_CVT_O<charset_converter_trait_S2L<UniqueStringL>>  CSU_S2L;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CHARSET_H__
////////////////////////////////////////////////////////////////////////////////
