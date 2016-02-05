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

// -----Iterator-----

// reverse_iterator<T>

template <typename T>
class reverse_iterator
{
public:
	reverse_iterator() throw()
	{
	}
	explicit reverse_iterator(const T& iter) throw() : m_iter(iter)
	{
	}
	reverse_iterator(const reverse_iterator<T>& src) throw() : m_iter(src.m_iter)
	{
	}
	~reverse_iterator() throw()
	{
	}

	//operators
	reverse_iterator<T>& operator=(const reverse_iterator<T>& src) throw()
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
	bool operator==(const reverse_iterator<T>& right) const throw()
	{
		return m_iter == right.m_iter;
	}
	bool operator!=(const reverse_iterator<T>& right) const throw()
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
	intptr CalcDelta(const reverse_iterator<T>& second) const throw()
	{
		return m_iter.CalcDelta(second.m_iter);
	}

private:
	T m_iter;
};

// -----Array-----

// array_iterator<T>

template <typename T>
class array_iterator
{
public:
	typedef T  EType;

public:
	array_iterator() throw()
	{
	}
	explicit array_iterator(const ref_ptr<T>& element) throw() : m_element(element)
	{
	}
	array_iterator(const array_iterator<T>& src) throw() : m_element(src.m_element)
	{
	}
	~array_iterator() throw()
	{
	}

	array_iterator<T>& operator=(const array_iterator<T>& src) throw()
	{
		if( this != &src ) {
			m_element = src.m_element;
		}
		return *this;
	}

	bool IsNull() const throw()
	{
		return m_element.IsNull();
	}

	const T& get_Value() const throw()
	{
		return m_element.Deref();
	}
	T& get_Value() throw()
	{
		return m_element.Deref();
	}
	void set_Value(const T& t)
	{
		m_element.Deref() = t;
	}
	void set_Value(T&& t)
	{
		m_element.Deref() = rv_forward(t);
	}

	//logical
	bool operator==(const array_iterator<T>& right) const throw()
	{
		return m_element == right.m_element;
	}
	bool operator!=(const array_iterator<T>& right) const throw()
	{
		return m_element != right.m_element;
	}
	bool operator<(const array_iterator<T>& right) const throw()
	{
		return m_element < right.m_element;
	}
	bool operator>(const array_iterator<T>& right) const throw()
	{
		return right < *this;
	}
	bool operator<=(const array_iterator<T>& right) const throw()
	{
		return !operator>(right);
	}
	bool operator>=(const array_iterator<T>& right) const throw()
	{
		return !operator<(right);
	}

	//methods
	void MoveNext() throw()
	{
		m_element = &(m_element.Deref()) + 1;
	}
	void MovePrev() throw()
	{
		m_element = &(m_element.Deref()) - 1;
	}
	void MoveDelta(intptr iDelta) throw()
	{
		m_element = &(m_element.Deref()) + iDelta;
	}
	intptr CalcDelta(const array_iterator<T>& second) const throw()
	{
		return &(m_element.Deref()) - &(second.m_element.Deref());
	}

private:
	ref_ptr<T> m_element;
};

//const definition:
// .h:   class C { ... public: static const T c_name; };
// .cpp: const T C::c_name=...;

// const_array_base<T>

template <typename T>
struct const_array_base
{
	const T*  m_first;
	uintptr   m_size;
};

// const_array<T>

template <typename T>
class const_array : public const_array_base<T>
{
private:
	typedef const_array_base<T>  baseClass;

public:
	typedef T  EType;
	typedef array_iterator<T>  Iterator;

public:
	const_array() throw()
	{
		baseClass::m_first = NULL;
		baseClass::m_size  = 0;
	}
	const_array(const T* p, uintptr size) throw()
	{
		baseClass::m_first = p;
		baseClass::m_size  = size;
	}
	explicit const_array(const const_array_base<T>& src) throw()
	{
		baseClass::m_first = src.m_first;
		baseClass::m_size  = src.m_size;
	}
	const_array(const const_array<T>& src) throw()
	{
		baseClass::m_first = src.m_first;
		baseClass::m_size  = src.m_size;
	}
	~const_array() throw()
	{
	}

	//operators
	const_array<T>& operator=(const const_array<T>& src) throw()
	{
		if( this != &src ) {
			baseClass::m_first = src.m_first;
			baseClass::m_size  = src.m_size;
		}
		return *this;
	}

	bool operator==(const const_array<T>& right) const throw()
	{
		return baseClass::m_first == right.m_first;
	}
	bool operator!=(const const_array<T>& right) const throw()
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
		return Iterator(ref_ptr<T>(baseClass::m_first));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(ref_ptr<T>(baseClass::m_first + GetCount()));
	}
	const reverse_iterator<Iterator> GetReverseBegin() const throw()
	{
		return reverse_iterator<Iterator>(GetEnd());
	}
	const reverse_iterator<Iterator> GetReverseEnd() const throw()
	{
		return reverse_iterator<Iterator>(GetBegin());
	}

	const Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Iterator(ref_ptr<T>(baseClass::m_first + index));
	}

private:
	friend class const_array_helper;
};

// macros

// --<header file>--

// define static constant array

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

// --<.h end>--

// --<source file>--

// for definitions in .cpp

//static constant array
#define BEGIN_STATIC_CONST_ARRAY(cls)    const cls::EType cls::c_array[] = {

#define STATIC_CONST_ARRAY_ENTRY(x)           x,
#define STATIC_CONST_ARRAY_ENTRY_LAST(y)      y

#define BEGIN_STATIC_CONST_ARRAY_GROUP()      {
#define END_STATIC_CONST_ARRAY_GROUP()        },
#define END_STATIC_CONST_ARRAY_GROUP_LAST()   }

#define END_STATIC_CONST_ARRAY(cls)   };  \
		const uintptr cls::c_size = sizeof(cls::c_array) / sizeof(cls::c_array[0]) - 1;

// --<.cpp end>--

// const_array_helper

class const_array_helper
{
public:
	//set internal pointer
	template <typename T>
	static void SetInternalPointer(const T* p, uintptr size, const_array<T>& arr) throw()
	{
		arr.m_first = p;
		arr.m_size  = size;
	}
	//get internal pointer
	template <typename T>
	static const T* GetInternalPointer(const const_array<T>& arr) throw()
	{
		return arr.m_first;
	}
};

// const_helper

class const_helper
{
public:
	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static const TDest& TypeCast(const TSrc& src) throw()
	{
		return static_cast<const TDest&>(src);
	}
};

////////////////////////////////////////////////////////////////////////////////
