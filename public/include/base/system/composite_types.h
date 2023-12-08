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

#include <assert.h>

// enumerator

#define BEGIN_ENUM(E)    struct E { \
	public: typedef E x; \
		explicit E(int value = 0) throw() : m_value(value) { } \
		E(const E& src) throw() : m_value(src.m_value) { } \
		E& operator=(const E& src) throw() \
			{ m_value = src.m_value; return *this; } \
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

//destructor
template <class T>
inline void call_destructor(T& t) throw()
{
	t.~T();
}

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

// -----Pointers-----

// ref_ptr<T>

#pragma pack(push, 1)

/*! \brief A template class for unmanaged type pointer.

A template class for unmanaged type pointer. This means it will not delete
object which is pointed by its member when it destroies.
\tparam T The type.
*/
template <typename T>
class ref_ptr
{
public:
	/*! \brief Constructor

	Constructor.
	*/
	ref_ptr() throw() : m_p(NULL)
	{
	}
	explicit ref_ptr(const T& t) throw() : m_p(&(const_cast<T&>(t)))
	{
	}
	explicit ref_ptr(const T* p) throw() : m_p(const_cast<T*>(p))
	{
	}
	/*! \brief Copy constructor

	Copy constructor.
	*/
	ref_ptr(const ref_ptr<T>& src) throw() : m_p(src.m_p)
	{
	}
	ref_ptr(ref_ptr<T>&& src) throw() : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	/*! \brief Destructor

	Destructor.
	*/
	~ref_ptr() throw()
	{
	}

	void Release() throw()
	{
		m_p = NULL;
	}

	//operators

	/*! \brief Assignment

	Assignment.
	\return A reference of this class
	*/
	ref_ptr<T>& operator=(const ref_ptr<T>& src) throw()
	{
		m_p = src.m_p;
		return *this;
	}
	ref_ptr<T>& operator=(ref_ptr<T>&& src) throw()
	{
		if( &src != this ) {
			m_p = src.m_p;
			src.m_p = NULL;
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
	bool IsNullObject() const throw()
	{
		return IsNull();
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
	T* m_p;  //!< type pointer

private:
	friend class ref_ptr_helper;
};

#pragma pack(pop)

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

// -----Pair-----

// union_pair<T1, T2>

template <typename T1, typename T2>
struct union_pair
{
	union { T1 v1; T2 v2; };
};

// -----Iterator-----

// reverse_iterator<T>
/*! \brief A reverse iterator class.

A reverse iterator class.
\tparam T An iterator class.
*/
template <typename T>
class reverse_iterator
{
public:
	/*! \brief Constructor.

	Constructor.
	*/
	reverse_iterator() throw()
	{
	}
	explicit reverse_iterator(const T& iter) throw() : m_iter(iter)
	{
	}
	/*! \brief Copy Constructor.

	Copy Constructor.
	*/
	reverse_iterator(const reverse_iterator<T>& src) throw() : m_iter(src.m_iter)
	{
	}
	~reverse_iterator() throw()
	{
	}

	//operators
	/*! \brief Assignment operator.

	Assignment operator.
	*/
	reverse_iterator<T>& operator=(const reverse_iterator<T>& src) throw()
	{
		m_iter = src.m_iter;
		return *this;
	}

	bool IsNull() const throw()
	{
		return m_iter.IsNull();
	}

	//logical
	/*! \brief Equal operator

	Equal operator.
	*/
	bool operator==(const reverse_iterator<T>& right) const throw()
	{
		return m_iter == right.m_iter;
	}
	/*! \brief Nonequal operator

	Nonequal operator.
	*/
	bool operator!=(const reverse_iterator<T>& right) const throw()
	{
		return m_iter != right.m_iter;
	}

	const typename T::EType& get_Value() const throw()
	{
		return m_iter.get_Value();
	}
	typename T::EType& get_Value() throw()
	{
		return m_iter.get_Value();
	}
	void set_Value(const typename T::EType& t)  //may throw
	{
		m_iter.set_Value(t);
	}
	void set_Value(typename T::EType&& t)  //may throw
	{
		m_iter.set_Value(rv_forward(t));
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

protected:
	T m_iter;  //!< A wrapper iterator object.
};

// reverse_iterator2<T>
/*! \brief Another reverse iterator class.

Another reverse iterator class.
\tparam T An iterator class.
*/
template <typename T>
class reverse_iterator2 : public reverse_iterator<T>
{
private:
	typedef reverse_iterator<T>  baseClass;

public:
	/*! \brief Constructor.

	Constructor.
	*/
	reverse_iterator2() throw()
	{
	}
	explicit reverse_iterator2(const T& iter) throw() : baseClass(iter)
	{
	}
	/*! \brief Copy Constructor.

	Copy Constructor.
	*/
	reverse_iterator2(const reverse_iterator2<T>& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	~reverse_iterator2() throw()
	{
	}

	//operators
	/*! \brief Assignment operator.

	Assignment operator.
	*/
	reverse_iterator2<T>& operator=(const reverse_iterator2<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}

	//logical
	/*! \brief Equal operator

	Equal operator.
	*/
	bool operator==(const reverse_iterator2<T>& right) const throw()
	{
		return baseClass::operator==(static_cast<const baseClass&>(right));
	}
	/*! \brief Nonequal operator

	Nonequal operator.
	*/
	bool operator!=(const reverse_iterator2<T>& right) const throw()
	{
		return baseClass::operator!=(static_cast<const baseClass&>(right));
	}

	const typename T::EType& get_Value() const throw()
	{
		T tmp(baseClass::m_iter);
		tmp.MovePrev();
		return tmp.get_Value();
	}
	typename T::EType& get_Value() throw()
	{
		T tmp(baseClass::m_iter);
		tmp.MovePrev();
		return tmp.get_Value();
	}
	void set_Value(const typename T::EType& t)  //may throw
	{
		T tmp(baseClass::m_iter);
		tmp.MovePrev();
		tmp.set_Value(t);
	}
	void set_Value(typename T::EType&& t)  //may throw
	{
		T tmp(baseClass::m_iter);
		tmp.MovePrev();
		tmp.set_Value(rv_forward(t));
	}
};

// -----Array-----

// INVALID_ARRAY_INDEX

#define INVALID_ARRAY_INDEX  ((uintptr)-1)

// array_position

class array_position
{
public:
	explicit array_position(uintptr uIndex = INVALID_ARRAY_INDEX) throw() : m_uIndex(uIndex)
	{
	}
	array_position(const array_position& src) throw() : m_uIndex(src.m_uIndex)
	{
	}
	~array_position() throw()
	{
	}

	//operators
	array_position& operator=(const array_position& src) throw()
	{
		m_uIndex = src.m_uIndex;
		return *this;
	}

	bool IsValid() const throw()
	{
		return m_uIndex != INVALID_ARRAY_INDEX;
	}

	uintptr GetIndex() const throw()
	{
		return m_uIndex;
	}
	void SetIndex(uintptr uIndex) throw()
	{
		m_uIndex = uIndex;
	}

	//logical
	bool operator==(const array_position& right) const throw()
	{
		return m_uIndex == right.m_uIndex;
	}
	bool operator!=(const array_position& right) const throw()
	{
		return m_uIndex != right.m_uIndex;
	}
	bool operator<(const array_position& right) const throw()
	{
		return m_uIndex < right.m_uIndex;
	}
	bool operator>(const array_position& right) const throw()
	{
		return right < *this;
	}
	bool operator<=(const array_position& right) const throw()
	{
		return !(*this > right);
	}
	bool operator>=(const array_position& right) const throw()
	{
		return !(*this < right);
	}

	//methods
	void MoveNext() throw()
	{
		m_uIndex ++;
	}
	void MovePrev() throw()
	{
		m_uIndex --;
	}
	void MoveDelta(intptr iDelta) throw()
	{
		m_uIndex += iDelta;
	}
	intptr CalcDelta(const array_position& second) const throw()
	{
		return (intptr)(m_uIndex - second.m_uIndex);
	}

private:
	uintptr  m_uIndex;
};

// array_iterator<T>
/*! \brief A template iterator class for array classes.

A template iterator class for array classes.
\tparam T A typename for element.
*/
template <typename T>
class array_iterator
{
public:
	typedef T  EType;

public:
	/*! \brief Constructor.

	Constructor.
	*/
	array_iterator() throw()
	{
	}
	explicit array_iterator(const ref_ptr<T>& element) throw() : m_element(element)
	{
	}
	/*! \brief Copy Constructor.

	Copy Constructor.
	*/
	array_iterator(const array_iterator<T>& src) throw() : m_element(src.m_element)
	{
	}
	~array_iterator() throw()
	{
	}

	/*! \brief Assignment operator.

	Assignment operator.
	*/
	array_iterator<T>& operator=(const array_iterator<T>& src) throw()
	{
		m_element = src.m_element;
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

	const ref_ptr<T> get_Ref() const throw()
	{
		return m_element;
	}
	ref_ptr<T> get_Ref() throw()
	{
		return m_element;
	}

	//logical

	/*! \brief Equal operator

	Equal operator.
	*/
	bool operator==(const array_iterator<T>& right) const throw()
	{
		return m_element == right.m_element;
	}
	/*! \brief Nonequal operator

	Nonequal operator.
	*/
	bool operator!=(const array_iterator<T>& right) const throw()
	{
		return m_element != right.m_element;
	}
	/*! \brief LT operator

	LT operator.
	*/
	bool operator<(const array_iterator<T>& right) const throw()
	{
		return m_element < right.m_element;
	}
	/*! \brief GT operator

	GT operator.
	*/
	bool operator>(const array_iterator<T>& right) const throw()
	{
		return right < *this;
	}
	/*! \brief LE operator

	LE operator.
	*/
	bool operator<=(const array_iterator<T>& right) const throw()
	{
		return !operator>(right);
	}
	/*! \brief GE operator

	GE operator.
	*/
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
	ref_ptr<T> m_element;  //!< A pointer to current element.
};

//const definition:
// .h:   class C { ... public: static const T c_name; };
// .cpp: const T C::c_name=...;

#pragma pack(push, 1)

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
	typedef array_position  Position;
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
	//use default destructor

	//operators
	const_array<T>& operator=(const const_array<T>& src) throw()
	{
		baseClass::m_first = src.m_first;
		baseClass::m_size  = src.m_size;
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

	const T& operator[](uintptr index) const throw()
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

	//position
	Position GetStartPosition() const throw()
	{
		return Position(0);
	}
	Position GetTailPosition() const throw()
	{
		return Position(GetCount() - 1);
	}
	Position GetPosition(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Position(index);
	}

	const Iterator ToIterator(const Position& pos) const throw()
	{
		return Iterator(ref_ptr<T>(baseClass::m_first + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(ref_ptr_helper::GetInternalPointer(iter.get_Ref()) - baseClass::m_first);
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
		return reverse_iterator<Iterator>(ToIterator(GetTailPosition()));
	}
	const reverse_iterator<Iterator> GetReverseEnd() const throw()
	{
		return reverse_iterator<Iterator>(Iterator(ref_ptr<T>(baseClass::m_first - 1)));
	}
	const reverse_iterator2<Iterator> GetReverseBegin2() const throw()
	{
		return reverse_iterator2<Iterator>(GetEnd());
	}
	const reverse_iterator2<Iterator> GetReverseEnd2() const throw()
	{
		return reverse_iterator2<Iterator>(GetBegin());
	}

	const T& GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return *(baseClass::m_first + index);
	}

private:
	friend class const_array_helper;
};

#pragma pack(pop)

// macros

// --<header file>--

// define static constant array

#define DECLARE_STATIC_CONST_ARRAY(cls, T)   class cls {  \
public:  \
	typedef T  EType;  \
	static uintptr GetCount() throw()  \
	{ return c_size; }  \
	static const T* GetAddress() throw()  \
	{ return c_array; }  \
public:  \
	static const T        c_array[];  \
	static const uintptr  c_size;  \
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

//array tools
template <typename T, typename... Args>
inline void call_array_constructors(T* p, uintptr size, Args&&... args)
{
	uintptr i;
	try {
		for( i = 0; i < size; i ++ ) {
			call_constructor(*(p + i), rv_forward<Args>(args)...);
		}
	}
	catch(...) {
		while( i > 0 ) {
			i --;
			p[i].~T();
		}
		throw;  //re-throw
	}
}
template <typename T>
inline void call_array_destructors(T* p, uintptr size) throw()
{
	T* pT = p;
	for( uintptr i = size; i > 0; i -- ) {
		pT->~T();
		++ pT;
	}
}
//copy
template <typename T>
inline void copy_array_elements(const T* pSrc, T* pDest, uintptr size)
{
	for( uintptr i = 0; i < size; i ++ ) {
		pDest[i] = pSrc[i];  //may throw
	}
}

#pragma pack(push, 1)

// const array with prefix length
template <typename T>
class const_prefix_array
{
public:
	explicit constexpr const_prefix_array(const T* p = NULL) noexcept : m_data(p)
	{
	}
	uintptr GetCount() const noexcept
	{
		return m_data == NULL ? 0 : *((const uintptr*)(const void*)m_data - 1);
	}
	const T* GetAddress() const noexcept
	{
		return m_data;
	}
	const_array<T> To_ConstArray() const noexcept
	{
		return const_array<T>(GetAddress(), GetCount());
	}

protected:
	const T* m_data;
};

#pragma pack(pop)

#pragma pack(push, 1)

// fixed_def_array<T, t_size>
/*! \brief A template structure for fixed array.

A template structure for fixed array.
\tparam T The element type.
\tparam t_size The size of array.
*/
template <typename T, uintptr t_size>
struct fixed_def_array
{
	uintptr uSize;
	T data[t_size];  //!< The fixed array.
};

// fixed_array<T, t_size>
/*! \brief A template class for fixed array.

A template class for fixed array.
\tparam T The element type.
\tparam t_size The size of array.
*/
template <typename T, uintptr t_size>
class fixed_array
{
public:
	static constexpr const uintptr c_size = t_size;  //!< The number of elements.

	typedef T EType;  //element type
	//position
	typedef array_position  Position;
	//iterator
	typedef array_iterator<T>  Iterator;

private:
	typedef fixed_array<T, t_size>  thisClass;

public:
	/*! \brief Constructor.

	Constructor.
	\note It may throw exceptions.
	*/
	fixed_array() //may throw
	{
		assert( t_size > 0 );
	}
	/*! \brief Copy constructor.

	Copy constructor.
	\note It may throw exceptions.
	*/
	fixed_array(const thisClass& src)  //may throw
	{
		assert( t_size > 0 );
		copy_array_elements(src.m_data, m_data, t_size);
	}
	~fixed_array() throw()
	{
	}

	//operators
	/*! \brief Assignment operator.

	Assignment operator.
	\note It may throw exceptions.
	*/
	thisClass& operator=(const thisClass& src)  //may throw
	{
		if( this != &src ) {
			copy_array_elements(src.m_data, m_data, t_size);
		}
		return *this;
	}

	/*! \brief Index operator.

	Index operator.
	*/
	const T& operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}
	T& operator[](uintptr index) throw()
	{
		return GetAt(index);
	}

	//position
	Position GetStartPosition() const throw()
	{
		return Position(0);
	}
	Position GetTailPosition() const throw()
	{
		return Position(c_size - 1);
	}
	Position GetPosition(uintptr index) const throw()
	{
		assert( index < t_size );
		return Position(index);
	}

	const Iterator ToIterator(const Position& pos) const throw()
	{
		return Iterator(ref_ptr<T>(m_data + pos.GetIndex()));
	}
	Iterator ToIterator(const Position& pos) throw()
	{
		return Iterator(ref_ptr<T>(m_data + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(ref_ptr_helper::GetInternalPointer(iter.get_Ref()) - m_data);
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(ref_ptr<T>(m_data));
	}
	Iterator GetBegin() throw()
	{
		return Iterator(ref_ptr<T>(m_data));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(ref_ptr<T>(m_data + t_size));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(ref_ptr<T>(m_data + t_size));
	}
	const reverse_iterator<Iterator> GetReverseBegin() const throw()
	{
		return reverse_iterator<Iterator>(ToIterator(GetTailPosition()));
	}
	reverse_iterator<Iterator> GetReverseBegin() throw()
	{
		return reverse_iterator<Iterator>(ToIterator(GetTailPosition()));
	}
	const reverse_iterator<Iterator> GetReverseEnd() const throw()
	{
		return reverse_iterator<Iterator>(Iterator(ref_ptr<T>(m_data - 1)));
	}
	reverse_iterator<Iterator> GetReverseEnd() throw()
	{
		return reverse_iterator<Iterator>(Iterator(ref_ptr<T>(m_data - 1)));
	}
	const reverse_iterator2<Iterator> GetReverseBegin2() const throw()
	{
		return reverse_iterator2<Iterator>(GetEnd());
	}
	reverse_iterator2<Iterator> GetReverseBegin2() throw()
	{
		return reverse_iterator2<Iterator>(GetEnd());
	}
	const reverse_iterator2<Iterator> GetReverseEnd2() const throw()
	{
		return reverse_iterator2<Iterator>(GetBegin());
	}
	reverse_iterator2<Iterator> GetReverseEnd2() throw()
	{
		return reverse_iterator2<Iterator>(GetBegin());
	}

	/*! \brief Get the element.

	Get the element.
	\param index [in] Specify the index.
	\return The reference to element.
	*/
	const T& GetAt(uintptr index) const throw()
	{
		assert( index < t_size );
		return *(m_data + index);
	}
	T& GetAt(uintptr index) throw()
	{
		assert( index < t_size );
		return *(m_data + index);
	}
	/*! \brief Set the element.

	Set the element.
	\param index [in] Specify the index.
	\param t [in] Specify the element value.
	\note It may throw exceptions.
	*/
	void SetAt(uintptr index, const T& t)  //may throw
	{
		assert( index < t_size );
		m_data[index] = t;
	}
	void SetAt(uintptr index, T&& t)  //may throw
	{
		assert( index < t_size );
		m_data[index] = rv_forward(t);
	}

protected:
	T m_data[t_size];  //!< The fixed array.

private:
	friend class fixed_array_helper;
};

#pragma pack(pop)

// fixed_array_helper

class fixed_array_helper
{
public:
	//internal pointer
	/*! \brief Get the data pointer.

	Get the data pointer.
	\tparam T Fixed array type, i.e., fixed_array<...>.
	\param arr [in] Specify the fixed array object.
	\return The data pointer.
	*/
	template <class T>
	static typename T::EType* GetInternalPointer(const T& arr) throw()
	{
		return const_cast<typename T::EType*>(arr.m_data);
	}

	//to const array
	template <typename T, uintptr t_size>
	static const_array<T> To_ConstArray(const fixed_array<T, t_size>& arr) noexcept
	{
		return const_array<T>(GetInternalPointer(arr), t_size);
	}
};

// delegate

template <typename T>
class delegate_base;

template <typename R, typename... Args>
class delegate_base<R(Args...)>
{
private:
	typedef delegate_base<R(Args...)>  thisClass;
	using func_type = R(*)(thisClass*, Args&&...);

public:
	explicit delegate_base(func_type fn = nullptr) noexcept : m_func(fn)
	{
	}
	delegate_base(const delegate_base& src) = delete;

	void Release() noexcept
	{
		m_func = nullptr;
	}
	bool IsNull() const noexcept
	{
		return m_func == nullptr;
	}
	bool IsNullObject() const noexcept
	{
		return IsNull();
	}

	R operator()(Args... args)
	{
		return m_func(this, rv_forward<Args>(args)...);
	}

private:
	func_type m_func;  //function pointer
};

template <typename... Args>
class delegate_base<void(Args...)>
{
private:
	typedef delegate_base<void(Args...)>  thisClass;
	using func_type = void(*)(thisClass*, Args&&...);

public:
	explicit delegate_base(func_type fn = nullptr) noexcept : m_func(fn)
	{
	}
	delegate_base(const delegate_base& src) = delete;

	void Release() noexcept
	{
		m_func = nullptr;
	}
	bool IsNull() const noexcept
	{
		return m_func == nullptr;
	}
	bool IsNullObject() const noexcept
	{
		return IsNull();
	}

	void operator()(Args... args)
	{
		m_func(this, rv_forward<Args>(args)...);
	}

private:
	func_type m_func;  //function pointer
};

// --<header file>--

#define DECLARE_DELEGATE_CLASS(T, method)  \
template <class T, typename R, typename... Args> class delegate_##method : public delegate_base<R, Args...>  \
{ public: typedef delegate_base<R, Args...> baseClass;  \
typedef delegate_##method <T, R, Args...> thisClass;  \
delegate_##method () noexcept : baseClass(&_f) {}  \
static R _f(baseClass* pThis, Args&&... args) {  \
T* pT = static_cast<T*>(static_cast<thisClass*>(pThis));  \
return pT-> method (rv_forward<Args>(args)...); } };  \
\
template <class T, typename... Args> class delegate_##method : public delegate_base<Args...>  \
{ public: typedef delegate_base<Args...> baseClass;  \
typedef delegate_##method <T, Args...> thisClass;  \
delegate_##method () noexcept : baseClass(&_f) {}  \
static void _f(baseClass* pThis, Args&&... args) {  \
T* pT = static_cast<T*>(static_cast<thisClass*>(pThis));  \
pT-> method (rv_forward<Args>(args)...); } };

#define USE_DELEGATE_CLASS_NAME(T, method, R, ...)  \
delegate##method <T, R(__VA_ARGS__)>

#define GET_DELEGATE_BASE(pThis, T, method, R, ...)  \
ref_ptr<delegate_base<R(__VA_ARGS__)>>(static_cast<delegate_base<R(__VA_ARGS__)>*>(static_cast<delegate##method <T, R(__VA_ARGS__)>*>(pThis)))

// --<.h end>--

////////////////////////////////////////////////////////////////////////////////
