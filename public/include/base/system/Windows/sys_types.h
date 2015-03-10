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

#include "_Base.h"

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
	call_result Read(GKC::RefPtr<byte>& buffer, uint uBytes, uint& uRead) throw()
	{
		assert( IsValid() );
		HRESULT hr = S_OK;
		if( !::ReadFile(m_h, (void*)(GKC::RefPtrHelper::GetInternalPointer(buffer)), uBytes, (LPDWORD)&uRead, NULL) )
			hr = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hr);
	}
	//write
	call_result Write(const GKC::RefPtr<byte>& buffer, uint uBytes, uint& uWritten) throw()
	{
		assert( IsValid() );
		HRESULT hr = S_OK;
		if( !::WriteFile(m_h, (const void*)(GKC::RefPtrHelper::GetInternalPointer(buffer)), uBytes, (LPDWORD)&uWritten, NULL) )
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
	//  iOpenType   : FileOpenTypes::*
	//  iShareMode  : FileShareModes::*
	//  iCreateType : FileCreationTypes::*
	static call_result Open(const GKC::RefPtr<CharS>& szFile, int iOpenType, int iShareMode, int iCreateType, io_handle& hd) throw()
	{
		assert( !hd.IsValid() );

		// access
		DWORD dwAccess = 0;
		switch( iOpenType & 3 ) {
		case GKC::FileOpenTypes::Read:
			dwAccess = GENERIC_READ;
			break;
		case GKC::FileOpenTypes::Write:
			dwAccess = GENERIC_WRITE;
			break;
		case GKC::FileOpenTypes::ReadWrite:
			dwAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		default:
			assert( false );  // invalid access mode
			break;
		}
		// map share mode
		DWORD dwShareMode = 0;
		switch( iShareMode & 0x70) { // map compatibility mode to exclusive
		case GKC::FileShareModes::Exclusive:
			dwShareMode = 0;
			break;
		case GKC::FileShareModes::DenyWrite:
			dwShareMode = FILE_SHARE_READ;
			break;
		case GKC::FileShareModes::DenyRead:
			dwShareMode = FILE_SHARE_WRITE;
			break;
		case GKC::FileShareModes::DenyNone:
			dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
			break;
		default:
			assert( false );  // invalid share mode
			break;
		}
		// set NoInherit, default is inherited.
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;  //inherited
		// map creation flags
		DWORD dwCreateFlag = OPEN_EXISTING;
		if( iCreateType & GKC::FileCreationTypes::Create ) {
			if( iCreateType & GKC::FileCreationTypes::NoTruncate )
				dwCreateFlag = OPEN_ALWAYS;
			else
				dwCreateFlag = CREATE_ALWAYS;
		}

		// special system-level access flags
		DWORD dwFlags = FILE_ATTRIBUTE_NORMAL;
		// Random access and sequential scan should be mutually exclusive
		//   FILE_FLAG_RANDOM_ACCESS and FILE_FLAG_SEQUENTIAL_SCAN
		//   no set : FILE_FLAG_NO_BUFFERING FILE_FLAG_WRITE_THROUGH

		HRESULT hRes = S_OK;

		// attempt file creation
		HANDLE hFile = ::CreateFileW(GKC::RefPtrHelper::GetInternalPointer(szFile),
									dwAccess, dwShareMode, &sa, dwCreateFlag, dwFlags, NULL);
		if( hFile == INVALID_HANDLE_VALUE )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		else
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
	void Flush(io_handle& hd) throw()
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
	static call_result GetStatus(io_handle& hd, GKC::FileStatus& status) throw()
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
		_cvt_system_time(&st, status.tmAccess);
		//modify
		if( !::FileTimeToSystemTime(&ftM, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_cvt_system_time(&st, status.tmModify);
		//create
		if( !::FileTimeToSystemTime(&ftC, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_cvt_system_time(&st, status.tmCreate);

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
