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
#define DEBUG
#endif

//Machine Type
#if ( !defined(CM_X86) && !defined(CM_X64) )
//windows
#define GKC_OS_WINDOWS
//WIN64
#ifdef _WIN64
#define WIN64
#define CM_X64
#else
#define CM_X86
#endif //WIN64
#else
//linux
#define GKC_OS_LINUX
#endif //Machine Type

//basic types
#include "system/base_types.h"

//composite types
#include "system/composite_types.h"

//os headers
#include "system/os_headers.h"

//other types
#include "system/other_types.h"

//string APIs
#include "system/str_api.h"

//console APIs
#include "system/console_api.h"

//------------------------------------------------------------------------------
//system types which can use exceptions

#include "system/sys_types.h"

#include "system/sys_locale.h"

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Time

// SystemTime
typedef system_time  SystemTime;

//------------------------------------------------------------------------------
// Pointers

// RefPtr<T>

template <typename T>
using RefPtr = ref_ptr<T>;

// RefPtrHelper
typedef ref_ptr_helper  RefPtrHelper;

//------------------------------------------------------------------------------
// Iterator

// ReverseIterator<T>
template <typename T>
using ReverseIterator = reverse_iterator<T>;

//------------------------------------------------------------------------------
// Array

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

//------------------------------------------------------------------------------
// Exceptions

// Exception
typedef exception_base  Exception;

// OutOfMemoryException
typedef outofmemory_exception  OutOfMemoryException;

// OverflowException
typedef overflow_exception  OverflowException;

//------------------------------------------------------------------------------
// number

// Limits<T>

template <typename T>
using Limits = limits_base<T>;

//------------------------------------------------------------------------------
// basic operators

// SafeOperators
typedef safe_operators  SafeOperators;

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

template <typename T1, typename T2>
class Pair
{
private:
	typedef Pair<T1, T2>  thisClass;

public:
	typedef T1  E1;
	typedef T2  E2;

public:
	Pair()
	{
	}
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

	thisClass& operator=(const thisClass& src)
	{
		if( this != &src ) {
			m_t1 = src.m_t1;
			m_t2 = src.m_t2;
		}
		return *this;
	}
	thisClass& operator=(thisClass&& src)
	{
		if( this != &src ) {
			m_t1 = rv_forward(src.m_t1);
			m_t2 = rv_forward(src.m_t2);
		}
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

private:
	T1 m_t1;
	T2 m_t2;
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
// Memory

// IMemoryManager

typedef i_memory_manager  IMemoryManager;

// AlignHelper

class AlignHelper
{
public:
	//T : Integer
	//uAlign : it must be a value as 2^N.
	template <typename T>
	static T RoundUp(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		//overflow is not checked
		return T( (n + (uAlign - 1)) & ~(T(uAlign) - 1) );
	}
	template <typename T>
	static T RoundUpThrow(IN T n, IN uint uAlign)
	{
		assert( uAlign > 0 );
		T v = SafeOperators::AddThrow(n, T(uAlign - 1));
		return T( (v) & ~(T(uAlign) - 1) );
	}
	template <typename T>
	static T RoundDown(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		return T( n & ~(T(uAlign) - 1) );
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

// FixedStringT<Tchar, t_size>
template <typename Tchar, uintptr t_size>
using FixedStringT = fixed_string_t<Tchar, t_size>;

// FixedStringCompareTrait<T>

template <class T>
class FixedStringCompareTrait
{
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

// FixedStringHelper
typedef fixed_string_helper  FixedStringHelper;

// MessageException<t_size>
template <uintptr t_size>
using MessageException = message_exception<t_size>;

//------------------------------------------------------------------------------
// Swap

template <typename T>
inline void Swap(T& t1, T& t2)
{
	assert( &t1 != &t2 );
	T tmp = static_cast<T&&>(t1);
	t1 = static_cast<T&&>(t2);
	t2 = static_cast<T&&>(tmp);
}

template <>
inline void Swap<int>(int& t1, int& t2)
{
	assert( &t1 != &t2 );
	t1 ^= t2 ^= t1 ^= t2;
}

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
[option] [lock] [check shared data] Signal or SignalAll for other thread [unlock]
*/
typedef inprocess_condition  Condition;

// RWLock
typedef inprocess_rwlock  RWLock;

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
