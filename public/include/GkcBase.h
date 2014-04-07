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
This file contains basic classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_BASE_H__
#define __GKC_BASE_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcBase.h requires GkcDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//functions

//Swap
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

// classes

// RefPtr<T>

template <class T>
class RefPtr
{
public:
	RefPtr() throw() : m_p(NULL)
	{
	}
	explicit RefPtr(T& t) throw() : m_p(&t)
	{
	}
	RefPtr(const RefPtr<T>& src) throw() : m_p(src.m_p)
	{
	}
	~RefPtr() throw()
	{
	}

	void Release() throw()
	{
		m_p = NULL;
	}

	RefPtr<T>& operator=(const RefPtr<T>& src) throw()
	{
		if( this != &src ) {
			m_p = src.m_p;
		}
		return *this;
	}

	bool operator==(const RefPtr<T>& src) const throw()
	{
		return m_p == src.m_p;
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_p;
	}

private:
	T* m_p;
};

// PtrHelper

class PtrHelper
{
public:
	template <class T>
	static RefPtr<T> ToRefPtr(T& t) throw()
	{
		return RefPtr<T>(t);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_BASE_H__
////////////////////////////////////////////////////////////////////////////////
