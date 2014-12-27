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

//------------------------------------------------------------------------------
//character

typedef CHAR           CharA;  //ANSI or UTF8
typedef WCHAR          CharH;  //word or UTF16
typedef unsigned long  CharL;  //long or UTF32

typedef CharH  CharS;  //system type, UTF16
//for const string
#define _S(x)  L##x

typedef CharH  CharW;  //for wide type, L"..."

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
//call_result

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

//------------------------------------------------------------------------------
// call_result constants

#define CR_OK                0
#define CR_OUTOFMEMORY       E_OUTOFMEMORY
#define CR_OVERFLOW          CR_FROM_ERROR(ERROR_ARITHMETIC_OVERFLOW)
#define CR_SABAD             CR_FROM_ERROR(ERROR_DLL_INIT_FAILED)

//------------------------------------------------------------------------------
// Synchronization

// inp_mutex

class inp_mutex
{
public:
	inp_mutex() throw() : m_bInitialized(false)
	{
		::ZeroMemory(&m_sec, sizeof(CRITICAL_SECTION));
	}
	~inp_mutex() throw()
	{
		Term();
	}

	void Lock() throw()
	{
		assert( m_bInitialized );
		::EnterCriticalSection(&m_sec);
	}
	void Unlock() throw()
	{
		::LeaveCriticalSection(&m_sec);
	}

	//methods
	call_result Init() throw()
	{
		assert( !m_bInitialized );

		HRESULT hRes = S_OK;

		if( !::InitializeCriticalSectionAndSpinCount(&m_sec, 0) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		}

		if( SUCCEEDED(hRes) )
			m_bInitialized = true;

		return call_result((int)hRes);
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			::DeleteCriticalSection(&m_sec);
			m_bInitialized = false;
		}
	}

private:
	CRITICAL_SECTION m_sec;
	bool m_bInitialized;

private:
	//noncopyable
	inp_mutex(const inp_mutex&) throw();
	inp_mutex& operator=(const inp_mutex&) throw();
};

////////////////////////////////////////////////////////////////////////////////
