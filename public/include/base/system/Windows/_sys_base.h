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
/*! \brief Create a message queue for a thread.

Create a message queue for a thread.
*/
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

	bool IsValid() const throw()
	{
		return m_bInitialized;
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
	call_result Init(uint uSpinCount = 4000) throw()
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
		//unique when m_h==h
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

// helper

class _sync_helper
{
public:
	static char_h* gen_global_name(const char_h* pSrc) throw()
	{
		//global
		DECLARE_LOCAL_CONST_STRING(char_h, c_szGlobal, c_uGlobalLength, L"Global\\")
		//generate
		uintptr uCount = calc_string_length(pSrc) + 1;
		uintptr uNewCount;
		call_result cr = safe_operators::Add(uCount, c_uGlobalLength, uNewCount);
		if( cr.IsFailed() )
			return NULL;
		char_h* pNew = (char_h*)crt_alloc(uNewCount * sizeof(char_h));
		if( pNew == NULL )
			return NULL;
		//copy
		mem_copy(c_szGlobal, c_uGlobalLength * sizeof(char_h), pNew);
		mem_copy(pSrc, uCount * sizeof(char_h), pNew + c_uGlobalLength);  //including '\0'
		return pNew;
	}
	static void free_global_name(char_h* p) throw()
	{
		crt_free((void*)p);
	}
	//tools
	static char_h* gen_sync_name(const char_h* pSrc, bool bGlobal) throw()
	{
		char_h* psz = (char_h*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(char_h* p, bool bGlobal) throw()
	{
		if( bGlobal )
			free_global_name(p);
	}
};

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
	_os_auto_local_mem(_os_auto_local_mem&& src) throw() : m_h(src.m_h)
	{
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
			if( m_h != src.m_h ) {
				Free();
				m_h = src.m_h;
				src.m_h = NULL;
			}
			else {
				assert( m_h == NULL );  //unique
			}
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

//------------------------------------------------------------------------------
// File System

// _os_file_finder

class _os_file_finder
{
public:
	_os_file_finder() throw() : m_hFind(INVALID_HANDLE_VALUE)
	{
	}
	~_os_file_finder() throw()
	{
		Close();
	}

	void Close() throw()
	{
		if( IsValid() ) {
			BOOL bRet = ::FindClose(m_hFind);
			assert( bRet );  //::GetLastError()
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}

	bool IsValid() const throw()
	{
		return m_hFind != INVALID_HANDLE_VALUE;
	}

// Operations

	//szName: use platform path prefix
	//        It can include wildcard characters, for example, an asterisk (*) or a question mark (?).
	//        A common wildcard format is [...\...\[xxx...]]*[.[xxx...]*[xxx...]].
	bool Find(const char_s* szName = NULL) throw()
	{
		assert( !IsValid() );
		if( szName == NULL )
			szName = _S("*");
		m_hFind = ::FindFirstFileW(szName, &m_fd);
		return IsValid();  //::GetLastError()
	}
	bool FindNext() throw()
	{
		assert( IsValid() );
		return ::FindNextFileW(m_hFind, &m_fd) != FALSE;  //::GetLastError()
	}

// Attributes

	int64 GetFileSize() const throw()
	{
		assert( IsValid() );
		//size
		assert( m_fd.nFileSizeHigh <= (DWORD)(limits_base<int>::Max) );
		LARGE_INTEGER li;
		li.LowPart = m_fd.nFileSizeLow;
		li.HighPart = m_fd.nFileSizeHigh;
		return li.QuadPart;
	}
	const char_s* GetFileName() const throw()
	{
		assert( IsValid() );
		return m_fd.cFileName;
	}
	const FILETIME& GetCreationTime() const throw()
	{
		assert( IsValid() );
		return m_fd.ftCreationTime;
	}
	const FILETIME& GetLastAccessTime() const throw()
	{
		assert( IsValid() );
		return m_fd.ftLastAccessTime;
	}
	const FILETIME& GetLastWriteTime() const throw()
	{
		assert( IsValid() );
		return m_fd.ftLastWriteTime;
	}
	// Return true if the file name is "." or "..".
	// It must be called after checking whether the file is a directory.
	bool IsDots() const throw()
	{
		assert( IsValid() );
		return m_fd.cFileName[0] == _S('.') && (m_fd.cFileName[1] == _S('\0') || (m_fd.cFileName[1] == _S('.') && m_fd.cFileName[2] == _S('\0')));
	}
	bool IsReadOnly() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_READONLY);
	}
	bool IsDirectory() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_DIRECTORY);
	}
	bool IsCompressed() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_COMPRESSED);
	}
	bool IsSystem() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_SYSTEM);
	}
	bool IsHidden() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_HIDDEN);
	}
	bool IsTemporary() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_TEMPORARY);
	}
	bool IsNormal() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_NORMAL);
	}
	bool IsArchived() const throw()
	{
		return matches_mask(FILE_ATTRIBUTE_ARCHIVE);
	}

private:
	bool matches_mask(DWORD dwMask) const throw()
	{
		assert( IsValid() );
		return (m_fd.dwFileAttributes & dwMask) != 0;
	}

private:
	WIN32_FIND_DATAW m_fd;
	HANDLE m_hFind;

private:
	//noncopyable
	_os_file_finder(const _os_file_finder&) throw();
	_os_file_finder& operator=(const _os_file_finder&) throw();
};

// _os_file_mapping

class _os_file_mapping
{
public:
	_os_file_mapping() throw() : m_pData(NULL), m_hMapping(NULL)
	{
	}
	~_os_file_mapping() throw()
	{
		Unmap();
	}

	//nOffset : must be a multiple of the allocation granularity obtained by ::GetSystemInfo.
	//dwMappingProtection : PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE, PAGE_EXECUTE_WRITECOPY,
	//                      PAGE_READONLY, PAGE_READWRITE, PAGE_WRITECOPY, ...
	//dwViewDesiredAccess (or operator) : FILE_MAP_ALL_ACCESS, FILE_MAP_READ, FILE_MAP_WRITE, FILE_MAP_COPY, FILE_MAP_EXECUTE, ...
	HRESULT MapFile(
		_In_ HANDLE hFile,
		_In_ SIZE_T nMappingSize = 0,
		_In_ ULONGLONG nOffset = 0,
		_In_ DWORD dwMappingProtection = PAGE_READONLY,
		_In_ DWORD dwViewDesiredAccess = FILE_MAP_READ) throw()
	{
		assert( m_pData == NULL );
		assert( m_hMapping == NULL );
		assert( hFile != INVALID_HANDLE_VALUE && hFile != NULL );

		ULARGE_INTEGER liFileSize;
		liFileSize.LowPart = ::GetFileSize(hFile, &liFileSize.HighPart);
		if( liFileSize.QuadPart < nMappingSize )
			liFileSize.QuadPart = nMappingSize;

		m_hMapping = ::CreateFileMappingW(hFile, NULL, dwMappingProtection, liFileSize.HighPart, liFileSize.LowPart, 0);
		if( m_hMapping == NULL )
			return HRESULT_FROM_WIN32(::GetLastError());

		if( nMappingSize == 0 )
			m_nMappingSize = (SIZE_T)(liFileSize.QuadPart - nOffset);
		else
			m_nMappingSize = nMappingSize;

		m_dwViewDesiredAccess = dwViewDesiredAccess;
		m_nOffset.QuadPart = nOffset;

		m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
		if( m_pData == NULL ) {
			HRESULT hr;
			hr = HRESULT_FROM_WIN32(::GetLastError());
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}
	HRESULT MapSharedMem(
		_In_ SIZE_T nMappingSize,
		_In_z_ LPCWSTR szName,
		_Out_opt_ BOOL* pbAlreadyExisted = NULL,
		_In_opt_ LPSECURITY_ATTRIBUTES lpsa = NULL,
		_In_ DWORD dwMappingProtection = PAGE_READWRITE,
		_In_ DWORD dwViewDesiredAccess = FILE_MAP_ALL_ACCESS) throw()
	{
		assert( m_pData == NULL );
		assert( m_hMapping == NULL );
		assert( nMappingSize > 0 );
		assert( szName != NULL ); // if you just want a regular chunk of memory, use a heap allocator

		m_nMappingSize = nMappingSize;

		ULARGE_INTEGER nSize;
		nSize.QuadPart = nMappingSize;
		m_hMapping = ::CreateFileMappingW(INVALID_HANDLE_VALUE, lpsa, dwMappingProtection, nSize.HighPart, nSize.LowPart, szName);
		if( m_hMapping == NULL ) {
			HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
			return hr;
		}

		if( pbAlreadyExisted != NULL )
			*pbAlreadyExisted = (::GetLastError() == ERROR_ALREADY_EXISTS);

		m_dwViewDesiredAccess = dwViewDesiredAccess;
		m_nOffset.QuadPart = 0;

		m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
		if( m_pData == NULL ) {
			HRESULT hr;
			hr = HRESULT_FROM_WIN32(::GetLastError());
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}
	HRESULT OpenMapping(
		_In_z_ LPCWSTR szName,
		_In_ SIZE_T nMappingSize,
		_In_ ULONGLONG nOffset = 0,
		_In_ DWORD dwViewDesiredAccess = FILE_MAP_ALL_ACCESS) throw()
	{
		assert( m_pData == NULL );
		assert( m_hMapping == NULL );
		assert( szName != NULL ); // if you just want a regular chunk of memory, use a heap allocator

		m_nMappingSize = nMappingSize;
		m_dwViewDesiredAccess = dwViewDesiredAccess;

		m_hMapping = ::OpenFileMappingW(m_dwViewDesiredAccess, FALSE, szName);
		if( m_hMapping == NULL )
			return HRESULT_FROM_WIN32(::GetLastError());

		m_nOffset.QuadPart = nOffset;

		m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
		if( m_pData == NULL ) {
			HRESULT hr;
			hr = HRESULT_FROM_WIN32(::GetLastError());
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}

	HRESULT Unmap() throw()
	{
		HRESULT hr = S_OK;

		if( m_pData != NULL ) {
			if( !::UnmapViewOfFile(m_pData) )
				hr = HRESULT_FROM_WIN32(::GetLastError());
			m_pData = NULL;
		}
		if( m_hMapping != NULL ) {
			if( !::CloseHandle(m_hMapping) && SUCCEEDED(hr) )
				hr = HRESULT_FROM_WIN32(::GetLastError());
			m_hMapping = NULL;
		}
		return hr;
	}

	const void* GetData() const throw()
	{
		return m_pData;
	}
	void* GetData() throw()
	{
		return m_pData;
	}

	HANDLE GetHandle() const throw()
	{
		return m_hMapping;
	}
	SIZE_T GetMappingSize() const throw()
	{
		return m_nMappingSize;
	}

	HRESULT CopyFrom(_In_ const _os_file_mapping& orig) throw()
	{
		if( this == &orig )
			return S_OK;
		assert( m_pData == NULL );
		assert( m_hMapping == NULL );
		assert( orig.m_pData != NULL );
		assert( orig.m_hMapping != NULL );

		m_dwViewDesiredAccess = orig.m_dwViewDesiredAccess;
		m_nOffset.QuadPart = orig.m_nOffset.QuadPart;
		m_nMappingSize = orig.m_nMappingSize;

		if( !::DuplicateHandle(::GetCurrentProcess(), orig.m_hMapping, ::GetCurrentProcess(),
				&m_hMapping, NULL, TRUE, DUPLICATE_SAME_ACCESS) )
			return HRESULT_FROM_WIN32(::GetLastError());

		m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
		if( m_pData == NULL ) {
			HRESULT hr;
			hr = HRESULT_FROM_WIN32(::GetLastError());
			::CloseHandle(m_hMapping);
			m_hMapping = NULL;
			return hr;
		}

		return S_OK;
	}

	_os_file_mapping(_In_ const _os_file_mapping& orig)
	{
		m_pData = NULL;
		m_hMapping = NULL;

		HRESULT hr = CopyFrom(orig);
		if( FAILED(hr) )
			throw exception_base(call_result((int)hr));
	}
	_os_file_mapping& operator=(_In_ const _os_file_mapping& orig)
	{
		HRESULT hr = CopyFrom(orig);
		if( FAILED(hr) )
			throw exception_base(call_result((int)hr));

		return *this;
	}

private:
	void* m_pData;
	SIZE_T m_nMappingSize;
	HANDLE m_hMapping;
	ULARGE_INTEGER m_nOffset;
	DWORD m_dwViewDesiredAccess;
};

////////////////////////////////////////////////////////////////////////////////
