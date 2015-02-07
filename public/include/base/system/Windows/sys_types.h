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
// Synchronization

// helper

class sync_helper
{
public:
	static CharW* gen_global_name(const CharW* pSrc) throw()
	{
		//global
		static const CharW c_szGlobal[] = L"Global\\";
		static const uintptr c_uGlobalLength = sizeof(c_szGlobal) / sizeof(CharW) - 1;
		uintptr uCount = calc_string_length(pSrc);
		uintptr uNewCount;
		call_result cr = GKC::SafeOperators::Add(uCount, c_uGlobalLength, uNewCount);
		if( cr.IsFailed() )
			return NULL;
		CharW* pNew = (CharW*)crt_alloc((uNewCount + 1) * sizeof(CharW));
		if( pNew == NULL )
			return NULL;
		//copy
		mem_copy(c_szGlobal, c_uGlobalLength * sizeof(CharW), pNew);
		mem_copy(pSrc, uCount * sizeof(CharW), pNew + c_uGlobalLength);
		pNew[uNewCount] = 0;
		return pNew;
	}
	static void free_global_name(const CharW* p) throw()
	{
		crt_free((uintptr)p);
	}
	//tools
	static CharW* gen_sync_name(const CharW* pSrc, bool bGlobal) throw()
	{
		CharW* psz = (CharW*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(const CharW* p, bool bGlobal) throw()
	{
		if( bGlobal )
			free_global_name(p);
	}
};

//Semaphore

// inprocess_semaphore

class inprocess_semaphore
{
public:
	inprocess_semaphore() throw() : m_hSema(NULL)
	{
	}
	~inprocess_semaphore() throw()
	{
		Term();
	}

	void Lock()  //may throw
	{
		assert( m_hSema != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hSema, INFINITE);
		if( dwRet != WAIT_OBJECT_0 )
			throw GKC::Exception(call_result(CR_FAIL));
	}
	void Unlock() throw()
	{
		assert( m_hSema != NULL );
		LONG lCount;
		BOOL bRet = ::ReleaseSemaphore(m_hSema, 1, &lCount);
		assert( bRet );
	}
	bool TryLock() throw()
	{
		assert( m_hSema != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hSema, 0);
		return dwRet == WAIT_OBJECT_0;
	}

	call_result Init(int iCount) throw()
	{
		assert( m_hSema == NULL );
		HRESULT hRes = S_OK;
		m_hSema = ::CreateSemaphoreW(NULL, (LONG)iCount, (LONG)iCount, NULL);
		if( m_hSema == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hRes);
	}
	void Term() throw()
	{
		if( m_hSema != NULL ) {
			BOOL bRet = ::CloseHandle(m_hSema);
			assert( bRet );
			m_hSema = NULL;
		}
	}

private:
	HANDLE m_hSema;

private:
	//noncopyable
	inprocess_semaphore(const inprocess_semaphore&) throw();
	inprocess_semaphore& operator=(const inprocess_semaphore&) throw();
};

// interprocess_semaphore

class interprocess_semaphore
{
public:
	interprocess_semaphore() throw() : m_hSema(NULL)
	{
	}
	~interprocess_semaphore() throw()
	{
		Term();
	}

	void Lock()  //may throw
	{
		assert( m_hSema != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hSema, INFINITE);
		if( dwRet != WAIT_OBJECT_0 )
			throw GKC::Exception(call_result(CR_FAIL));
	}
	void Unlock() throw()
	{
		assert( m_hSema != NULL );
		LONG lCount;
		BOOL bRet = ::ReleaseSemaphore(m_hSema, 1, &lCount);
		assert( bRet );
	}
	bool TryLock() throw()
	{
		assert( m_hSema != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hSema, 0);
		return dwRet == WAIT_OBJECT_0;
	}

	// name is case sensitive and limited to MAX_PATH (255) characters.
	// recommend: maximum 240 characters
	call_result Create(const GKC::RefPtr<CharS>& str, bool bGlobal, int iCount) throw()
	{
		assert( !str.IsNull() );
		assert( m_hSema == NULL );
		//name
		CharW* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		HRESULT hRes = S_OK;
		m_hSema = ::CreateSemaphoreW(NULL, (LONG)iCount, (LONG)iCount, psz);
		if( m_hSema == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	call_result Open(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hSema == NULL );
		//name
		CharW* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		HRESULT hRes = S_OK;
		m_hSema = ::OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, psz);
		if( m_hSema == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	void Term() throw()
	{
		if( m_hSema != NULL ) {
			BOOL bRet = ::CloseHandle(m_hSema);
			assert( bRet );
			m_hSema = NULL;
		}
	}

private:
	HANDLE m_hSema;

private:
	//noncopyable
	interprocess_semaphore(const interprocess_semaphore&) throw();
	interprocess_semaphore& operator=(const interprocess_semaphore&) throw();
};

//Mutex

// inprocess_mutex

class inprocess_mutex
{
public:
	inprocess_mutex() throw() : m_bInitialized(false)
	{
		::ZeroMemory(&m_sec, sizeof(CRITICAL_SECTION));
	}
	~inprocess_mutex() throw()
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
		assert( m_bInitialized );
		::LeaveCriticalSection(&m_sec);
	}
	bool TryLock() throw()
	{
		assert( m_bInitialized );
		return ::TryEnterCriticalSection(&m_sec) != FALSE;
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
	friend class inprocess_condition;

private:
	//noncopyable
	inprocess_mutex(const inprocess_mutex&) throw();
	inprocess_mutex& operator=(const inprocess_mutex&) throw();
};

// interprocess_mutex

class interprocess_mutex
{
public:
	interprocess_mutex() throw() : m_hMutex(NULL)
	{
	}
	~interprocess_mutex() throw()
	{
		Term();
	}

	void Lock()  //may throw
	{
		assert( m_hMutex != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hMutex, INFINITE);
		if( dwRet != WAIT_OBJECT_0 )
			throw GKC::Exception(call_result(CR_FAIL));
	}
	void Unlock() throw()
	{
		assert( m_hMutex != NULL );
		BOOL bRet = ::ReleaseMutex(m_hMutex);
		assert( bRet );
	}
	bool TryLock() throw()
	{
		assert( m_hMutex != NULL );
		DWORD dwRet = ::WaitForSingleObject(m_hMutex, 0);
		return dwRet == WAIT_OBJECT_0;
	}

	// name is case sensitive and limited to MAX_PATH (255) characters.
	// recommend: maximum 240 characters
	call_result Create(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hMutex == NULL );
		//name
		CharW* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		HRESULT hRes = S_OK;
		m_hMutex = ::CreateMutexW(NULL, FALSE, psz);
		if( m_hMutex == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	call_result Open(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hMutex == NULL );
		//name
		CharW* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		HRESULT hRes = S_OK;
		m_hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, psz);
		if( m_hMutex == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	void Term() throw()
	{
		if( m_hMutex != NULL ) {
			BOOL bRet = ::CloseHandle(m_hMutex);
			assert( bRet );
			m_hMutex = NULL;
		}
	}

private:
	HANDLE m_hMutex;  //mutex

private:
	//noncopyable
	interprocess_mutex(const interprocess_mutex&) throw();
	interprocess_mutex& operator=(const interprocess_mutex&) throw();
};

//Condition

// inprocess_condition

class inprocess_condition
{
public:
	inprocess_condition() throw() : m_bInitialized(false)
	{
	}
	~inprocess_condition() throw()
	{
		Term();
	}

	void Wait(inprocess_mutex& mtx) throw()
	{
		assert( m_bInitialized );
		BOOL bRet = ::SleepConditionVariableCS(&m_cv, &mtx.m_sec, INFINITE);
		assert( bRet );
	}
	// uTimeout: 0 or number (ms)
	bool TryWait(inprocess_mutex& mtx, uint uTimeout) throw()
	{
		assert( m_bInitialized );
		BOOL bRet = ::SleepConditionVariableCS(&m_cv, &mtx.m_sec, uTimeout);
		return bRet ? true : false;
	}
	void Signal() throw()
	{
		assert( m_bInitialized );
		::WakeConditionVariable(&m_cv);
	}
	void SignalAll() throw()
	{
		assert( m_bInitialized );
		::WakeAllConditionVariable(&m_cv);
	}

	void Init() throw()
	{
		assert( !m_bInitialized );
		::InitializeConditionVariable(&m_cv);
		m_bInitialized = true;
	}
	void Term() throw()
	{
	}

private:
	CONDITION_VARIABLE  m_cv;
	bool  m_bInitialized;

private:
	inprocess_condition(const inprocess_condition&) throw();
	inprocess_condition& operator=(const inprocess_condition&) throw();
};

//RWLock

//------------------------------------------------------------------------------
// Thread

// thread_sleep

////////////////////////////////////////////////////////////////////////////////
