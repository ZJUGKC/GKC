/*
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
// internal header
////////////////////////////////////////////////////////////////////////////////

//Linux

//------------------------------------------------------------------------------
// Shared Assembly

// sa_handle

class sa_handle
{
public:
	sa_handle() throw() : m_hd(NULL)
	{
	}
	sa_handle(sa_handle&& src) throw() : m_hd(src.m_hd)
	{
		src.m_hd = NULL;
	}
	~sa_handle() throw()
	{
		Free();
	}

	sa_handle& operator=(sa_handle&& src) throw()
	{
		if( &src != this ) {
			Attach(src.Detach());
		}
		return *this;
	}

	bool IsNull() const throw()
	{
		return m_hd == NULL;
	}

	uintptr GetHandle() const throw()
	{
		return (uintptr)m_hd;
	}
	void Attach(uintptr hd) throw()
	{
		//shared when m_hd==hd
		Free();
		m_hd = (void*)hd;
	}
	uintptr Detach() throw()
	{
		uintptr hd = (uintptr)m_hd;
		m_hd = NULL;
		return hd;
	}

//methods
	void Free() throw()
	{
		if( m_hd != NULL ) {
			int ret = ::dlclose(m_hd);
			(void)ret;
			assert( ret == 0 );
			m_hd = NULL;
		}
	}

	bool Load(const char_s* szFile) throw()
	{
		assert( szFile != NULL );
		assert( m_hd == NULL );
		m_hd = ::dlopen(szFile, RTLD_LAZY);
		return m_hd != NULL;
	}
	uintptr GetFunctionAddress(const char_a* szFunc) throw()
	{
		assert( m_hd != NULL );
		::dlerror();
		uintptr ret = (uintptr)::dlsym(m_hd, szFunc);
		return (::dlerror() == NULL) ? ret : 0;
	}

private:
	void* m_hd;

private:
	//noncopyable
	sa_handle(const sa_handle& src) throw();
	sa_handle& operator=(const sa_handle& src) throw();
};

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
