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

// ConstString<T>
//   T : CharA CharH CharL, CharS CharW

template <typename T>
class ConstString : public ConstArray<T>
{
private:
	typedef ConstArray<T>   baseClass;
	typedef ConstString<T>  thisClass;

public:
	ConstString() throw()
	{
	}
	ConstString(const T* p, uintptr size) throw() : baseClass(p, size)
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
	//set pointer
	template <typename T>
	static void SetPointer(const T* p, uintptr size, ConstArray<T>& arr) throw()
	{
		arr.m_first = p;
		arr.m_size  = size;
	}
	//type cast
	template <class T, class TBase>
	static const TBase& TypeCast(const T& src) throw()
	{
		return static_cast<const TBase&>(src);
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
