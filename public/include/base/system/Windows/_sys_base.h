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

//------------------------------------------------------------------------------
// Thread

// Message Queue for thread
inline void _os_create_thread_message_queue() throw()
{
	MSG msg;
	::PeekMessageW(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);  //no check
}

//------------------------------------------------------------------------------
// Synchronization

// _os_critical_section

class _os_critical_section
{
public:
	_os_critical_section() throw() : m_bInitialized(false)
	{
		mem_zero(&m_sect, sizeof(CRITICAL_SECTION));
	}
	explicit _os_critical_section(uint uSpinCount) : m_bInitialized(false)
	{
		mem_zero(&m_sect, sizeof(CRITICAL_SECTION));
		call_result cr = Init(uSpinCount);
		if( !m_bInitialized )
			throw exception_base(cr);
	}
	~_os_critical_section() throw()
	{
		Term();
	}

	void Lock() throw()
	{
		assert( m_bInitialized );
		::EnterCriticalSection(&m_sect);
	}
	void Unlock() throw()
	{
		assert( m_bInitialized );
		::LeaveCriticalSection(&m_sect);
	}
	bool TryLock() throw()
	{
		assert( m_bInitialized );
		return ::TryEnterCriticalSection(&m_sect) != FALSE;
	}

	//methods
	call_result Init(uint uSpinCount = 0) throw()
	{
		assert( !m_bInitialized );

		HRESULT hRes = S_OK;
		if( !::InitializeCriticalSectionEx(&m_sect, uSpinCount, 0) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		}

		if( SUCCEEDED(hRes) )
			m_bInitialized = true;

		return call_result((int)hRes);
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			::DeleteCriticalSection(&m_sect);
			m_bInitialized = false;
		}
	}

	// Set the spin count for the critical section
	uint SetSpinCount(uint uSpinCount) throw()
	{
		assert( m_bInitialized );
		return ::SetCriticalSectionSpinCount(&m_sect, uSpinCount);
	}

	//internal structure
	CRITICAL_SECTION& GetCS() throw()
	{
		return m_sect;
	}

private:
	CRITICAL_SECTION m_sect;
	bool m_bInitialized;

private:
	//noncopyable
	_os_critical_section(const _os_critical_section&) throw();
	_os_critical_section& operator=(const _os_critical_section&) throw();
};

// _os_event

class _os_event
{
public:
	explicit _os_event(HANDLE h = NULL) throw() : m_h(h)
	{
	}
	~_os_event() throw()
	{
		Close();
	}

	void Close() throw()
	{
		if( m_h != NULL ) {
			BOOL bRet = ::CloseHandle(m_h);
			assert( bRet );
			m_h = NULL;
		}
	}

	void Attach(HANDLE h) throw()
	{
		if( m_h != h )
			Close();
		m_h = h;
	}
	HANDLE Detach() throw()
	{
		HANDLE h = m_h;
		m_h = NULL;
		return h;
	}

	HANDLE GetHandle() const throw()
	{
		return m_h;
	}
	bool IsNull() const throw()
	{
		return m_h == NULL;
	}

	// Create a new event
	call_result Create(LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCWSTR pszName) throw()
	{
		assert( IsNull() );
		HRESULT hRes = S_OK;
		m_h = ::CreateEventW(pSecurity, bManualReset, bInitialState, pszName);  //::GetLastError() may return ERROR_ALREADY_EXISTS
		if( m_h == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hRes);
	}
	// Open an existing named event
	call_result Open(DWORD dwAccess, BOOL bInheritHandle, LPCWSTR pszName) throw()
	{
		assert( IsNull() );
		HRESULT hRes = S_OK;
		m_h = ::OpenEventW(dwAccess, bInheritHandle, pszName);
		if( m_h == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hRes);
	}

	// Pulse the event (signals waiting objects, then resets)
	BOOL Pulse() throw()
	{
		assert( !IsNull() );
		return ::PulseEvent(m_h);
	}
	// Set the event to the non-signaled state
	BOOL Reset() throw()
	{
		assert( !IsNull() );
		return ::ResetEvent(m_h);
	}
	// Set the event to the signaled state
	BOOL Set() throw()
	{
		assert( !IsNull() );
		return ::SetEvent(m_h);
	}

	//wait
	//  return : WAIT_OBJECT_0, WAIT_TIMEOUT, WAIT_FAILED
	DWORD Wait(DWORD dwMilliseconds = INFINITE) throw()
	{
		assert( !IsNull() );
		return ::WaitForSingleObject(m_h, dwMilliseconds);
	}

private:
	HANDLE m_h;  //event

private:
	//noncopyable
	_os_event(const _os_event&) throw();
	_os_event& operator=(const _os_event&) throw();
};

//------------------------------------------------------------------------------
// Time

// _os_cvt_system_time

inline void _os_cvt_system_time(SYSTEMTIME* pST, system_time& tm) throw()
{
	tm.uYear         = pST->wYear;
	tm.uMonth        = pST->wMonth;
	tm.uDayOfWeek    = pST->wDayOfWeek;
	tm.uDay          = pST->wDay;
	tm.uHour         = pST->wHour;
	tm.uMinute       = pST->wMinute;
	tm.uSecond       = pST->wSecond;
	tm.uMilliseconds = pST->wMilliseconds;
}

// _os_filetime_to_ns_value
//   unit: 100ns
inline void _os_filetime_to_ns_value(FILETIME* pFT, uint64& v) throw()
{
	ULARGE_INTEGER uli;
	uli.LowPart  = pFT->dwLowDateTime;
	uli.HighPart = pFT->dwHighDateTime;
	v = uli.QuadPart;
}
// _os_ns_value_to_filetime
inline void _os_ns_value_to_filetime(uint64 v, FILETIME& ft) throw()
{
	ULARGE_INTEGER uli;
	uli.QuadPart = v;
	ft.dwLowDateTime  = uli.LowPart;
	ft.dwHighDateTime = uli.HighPart;
}

//------------------------------------------------------------------------------
// Memory

// _os_auto_local_mem

class _os_auto_local_mem
{
public:
	_os_auto_local_mem() throw() : m_h(NULL)
	{
	}
	explicit _os_auto_local_mem(HLOCAL h) throw() : m_h(h)
	{
	}
	_os_auto_local_mem(_os_auto_local_mem&& src) throw()
	{
		m_h = src.m_h;
		src.m_h = NULL;
	}
	~_os_auto_local_mem() throw()
	{
		Free();
	}

	//operators
	_os_auto_local_mem& operator=(_os_auto_local_mem&& src) throw()
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
			throw outofmemory_exception();
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
	_os_auto_local_mem(const _os_auto_local_mem& src) throw();
	_os_auto_local_mem& operator=(const _os_auto_local_mem& src) throw();
};

//------------------------------------------------------------------------------
// for COM programming

// Author: Lijuan Mei

//assign

inline IUnknown* _os_com_ptr_assign(INOUT IUnknown** pp, IN IUnknown* lp) throw()
{
	if( pp == NULL )
		return NULL;
	if( lp != NULL )
		lp->AddRef();
	if( *pp )
		(*pp)->Release();
	*pp = lp;
	return lp;
}

inline IUnknown* _os_com_qi_ptr_assign(INOUT IUnknown** pp, IN IUnknown* lp, IN REFIID riid) throw()
{
	if( pp == NULL )
		return NULL;
	IUnknown* pTemp = *pp;
	*pp = NULL;
	if( lp != NULL )
		lp->QueryInterface(riid, (void**)pp);
	if( pTemp )
		pTemp->Release();
	return *pp;
}

// _os_com_ptr<T>

template <class T>
class _os_no_addref_release_on_com_ptr : public T
{
private:
	STDMETHOD_(ULONG, AddRef)() = 0;
	STDMETHOD_(ULONG, Release)() = 0;
};

template <class T>
class _os_com_ptr
{
public:
	_os_com_ptr() throw() : m_p(NULL)
	{
	}
	explicit _os_com_ptr(INOUT T* lp) throw() : m_p(lp)
	{
		if( m_p != NULL )
			m_p->AddRef();
	}
	~_os_com_ptr() throw()
	{
		if( m_p != NULL )
			m_p->Release();
	}

	_os_com_ptr(INOUT const _os_com_ptr<T>& sp) throw() : m_p(sp.m_p)
	{
		if( m_p != NULL )
			m_p->AddRef();
	}
	_os_com_ptr(INOUT _os_com_ptr<T>&& lp) throw() : m_p(lp.p)
	{
		lp.p = NULL;
	}

	T* operator=(INOUT T* lp) throw()
	{
		if( *this != lp ) {
			return static_cast<T*>(_os_com_ptr_assign((IUnknown**)&m_p, lp));
		}
		return *this;
	}
	template <typename Q>
	T* operator=(INOUT const _os_com_ptr<Q>& sp) throw()
	{
		if( !IsEqualObject(sp) ) {
			return static_cast<T*>(_os_com_qi_ptr_assign((IUnknown**)&m_p, sp, __uuidof(T)));
		}
		return *this;
	}
	T* operator=(INOUT const _os_com_ptr<T>& sp) throw()
	{
		if( *this != sp ) {
			return static_cast<T*>(_os_com_ptr_assign((IUnknown**)&m_p, sp));
		}
		return *this;
	}
	T* operator=(INOUT _os_com_ptr<T>&& lp) throw()
	{
		if( *this != lp ) {
			if( m_p != NULL )
				m_p->Release();
			m_p = lp.p;
			lp.p = NULL;
		}
		return *this;
	}

	//operators
	operator T*() const throw()
	{
		return m_p;
	}
	T& operator*() const throw()
	{
		assert( m_p != NULL );
		return *m_p;
	}
	T** operator&() throw()
	{
		assert( m_p == NULL );
		return &m_p;
	}
	_os_no_addref_release_on_com_ptr<T>* operator->() const throw()
	{
		assert( m_p != NULL );
		return (_os_no_addref_release_on_com_ptr<T>*)m_p;
	}
	bool operator!() const throw()
	{	
		return (m_p == NULL);
	}
	bool operator<(IN T* pT) const throw()
	{
		return m_p < pT;
	}
	bool operator!=(IN T* pT) const throw()
	{
		return !operator==(pT);
	}
	bool operator==(IN T* pT) const throw()
	{
		return m_p == pT;
	}

	//methods
	bool IsNull() const throw()
	{
		return operator!();
	}
	T* Get() const throw()
	{
		return m_p;
	}
	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = m_p;
		if( pTemp != NULL ) {
			m_p = NULL;
			pTemp->Release();
		}
	}
	// Compare two objects
	bool IsEqualObject(IN IUnknown* pOther) const throw()
	{
		if( m_p == NULL && pOther == NULL )
			return true;   // They are both NULL objects
		if( m_p == NULL || pOther == NULL )
			return false;  // One is NULL the other is not
		_os_com_ptr<IUnknown> punk1;
		_os_com_ptr<IUnknown> punk2;
		m_p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(IN T* p2) throw()
	{
		if( m_p != NULL && m_p != p2 )
			m_p->Release();
		m_p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = m_p;
		m_p = NULL;
		return pt;
	}
	HRESULT CopyTo(OUT T** ppT) throw()
	{
		assert( ppT != NULL );
		if( ppT == NULL )
			return E_POINTER;
		*ppT = m_p;
		if( m_p != NULL )
			m_p->AddRef();
		return S_OK;
	}
	HRESULT CoCreateInstance(IN REFCLSID rclsid, INOUT LPUNKNOWN pUnkOuter = NULL, IN DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		assert( m_p == NULL );
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_p);
	}
	HRESULT CoCreateInstance(IN LPCOLESTR szProgID, INOUT LPUNKNOWN pUnkOuter = NULL, IN DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid;
		HRESULT hr = ::CLSIDFromProgID(szProgID, &clsid);
		assert( m_p == NULL );
		if( SUCCEEDED(hr) )
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(OUT Q** pp) const throw()
	{
		assert( pp != NULL );
		assert( m_p != NULL );
		return m_p->QueryInterface(__uuidof(Q), (void**)pp);
	}

private:
	T* m_p;
};

//------------------------------------------------------------------------------
// Module of Assembly
/*
Author: Lijuan Mei
*/

// _os_module

class _os_module
{
public:	
	_os_module() throw();
	~_os_module() throw()
	{
	}

	HINSTANCE GetModuleInstance() const throw()
	{
		return m_hInst;
	}
	HINSTANCE GetResourceInstance() const throw()
	{
		return m_hInstResource;
	}
	//set instance
	HINSTANCE SetResourceInstance(IN HINSTANCE hInst) throw()
	{
		return static_cast< HINSTANCE >(::InterlockedExchangePointer((void**)&m_hInstResource, hInst));
	}

private:
	HINSTANCE m_hInst;
	HINSTANCE m_hInstResource;

private:
	_os_module(const _os_module&) throw();
	_os_module& operator=(const _os_module&) throw();
};

//global variable
extern _os_module _os_g_module;

////////////////////////////////////////////////////////////////////////////////
