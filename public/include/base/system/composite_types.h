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

#if defined(GKC_OS_WINDOWS)
	#include "Windows/composite_types.h"
#elif defined(GKC_OS_LINUX)
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
template <class T, typename... Args>
inline void call_constructor(T& t, Args&&... args)
{
	::new(&t) T(rv_forward<Args>(args)...);
}

#pragma pop_macro("new")

//check derivations
template <class T, class TBase>
inline bool is_derived_from() throw()
{
	T* p = (T*)65536;
	TBase* pb = static_cast<TBase*>(p);  //may be a compile time error
	return pb != (TBase*)0;
}

//------------------------------------------------------------------------------
//classes

// -----Time-----

// system_time

struct system_time
{
	ushort uYear;
	ushort uMonth;         //1--12
	ushort uDayOfWeek;     //0--6, since Sunday
	ushort uDay;           //1--31
	ushort uHour;          //0--23
	ushort uMinute;        //0--59
	ushort uSecond;        //0--59/60
	ushort uMilliseconds;  //0--999
};

// -----Pointers-----

// ref_ptr<T>

template <typename T>
class ref_ptr
{
public:
	ref_ptr() throw() : m_p(NULL)
	{
	}
	explicit ref_ptr(const T& t) throw() : m_p(&(const_cast<T&>(t)))
	{
	}
	explicit ref_ptr(const T* p) throw() : m_p(const_cast<T*>(p))
	{
	}
	ref_ptr(const ref_ptr<T>& src) throw() : m_p(src.m_p)
	{
	}
	~ref_ptr() throw()
	{
	}

	void Release() throw()
	{
		m_p = NULL;
	}

	//operators
	ref_ptr<T>& operator=(const ref_ptr<T>& src) throw()
	{
		if( this != &src ) {
			m_p = src.m_p;
		}
		return *this;
	}
	ref_ptr<T>& operator=(T* p) throw()
	{
		m_p = p;
		return *this;
	}

	//logical
	bool operator==(const ref_ptr<T>& right) const throw()
	{
		return m_p == right.m_p;
	}
	bool operator!=(const ref_ptr<T>& right) const throw()
	{
		return !(*this == right);
	}
	bool operator<(const ref_ptr<T>& right) const throw()
	{
		return m_p < right.m_p;
	}
	bool operator>(const ref_ptr<T>& right) const throw()
	{
		return right < *this;
	}
	bool operator<=(const ref_ptr<T>& right) const throw()
	{
		return !operator>(right);
	}
	bool operator>=(const ref_ptr<T>& right) const throw()
	{
		return !operator<(right);
	}

	//methods
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	const T& Deref() const throw()
	{
		assert( !IsNull() );
		return *m_p;
	}
	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_p;
	}

private:
	T* m_p;

private:
	friend class ref_ptr_helper;
};

// ref_ptr_helper

class ref_ptr_helper
{
public:
	template <typename T>
	static ref_ptr<T> MakeRefPtr(const T& t) throw()
	{
		return ref_ptr<T>(t);
	}
	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static ref_ptr<TDest> TypeCast(const ref_ptr<TSrc>& t) throw()
	{
		assert( !t.IsNull() );
		return ref_ptr<TDest>(static_cast<const TDest&>(t.Deref()));
	}
	//clone
	template <typename T>
	static void Clone(const ref_ptr<T>& tSrc, ref_ptr<T>& tDest) //may throw
	{
		if( tSrc.m_p != tDest.m_p && !tSrc.IsNull() && !tDest.IsNull() ) {
			tDest.Deref() = tSrc.Deref();
		}
	}

	//get internal pointer
	template <typename T>
	static T* GetInternalPointer(const ref_ptr<T>& t) throw()
	{
		return t.m_p;
	}
};

// weak_object_ref<T>
//   T: must have GetHandle, Attach and Detach methods
template <class T>
class weak_object_ref
{
public:
	weak_object_ref() throw()
	{
	}
	weak_object_ref(const weak_object_ref& src) throw()
	{
		//m_t is null
		m_t.Attach(src.m_t.GetHandle());
	}
	weak_object_ref(const T& t) throw()
	{
		m_t.Attach(t.GetHandle());
	}
	~weak_object_ref() throw()
	{
		m_t.Detach();
	}

	//operators
	weak_object_ref& operator=(const weak_object_ref& src) throw()
	{
		if( this != &src ) {
			m_t.Detach();
			m_t.Attach(src.m_t.GetHandle());
		}
		return *this;
	}
	weak_object_ref& operator=(const T& t) throw()
	{
		m_t.Detach();
		m_t.Attach(t.GetHandle());
		return *this;
	}

	//methods
	const T& GetObject() const throw()
	{
		return m_t;
	}
	T& GetObject() throw()
	{
		return m_t;
	}

private:
	T m_t;  //object may contain a pointer (or a handle) from system call or third party library

private:
	weak_object_ref(T&& t) throw();
	weak_object_ref& operator=(T&& t) throw();
};

////////////////////////////////////////////////////////////////////////////////
