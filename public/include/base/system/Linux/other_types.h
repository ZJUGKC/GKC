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

//Linux

#include <uuid/uuid.h>

//------------------------------------------------------------------------------
//guid
typedef uuid_t  guid;

inline void guid_create(guid& id) throw()
{
	::uuid_generate(id);
}

inline bool guid_equal(const guid& id1, const guid& id2) throw()
{
	return ::uuid_compare(id1, id2) == 0;
}

//------------------------------------------------------------------------------
//character

typedef char            CharA;  //ANSI or UTF8
typedef unsigned short  CharH;  //word or UTF16
typedef wchar_t         CharL;  //long or UTF32

typedef CharA  CharS;  //system type, UTF8
//for const string
#define _S(x)  x

typedef CharL  CharW;  //for wide type, L"..."

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

//------------------------------------------------------------------------------
//memory operators

inline void* mem_zero(void* dest, uintptr count) throw()
{
	return ::memset(dest, 0, count);
}
inline void* mem_fill(byte ch, void* dest, uintptr count) throw()
{
	return ::memset(dest, (int)ch, count);
}
inline void* mem_copy(const void* src, uintptr count, void* dest) throw()
{
	return ::memcpy(dest, src, count);
}
inline void* mem_move(const void* src, uintptr count, void* dest) throw()
{
	return ::memmove(dest, src, count);
}

//------------------------------------------------------------------------------
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

	bool operator==(const call_result& right) const throw()
	{
		return m_result == right.m_result;
	}
	bool operator!=(const call_result& right) const throw()
	{
		return !operator==(right);
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

#define CR_S_EOF             38
#define CR_S_FALSE           1
#define CR_OK                0
#define CR_FAIL              CR_FROM_ERROR(EFAULT)
#define CR_OUTOFMEMORY       CR_FROM_ERROR(ENOMEM)
#define CR_OVERFLOW          CR_FROM_ERROR(EOVERFLOW)
#define CR_SABAD             CR_FROM_ERROR(ELIBBAD)
#define CR_INVALID           CR_FROM_ERROR(EINVAL)
#define CR_NOTIMPL           CR_FROM_ERROR(ENOSYS)
#define CR_NAMETOOLONG       CR_FROM_ERROR(ENAMETOOLONG)

//------------------------------------------------------------------------------
// Service

// log types
#define SERVICE_LOG_SUCCESS           LOG_NOTICE
#define SERVICE_LOG_ERROR             LOG_ERR
#define SERVICE_LOG_INFORMATION       LOG_INFO
#define SERVICE_LOG_WARNING           LOG_WARNING

// report_service_log
//  type: SERVICE_LOG_*
inline void report_service_log(const CharS* szService, uint type, const CharS* szMsg) throw()
{
	::openlog(szService, LOG_PID | LOG_NDELAY, LOG_USER);
	::syslog(LOG_USER | type, "%s", szMsg);
	::closelog();
}

////////////////////////////////////////////////////////////////////////////////
