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

// ConstArrayBase<T>

template <typename T>
struct ConstArrayBase
{
	const T*  m_first;
	uintptr   m_size;
};

// ConstArray<T>

template <typename T>
class ConstArray : public ConstArrayBase<T>
{
private:
	typedef ConstArrayBase<T>  baseClass;

public:
	typedef T  EType;
	typedef ArrayIterator<T>  Iterator;

public:
	ConstArray() throw()
	{
		baseClass::m_first = NULL;
		baseClass::m_size  = 0;
	}
	ConstArray(const T* p, uintptr size) throw()
	{
		baseClass::m_first = p;
		baseClass::m_size  = size;
	}
	ConstArray(const ConstArrayBase<T>& src) throw()
	{
		baseClass::m_first = src.m_first;
		baseClass::m_size  = src.m_size;
	}
	ConstArray(const ConstArray<T>& src) throw()
	{
		baseClass::m_first = src.m_first;
		baseClass::m_size  = src.m_size;
	}
	~ConstArray() throw()
	{
	}

	//operators
	ConstArray<T>& operator=(const ConstArray<T>& src) throw()
	{
		if( this != &src ) {
			baseClass::m_first = src.m_first;
			baseClass::m_size  = src.m_size;
		}
		return *this;
	}

	bool operator==(const ConstArray<T>& right) const throw()
	{
		return baseClass::m_first == right.m_first;
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
		return baseClass::m_size;
	}
	bool IsNull() const throw()
	{
		return baseClass::m_first == NULL;
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(RefPtr<T>(baseClass::m_first));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<T>(baseClass::m_first + GetCount()));
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
		return Iterator(RefPtr<T>(baseClass::m_first + index));
	}

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

// ConstStringT<Tchar>
//   Tchar : CharA CharH CharL, CharS CharW

template <typename Tchar>
class ConstStringT : public ConstArray<Tchar>
{
private:
	typedef ConstArray<Tchar>    baseClass;
	typedef ConstStringT<Tchar>  thisClass;

public:
	ConstStringT() throw()
	{
	}
	ConstStringT(const Tchar* p, uintptr size) throw() : baseClass(p, size)
	{
	}
	ConstStringT(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	ConstStringT(const ConstArrayBase<Tchar>& src) throw() : baseClass(src)
	{
	}
	~ConstStringT() throw()
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
typedef ConstStringT<CharA>  ConstStringA;   //ansi or UTF8
typedef ConstStringT<CharH>  ConstStringH;   //UTF16
typedef ConstStringT<CharL>  ConstStringL;   //UTF32
typedef ConstStringT<CharS>  ConstStringS;   //system
typedef ConstStringT<CharW>  ConstStringW;   //wide version

// macros

// define const string by constant array in a method body

#define DECLARE_LOCAL_CONST_STRING(char_type, array_name, len_name, x)  \
const char_type array_name[] = x;  \
const uintptr len_name = sizeof(array_name) / sizeof(char_type) - 1;

// define temporary const string object by constant array in a method body

#define DECLARE_TEMP_CONST_STRING(const_string_type, x)  \
const_string_type(x, sizeof(x) / sizeof(const_string_type::EType) - 1)

// define Static Constant String

#define DECLARE_STATIC_CONST_STRING(cls)  \
	DECLARE_STATIC_CONST_ARRAY(cls, cls::EType)

// define const string member

#define DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  GKC::ConstArrayBase<char_type>

#define DECLARE_CONST_STRING_STRUCT_MEMBER(v_name, char_type)  \
DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  v_name;

#define DECLARE_STATIC_CONST_STRING_MEMBER(c_name, char_type)  \
static const DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)  c_name;

// DECLARE_CONST_STRING_ARRAY(char_type)
#define DECLARE_CONST_STRING_ARRAY(char_type)  GKC::ConstArray<DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type)>

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

	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static const TDest& TypeCast(const TSrc& src) throw()
	{
		return static_cast<const TDest&>(src);
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
#define BEGIN_STATIC_CONST_ARRAY(cls)    const GKC::cls::EType GKC::cls::c_array[] = {

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
#define BEGIN_STATIC_CONST_STRING(cls)   const GKC::cls::EType GKC::cls::c_array[] =
#define STATIC_CONST_STRING_ENTRY(x)     x
#define END_STATIC_CONST_STRING(cls)     ;  \
		const uintptr GKC::cls::c_size = sizeof(GKC::cls::c_array) / sizeof(GKC::cls::c_array[0]) - 1;

//const string member
#define IMPLEMENT_CONST_STRING_ENTRY(char_type, x)   { x, sizeof(x) / sizeof(char_type) - 1 }  //this macro can be used for implementing const string member

#define IMPLEMENT_STATIC_CONST_STRING_MEMBER(cls, c_name, char_type, x)  \
const DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_type) GKC::cls::c_name = \
IMPLEMENT_CONST_STRING_ENTRY(char_type, x) ;

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_CONST_H__
////////////////////////////////////////////////////////////////////////////////
