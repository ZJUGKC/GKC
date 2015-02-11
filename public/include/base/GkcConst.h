﻿/*
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

/*
This file contains classes for const array.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_CONST_H__
#define __GKC_CONST_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcConst.h requires GkcBase.h to be included first.
#endif

//const definition:
// h:   class C { ... public: static const T c_name; };
// cpp: const T C::c_name=...;

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//classes

//------------------------------------------------------------------------------

// ConstArray<T>

template <typename T>
class ConstArray
{
public:
	typedef T  EType;
	typedef ArrayIterator<T>  Iterator;

public:
	ConstArray() throw() : m_first(NULL), m_size(0)
	{
	}
	ConstArray(const T* p, uintptr size) throw() : m_first(p), m_size(size)
	{
	}
	ConstArray(const ConstArray<T>& src) throw() : m_first(src.m_first), m_size(src.m_size)
	{
	}
	~ConstArray() throw()
	{
	}

	//operators
	ConstArray<T>& operator=(const ConstArray<T>& src) throw()
	{
		if( this != &src ) {
			m_first = src.m_first;
			m_size  = src.m_size;
		}
		return *this;
	}

	bool operator==(const ConstArray<T>& right) const throw()
	{
		return m_first == right.m_first;
	}
	bool operator!=(const ConstArray<T>& right) const throw()
	{
		return !operator==(right);
	}

	const Iterator operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}

	//methods

	//count
	uintptr GetCount() const throw()
	{
		return m_size;
	}
	bool IsNull() const throw()
	{
		return m_first == NULL;
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(RefPtr<T>(m_first));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<T>(m_first + GetCount()));
	}
	const ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	const ReverseIterator<Iterator> GetReverseEnd() const throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	const Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Iterator(RefPtr<T>(m_first + index));
	}

private:
	const T*  m_first;
	uintptr   m_size;

private:
	friend class ConstHelper;
};

// macros

// define Static Const Array

#define DECLARE_STATIC_CONST_ARRAY(cls, T)   class cls {  \
public:  \
	typedef T  EType;  \
	static uintptr GetCount() throw() \
	{ return c_size; } \
	static const T* GetAddress() throw() \
	{ return c_array; } \
public:  \
	static const T        c_array[]; \
	static const uintptr  c_size;    \
};

//------------------------------------------------------------------------------

// ConstString<Tchar>
//   Tchar : CharA CharH CharL, CharS CharW

template <typename Tchar>
class ConstString : public ConstArray<Tchar>
{
private:
	typedef ConstArray<Tchar>   baseClass;
	typedef ConstString<Tchar>  thisClass;

public:
	ConstString() throw()
	{
	}
	ConstString(const Tchar* p, uintptr size) throw() : baseClass(p, size)
	{
	}
	ConstString(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	~ConstString() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
};

// ConstStringX
typedef ConstString<CharA>  ConstStringA;   //ansi or UTF8
typedef ConstString<CharH>  ConstStringH;   //UTF16
typedef ConstString<CharL>  ConstStringL;   //UTF32
typedef ConstString<CharS>  ConstStringS;   //system
typedef ConstString<CharW>  ConstStringW;   //wide version

// macros

// define Static Const String

#define DECLARE_STATIC_CONST_STRING(cls)  \
	DECLARE_STATIC_CONST_ARRAY(cls, typename cls::EType)

//------------------------------------------------------------------------------
// Helper

// ConstHelper

class ConstHelper
{
public:
	//set internal pointer
	template <typename T>
	static void SetInternalPointer(const T* p, uintptr size, ConstArray<T>& arr) throw()
	{
		arr.m_first = p;
		arr.m_size  = size;
	}
	//get internal pointer
	template <typename T>
	static const T* GetInternalPointer(const ConstArray<T>& arr) throw()
	{
		return arr.m_first;
	}

	//type cast
	template <class T, class TBase>
	static const TBase& TypeCast(const T& src) throw()
	{
		return static_cast<const TBase&>(src);
	}
};

//------------------------------------------------------------------------------
// Traits

// ConstStringCompareTrait<T>

template <class T>
class ConstStringCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// ConstStringCaseIgnoreCompareTrait<T>

template <class T>
class ConstStringCaseIgnoreCompareTrait
{
public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value())) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(&(t1.GetBegin().get_Value()), &(t2.GetBegin().get_Value()));
	}
};

// ConstStringHashTrait<T>

template <class T>
class ConstStringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = ConstHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// ConstStringCaseIgnoreHashTrait<T>

template <class T>
class ConstStringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = ConstHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

// for definitions in cpp

//static const array
#define BEGIN_STATIC_CONST_ARRAY(cls)    const typename GKC::cls::EType GKC::cls::c_array[] = {

#define STATIC_CONST_ARRAY_ENTRY(x)           x,
#define STATIC_CONST_ARRAY_ENTRY_LAST(y)      y

#define BEGIN_STATIC_CONST_ARRAY_GROUP()      {
#define END_STATIC_CONST_ARRAY_GROUP()        },
#define END_STATIC_CONST_ARRAY_GROUP_LAST()   }

#define END_STATIC_CONST_ARRAY(cls)   };  \
		const uintptr GKC::cls::c_size = sizeof(GKC::cls::c_array) / sizeof(GKC::cls::c_array[0]) - 1;

//static const string
//  STATIC_CONST_STRING_ENTRY(x) can be used repeatedly with or without CRLF
//  "some" or L"some" or _S("some")
#define BEGIN_STATIC_CONST_STRING(cls)   const typename GKC::cls::EType GKC::cls::c_array[] =
#define STATIC_CONST_STRING_ENTRY(x)     x
#define END_STATIC_CONST_STRING(cls)     ;  \
		const uintptr GKC::cls::c_size = sizeof(GKC::cls::c_array) / sizeof(GKC::cls::c_array[0]) - 1;

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CONST_H__
////////////////////////////////////////////////////////////////////////////////
