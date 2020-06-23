﻿/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
//internal header
////////////////////////////////////////////////////////////////////////////////

// _auto_mem

class _auto_mem
{
public:
	_auto_mem() throw() : m_p(NULL)
	{
	}
	_auto_mem(_auto_mem&& src) throw() : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	~_auto_mem() throw()
	{
		Free();
	}

	//operators
	_auto_mem& operator=(_auto_mem&& src) throw()
	{
		if( this != &src ) {
			if( m_p != src.m_p ) {
				Free();
				m_p = src.m_p;
				src.m_p = NULL;
			}
			else {
				assert( m_p == NULL );  //unique
			}
		}
		return *this;
	}

	//methods
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	void* GetAddress() const throw()
	{
		return m_p;
	}

	//alloc
	void Allocate(uintptr uSize)  //may throw
	{
		assert( m_p == NULL );
		m_p = crt_alloc(uSize);
		if( m_p == NULL )
			throw outofmemory_exception();
	}
	//free
	void Free() throw()
	{
		if( m_p != NULL ) {
			crt_free(m_p);
			m_p = NULL;
		}
	}

private:
	void* m_p;

private:
	//noncopyable
	_auto_mem(const _auto_mem& src) throw();
	_auto_mem& operator=(const _auto_mem& src) throw();
};

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/_sys_base.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/_sys_base.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
