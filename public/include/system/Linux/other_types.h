﻿/*
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

//Linux

#include <uuid/uuid.h>

//------------------------------------------------------------------------------
//guid
typedef uuid_t  guid;

inline void guid_create(guid& id) throw()
{
	::uuid_generate(id);
}

inline bool guid_equal(guid& id1, guid& id2) throw()
{
	return ::uuid_compare(id1, id2) == 0;
}

//------------------------------------------------------------------------------
//atomic

inline int atomic_increment(int& v) throw()
{
	return __sync_add_and_fetch(&v, 1);
}

inline int atomic_decrement(int& v) throw()
{
	return __sync_sub_and_fetch(&v, 1);
}

inline int atomic_compare_exchange(int& v, int oldval, int newval) throw()
{
	return __sync_val_compare_and_swap(&v, oldval, newval);
}

// errno
#define CR_FROM_ERROR(err)  ((int)(0x80000000 | (err)))
#define CR_FROM_ERRORNO()   CR_FROM_ERROR(errno)

//------------------------------------------------------------------------------
//call_result

class call_result
{
public:
	call_result() throw() : m_result(0)
	{
	}
	explicit call_result(int res) throw() : m_result(res)
	{
	}
	call_result(const call_result& src) throw() : m_result(src.m_result)
	{
	}
	~call_result() throw()
	{
	}

	//operators
	call_result& operator=(const call_result& src) throw()
	{
		if( this != &src ) {
			m_result = src.m_result;
		}
		return *this;
	}

	//methods
	int GetResult() const throw()
	{
		return m_result;
	}
	void SetResult(int res) throw()
	{
		m_result = res;
	}

	//check
	bool IsSucceeded() const throw()
	{
		return m_result >= 0;
	}
	bool IsFailed() const throw()
	{
		return m_result < 0;
	}
	bool IsOK() const throw()
	{
		return m_result == 0;
	}

private:
	int m_result;
};

//------------------------------------------------------------------------------
// call_result constants

#define CR_OK                0
#define CR_OUTOFMEMORY       CR_FROM_ERROR(ENOMEM)
#define CR_OVERFLOW          CR_FROM_ERROR(EOVERFLOW)

//------------------------------------------------------------------------------
// Synchronization

// inp_mutex

class inp_mutex
{
public:
	inp_mutex() throw() : m_bInitialized(false)
	{
	}
	~inp_mutex() throw()
	{
		Term();
	}

	void Lock() throw()
	{
		assert( m_bInitialized );
		::pthread_mutex_lock(&m_mtx);
	}
	void Unlock() throw()
	{
		::pthread_mutex_unlock(&m_mtx);
	}

	//methods
	call_result Init() throw()
	{
		assert( !m_bInitialized );

		int res = ::pthread_mutex_init(&m_mtx, NULL);
		if( res > 0 ) {
			res = CR_FROM_ERROR(res); 
		}
		else if( res == 0 ) {
			m_bInitialized = true;
		}

		return call_result(res);
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			::pthread_mutex_destroy(&m_mtx);
			m_bInitialized = false;
		}
	}

private:
	pthread_mutex_t m_mtx;
	bool m_bInitialized;

private:
	//noncopyable
	inp_mutex(const inp_mutex&) throw();
	inp_mutex& operator=(const inp_mutex&) throw();
};

////////////////////////////////////////////////////////////////////////////////
