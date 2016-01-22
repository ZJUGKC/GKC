/*
** Copyright (c) 2014, Xin YUAN, courses of Zhejiang University
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
// IO

#define IO_SEEK_BEGIN        FILE_BEGIN
#define IO_SEEK_CURRENT      FILE_CURRENT
#define IO_SEEK_END          FILE_END

// io_handle

class io_handle
{
public:
	io_handle() throw() : m_h(NULL)
	{
	}
	io_handle(io_handle&& src) throw()
	{
		m_h = src.m_h;
		src.m_h = NULL;
	}
	~io_handle() throw()
	{
		Close();
	}

	io_handle& operator=(io_handle&& src) throw()
	{
		if( &src != this ) {
			if( src.m_h != m_h ) {
				Close();
				m_h = src.m_h;
				src.m_h = NULL;
			}
		}
		return *this;
	}

	bool IsValid() const throw()
	{
		return m_h != NULL;
	}

	//close
	void Close() throw()
	{
		if( IsValid() ) {
			BOOL bRet = ::CloseHandle(m_h);
			assert( bRet );
			m_h = NULL;
		}
	}

	//read
	call_result Read(void* pBuffer, uint uBytes, uint& uRead) throw()
	{
		assert( IsValid() );
		HRESULT hr = S_OK;
		if( !::ReadFile(m_h, pBuffer, uBytes, (LPDWORD)&uRead, NULL) )
			hr = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hr);
	}
	//write
	call_result Write(const void* pBuffer, uint uBytes, uint& uWritten) throw()
	{
		assert( IsValid() );
		HRESULT hr = S_OK;
		if( !::WriteFile(m_h, pBuffer, uBytes, (LPDWORD)&uWritten, NULL) )
			hr = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hr);
	}

protected:
	HANDLE  m_h;

private:
	io_handle(const io_handle&) throw();
	io_handle& operator=(const io_handle&) throw();

private:
	friend class io_handle_helper;
};

// io_handle_helper

class io_handle_helper
{
public:
	//open
	//  The default behavior is sharing between processes and threads.
	//  iOpenType   : file_open_types::*
	//  iCreateType : file_creation_types::*
	static call_result Open(const char_s* szFile, int iOpenType, int iCreateType, io_handle& hd) throw()
	{
		assert( !hd.IsValid() );
		HANDLE  hFile;
		HRESULT hRes = _os_open_file(szFile, iOpenType, _os_file_share_modes::DenyNone, iCreateType, hFile);
		hd.m_h = hFile;
		return call_result((int)hRes);
	}
	//seek
	//  uMethod: IO_SEEK_*
	//  If uMethod = IO_SEEK_BEGIN, iOffset is regarded as an unsigned value.
	static call_result Seek(io_handle& hd, int64 iOffset, int64& iNewPos, uint uMethod) throw()
	{
		assert( hd.IsValid() );
		LARGE_INTEGER liMove;
		liMove.QuadPart = iOffset;
		LARGE_INTEGER liNew;
		HRESULT hr = S_OK;
		if( !::SetFilePointerEx(hd.m_h, liMove, &liNew, uMethod) ) {
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		else {
			iNewPos = liNew.QuadPart;
		}
		return call_result((int)hr);
	}
	//flush
	static void Flush(io_handle& hd) throw()
	{
		assert( hd.IsValid() );
		BOOL bRet = ::FlushFileBuffers(hd.m_h);
		assert( bRet );
	}
	//set size
	static call_result SetSize(io_handle& hd, int64 iSize) throw()
	{
		assert( hd.IsValid() );
		int64 iNewPos;
		call_result cr = Seek(hd, iSize, iNewPos, IO_SEEK_BEGIN);
		if( cr.IsFailed() )
			return cr;
		if( !::SetEndOfFile(hd.m_h) )
			cr.SetResult((int)(HRESULT_FROM_WIN32(::GetLastError())));
		return cr;
	}
	//get status
	static call_result GetStatus(io_handle& hd, storage_status& status) throw()
	{
		assert( hd.IsValid() );

		HRESULT hRes = S_OK;
		//size
		LARGE_INTEGER li;
		if( !::GetFileSizeEx(hd.m_h, &li) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		status.iSize = li.QuadPart;  //64 bits

		FILETIME   ftA, ftM, ftC;
		SYSTEMTIME st;
		if( !::GetFileTime(hd.m_h, &ftC, &ftA, &ftM) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		//access
		if( !::FileTimeToSystemTime(&ftA, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmAccess);
		//modify
		if( !::FileTimeToSystemTime(&ftM, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmModify);
		//create
		if( !::FileTimeToSystemTime(&ftC, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmCreate);

		return call_result((int)hRes);
	}
	//lock
	//  bShare : true for shared lock, false for exclusive lock
	//  bBlock : true for blocking, false for nonblocking
	//  if iLen = 0, lock all bytes from iOffset to the end of file
	static call_result Lock(io_handle& hd, int64 iOffset, int64 iLen, bool bShare = false, bool bBlock = false) throw()
	{
		assert( hd.IsValid() );
		HRESULT hRes = S_OK;
		OVERLAPPED ov;
		mem_zero(&ov, sizeof(OVERLAPPED));
		LARGE_INTEGER li;
		li.QuadPart = iOffset;
		ov.Offset     = li.LowPart;
		ov.OffsetHigh = li.HighPart;
		li.QuadPart = iLen;
		if( !::LockFileEx(hd.m_h, ((!bShare) ? LOCKFILE_EXCLUSIVE_LOCK : 0) | ((!bBlock) ? LOCKFILE_FAIL_IMMEDIATELY : 0),
							0, li.LowPart, li.HighPart, &ov) )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hRes);
	}
	//unlock
	static void Unlock(io_handle& hd, int64 iOffset, int64 iLen) throw()
	{
		assert( hd.IsValid() );
		OVERLAPPED ov;
		mem_zero(&ov, sizeof(OVERLAPPED));
		LARGE_INTEGER li;
		li.QuadPart = iOffset;
		ov.Offset     = li.LowPart;
		ov.OffsetHigh = li.HighPart;
		li.QuadPart = iLen;
		BOOL bRet = ::UnlockFileEx(hd.m_h, 0, li.LowPart, li.HighPart, &ov);
		assert( bRet );
	}
};

//------------------------------------------------------------------------------
// Synchronization

// helper

class _sync_helper
{
public:
	static char_h* gen_global_name(const char_h* pSrc) throw()
	{
		//global
		DECLARE_LOCAL_STATIC_CONST_STRING(char_h, c_szGlobal, c_uGlobalLength, L"Global\\")
		//generate
		uintptr uCount = calc_string_length(pSrc);
		uintptr uNewCount;
		call_result cr = safe_operators::Add(uCount, c_uGlobalLength, uNewCount);
		if( cr.IsFailed() )
			return NULL;
		char_h* pNew = (char_h*)crt_alloc((uNewCount + 1) * sizeof(char_h));
		if( pNew == NULL )
			return NULL;
		//copy
		mem_copy(c_szGlobal, c_uGlobalLength * sizeof(char_h), pNew);
		mem_copy(pSrc, uCount * sizeof(char_h), pNew + c_uGlobalLength);
		pNew[uNewCount] = 0;
		return pNew;
	}
	static void free_global_name(const char_h* p) throw()
	{
		crt_free(p);
	}
	//tools
	static char_h* gen_sync_name(const char_h* pSrc, bool bGlobal) throw()
	{
		char_h* psz = (char_h*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(const char_h* p, bool bGlobal) throw()
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
			throw exception_base(call_result(CR_FAIL));
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
			throw exception_base(call_result(CR_FAIL));
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
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal, int iCount) throw()
	{
		assert( !str.IsNull() );
		assert( m_hSema == NULL );
		//name
		char_h* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		HRESULT hRes = S_OK;
		m_hSema = ::CreateSemaphoreW(NULL, (LONG)iCount, (LONG)iCount, psz);
		if( m_hSema == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		_sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	call_result Open(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hSema == NULL );
		//name
		char_h* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		HRESULT hRes = S_OK;
		m_hSema = ::OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE, psz);
		if( m_hSema == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		_sync_helper::free_sync_name(psz, bGlobal);
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
	inprocess_mutex() throw()
	{
	}
	~inprocess_mutex() throw()
	{
	}

	void Lock() throw()
	{
		m_sect.Lock();
	}
	void Unlock() throw()
	{
		m_sect.Unlock();
	}
	bool TryLock() throw()
	{
		return m_sect.TryLock();
	}

	//methods
	call_result Init() throw()
	{
		return m_sect.Init();
	}
	void Term() throw()
	{
		m_sect.Term();
	}

private:
	_os_critical_section m_sect;

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
			throw exception_base(call_result(CR_FAIL));
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
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hMutex == NULL );
		//name
		char_h* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		HRESULT hRes = S_OK;
		m_hMutex = ::CreateMutexW(NULL, FALSE, psz);
		if( m_hMutex == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		_sync_helper::free_sync_name(psz, bGlobal);
		return call_result((int)hRes);
	}
	call_result Open(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_hMutex == NULL );
		//name
		char_h* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		HRESULT hRes = S_OK;
		m_hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, psz);
		if( m_hMutex == NULL )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		_sync_helper::free_sync_name(psz, bGlobal);
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
		BOOL bRet = ::SleepConditionVariableCS(&m_cv, &mtx.m_sec, uTimeout);  //::GetLastError()==ERROR_TIMEOUT, failed
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
	//noncopyable
	inprocess_condition(const inprocess_condition&) throw();
	inprocess_condition& operator=(const inprocess_condition&) throw();
};

//RWLock

// inprocess_rwlock

class inprocess_rwlock
{
public:
	inprocess_rwlock() throw() : m_bInitialized(false)
	{
	}
	~inprocess_rwlock() throw()
	{
		Term();
	}

	void LockShared() throw()
	{
		assert( m_bInitialized );
		::AcquireSRWLockShared(&m_rw);
	}
	void LockExclusive() throw()
	{
		assert( m_bInitialized );
		::AcquireSRWLockExclusive(&m_rw);
	}
	void UnlockShared() throw()
	{
		assert( m_bInitialized );
		::ReleaseSRWLockShared(&m_rw);
	}
	void UnlockExclusive() throw()
	{
		assert( m_bInitialized );
		::ReleaseSRWLockExclusive(&m_rw);
	}
	bool TryLockShared() throw()
	{
		assert( m_bInitialized );
		return ::TryAcquireSRWLockShared(&m_rw) ? true: false;
	}
	bool TryLockExclusive() throw()
	{
		assert( m_bInitialized );
		return ::TryAcquireSRWLockExclusive(&m_rw) ? true : false;
	}

	void Init() throw()
	{
		assert( !m_bInitialized );
		::InitializeSRWLock(&m_rw);
		m_bInitialized = true;
	}
	void Term() throw()
	{
	}

private:
	SRWLOCK  m_rw;
	bool  m_bInitialized;

private:
	//noncopyable
	inprocess_rwlock(const inprocess_rwlock&) throw();
	inprocess_rwlock& operator=(const inprocess_rwlock&) throw();
};

//------------------------------------------------------------------------------
// Thread

// thread_sleep
//  uTimeout: ms
inline void thread_sleep(uint uTimeout) throw()
{
	::Sleep(uTimeout);
}

////////////////////////////////////////////////////////////////////////////////
