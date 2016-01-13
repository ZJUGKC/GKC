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
// logic

// LogicalOperators

class LogicalOperators
{
public:
	template <class T>
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return t1 == t2;
	}
	template <class T>
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return t1 != t2;
	}
	template <class T>
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return t1 < t2;
	}
	template <class T>
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return t1 > t2;
	}
	template <class T>
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return t1 <= t2;
	}
	template <class T>
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return t1 >= t2;
	}
};

//special versions
template <>
inline bool LogicalOperators::IsEQ<float>(const float& t1, const float& t2) throw()
{
	return ::fabsf(t1 - t2) < FLT_EPSILON;
}

template <>
inline bool LogicalOperators::IsEQ<double>(const double& t1, const double& t2) throw()
{
	return ::fabs(t1 - t2) < DBL_EPSILON;
}

template <>
inline bool LogicalOperators::IsNE<float>(const float& t1, const float& t2) throw()
{
	return ::fabsf(t1 - t2) >= FLT_EPSILON;
}

template <>
inline bool LogicalOperators::IsNE<double>(const double& t1, const double& t2) throw()
{
	return ::fabs(t1 - t2) >= DBL_EPSILON;
}

template <>
inline bool LogicalOperators::IsLT<float>(const float& t1, const float& t2) throw()
{
	return t1 + FLT_EPSILON <= t2;
}

template <>
inline bool LogicalOperators::IsLT<double>(const double& t1, const double& t2) throw()
{
	return t1 + DBL_EPSILON <= t2;
}

template <>
inline bool LogicalOperators::IsGT<float>(const float& t1, const float& t2) throw()
{
	return IsLT<float>(t2, t1);
}

template <>
inline bool LogicalOperators::IsGT<double>(const double& t1, const double& t2) throw()
{
	return IsLT<double>(t2, t1);
}

template <>
inline bool LogicalOperators::IsLE<float>(const float& t1, const float& t2) throw()
{
	return !IsGT<float>(t1, t2);
}

template <>
inline bool LogicalOperators::IsLE<double>(const double& t1, const double& t2) throw()
{
	return !IsGT<double>(t1, t2);
}

template <>
inline bool LogicalOperators::IsGE<float>(const float& t1, const float& t2) throw()
{
	return !IsLT<float>(t1, t2);
}

template <>
inline bool LogicalOperators::IsGE<double>(const double& t1, const double& t2) throw()
{
	return !IsLT<double>(t1, t2);
}

//------------------------------------------------------------------------------
//Traits

// DefaultCompareTrait<T>

template <typename T>
class DefaultCompareTrait
{
public:
	//common versions
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsEQ(t1, t2);
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsNE(t1, t2);
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsGT(t1, t2);
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsLT(t1, t2);
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsGE(t1, t2);
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return LogicalOperators::IsLE(t1, t2);
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		if( IsLT(t1, t2 ) )
			return -1;
		if( IsEQ(t1, t2) )
			return 0;
		assert( IsGT(t1, t2) );
		return 1;
	}
};

//special versions


// DefaultHashTrait<T>

template <typename T>
class DefaultHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		return (uintptr)t;
	}
};

//special versions


/*
3 layers:
native : == != ...
LogicalOperators
<Element>Trait
*/

//------------------------------------------------------------------------------
//iterator

// ReverseIterator<T>

template <typename T>
class ReverseIterator
{
public:
	ReverseIterator() throw()
	{
	}
	explicit ReverseIterator(const T& iter) throw() : m_iter(iter)
	{
	}
	ReverseIterator(const ReverseIterator<T>& src) throw() : m_iter(src.m_iter)
	{
	}
	~ReverseIterator() throw()
	{
	}

	//operators
	ReverseIterator<T>& operator=(const ReverseIterator<T>& src) throw()
	{
		if( this != &src ) {
			m_iter = src.m_iter;
		}
		return *this;
	}

	bool IsNull() const throw()
	{
		return m_iter.IsNull();
	}

	//logical
	bool operator==(const ReverseIterator<T>& right) const throw()
	{
		return m_iter == right.m_iter;
	}
	bool operator!=(const ReverseIterator<T>& right) const throw()
	{
		return m_iter != right.m_iter;
	}

	const typename T::EType& get_Value() const throw()
	{
		T tmp(m_iter);
		tmp.MovePrev();
		return tmp.get_Value();
	}
	typename T::EType& get_Value() throw()
	{
		T tmp(m_iter);
		tmp.MovePrev();
		return tmp.get_Value();
	}
	void set_Value(const typename T::EType& t)  //may throw
	{
		T tmp(m_iter);
		tmp.MovePrev();
		tmp.set_Value(t);
	}
	void set_Value(typename T::EType&& t)  //may throw
	{
		T tmp(m_iter);
		tmp.MovePrev();
		tmp.set_Value(rv_forward(t));
	}

	//methods
	void MoveNext() throw()
	{
		m_iter.MovePrev();
	}
	void MovePrev() throw()
	{
		m_iter.MoveNext();
	}
	void MoveDelta(intptr iDelta) throw()
	{
		m_iter.MoveDelta(iDelta);
	}
	intptr CalcDelta(const ReverseIterator<T>& second) const throw()
	{
		return m_iter.CalcDelta(second.m_iter);
	}

private:
	T m_iter;
};

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
// Memory

// IMemoryManager

typedef i_memory_manager  IMemoryManager;

//------------------------------------------------------------------------------
// File

// file open types
typedef file_open_types  FileOpenTypes;

// file creation types (can combine with <or> operation)
typedef file_creation_types  FileCreationTypes;

// FileStatus
typedef file_status  FileStatus;

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// The macro of SERVICE:
// User must define a macro: GKC_SERVICE_NAME
//   e.g., #define GKC_SERVICE_NAME  _S("Host Server")

//------------------------------------------------------------------------------
//system types which can use exceptions

#include "system/sys_types.h"

#include "system/sys_locale.h"

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// IO

// IoHandle

typedef io_handle  IoHandle;

// IoHandleHelper

typedef io_handle_helper  IoHandleHelper;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_DEF_H__
////////////////////////////////////////////////////////////////////////////////
