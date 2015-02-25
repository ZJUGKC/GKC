/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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

#ifndef __IWIN_BASE_H__
#define __IWIN_BASE_H__

////////////////////////////////////////////////////////////////////////////////

// _auto_local_mem

class _auto_local_mem
{
public:
	_auto_local_mem() throw() : m_h(NULL)
	{
	}
	explicit _auto_local_mem(HLOCAL h) throw() : m_h(h)
	{
	}
	_auto_local_mem(_auto_local_mem&& src) throw()
	{
		m_h = src.m_h;
		src.m_h = NULL;
	}
	~_auto_local_mem() throw()
	{
		Free();
	}

	//operators
	_auto_local_mem& operator=(_auto_local_mem&& src) throw()
	{
		if( this != &src ) {
			assert( m_h != src.m_h );  //unique
			Free();
			m_h = src.m_h;
			src.m_h = NULL;
		}
		return *this;
	}

	//methods
	bool IsNull() const throw()
	{
		return m_h == NULL;
	}

	HLOCAL GetAddress() const throw()
	{
		return m_h;
	}
	void SetAddress(HLOCAL h) throw()
	{
		Free();
		m_h = h;
	}

	//alloc
	void Allocate(uint uFlags, uintptr uSize)  //may throw
	{
		assert( m_h == NULL );
		m_h = ::LocalAlloc(uFlags, uSize);
		if( m_h == NULL )
			throw GKC::OutOfMemoryException();
	}
	//free
	void Free() throw()
	{
		if( m_h != NULL ) {
			::LocalFree(m_h);
			m_h = NULL;
		}
	}

protected:
	HLOCAL m_h;

private:
	//noncopyable
	_auto_local_mem(const _auto_local_mem& src) throw();
	_auto_local_mem& operator=(const _auto_local_mem& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
#endif  //__IWIN_BASE_H__
////////////////////////////////////////////////////////////////////////////////
