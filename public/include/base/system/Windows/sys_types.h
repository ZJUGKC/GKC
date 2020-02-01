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

	uintptr GetHandle() const throw()
	{
		return (uintptr)m_h;
	}
	void Attach(uintptr h) throw()
	{
		if( (uintptr)m_h != h )
			Close();
		m_h = (HANDLE)h;
	}
	uintptr Detach() throw()
	{
		uintptr h = (uintptr)m_h;
		m_h = NULL;
		return h;
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
	//dup (a new reference, inherited)
	call_result Duplicate(io_handle& hd) throw()
	{
		assert( IsValid() );
		assert( !hd.IsValid() );
		HRESULT hRes = S_OK;
		HANDLE hDup;
		if( !::DuplicateHandle(::GetCurrentProcess(), m_h, ::GetCurrentProcess(), &hDup, 0, TRUE, DUPLICATE_SAME_ACCESS) )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		else
			hd.m_h = hDup;
		return call_result((int)hRes);
	}

protected:
	HANDLE m_h;

private:
	io_handle(const io_handle&) throw();
	io_handle& operator=(const io_handle&) throw();
};

//------------------------------------------------------------------------------
// File

// file share modes
BEGIN_ENUM(_os_file_share_modes)
	ENUM_VALUE_ENTRY(Exclusive,  0x00000010)
	ENUM_VALUE_ENTRY(DenyWrite,  0x00000020)
	ENUM_VALUE_ENTRY(DenyRead,   0x00000030)
	ENUM_VALUE_ENTRY(DenyNone,   0x00000040)
END_ENUM()

//open file
//  iOpenType   : file_open_types::*
//  iShareMode  : _os_file_share_modes::*
//  iCreateType : file_creation_types::*
//  use platform path prefix
inline HRESULT _os_open_file(const char_s* szFile, int iOpenType, int iShareMode, int iCreateType, HANDLE& hd) throw()
{
	// access
	DWORD dwAccess = 0;
	switch( iOpenType & 3 ) {
	case file_open_types::Read:
		dwAccess = GENERIC_READ;
		break;
	case file_open_types::Write:
		dwAccess = GENERIC_WRITE;
		break;
	case file_open_types::ReadWrite:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	default:
		assert( false );  // invalid access mode
		break;
	}
	// map share mode
	DWORD dwShareMode = 0;
	switch( iShareMode & 0x70) { // map compatibility mode to exclusive
	case _os_file_share_modes::Exclusive:
		dwShareMode = 0;
		break;
	case _os_file_share_modes::DenyWrite:
		dwShareMode = FILE_SHARE_READ;
		break;
	case _os_file_share_modes::DenyRead:
		dwShareMode = FILE_SHARE_WRITE;
		break;
	case _os_file_share_modes::DenyNone:
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
	if( iCreateType & file_creation_types::Create ) {
		if( iCreateType & file_creation_types::NoTruncate )
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
	HANDLE hFile = ::CreateFileW(szFile, dwAccess, dwShareMode, &sa, dwCreateFlag, dwFlags, NULL);
	if( hFile == INVALID_HANDLE_VALUE ) {
		hRes  = HRESULT_FROM_WIN32(::GetLastError());
		hFile = NULL;
	}
	hd = hFile;

	return hRes;
}

// file_io_handle_helper

class file_io_handle_helper
{
public:
	//open
	//  The default behavior is sharing between processes and threads.
	//  iOpenType   : file_open_types::*
	//  iCreateType : file_creation_types::*
	//  use platform path prefix
	static call_result Open(const char_s* szFile, int iOpenType, int iCreateType, io_handle& hd) throw()
	{
		assert( !hd.IsValid() );
		HANDLE  hFile;
		HRESULT hRes = _os_open_file(szFile, iOpenType, _os_file_share_modes::DenyNone, iCreateType, hFile);
		hd.Attach((uintptr)hFile);
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
		if( !::SetFilePointerEx((HANDLE)(hd.GetHandle()), liMove, &liNew, uMethod) ) {
			hr = HRESULT_FROM_WIN32(::GetLastError());
		}
		else {
			iNewPos = liNew.QuadPart;
		}
		return call_result((int)hr);
	}
	//flush
	static call_result Flush(io_handle& hd) throw()
	{
		assert( hd.IsValid() );
		HRESULT hRes = S_OK;
		BOOL bRet = ::FlushFileBuffers((HANDLE)(hd.GetHandle()));
		if( !bRet )
			hRes = HRESULT_FROM_WIN32(::GetLastError());
		return call_result((int)hRes);
	}
	//set size
	static call_result SetSize(io_handle& hd, int64 iSize) throw()
	{
		assert( hd.IsValid() );
		int64 iNewPos;
		call_result cr = Seek(hd, iSize, iNewPos, IO_SEEK_BEGIN);
		if( cr.IsFailed() )
			return cr;
		if( !::SetEndOfFile((HANDLE)(hd.GetHandle())) )
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
		if( !::GetFileSizeEx((HANDLE)(hd.GetHandle()), &li) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		status.iSize = li.QuadPart;  //64 bits

		FILETIME ftA, ftM, ftC;
		if( !::GetFileTime((HANDLE)(hd.GetHandle()), &ftC, &ftA, &ftM) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		//access
		_os_filetime_to_timevalue(ftA, status.tmAccess);
		//modify
		_os_filetime_to_timevalue(ftM, status.tmModify);
		//create
		_os_filetime_to_timevalue(ftC, status.tmCreate);

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
		if( !::LockFileEx((HANDLE)(hd.GetHandle()), ((!bShare) ? LOCKFILE_EXCLUSIVE_LOCK : 0) | ((!bBlock) ? LOCKFILE_FAIL_IMMEDIATELY : 0),
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
		BOOL bRet = ::UnlockFileEx((HANDLE)(hd.GetHandle()), 0, li.LowPart, li.HighPart, &ov);
		assert( bRet );
	}
};

//------------------------------------------------------------------------------
// File System

// file_searcher

class file_searcher
{
public:
	file_searcher() throw() : m_uLength(0)
	{
	}
	~file_searcher() throw()
	{
	}

	void Close() throw()
	{
		m_ff.Close();
		m_uLength = 0;
	}

	bool IsValid() const throw()
	{
		return m_ff.IsValid();
	}

// Operations

	//strName: It can be a file name, or a directory without unnecessary trailing separator,
	//         or a directory with format '...\*'.
	bool Find(const const_string_s& strName) throw()
	{
		bool bRet = m_ff.Find(const_array_helper::GetInternalPointer(strName));
		if( bRet )
			m_uLength = calc_string_length(m_ff.GetFileName());
		return bRet;
	}
	bool FindNext() throw()
	{
		bool bRet = m_ff.FindNext();
		if( bRet )
			m_uLength = calc_string_length(m_ff.GetFileName());
		return bRet;
	}

// Attributes

	int64 GetFileSize() const throw()
	{
		return m_ff.GetFileSize();
	}
	const_string_s GetFileName() const throw()
	{
		return const_string_s(m_ff.GetFileName(), m_uLength);
	}
	void GetCreationTime(time_value& tv) const throw()
	{
		_os_filetime_to_timevalue(m_ff.GetCreationTime(), tv);
	}
	void GetAccessTime(time_value& tv) const throw()
	{
		_os_filetime_to_timevalue(m_ff.GetLastAccessTime(), tv);
	}
	void GetModifyTime(time_value& tv) const throw()
	{
		_os_filetime_to_timevalue(m_ff.GetLastWriteTime(), tv);
	}
	//It must be called after checking whether the file is a directory.
	bool IsDots() const throw()
	{
		return m_ff.IsDots();
	}
	bool IsDirectory() const throw()
	{
		return m_ff.IsDirectory();
	}

private:
	_os_file_finder m_ff;
	//for file name
	uintptr m_uLength;

private:
	//noncopyable
	file_searcher(const file_searcher&) throw();
	file_searcher& operator=(const file_searcher&) throw();
};

//------------------------------------------------------------------------------
// Synchronization

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
		BOOL bRet = ::SleepConditionVariableCS(&m_cv, &(mtx.m_sect.GetCS()), INFINITE);
		assert( bRet );
	}
	// uTimeout: 0 or number (ms)
	bool TryWait(inprocess_mutex& mtx, uint uTimeout) throw()
	{
		assert( m_bInitialized );
		BOOL bRet = ::SleepConditionVariableCS(&m_cv, &(mtx.m_sect.GetCS()), uTimeout);  //::GetLastError()==ERROR_TIMEOUT, failed
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
// File Mapping

inline void _os_get_file_mapping_flags(int iMapType, DWORD& dwMappingProtection, DWORD& dwViewDesiredAccess) noexcept
{
	bool bExec = (iMapType & file_mapping_types::Execute) != 0;
	dwMappingProtection = bExec ? PAGE_EXECUTE_READ : PAGE_READONLY;
	dwViewDesiredAccess = 0;
	if( iMapType & file_mapping_types::Write ) {
		dwMappingProtection = bExec ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
		dwViewDesiredAccess |= FILE_MAP_WRITE;
	}
	if( iMapType & file_mapping_types::Read )
		dwViewDesiredAccess |= FILE_MAP_READ;
	if( bExec )
		dwViewDesiredAccess |= FILE_MAP_EXECUTE;
}

// file_mapping

class file_mapping
{
public:
	file_mapping() noexcept
	{
	}
	~file_mapping() noexcept
	{
	}

	//iMapType : file_mapping_types::*
	call_result Map(const io_handle& hd, int64 iOffset, uintptr uSize, int iMapType) noexcept
	{
		assert( iOffset >= 0 );

		DWORD dwMappingProtection, dwViewDesiredAccess;
		_os_get_file_mapping_flags(iMapType, dwMappingProtection, dwViewDesiredAccess);

		return call_result((int)m_map.MapFile((HANDLE)hd.GetHandle(), uSize, iOffset,
						dwMappingProtection, dwViewDesiredAccess));
	}
	void Unmap() noexcept
	{
		HRESULT hr = m_map.Unmap();
		(void)hr;
		assert( SUCCEEDED(hr) );
	}

	uintptr GetAddress() const noexcept
	{
		return (uintptr)m_map.GetData();
	}
	uintptr GetSize() const noexcept
	{
		return m_map.GetMappingSize();
	}

private:
	_os_file_mapping m_map;

private:
	//noncopyable
	file_mapping(const file_mapping&) noexcept;
	file_mapping& operator=(const file_mapping&) noexcept;
};

// shared_memory

class shared_memory
{
public:
	shared_memory() noexcept
	{
	}
	~shared_memory() noexcept
	{
	}

	//iMapType : file_mapping_types::*
	call_result Create(const const_string_s& strName, bool bGlobal, uintptr uSize, int iMapType, bool& bAlreadyExisted) noexcept
	{
		char_h* psz = _sync_helper::gen_sync_name(const_array_helper::GetInternalPointer(strName), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);

		DWORD dwMappingProtection, dwViewDesiredAccess;
		_os_get_file_mapping_flags(iMapType, dwMappingProtection, dwViewDesiredAccess);

		BOOL bExisting = FALSE;
		call_result cr((int)m_map.MapSharedMem(uSize, psz, &bExisting, NULL, dwMappingProtection, dwViewDesiredAccess));
		bAlreadyExisted = bExisting ? true : false;

		_sync_helper::free_sync_name(psz, bGlobal);
		return cr;
	}
	call_result Open(const const_string_s& strName, bool bGlobal, int64 iOffset, uintptr uSize, int iMapType) noexcept
	{
		assert( iOffset >= 0 );

		char_h* psz = _sync_helper::gen_sync_name(const_array_helper::GetInternalPointer(strName), bGlobal);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);

		DWORD dwMappingProtection, dwViewDesiredAccess;
		_os_get_file_mapping_flags(iMapType, dwMappingProtection, dwViewDesiredAccess);

		call_result cr((int)m_map.OpenMapping(psz, uSize, iOffset, dwViewDesiredAccess));

		_sync_helper::free_sync_name(psz, bGlobal);
		return cr;
	}
	void Destroy() noexcept
	{
		HRESULT hr = m_map.Unmap();
		(void)hr;
		assert( SUCCEEDED(hr) );
	}

	uintptr GetAddress() const noexcept
	{
		return (uintptr)m_map.GetData();
	}
	uintptr GetSize() const noexcept
	{
		return m_map.GetMappingSize();
	}

private:
	_os_file_mapping m_map;

private:
	//noncopyable
	shared_memory(const shared_memory&) noexcept;
	shared_memory& operator=(const shared_memory&) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
