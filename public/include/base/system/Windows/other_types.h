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

//Windows

//------------------------------------------------------------------------------
//guid
typedef GUID  guid;

inline void guid_create(guid& id) throw()
{
	HRESULT hr = ::CoCreateGuid(&id);
	assert( SUCCEEDED(hr) );
}

inline bool guid_equal(const guid& id1, const guid& id2) throw()
{
	return ::IsEqualGUID(id1, id2) ? true : false;
}

//constant

// in header file
#define DECLARE_GUID(name)  \
	extern "C" const GUID FAR name;

// in cpp file
#define IMPLEMENT_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)  \
	const GUID FAR name = { (l), (w1), (w2), { (b1), (b2), (b3), (b4), (b5), (b6), (b7), (b8) } };

// use
#define USE_GUID(name)  (name)

//------------------------------------------------------------------------------
//character

typedef CHAR           char_a;  //ANSI or UTF8
typedef WCHAR          char_h;  //word or UTF16
typedef unsigned long  char_l;  //long or UTF32

typedef char_h  char_s;  //system type, UTF16
//for const string
#define _OS_WIDEN2(x)  L##x
#define _OS_WIDEN(x)   _OS_WIDEN2(x)

#define _S(x)  _OS_WIDEN(x)

typedef char_h  char_w;  //for wide type, L"..."

//------------------------------------------------------------------------------
//atomic

inline int atomic_increment(int& v) throw()
{
	return (int)::InterlockedIncrement((LONG volatile *)(&v));
}

inline int atomic_decrement(int& v) throw()
{
	return (int)::InterlockedDecrement((LONG volatile *)(&v));
}

inline int atomic_compare_exchange(int& v, int oldval, int newval) throw()
{
	return (int)::InterlockedCompareExchange((LONG volatile *)(&v), newval, oldval);
}

//------------------------------------------------------------------------------
//memory operators

inline void* mem_zero(void* dest, uintptr count) throw()
{
	::ZeroMemory(dest, count);
	return dest;
}
inline void* mem_fill(byte ch, void* dest, uintptr count) throw()
{
	::FillMemory(dest, count, ch);
	return dest;
}
inline void* mem_copy(const void* src, uintptr count, void* dest) throw()
{
	::CopyMemory(dest, src, count);
	return dest;
}
inline void* mem_move(const void* src, uintptr count, void* dest) throw()
{
	::MoveMemory(dest, src, count);
	return dest;
}

//------------------------------------------------------------------------------
//error code
#define CR_FROM_ERROR(err)  ((HRESULT)(((err) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000))

//------------------------------------------------------------------------------
//call result

#pragma pack(push, 1)

class call_result
{
public:
	call_result() throw() : m_result(S_OK)
	{
	}
	explicit call_result(int res) throw() : m_result((HRESULT)res)
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
		return (int)m_result;
	}
	void SetResult(int res) throw()
	{
		m_result = (HRESULT)res;
	}

	//check
	bool IsSucceeded() const throw()
	{
		return SUCCEEDED(m_result);
	}
	bool IsFailed() const throw()
	{
		return FAILED(m_result);
	}
	bool IsOK() const throw()
	{
		return m_result == S_OK;
	}

private:
	HRESULT m_result;
};

#pragma pack(pop)

//------------------------------------------------------------------------------
// call result constants

#define CR_S_EOF             (ERROR_HANDLE_EOF)
#define CR_S_FALSE           (1)
#define CR_OK                (0)
#define CR_FAIL              E_FAIL
#define CR_OUTOFMEMORY       E_OUTOFMEMORY
#define CR_OVERFLOW          CR_FROM_ERROR(ERROR_ARITHMETIC_OVERFLOW)
#define CR_SABAD             CR_FROM_ERROR(ERROR_DLL_INIT_FAILED)
#define CR_INVALID           E_INVALIDARG
#define CR_NOTIMPL           E_NOTIMPL
#define CR_NAMETOOLONG       CO_E_PATHTOOLONG
#define CR_DISKFULL          CR_FROM_ERROR(ERROR_DISK_FULL)

//------------------------------------------------------------------------------
// Service

// log types
#define SERVICE_LOG_SUCCESS           EVENTLOG_SUCCESS
#define SERVICE_LOG_ERROR             EVENTLOG_ERROR_TYPE
#define SERVICE_LOG_INFORMATION       EVENTLOG_INFORMATION_TYPE
#define SERVICE_LOG_WARNING           EVENTLOG_WARNING_TYPE

// report_service_log
//  type: SERVICE_LOG_*
inline void report_service_log(const char_s* szService, uint type, const char_s* szMsg) throw()
{
	HANDLE hEventSource = ::RegisterEventSourceW(NULL, szService);
	if( hEventSource != NULL ) {
		::ReportEvent(hEventSource, type, (WORD)0, 0, NULL, (WORD)1, 0, &szMsg, NULL);  //no check
		::DeregisterEventSource(hEventSource);  //no check
	}
}

//------------------------------------------------------------------------------
// Shared Assembly

// sa_handle

class sa_handle
{
public:
	sa_handle() throw() : m_hd(NULL)
	{
	}
	sa_handle(sa_handle&& src) throw()
	{
		m_hd = src.m_hd;
		src.m_hd = NULL;
	}
	~sa_handle() throw()
	{
		Free();
	}

	sa_handle& operator=(sa_handle&& src) throw()
	{
		if( &src != this ) {
			Free();
			m_hd = src.m_hd;
			src.m_hd = NULL;
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
		if( (uintptr)m_hd != hd )
			Free();
		m_hd = (HMODULE)hd;
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
			BOOL bRet = ::FreeLibrary(m_hd);
			assert( bRet );
			m_hd = NULL;
		}
	}

	bool Load(const char_s* szFile) throw()
	{
		assert( m_hd == NULL );
		m_hd = ::LoadLibraryW(szFile);
		return m_hd != NULL;
	}
	uintptr GetFunctionAddress(const char_a* szFunc) throw()
	{
		assert( m_hd != NULL );
		return (uintptr)::GetProcAddress(m_hd, szFunc);
	}

private:
	HMODULE m_hd;

private:
	//noncopyable
	sa_handle(const sa_handle& src) throw();
	sa_handle& operator=(const sa_handle& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
