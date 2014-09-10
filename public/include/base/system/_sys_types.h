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
//internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//classes

// _auto_mem

class _auto_mem
{
public:
	_auto_mem() throw() : m_p(NULL)
	{
	}
	explicit _auto_mem(void* p) throw() : m_p(p)
	{
	}
	_auto_mem(_auto_mem&& src) throw()
	{
		m_p = src.m_p;
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
			assert( m_p != src.m_p );  //unique
			Free();
			m_p = src.m_p;
			src.m_p = NULL;
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
		m_p = (void*)crt_alloc(uSize);
		if( m_p == NULL )
			throw GKC::OutOfMemoryException();
	}
	//free
	void Free() throw()
	{
		if( m_p != NULL ) {
			crt_free((uintptr)m_p);
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

////////////////////////////////////////////////////////////////////////////////
