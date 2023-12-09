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
This file contains preprocessor directives used to help programming
for cross-platform.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_DEF_H__
#define __GKC_DEF_H__
////////////////////////////////////////////////////////////////////////////////

//macros

//DEBUG
#if ( defined(_DEBUG) || !defined(NDEBUG) )
	#define GKC_DEBUG
#endif

//Machine Type
#if defined(WIN32)
	//windows
	#define GKC_OS_WINDOWS
	//WIN64
	#ifdef _WIN64
		#ifndef WIN64
			#define WIN64
		#endif
		#define CM_X64
	#else
		#define CM_X86
	#endif //WIN64
#elif defined(__linux__)
	//linux
	#define GKC_OS_LINUX
	//X64
	#if defined(__x86_64__)
		#define CM_X64
	#else //__i386__
		#define CM_X86
	#endif
#else
	#error This OS system is not supported!
#endif //Machine Type

//basic types
#include "system/base_types.h"

//composite types
#include "system/composite_types.h"

//os headers
#include "system/os_headers.h"

//other types
#include "system/other_types.h"

//numeric types
#include "system/numeric_types.h"

//unique types
#include "system/heap_types.h"

//string APIs
#include "system/str_api.h"

//console APIs
#include "system/console_api.h"

//file management
#include "system/fm_api.h"

//time
#include "system/time_api.h"

//------------------------------------------------------------------------------
//system types which can use exceptions

#include "system/sys_types.h"

#include "system/sys_thread.h"

#include "system/sys_locale.h"

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Pointers

// RefPtr<T>

template <typename T>
using RefPtr = ref_ptr<T>;

// RefPtrHelper
typedef ref_ptr_helper  RefPtrHelper;

//------------------------------------------------------------------------------
// Pair

// UnionPair<T1, T2>

template <typename T1, typename T2>
using UnionPair = union_pair<T1, T2>;

//------------------------------------------------------------------------------
// Iterator

// ReverseIterator<T>
template <typename T>
using ReverseIterator = reverse_iterator<T>;

// ReverseIterator2<T>
template <typename T>
using ReverseIterator2 = reverse_iterator2<T>;

//------------------------------------------------------------------------------
// Array

// ArrayPosition
typedef array_position  ArrayPosition;

// ArrayIterator<T>
template <typename T>
using ArrayIterator = array_iterator<T>;

// ConstArray<T>
template <typename T>
using ConstArray = const_array<T>;

// ConstArrayHelper
typedef const_array_helper  ConstArrayHelper;

// FixedArray<T, t_size>
template <typename T, uintptr t_size>
using FixedArray = fixed_array<T, t_size>;

// FixedArrayHelper
typedef fixed_array_helper  FixedArrayHelper;

//------------------------------------------------------------------------------
// Character

typedef char_a  CharA;
typedef char_h  CharH;
typedef char_l  CharL;

typedef char_s  CharS;
typedef char_w  CharW;
typedef char_f  CharF;

//------------------------------------------------------------------------------
// CallResult

typedef call_result  CallResult;

// CallResult constants
typedef system_call_results  SystemCallResults;

//------------------------------------------------------------------------------
// Byte Order

/* author: Lijuan Mei */

// ByteOrderHelper
typedef byte_order_helper  ByteOrderHelper;

// SeType<T>
template <typename T>
using SeType = se_type<T>;

// LeType<T>
template <typename T>
using LeType = le_type<T>;

// BeType<T>
template <typename T>
using BeType = be_type<T>;

//------------------------------------------------------------------------------
// Exceptions

// Exception
typedef exception_base  Exception;

// OutOfMemoryException
typedef outofmemory_exception  OutOfMemoryException;

// OverflowException
typedef overflow_exception  OverflowException;

//------------------------------------------------------------------------------
//Traits

// DefaultCompareTrait<T>
template <typename T>
using DefaultCompareTrait = default_compare_trait<T>;

// CharCaseIgnoreCompareTrait<Tchar>

template <typename Tchar>
class CharCaseIgnoreCompareTrait
{
public:
	typedef Tchar  EType;

public:
	static bool IsEQ(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) == char_upper(t2);
	}
	static bool IsNE(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) != char_upper(t2);
	}
	static bool IsGT(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) > char_upper(t2);
	}
	static bool IsLT(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) < char_upper(t2);
	}
	static bool IsGE(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) >= char_upper(t2);
	}
	static bool IsLE(const Tchar& t1, const Tchar& t2) throw()
	{
		return char_upper(t1) <= char_upper(t2);
	}
	static int Compare(const Tchar& t1, const Tchar& t2) throw()
	{
		return (int)(char_upper(t1) - char_upper(t2));
	}
};

// DefaultHashTrait<T>
template <typename T>
using DefaultHashTrait = default_hash_trait<T>;

//------------------------------------------------------------------------------
//tuple

// Pair<T1, T2>
/*! \brief A template class for two types.

A template class for two types.
\tparam T1, T2 Two types.
*/
template <typename T1, typename T2>
class Pair
{
private:
	typedef Pair<T1, T2>  thisClass;

public:
	typedef T1  E1;
	typedef T2  E2;

public:
	/*! \brief Constructor.

	Constructor.
	\note It may throw exceptions.
	*/
	Pair()
	{
	}
	/*! \brief Copy constructor.

	Copy constructor.
	\note It may throw exceptions.
	*/
	explicit Pair(const T1& t1) : m_t1(t1)
	{
	}
	explicit Pair(T1&& t1) : m_t1(rv_forward(t1))
	{
	}
	Pair(const T1& t1, const T2& t2) : m_t1(t1), m_t2(t2)
	{
	}
	Pair(T1&& t1, T2&& t2) : m_t1(rv_forward(t1)), m_t2(rv_forward(t2))
	{
	}
	Pair(const thisClass& src) : m_t1(src.m_t1), m_t2(src.m_t2)
	{
	}
	Pair(thisClass&& src) : m_t1(rv_forward(src.m_t1)), m_t2(rv_forward(src.m_t2))
	{
	}
	~Pair() throw()
	{
	}

	/*! \brief Assignment operator.

	Assignment operator.
	\note It may throw exceptions.
	*/
	thisClass& operator=(const thisClass& src)
	{
		m_t1 = src.m_t1;
		m_t2 = src.m_t2;
		return *this;
	}
	thisClass& operator=(thisClass&& src)
	{
		m_t1 = rv_forward(src.m_t1);
		m_t2 = rv_forward(src.m_t2);
		return *this;
	}

	bool operator==(const thisClass& src) const throw()
	{
		return m_t1 == src.m_t1 && m_t2 == src.m_t2;
	}
	bool operator!=(const thisClass& src) const throw()
	{
		return m_t1 != src.m_t1 || m_t2 != src.m_t2;
	}

	//properties
	const T1& get_First() const throw()
	{
		return m_t1;
	}
	T1& get_First() throw()
	{
		return m_t1;
	}
	void set_First(const T1& t)
	{
		m_t1 = t;
	}
	void set_First(T1&& t)
	{
		m_t1 = rv_forward(t);
	}

	const T2& get_Second() const throw()
	{
		return m_t2;
	}
	T2& get_Second() throw()
	{
		return m_t2;
	}
	void set_Second(const T2& t)
	{
		m_t2 = t;
	}
	void set_Second(T2&& t)
	{
		m_t2 = rv_forward(t);
	}

	void Set(const T1& t1, const T2& t2)
	{
		m_t1 = t1;
		m_t2 = t2;
	}
	void Set(T1&& t1, T2&& t2)
	{
		m_t1 = rv_forward(t1);
		m_t2 = rv_forward(t2);
	}

private:
	T1 m_t1;  //!< first
	T2 m_t2;  //!< second
};

// KeyHelper

class KeyHelper
{
public:
	// T : can be const T
	template <typename T>
	static T& GetKey(T& pair) throw()
	{
		return pair;
	}
	//pair TKey : can be const TKey
	template <typename TKey, typename TValue>
	static const TKey& GetKey(const Pair<TKey, TValue>& pair) throw()
	{
		return pair.get_First();
	}
	template <typename TKey, typename TValue>
	static TKey& GetKey(Pair<TKey, TValue>& pair) throw()
	{
		return pair.get_First();
	}
	template <typename TKey, typename TValue>
	static const TValue& GetValue(const Pair<TKey, TValue>& pair) throw()
	{
		return pair.get_Second();
	}
	template <typename TKey, typename TValue>
	static TValue& GetValue(Pair<TKey, TValue>& pair) throw()
	{
		return pair.get_Second();
	}
	template <typename TKey, typename TValue>
	void SetValue(Pair<TKey, TValue>& pair, const TValue& t)  //may throw
	{
		pair.set_Second(t);
	}
	template <typename TKey, typename TValue>
	void SetValue(Pair<TKey, TValue>& pair, TValue&& t)  //may throw
	{
		pair.set_Second(rv_forward(t));
	}
};

//------------------------------------------------------------------------------
// Fixed Array

// FixedArrayCompareTrait<T, TCompareTrait>
//  T: FixedArray<...>

template <class T, class TCompareTrait = DefaultCompareTrait<typename T::EType>>
class FixedArrayCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = 0; i < T::c_size; i ++ ) {
			if( TCompareTrait::IsNE(p1[i], p2[i]) )
				return false;
		}
		return true;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = 0; i < T::c_size; i ++ ) {
			if( TCompareTrait::IsNE(p1[i], p2[i]) )
				return true;
		}
		return false;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = T::c_size; i > 0; i -- ) {
			int res = TCompareTrait::Compare(p1[i - 1], p2[i - 1]);
			if( res > 0 )
				return true;
			if( res < 0 )
				return false;
		}
		return false;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = T::c_size; i > 0; i -- ) {
			int res = TCompareTrait::Compare(p1[i - 1], p2[i - 1]);
			if( res > 0 )
				return false;
			if( res < 0 )
				return true;
		}
		return false;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return !IsLT(t1, t2);
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return !IsGT(t1, t2);
	}
	//Compare
	static int Compare(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = T::c_size; i > 0; i -- ) {
			int res = TCompareTrait::Compare(p1[i - 1], p2[i - 1]);
			if( res > 0 )
				return 1;
			if( res < 0 )
				return -1;
		}
		return 0;
	}
};

// FixedArrayBigEndianCompareTrait<T, TCompareTrait>
//  T: FixedArray<...>

template <class T, class TCompareTrait = DefaultCompareTrait<typename T::EType>>
class FixedArrayBigEndianCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return FixedArrayCompareTrait<T, TCompareTrait>::IsEQ(t1, t2);
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return FixedArrayCompareTrait<T, TCompareTrait>::IsNE(t1, t2);
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = 0; i < T::c_size; i ++ ) {
			int res = TCompareTrait::Compare(p1[i], p2[i]);
			if( res > 0 )
				return true;
			if( res < 0 )
				return false;
		}
		return false;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = 0; i < T::c_size; i ++ ) {
			int res = TCompareTrait::Compare(p1[i], p2[i]);
			if( res > 0 )
				return false;
			if( res < 0 )
				return true;
		}
		return false;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return !IsLT(t1, t2);
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return !IsGT(t1, t2);
	}
	//Compare
	static int Compare(const T& t1, const T& t2) throw()
	{
		typename T::EType* p1 = FixedArrayHelper::GetInternalPointer(t1);
		typename T::EType* p2 = FixedArrayHelper::GetInternalPointer(t2);
		for( uintptr i = 0; i < T::c_size; i ++ ) {
			int res = TCompareTrait::Compare(p1[i], p2[i]);
			if( res > 0 )
				return 1;
			if( res < 0 )
				return -1;
		}
		return 0;
	}
};

//------------------------------------------------------------------------------
// Constant String

// ConstStringT<Tchar>
template <typename Tchar>
using ConstStringT = const_string_t<Tchar>;

// ConstString*
typedef const_string_a  ConstStringA;
typedef const_string_h  ConstStringH;
typedef const_string_l  ConstStringL;
typedef const_string_s  ConstStringS;
typedef const_string_w  ConstStringW;

// Traits

// ConstStringCompareTrait<T>

template <class T>
class ConstStringCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2));
	}
};

// ConstStringCaseIgnoreCompareTrait<T>

template <class T>
class ConstStringCaseIgnoreCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(ConstArrayHelper::GetInternalPointer(t1), ConstArrayHelper::GetInternalPointer(t2));
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
		const typename T::EType* pch = ConstArrayHelper::GetInternalPointer(t);
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
		const typename T::EType* pch = ConstArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

// ConsStringHelper
typedef const_string_helper  ConstStringHelper;

//------------------------------------------------------------------------------
// Fixed String

#pragma pack(push, 1)

// FixedStringT<Tchar, t_size>
//   Tchar: CharA CharH CharL, CharS CharW
template <typename Tchar, uintptr t_size>
class FixedStringT : public FixedArray<Tchar, t_size>
{
private:
	typedef FixedArray<Tchar, t_size>  baseClass;
	typedef FixedStringT<Tchar, t_size>  thisClass;

public:
	FixedStringT() throw() : m_uLength(0)
	{
		baseClass::m_data[0] = 0;
	}
	FixedStringT(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src)), m_uLength(src.m_uLength)
	{
		assert( m_uLength < t_size );
		mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), baseClass::m_data);
	}
	~FixedStringT() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		if( this != &src ) {
			m_uLength = src.m_uLength;
			assert( m_uLength < t_size );
			mem_copy(src.m_data, sizeof(Tchar) * (m_uLength + 1), baseClass::m_data);
		}
		return *this;
	}

	uintptr GetLength() const throw()
	{
		return m_uLength;
	}
	void SetLength(uintptr uLength) throw()
	{
		assert( uLength < t_size );
		m_uLength = uLength;
		baseClass::m_data[m_uLength] = 0;
	}

	bool IsEmpty() const throw()
	{
		return GetLength() == 0;
	}

	//position
	typename thisClass::Position GetTailPosition() const throw()
	{
		return typename thisClass::Position(m_uLength - 1);
	}

	//iterators
	const typename thisClass::Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<Tchar>(baseClass::m_data + m_uLength));
	}
	typename thisClass::Iterator GetEnd() throw()
	{
		return Iterator(RefPtr<Tchar>(baseClass::m_data + m_uLength));
	}
	const ReverseIterator<typename thisClass::Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	ReverseIterator<typename thisClass::Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	const ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() const throw()
	{
		return ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}
	ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() throw()
	{
		return ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}

	//methods
	void RecalcLength() throw()
	{
		m_uLength = calc_string_length(baseClass::m_data);
	}

private:
	uintptr m_uLength;

private:
	//logical
	bool operator==(const thisClass& right) const throw();
	bool operator!=(const thisClass& right) const throw();
	bool operator<(const thisClass& right) const throw();
	bool operator>(const thisClass& right) const throw();
	bool operator<=(const thisClass& right) const throw();
	bool operator>=(const thisClass& right) const throw();
};

#pragma pack(pop)

// FixedStringHelper

class FixedStringHelper
{
public:
	//To C-style string
	template <typename Tchar, uintptr t_size>
	static RefPtr<Tchar> To_C_Style(const FixedStringT<Tchar, t_size>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetLength() );
		return RefPtr<Tchar>(FixedArrayHelper::GetInternalPointer(str) + uStart);
	}

	//append character
	//  return: 0 or 1, whether the character is copied
	template <typename Tchar, uintptr t_size>
	static uintptr Append(const Tchar& ch, INOUT FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount = strDest.GetLength();
		if( uCount >= t_size - 1 )
			return 0;
		strDest.SetLength(uCount + 1);
		strDest.SetAt(uCount, ch);
		return 1;
	}

	//insert character
	template <typename Tchar, uintptr t_size>
	static uintptr Insert(uintptr uPos, const Tchar& ch, FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( uPos > uLength || uLength >= t_size - 1 )
			return 0;
		mem_move(FixedArrayHelper::GetInternalPointer(str) + uPos, (uLength - uPos) * sizeof(Tchar), FixedArrayHelper::GetInternalPointer(str) + uPos + 1);
		str.SetLength(uLength + 1);
		str.SetAt(uPos, ch);
		return 1;
	}

	//delete
	template <typename Tchar, uintptr t_size>
	static uintptr Delete(uintptr uPos, uintptr uLength, FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uCount = str.GetLength();
		uintptr uRet = calc_sub_string_act_length(uCount, uPos, uLength);
		if( uRet == 0 )
			return 0;
		mem_move(FixedArrayHelper::GetInternalPointer(str) + uPos + uRet, (uCount - uPos - uRet) * sizeof(Tchar), FixedArrayHelper::GetInternalPointer(str) + uPos);
		str.SetLength(uCount - uRet);
		return uRet;
	}

	//replace
	template <typename Tchar, uintptr t_size, class TCompareTrait = DefaultCompareTrait<Tchar>>
	static uintptr Replace(const Tchar& chOld, const Tchar& chNew, INOUT FixedStringT<Tchar, t_size>& str) throw()
	{
		assert( chOld != 0 && chNew != 0 && TCompareTrait::IsNE(chOld, chNew) );
		if( str.IsEmpty() )
			return 0;
		return coll_replace_elements<typename FixedStringT<Tchar, t_size>::Iterator, TCompareTrait>(chOld, chNew, str.GetBegin(), str.GetEnd());
	}

	//find (return value : check null)
	template <typename Tchar, uintptr t_size>
	static typename FixedStringT<Tchar, t_size>::Iterator Find(const FixedStringT<Tchar, t_size>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		return typename FixedStringT<Tchar, t_size>::Iterator(RefPtr<Tchar>(find_string_char(FixedArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}
	//find last (return value : check null)
	template <typename Tchar, uintptr t_size>
	static typename FixedStringT<Tchar, t_size>::Iterator FindLast(const FixedStringT<Tchar, t_size>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		return typename FixedStringT<Tchar, t_size>::Iterator(RefPtr<Tchar>(find_string_last_char(FixedArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}
};

// FixedStringCompareTrait<T>

template <class T>
class FixedStringCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2));
	}
};

// FixedStringCaseIgnoreCompareTrait<T>

template <class T>
class FixedStringCaseIgnoreCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(FixedArrayHelper::GetInternalPointer(t1), FixedArrayHelper::GetInternalPointer(t2));
	}
};

// FixedStringHashTrait<T>

template <class T>
class FixedStringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = FixedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// FixedStringCaseIgnoreHashTrait<T>

template <class T>
class FixedStringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = FixedArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

// MessageException<t_size>

template <uintptr t_size>
class MessageException : public Exception
{
public:
	typedef FixedStringT<CharS, t_size>  MessageBufferClass;

public:
	MessageException() throw()
	{
	}
	MessageException(const MessageException<t_size>& src) throw() : Exception(static_cast<const Exception&>(src)), m_messageBuffer(src.m_messageBuffer)
	{
	}
	~MessageException() throw()
	{
	}

	MessageException<t_size>& operator=(const MessageException<t_size>& src) throw()
	{
		Exception::operator=(static_cast<const Exception&>(src));
		m_messageBuffer = src.m_messageBuffer;  //no check self
		return *this;
	}

	const MessageBufferClass& GetMessageBuffer() const throw()
	{
		return m_messageBuffer;
	}
	MessageBufferClass& GetMessageBuffer() throw()
	{
		return m_messageBuffer;
	}

private:
	MessageBufferClass  m_messageBuffer;
};

//------------------------------------------------------------------------------
// Swap

/*! \brief Swap two variables.

Swap two variables.
\tparam T The variable type.
\param t1, t2 [in, out] Two variables.
\note Do not throw exceptions in move operator of T.
*/
template <typename T>
inline void Swap(T& t1, T& t2) throw()
{
	assert( &t1 != &t2 );
	T tmp = static_cast<T&&>(t1);
	t1 = static_cast<T&&>(t2);
	t2 = static_cast<T&&>(tmp);
}

//special versions

#define _DECLARE_SPECIAL_SWAP(T)  \
	template <> inline void Swap<T>(T& t1, T& t2) throw()  \
	{ assert( &t1 != &t2 );  \
	t1 ^= t2 ^= t1 ^= t2; }

_DECLARE_SPECIAL_SWAP(char)
_DECLARE_SPECIAL_SWAP(byte)
_DECLARE_SPECIAL_SWAP(short)
_DECLARE_SPECIAL_SWAP(ushort)
_DECLARE_SPECIAL_SWAP(int)
_DECLARE_SPECIAL_SWAP(uint)
_DECLARE_SPECIAL_SWAP(int64)
_DECLARE_SPECIAL_SWAP(uint64)

//------------------------------------------------------------------------------
// number

// Limits<T>

template <typename T>
using Limits = limits_base<T>;

//------------------------------------------------------------------------------
// basic operators

// FloatHelper

class FloatHelper : public float_helper
{
public:
	/*! \brief Calculate the arccosine.

	Calculate the arccosine.
	\param x [in] The input value between -1 and 1.
	\return The arccosine of x in the range 0 to PI radians.
	*/
	static double acos(double x) throw()
	{
		return ::acos(x);
	}
	static float acos(float x) throw()
	{
		return ::acosf(x);
	}
	/*! \brief Calculate the inverse hyperbolic cosine.

	Calculate the inverse hyperbolic cosine.
	\param x [in] Floating-point value.
	\return The inverse hyperbolic cosine of x.
	*/
	static double acosh(double x) throw()
	{
		return ::acosh(x);
	}
	static float acosh(float x) throw()
	{
		return ::acoshf(x);
	}
	/*! \brief Calculate the arcsine.

	Calculate the arcsine.
	\param x [in] The input value.
	\return The arcsine of x in the range -PI/2 to PI/2 radians.
	*/
	static double asin(double x) throw()
	{
		return ::asin(x);
	}
	static float asin(float x) throw()
	{
		return ::asinf(x);
	}
	/*! \brief Calculate the inverse hyperbolic sine.

	Calculate the inverse hyperbolic sine.
	\param x [in] Floating-point value.
	\return The inverse hyperbolic sine of x.
	*/
	static double asinh(double x) throw()
	{
		return ::asinh(x);
	}
	static float asinh(float x) throw()
	{
		return ::asinhf(x);
	}
	/*! \brief Calculate the arctangent.

	Calculate the arctangent of y/x.
	\param y, x [in] The parameters.
	\return The arctangent of y/x in the range -PI to PI radians.
	*/
	static double atan2(double y, double x) throw()
	{
		return ::atan2(y, x);
	}
	static float atan2(float y, float x) throw()
	{
		return ::atan2f(y, x);
	}
	/*! \brief Calculate the inverse hyperbolic tangent.

	Calculate the inverse hyperbolic tangent.
	\param x [in] Floating-point value.
	\return The inverse hyperbolic tangent of x.
	*/
	static double atanh(double x) throw()
	{
		return ::atanh(x);
	}
	static float atanh(float x) throw()
	{
		return ::atanhf(x);
	}
	/*! \brief Calculate the cosine.

	Calculate the cosine.
	\param x [in] Angle in radians.
	\return The cosine of x.
	*/
	static double cos(double x) throw()
	{
		return ::cos(x);
	}
	static float cos(float x) throw()
	{
		return ::cosf(x);
	}
	/*! \brief Calculate the hyperbolic cosine.

	Calculate the hyperbolic cosine.
	\param x [in] Angle in radians.
	\return The hyperbolic cosine of x.
	*/
	static double cosh(double x) throw()
	{
		return ::cosh(x);
	}
	static float cosh(float x) throw()
	{
		return ::coshf(x);
	}
	/*! \brief Calculates the exponential.

	Calculates the exponential.
	\param x [in] Floating-point value.
	\return The exponential value of the floating-point parameter x.
	*/
	static double exp(double x) throw()
	{
		return ::exp(x);
	}
	static float exp(float x) throw()
	{
		return ::expf(x);
	}
	/*! \brief Computes a real number from the mantissa and exponent.

	Computes a real number from the mantissa and exponent.
	\param x [in] Floating-point value.
	\param exp [in] Integer exponent.
	\return The value of x * 2^exp if successful. On overflow (depending on the sign of x),
	        ldexp returns +/- HUGE_VAL.
	*/
	static double ldexp(double x, int exp) throw()
	{
		return ::ldexp(x, exp);
	}
	static float ldexp(float x, int exp) throw()
	{
		return ::ldexpf(x, exp);
	}
	/*! \brief Calculates logarithms.

	Calculates logarithms.
	\param x [in] Value whose logarithm is to be found.
	\return The log functions return the natural logarithm(base e) of x if successful.
	        The log10 functions return the base-10 logarithm.
	        The log2 functions return the base-2 logarithm.
	        If x is negative, these functions return an indefinite, by default.
	        If x is 0, they return INF(infinite).
	*/
	static double log(double x) throw()
	{
		return ::log(x);
	}
	static float log(float x) throw()
	{
		return ::logf(x);
	}
	static double log10(double x) throw()
	{
		return ::log10(x);
	}
	static float log10(float x) throw()
	{
		return ::log10f(x);
	}
	static double log2(double x) throw()
	{
		return ::log2(x);
	}
	static float log2(float x) throw()
	{
		return ::log2f(x);
	}
	/*! \brief Calculates x raised to the power of y.

	Calculates x raised to the power of y.
	\param x [in] Base.
	\param y [in] Exponent.
	\return The value of x^y.
	*/
	static double pow(double x, double y) throw()
	{
		return ::pow(x, y);
	}
	static float pow(float x, float y) throw()
	{
		return ::powf(x, y);
	}
	/*! \brief Calculate sines.

	Calculate sines.
	\param x [in] Angle in radians.
	\return The sine of x.
	*/
	static double sin(double x) throw()
	{
		return ::sin(x);
	}
	static float sin(float x) throw()
	{
		return ::sinf(x);
	}
	/*! \brief Calculate hyperbolic sines.

	Calculate hyperbolic sines.
	\param x [in] Angle in radians.
	\return The hyperbolic sine of x.
	*/
	static double sinh(double x) throw()
	{
		return ::sinh(x);
	}
	static float sinh(float x) throw()
	{
		return ::sinhf(x);
	}
	/*! \brief Calculate the square root.

	Calculate the square root.
	\param x [in] The input parameter.
	\return The square root of x.
	*/
	static double sqrt(double x) throw()
	{
		return ::sqrt(x);
	}
	static float sqrt(float x) throw()
	{
		return ::sqrtf(x);
	}
	/*! \brief Calculate the tangent.

	Calculate the tangent.
	\param x [in] Angle in radians.
	\return The tangent of x.
	*/
	static double tan(double x) throw()
	{
		return ::tan(x);
	}
	static float tan(float x) throw()
	{
		return ::tanf(x);
	}
	/*! \brief Calculate hyperbolic tangent.

	Calculate hyperbolic tangent.
	\param x [in] Angle in radians.
	\return The hyperbolic tangent of x.
	*/
	static double tanh(double x) throw()
	{
		return ::tanh(x);
	}
	static float tanh(float x) throw()
	{
		return ::tanhf(x);
	}

	static float ceil(float x) throw()
	{
		return ::ceilf(x);
	}
	static double ceil(double x) throw()
	{
		return ::ceil(x);
	}
	static float floor(float x) throw()
	{
		return ::floorf(x);
	}
	static double floor(double x) throw()
	{
		return ::floor(x);
	}
	/*! \brief Round-off a value to integer.

	Round-off a value to integer.
	\param x [in] The input value.
	\return The rounded result. It can be casted to integer value.
	*/
	static float round(float x) throw()
	{
		return ::roundf(x);
	}
	static double round(double x) throw()
	{
		return ::round(x);
	}
	static float trunc(float x) throw()
	{
		return ::truncf(x);
	}
	static double trunc(double x) throw()
	{
		return ::trunc(x);
	}
};

// SafeOperators
typedef safe_operators  SafeOperators;

// RandHelper
typedef rand_helper  RandHelper;

// NumberHelper
/*! \brief A helper class for number.

A helper class for number.
*/
class NumberHelper
{
public:
	/*! \brief Calculate the greatest common divisor.

	Calculate the greatest common divisor.
	\tparam TType The typename of integer. It can be char,byte,short,ushort,int,uint,int64,uint64.
	\param m, n [in] Two input numbers.
	\return The greatest common divisor.
	*/
	template <typename TType>
	static TType Gcd(const TType& m, const TType& n) throw()
	{
		if( m == 0 )  return n;
		if( n == 0 )  return m;
		TType p = m > n ? m : n;
		TType q = p == m ? n : m;
		TType r = p % q;
		while( r != 0 ) {
			p = q;
			q = r;
			r = p % q;
		}
		return q;
	}

	/*! \brief A sign function.

	A sign function.
	\param x [in] The input value.
	\return An integer value. 0 means x is (or almost) 0, 1 means x is a positive number, -1 means x is a negative number.
	*/
	static int Sign(char x) throw()
	{
		return x == 0 ? 0 : (x & 0x80) ? -1 : 1;
	}
	static int Sign(short x) throw()
	{
		return x == 0 ? 0 : (x & 0x8000) ? -1 : 1;
	}
	static int Sign(int x) throw()
	{
		return x == 0 ? 0 : (x & ((uint)1 << 31)) ? -1 : 1;
	}
	static int Sign(int64 x) throw()
	{
		return x == 0 ? 0 : (x & ((uint64)1 << 63)) ? -1 : 1;
	}
	static int Sign(double x) throw()
	{
		UnionPair<double, uint64> up{x};
		return (x > -DBL_EPSILON && x < DBL_EPSILON) ? 0 : ( up.v2 & ((uint64)1 << 63) ) ? -1 : 1;
	}
	static int Sign(float x) throw()
	{
		UnionPair<float, uint> up{x};
		return (x > -FLT_EPSILON && x < FLT_EPSILON) ? 0 : ( up.v2 & ((uint)1 << 31) ) ? -1 : 1;
	}
	/*! \brief Absolute function.

	Absolute function.
	\param x [in] The input data.
	\return The absolute value.
	*/
	static char Abs(char x) throw()
	{
		return (char)::abs((int)x);
	}
	static short Abs(short x) throw()
	{
		return (short)::abs((int)x);
	}
	static int Abs(int x) throw()
	{
		return ::abs(x);
	}
	static int64 Abs(int64 x) throw()
	{
		return ::llabs(x);  //::_abs64(src); /* for windows */
	}
	static float Abs(float x) throw()
	{
		return ::fabsf(x);
	}
	static double Abs(double x) throw()
	{
		return ::fabs(x);
	}

	//IsOdd
	/*! \brief Check if the number is odd.

	Check if the number is odd.
	\tparam T The number type. It can be char, byte, short, ushort, int, uint, int64 or uint64.
	\param t [in] The integer value.
	\return true for odd number, false for otherwise.
	*/
	template <typename T>
	static bool IsOdd(T t) throw()
	{
		return (t & (T)1) != 0;
	}
};

//------------------------------------------------------------------------------
// Memory

// IMemoryManager

typedef i_memory_manager  IMemoryManager;

// IMemoryAllocatorRef32

typedef i_memory_allocator_ref_32  IMemoryAllocatorRef32;

// IMemoryAllocatorRef32Full

typedef i_memory_allocator_ref_32_full  IMemoryAllocatorRef32Full;

// IMemoryAllocatorRef64

typedef i_memory_allocator_ref_64  IMemoryAllocatorRef64;

// IMemoryAllocatorRef64Full

typedef i_memory_allocator_ref_64_full  IMemoryAllocatorRef64Full;

// UniquePtr<T>
//   This class cannot cross assembly
template <typename T>
using UniquePtr = unique_ptr<T>;

// UniquePtrHelper

typedef unique_ptr_helper  UniquePtrHelper;

// AlignHelper

class AlignHelper
{
public:
	/*! \brief Round up a value to power of 2.

	Round up a value to power of 2.
	\tparam T Integer type.
	\param n [in] The input value.
	\param uAlign [in] It must be a value as 2^N.
	\return A rounded up value.
	*/
	template <typename T>
	static T RoundUp(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		//overflow is not checked
		return T( (n + (uAlign - 1)) & ~(T(uAlign) - 1) );
	}
	template <typename T>
	static CallResult RoundUp(IN T n, IN uint uAlign, OUT T& r) throw()
	{
		assert( uAlign > 0 );
		T v;
		CallResult cr(SafeOperators::Add(n, T(uAlign - 1), v));
		if( cr.IsFailed() )
			return cr;
		r = T( (v) & ~(T(uAlign) - 1) );
		return cr;
	}
	template <typename T>
	static T RoundUpThrow(IN T n, IN uint uAlign)
	{
		assert( uAlign > 0 );
		T v = SafeOperators::AddThrow(n, T(uAlign - 1));
		return T( (v) & ~(T(uAlign) - 1) );
	}

	/*! \brief Round down a value to power of 2.

	Round down a value to power of 2.
	\tparam T Integer type.
	\param n [in] The input value.
	\param uAlign [in] It must be a value as 2^N.
	\return A rounded down value.
	*/
	template <typename T>
	static T RoundDown(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		return T( n & ~(T(uAlign) - 1) );
	}
};

//------------------------------------------------------------------------------
// Time

// TimeSpan
typedef time_span  TimeSpan;

// TimeValue
typedef time_value  TimeValue;

// TimeDetail
typedef time_detail  TimeDetail;

// TimeHelper

class TimeHelper
{
public:
	static bool IsLeapYear(uint uYear) throw()
	{
		return is_leap_year(uYear);
	}
	static void GetTickCount(TimeValue& tv) throw()
	{
		get_tick_count(tv);
	}
	static void FetchCurrentTime(TimeValue& tv) throw()
	{
		get_current_time(tv);
	}
	static bool MakeLocalTime(int iYear, int iMonth, int iDay,
							int iHour, int iMinute, int iSecond, int64 iNanoseconds,
							TimeValue& tv) throw()
	{
		return make_local_time(iYear, iMonth, iDay,
							iHour, iMinute, iSecond, iNanoseconds,
							tv);
	}
	static bool ToGmtDetail(const TimeValue& tv, TimeDetail& td) throw()
	{
		return time_to_gmt_detail(tv, td);
	}
	static bool ToLocalDetail(const TimeValue& tv, TimeDetail& td) throw()
	{
		return time_to_local_detail(tv, td);
	}
};

//------------------------------------------------------------------------------
// File

// file open types
typedef file_open_types  FileOpenTypes;

// file creation types (can combine with <or> operation)
typedef file_creation_types  FileCreationTypes;

// StorageStatus
typedef storage_status  StorageStatus;

//------------------------------------------------------------------------------
// File System

// FileSearcher
typedef file_searcher  FileSearcher;

//------------------------------------------------------------------------------
// Synchronization

// Semaphore
typedef inprocess_semaphore      Semaphore;
typedef interprocess_semaphore   ProcessSemaphore;

// Mutex
typedef inprocess_mutex      Mutex;
typedef interprocess_mutex   ProcessMutex;

// Condition
/*
lock Mutex
while( !test_predicate() )
	Wait
change shared data
unlock Mutex
[option] [lock] [check shared data] Signal or SignalAll for other threads [unlock]
*/
typedef inprocess_condition  Condition;

// RWLock
typedef inprocess_rwlock  RWLock;

// MutexLock

class MutexLock
{
public:
	MutexLock(Mutex& t, bool bInitialLock = true) throw() : m_mtx(t), m_bLocked(false)
	{
		if( bInitialLock )
			Lock();
	}
	~MutexLock() throw()
	{
		if( m_bLocked )
			Unlock();
	}

	void Lock() throw()
	{
		assert( !m_bLocked );
		m_mtx.Deref().Lock();
		m_bLocked = true;
	}
	void Unlock() throw()
	{
		assert( m_bLocked );
		m_mtx.Deref().Unlock();
		m_bLocked = false;
	}
	bool TryLock() throw()
	{
		assert( !m_bLocked );
		bool bRet = m_mtx.Deref().TryLock();
		if( bRet )
			m_bLocked = true;
		return bRet;
	}

private:
	RefPtr<Mutex>  m_mtx;
	bool  m_bLocked;

private:
	//noncopyable
	MutexLock(const MutexLock&) throw();
	MutexLock& operator=(const MutexLock&) throw();
};

// SyncLock<T>

template <class T>
class SyncLock
{
public:
	SyncLock(T& t, bool bInitialLock = true) : m_t(t), m_bLocked(false)
	{
		if( bInitialLock )
			Lock();  //may throw
	}
	~SyncLock() throw()
	{
		if( m_bLocked )
			Unlock();
	}

	void Lock()
	{
		assert( !m_bLocked );
		m_t.Deref().Lock();  //may throw
		m_bLocked = true;
	}
	void Unlock() throw()
	{
		assert( m_bLocked );
		m_t.Deref().Unlock();
		m_bLocked = false;
	}
	bool TryLock() throw()
	{
		assert( !m_bLocked );
		bool bRet = m_t.Deref().TryLock();
		if( bRet )
			m_bLocked = true;
		return bRet;
	}

private:
	RefPtr<T>  m_t;
	bool       m_bLocked;

private:
	//noncopyable
	SyncLock(const SyncLock<T>&) throw();
	SyncLock<T>& operator=(const SyncLock<T>&) throw();
};

// RWLockShared

class RWLockShared
{
public:
	RWLockShared(RWLock& rw, bool bInitialLock = true) throw() : m_rw(rw), m_bLocked(false)
	{
		if( bInitialLock )
			Lock();
	}
	~RWLockShared() throw()
	{
		if( m_bLocked )
			Unlock();
	}

	void Lock() throw()
	{
		assert( !m_bLocked );
		m_rw.Deref().LockShared();
		m_bLocked = true;
	}
	void Unlock() throw()
	{
		assert( m_bLocked );
		m_rw.Deref().UnlockShared();
		m_bLocked = false;
	}
	bool TryLock() throw()
	{
		assert( !m_bLocked );
		bool bRet = m_rw.Deref().TryLockShared();
		if( bRet )
			m_bLocked = true;
		return bRet;
	}

private:
	RefPtr<RWLock>  m_rw;
	bool            m_bLocked;

private:
	//noncopyable
	RWLockShared(const RWLockShared&) throw();
	RWLockShared& operator=(const RWLockShared&) throw();
};

// RWLockExclusive

class RWLockExclusive
{
public:
	RWLockExclusive(RWLock& rw, bool bInitialLock = true) throw() : m_rw(rw), m_bLocked(false)
	{
		if( bInitialLock )
			Lock();
	}
	~RWLockExclusive() throw()
	{
		if( m_bLocked )
			Unlock();
	}

	void Lock() throw()
	{
		assert( !m_bLocked );
		m_rw.Deref().LockExclusive();
		m_bLocked = true;
	}
	void Unlock() throw()
	{
		assert( m_bLocked );
		m_rw.Deref().UnlockExclusive();
		m_bLocked = false;
	}
	bool TryLock() throw()
	{
		assert( !m_bLocked );
		bool bRet = m_rw.Deref().TryLockExclusive();
		if( bRet )
			m_bLocked = true;
		return bRet;
	}

private:
	RefPtr<RWLock>  m_rw;
	bool            m_bLocked;

private:
	//noncopyable
	RWLockExclusive(const RWLockExclusive&) throw();
	RWLockExclusive& operator=(const RWLockExclusive&) throw();
};

//------------------------------------------------------------------------------
// Special Array

// SObjArray<TObj>
/*! \brief An array class using cookie mode.

An array class using cookie mode.
\tparam TObj A type name.
\note This class is not thread-safe.
*/
template <class TObj>
class SObjArray
{
public:
	typedef TObj  EObj;

public:
	/*! \brief Constructor.

	Constructor.
	*/
	SObjArray() throw() : m_p(NULL), m_uCount(0), m_uAlloc(0)
	{
	}
	/*! \brief Destructor.

	Destructor.
	*/
	~SObjArray() throw()
	{
		if( m_p != NULL ) {
			TObj* p = m_p;
			for( uintptr i = 0; i < m_uCount; i ++ ) {
				p->~TObj();
				p ++;
			}
			crt_free(m_p);
		}
	}

	//add
	/*! \brief Add an element.

	Add an element.
	\param obj [in] The element of type T.
	\return A cookie. 0 means failed.
	*/
	uintptr Add(const TObj& obj) throw()
	{
		//find null
		//  Walk array and use empty slots if any.
		uintptr uFind = INVALID_ARRAY_INDEX;
		TObj* p = m_p;
		for( uintptr i = 0; i < m_uCount; i ++ ) {
			if( (*p).IsNullObject() ) {
				uFind = i;
				break;
			}
			p ++;
		}
		if( uFind != INVALID_ARRAY_INDEX ) {
			m_p[uFind] = obj;
			return uFind + 1;  //cookie minus one is index into array
		}
		//No empty slots so resize array.
		//# of new slots is double of current size.
		//add
		if( m_uCount < m_uAlloc ) {
			call_constructor(m_p[m_uCount], obj);  //no throw
			m_uCount ++;
			return m_uCount;  //cookie minus one is index into array
		}
		//resize
		uintptr uAlloc;
		if( m_uCount == 0 ) {
			uAlloc = 4;
		}
		else {
			if( GKC::SafeOperators::Multiply(m_uCount, (uintptr)2, uAlloc).IsFailed() )
				return 0;
		}
		uintptr uBytes;
		if( GKC::SafeOperators::Multiply(uAlloc, (uintptr)sizeof(TObj), uBytes).IsFailed() )
			return 0;
		if( m_p == NULL )
			p = (TObj*)crt_alloc(uBytes);
		else
			p = (TObj*)crt_realloc(m_p, uBytes);
		if( p == NULL )
			return 0;
		//set
		m_p = p;
		m_uAlloc = uAlloc;
		call_constructor(m_p[m_uCount], obj);  //no throw
		m_uCount ++;
		return m_uCount;  //cookie minus one is index into array
	}
	//remove
	/*! \brief Remove an element.

	Remove an element.
	\param uCookie [in] Specify the cookie to be removed.
	*/
	void Remove(uintptr uCookie) throw()
	{
		assert( uCookie > 0 && uCookie <= m_uCount );
		m_p[uCookie - 1].Release();  //cookie minus one is an index into the array
	}
	//get
	/*! \brief Get element by index.

	Get element by index.
	\param index [in] Specify the index.
	\param obj [out] Revice the object.
	*/
	void GetObject(uintptr index, TObj& obj) const throw()
	{
		assert( index >= 0 && index < m_uCount );
		obj = m_p[index];
	}

	/*! \brief Get total size of array.

	Get total size of array.
	\return The number of total elements.
	*/
	uintptr GetCount() const throw()
	{
		return m_uCount;
	}

private:
	TObj*    m_p;       //!< Array of Object
	uintptr  m_uCount;  //!< The number of elements
	uintptr  m_uAlloc;  //!< The allocated number of elements

private:
	//noncopyable
	SObjArray(const SObjArray&) throw();
	SObjArray& operator=(const SObjArray&) throw();
};

// SObjArrayWithLock<TObj>

template <class TObj>
class SObjArrayWithLock : public SObjArray<TObj>
{
private:
	typedef SObjArray<TObj>  baseClass;

public:
	SObjArrayWithLock() throw()
	{
	}
	~SObjArrayWithLock() throw()
	{
	}

	void SetMutex(const RefPtr<Mutex>& mtx) throw()
	{
		m_mtx = mtx;
	}

	uintptr Add(const TObj& obj) throw()
	{
		MutexLock lock(m_mtx.Deref());
		return baseClass::Add(obj);
	}
	void Remove(uintptr uCookie) throw()
	{
		MutexLock lock(m_mtx.Deref());
		baseClass::Remove(uCookie);
	}
	void GetObject(uintptr index, TObj& obj) const throw()
	{
		MutexLock lock(m_mtx.Deref());
		baseClass::GetObject(index, obj);
	}

private:
	RefPtr<Mutex> m_mtx;

private:
	//noncopyable
	SObjArrayWithLock(const SObjArrayWithLock&) throw();
	SObjArrayWithLock& operator=(const SObjArrayWithLock&) throw();
};

//------------------------------------------------------------------------------
//Thread

// ThreadHelper

class ThreadHelper
{
public:
	static void Sleep(uint uTimeout) throw()
	{
		thread_sleep(uTimeout);
	}
};

// ThreadImpl<T>
template <class T>
using ThreadImpl = thread_impl<T>;

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// The macro of SERVICE:
// User must define a macro: GKC_SERVICE_NAME
//   e.g., #define GKC_SERVICE_NAME  _S("Host Server")
//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_DEF_H__
////////////////////////////////////////////////////////////////////////////////
