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

// internal
#pragma pack(push, 1)
typedef struct _tag_os_guid
{
//native endian
	uint    l;
	ushort  w1;
	ushort  w2;
//big endian
	byte    d[8];
} _os_guid;
#pragma pack(pop)

//constant

// in header file
#define DECLARE_GUID(name)  \
	extern "C" const guid* name;

// in cpp file
#define IMPLEMENT_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)  \
	const _os_guid _os_g_guid_##name = { (l), (w1), (w2), { (b1), (b2), (b3), (b4), (b5), (b6), (b7), (b8) } };  \
	const guid* name = (const guid*)(uintptr)(&_os_g_guid_##name);

// use
#define USE_GUID(name)  (*(name))

//------------------------------------------------------------------------------
//character

typedef char            char_a;  //ANSI or UTF8
typedef unsigned short  char_h;  //word or UTF16
typedef wchar_t         char_l;  //long or UTF32

typedef char_a  char_s;  //system type, UTF8
//for const string
#define _S(x)  x

typedef char_l  char_w;  //for wide type, L"..."

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
#define CR_FROM_ERROR(err)       ((int)(0x80000000 | (err)))

#define _OS_CR_FROM_ERRORNO()    CR_FROM_ERROR(errno)

//------------------------------------------------------------------------------
//call result

#pragma pack(push, 1)

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
		m_result = src.m_result;
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

#pragma pack(pop)

//------------------------------------------------------------------------------
// call result constants

#define CR_S_EOF             (38)
#define CR_S_FALSE           (1)
#define CR_OK                (0)
#define CR_FAIL              CR_FROM_ERROR(1000)
#define CR_UNEXPECTED        CR_FROM_ERROR(1001)
#define CR_OUTOFMEMORY       CR_FROM_ERROR(ENOMEM)
#define CR_BADADDRESS        CR_FROM_ERROR(EFAULT)
#define CR_OVERFLOW          CR_FROM_ERROR(EOVERFLOW)
#define CR_SABAD             CR_FROM_ERROR(ELIBBAD)
#define CR_INVALID           CR_FROM_ERROR(EINVAL)
#define CR_NOTIMPL           CR_FROM_ERROR(ENOSYS)
#define CR_NAMETOOLONG       CR_FROM_ERROR(ENAMETOOLONG)
#define CR_DISKFULL          CR_FROM_ERROR(ENOSPC)
#define CR_FDBAD             CR_FROM_ERROR(EBADF)
#define CR_CORRUPT           CR_FROM_ERROR(EILSEQ)
#define CR_NOACCESS          CR_FROM_ERROR(EACCES)
#define CR_ABORT             CR_FROM_ERROR(EINTR)
#define CR_CANCELED          CR_FROM_ERROR(ECANCELED)

////////////////////////////////////////////////////////////////////////////////
