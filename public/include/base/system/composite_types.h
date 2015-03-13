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

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

// composite types

#if defined(OS_WINDOWS)
	#include "Windows/composite_types.h"
#elif defined(OS_LINUX)
	#include "Linux/composite_types.h"
#else
	#error Error OS type!
#endif

// enumerator

#define BEGIN_ENUM(E)    struct E { \
	public: typedef E x; \
		explicit E(int value = 0) throw() : m_value(value) { } \
		E(const E& src) throw() : m_value(src.m_value) { } \
		E& operator=(const E& src) throw() { \
			if(this!=&src) { m_value = src.m_value; } return *this; } \
		E& operator=(int value) throw() \
			{ m_value = value; return *this; } \
		bool operator==(const E& right) const throw() \
			{ return m_value == right.m_value; } \
		bool operator==(int value) const throw() \
			{ return m_value == value; } \
		bool operator!=(const E& right) const throw() \
			{ return !operator==(right); } \
		bool operator!=(int value) const throw() \
			{ return !operator==(value); } \
		operator int() const throw() { return m_value; } \
		enum __Enum {
#define ENUM_ENTRY(name)      name,
#define ENUM_VALUE_ENTRY(name, value)    name = (value),
#define END_ENUM()  }; private: int m_value; };

//function parameter

#define IN
#define OUT
#define INOUT

//right value
template <typename T>
inline T&& rv_forward(T& t) throw()
{
	return static_cast<T&&>(t);
}

#pragma push_macro("new")
#undef new

//constructor
template <class T, class... Args>
inline void call_constructor(T& t, const Args&... args)
{
	::new(&t) T(args...);
}
template <class T, class... Args>
inline void call_constructor(T& t, Args&&... args)
{
	::new(&t) T(rv_forward<Args>(args)...);
}

#pragma pop_macro("new")

////////////////////////////////////////////////////////////////////////////////
