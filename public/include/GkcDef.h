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
#define OS_WINDOWS
//WIN64
#ifdef _WIN64
#define WIN64
#define CM_X64
#else
#define CM_X86
#endif //WIN64
#else
//linux
#define OS_LINUX
#endif //Machine Type

//basic types
#include "system/base_types.h"

//composite types
#include "system/composite_types.h"

//os headers
#include "system/os_headers.h"

//other types
#include "system/other_types.h"

//string types
#include "system/str_types.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CallResult

typedef call_result  CallResult;

// CallResult constants

BEGIN_ENUM(SystemCallResults)
	ENUM_VALUE_ENTRY(OK, CR_OK)
	ENUM_VALUE_ENTRY(OutOfMemory, CR_OUTOFMEMORY)
	ENUM_VALUE_ENTRY(Overflow, CR_OVERFLOW)
END_ENUM()

//------------------------------------------------------------------------------
// Exceptions

// Exception

class Exception
{
public:
	Exception() throw()
	{
	}
	explicit Exception(const CallResult& res) throw() : m_result(res)
	{
	}
	Exception(const Exception& src) throw() : m_result(src.m_result)
	{
	}
	~Exception() throw()
	{
	}

	//operators
	Exception& operator=(const Exception& src) throw()
	{
		if( &src != this ) {
			m_result = src.m_result;
		}
		return *this;
	}

	//methods
	CallResult GetResult() const throw()
	{
		return m_result;
	}
	void SetResult(const CallResult& res) throw()
	{
		m_result = res;
	}

protected:
	CallResult m_result;
};

// OutOfMemoryException

class OutOfMemoryException : public Exception
{
public:
	OutOfMemoryException() throw() : Exception(CallResult(SystemCallResults::OutOfMemory))
	{
	}
};

// OverflowException

class OverflowException : public Exception
{
public:
	OverflowException() throw() : Exception(CallResult(SystemCallResults::Overflow))
	{
	}
};

//------------------------------------------------------------------------------
// number

// Limits<T>

template <typename T>
class Limits;

//special
template <>
class Limits<char>
{
public:
	static const char Lowest = SCHAR_MIN;
	static const char Min = SCHAR_MIN;
	static const char Max = SCHAR_MAX;
};
template <>
class Limits<byte>
{
public:
	static const byte Lowest = 0;
	static const byte Min = 0;
	static const byte Max = UCHAR_MAX;
};

template <>
class Limits<short>
{
public:
	static const short Lowest = SHRT_MIN;
	static const short Min = SHRT_MIN;
	static const short Max = SHRT_MAX;
};
template <>
class Limits<ushort>
{
public:
	static const ushort Lowest = 0;
	static const ushort Min = 0;
	static const ushort Max = USHRT_MAX;
};

template <>
class Limits<int>
{
public:
	static const int Lowest = INT_MIN;
	static const int Min = INT_MIN;
	static const int Max = INT_MAX;
};
template <>
class Limits<uint>
{
public:
	static const uint Lowest = 0;
	static const uint Min = 0;
	static const uint Max = UINT_MAX;
};

template <>
class Limits<int64>
{
public:
	static const int64 Lowest = LLONG_MIN;
	static const int64 Min = LLONG_MIN;
	static const int64 Max = LLONG_MAX;
};
template <>
class Limits<uint64>
{
public:
	static const uint64 Lowest = 0;
	static const uint64 Min = 0;
	static const uint64 Max = ULLONG_MAX;
};

template <>
class Limits<float>
{
public:
	static const float Lowest;
	static const float Min;
	static const float Max;
};
template <>
class Limits<double>
{
public:
	static const double Lowest;
	static const double Min;
	static const double Max;
};

//------------------------------------------------------------------------------
// basic operators

// SafeOperators

class SafeOperators
{
public:
	template <typename T>
	static CallResult Add(IN T& left, IN T& right, OUT T& result) throw()
	{
		if( Limits<T>::Max - left < right ) {
			return CallResult(SystemCallResults::Overflow);
		}
		result = left + right;
		return CallResult(SystemCallResults::OK);
	}

	template <typename T>
	static CallResult Multiply(IN T& left, IN T& right, OUT T& result) throw()
	{
		//avoid divide 0
		if( left == 0 ) {
			result = 0;
			return CallResult(SystemCallResults::OK);
		}
		if( Limits<T>::Max / left < right ) {
			return CallResult(SystemCallResults::Overflow);
		}
		result = left * right;
		return CallResult(SystemCallResults::OK);
	}

	//throw version
	template <typename T>
	static T AddThrow(IN T& left, IN T& right)
	{
		T result;
		CallResult cr = Add(left, right, result);
		if( cr.IsFailed() ) {
			throw( Exception(cr) );
		}
		return result;
	}
	template <typename T>
	static T MultiplyThrow(IN T& left, IN T& right)
	{
		T result;
		CallResult cr = Multiply(left, right, result);
		if( cr.IsFailed() ) {
			throw( Exception(cr) );
		}
		return result;
	}
};

//special
template <>
inline CallResult SafeOperators::Multiply<int>(IN int& left, IN int& right, OUT int& result) throw()
{
	int64 result64 = static_cast<int64>(left) * static_cast<int64>(right);
	if( result64 > Limits<int>::Max || result64 < Limits<int>::Min ) {
		return CallResult(SystemCallResults::Overflow);
	}
	result = static_cast<int>(result64);
	return CallResult(SystemCallResults::OK);
}

template <>
inline CallResult SafeOperators::Multiply<uint>(IN uint& left, IN uint& right, OUT uint& result) throw()
{
	uint64 result64 = static_cast<uint64>(left) * static_cast<uint64>(right);
	if( result64 > Limits<uint>::Max ) {
		return CallResult(SystemCallResults::Overflow);
	}
	result = static_cast<uint>(result64);
	return CallResult(SystemCallResults::OK);
}

//------------------------------------------------------------------------------
// logic

// LogicalOperators

class LogicalOperators
{
public:
	template <class T>
	static bool IsEqual(const T& t1, const T& t2) throw()
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
		return IsLT<T>(t2, t1);
	}
	template <class T>
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return !IsGT<T>(t1, t2);
	}
	template <class T>
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return !IsLT<T>(t1, t2);
	}
};

//special
template <>
inline bool LogicalOperators::IsEqual<float>(const float& t1, const float& t2) throw()
{
	return ::fabsf(t1 - t2) < FLT_EPSILON;
}

template <>
inline bool LogicalOperators::IsEqual<double>(const double& t1, const double& t2) throw()
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

	//logical
	bool operator==(const ReverseIterator<T>& right) const throw()
	{
		return m_iter == right.m_iter;
	}
	bool operator!=(const ReverseIterator<T>& right) const throw()
	{
		return m_iter != right.m_iter;
	}

	T& get_Value() throw()
	{
		T tmp(m_iter);
		tmp.MovePrev();
		return tmp.get_Value();
	}
	void set_Value(T& t)
	{
		T tmp(m_iter);
		tmp.MovePrev();
		tmp.set_Value(t);
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

private:
	T m_iter;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_DEF_H__
////////////////////////////////////////////////////////////////////////////////
