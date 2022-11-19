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

//Linux

//------------------------------------------------------------------------------
// Shared Assembly

#pragma pack(push, 1)

// sa_handle

class sa_handle
{
public:
	sa_handle() throw() : m_hd(NULL)
	{
	}
	sa_handle(sa_handle&& src) throw() : m_hd(src.m_hd)
	{
		src.m_hd = NULL;
	}
	~sa_handle() throw()
	{
		Free();
	}

	sa_handle& operator=(sa_handle&& src) throw()
	{
		if( &src != this ) {
			Attach(src.Detach());
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
		//shared when m_hd==hd
		Free();
		m_hd = (void*)hd;
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
			int ret = ::dlclose(m_hd);
			(void)ret;
			assert( ret == 0 );
			m_hd = NULL;
		}
	}

	bool Load(const char_s* szFile) throw()
	{
		assert( szFile != NULL );
		assert( m_hd == NULL );
		m_hd = ::dlopen(szFile, RTLD_LAZY);
		return m_hd != NULL;
	}
	uintptr GetFunctionAddress(const char_a* szFunc) const throw()
	{
		assert( m_hd != NULL );
		::dlerror();
		uintptr ret = (uintptr)::dlsym(m_hd, szFunc);
		return (::dlerror() == NULL) ? ret : 0;
	}

private:
	void* m_hd;

private:
	//noncopyable
	sa_handle(const sa_handle& src) throw();
	sa_handle& operator=(const sa_handle& src) throw();
};

#pragma pack(pop)

//------------------------------------------------------------------------------
// IO

#define IO_SEEK_BEGIN        SEEK_SET
#define IO_SEEK_CURRENT      SEEK_CUR
#define IO_SEEK_END          SEEK_END

// io_handle

class io_handle
{
public:
	io_handle() throw() : m_fd(-1)
	{
	}
	io_handle(io_handle&& src) throw() : m_fd(src.m_fd)
	{
		src.m_fd = -1;
	}
	~io_handle() throw()
	{
		Close();
	}

	io_handle& operator=(io_handle&& src) throw()
	{
		if( &src != this ) {
			Attach(src.Detach());
		}
		return *this;
	}

	bool IsValid() const throw()
	{
		return m_fd != -1;
	}

	uintptr GetHandle() const throw()
	{
		return (uintptr)m_fd;
	}
	void Attach(uintptr h) throw()
	{
		//unique when m_fd==h
		if( (uintptr)m_fd != h )
			Close();
		m_fd = (int)h;
	}
	uintptr Detach() throw()
	{
		uintptr h = (uintptr)m_fd;
		m_fd = -1;
		return h;
	}

	//close
	void Close() throw()
	{
		if( IsValid() ) {
			int res = ::close(m_fd);
			(void)res;
			assert( res == 0 );
			m_fd = -1;
		}
	}

	//read
	call_result Read(void* pBuffer, uint uBytes, uint& uRead) throw()
	{
		assert( IsValid() );
		assert( uBytes <= SSIZE_MAX );
		ssize_t uRet = ::read(m_fd, pBuffer, (size_t)uBytes);
		int res = 0;
		if( uRet == -1 )
			res = _OS_CR_FROM_ERRORNO();
		else
			uRead = (uint)uRet;
		return call_result(res);
	}
	//write
	//  may have a bug in 32-bit system: uWritten may be overflow.
	call_result Write(const void* pBuffer, uint uBytes, uint& uWritten) throw()
	{
		assert( IsValid() );
		ssize_t uRet = ::write(m_fd, pBuffer, (size_t)uBytes);
		int res = 0;
		if( uRet == -1 )
			res = _OS_CR_FROM_ERRORNO();
		else
			uWritten = (uint)uRet;
		return call_result(res);
	}
	//dup (a new reference, inherited)
	call_result Duplicate(io_handle& hd) throw()
	{
		assert( IsValid() );
		assert( !hd.IsValid() );
		int res = 0;
		int fdup = ::dup(m_fd);
		if( fdup == -1 )
			res = _OS_CR_FROM_ERRORNO();
		else
			hd.m_fd = fdup;
		return call_result(res);
	}

protected:
	int m_fd;

private:
	io_handle(const io_handle&) throw();
	io_handle& operator=(const io_handle&) throw();
};

//------------------------------------------------------------------------------
// File

// file_io_handle_helper

class file_io_handle_helper
{
public:
	//open
	//  The default behavior is sharing between processes and threads.
	//  iOpenType   : file_open_types::*
	//  iCreateType : file_creation_types::*
	static call_result Open(const char_s* szFile, int iOpenType, int iCreateType, io_handle& hd) throw()
	{
		assert( !hd.IsValid() );

		// flags
		int flags = 0;
		// access
		switch( iOpenType ) {
		case file_open_types::Read:
			flags |= O_RDONLY;
			break;
		case file_open_types::Write:
			flags |= O_WRONLY;
			break;
		case file_open_types::ReadWrite:
			flags |= O_RDWR;
			break;
		default:
			assert( false );
			break;
		}
		// creation type
		if( iCreateType & file_creation_types::Create ) {
			flags |= O_CREAT;
			if( !(iCreateType & file_creation_types::NoTruncate) )
				flags |= O_TRUNC;
		}

		//open
		int res = 0;
/*
S_IRUSR S_IRGRP S_IROTH
S_IWUSR S_IWGRP S_IWOTH
S_IXUSR S_IXGRP S_IXOTH
=======================
S_IRWXU S_IRWXG S_IRWXO
*/
		int fd = ::open(szFile, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		if( fd == -1 )
			res = _OS_CR_FROM_ERRORNO();
		else
			hd.Attach((uintptr)fd);

		return call_result(res);
	}
	//seek
	//  uMethod: IO_SEEK_*
	//  If uMethod = IO_SEEK_BEGIN, iOffset is regarded as an unsigned value.
	static call_result Seek(io_handle& hd, int64 iOffset, int64& iNewPos, uint uMethod) throw()
	{
		assert( hd.IsValid() );
		assert( sizeof(off_t) == 8 );
		off_t npos = ::lseek((int)(hd.GetHandle()), (off_t)iOffset, (int)uMethod);  //64 bits
		int res = 0;
		if( npos == (off_t)-1 )
			res = _OS_CR_FROM_ERRORNO();
		else
			iNewPos = npos;
		return call_result(res);
	}
	//flush
	static call_result Flush(io_handle& hd) throw()
	{
		assert( hd.IsValid() );
		int res = 0;
		if( ::fsync((int)(hd.GetHandle())) == -1 )
			res = _OS_CR_FROM_ERRORNO();
		return call_result(res);
	}
	//set size
	static call_result SetSize(io_handle& hd, int64 iSize) throw()
	{
		assert( hd.IsValid() );
		int res = 0;
		if( ::ftruncate((int)(hd.GetHandle()), iSize) == -1 )
			res = _OS_CR_FROM_ERRORNO();
		return call_result(res);
	}
	//get status
	static call_result GetStatus(io_handle& hd, storage_status& status) throw()
	{
		assert( hd.IsValid() );

		int res = 0;
		//stat
		struct stat st;
		if( ::fstat((int)(hd.GetHandle()), &st) == -1 ) {
			res = _OS_CR_FROM_ERRORNO();
			return call_result(res);
		}
		//size
		status.iSize = st.st_size;  //64 bits

		//access
		_os_timespec_to_timevalue(st.st_atim, status.tmAccess);
		//modify
		_os_timespec_to_timevalue(st.st_mtim, status.tmModify);
		//create
		_os_timespec_to_timevalue(st.st_ctim, status.tmCreate);

		return call_result(res);
	}
	//lock
	//  bShare : true for shared lock, false for exclusive lock
	//  bBlock : true for blocking, false for nonblocking
	//  if iLen = 0, lock all bytes from iOffset to the end of file
	static call_result Lock(io_handle& hd, int64 iOffset, int64 iLen, bool bShare = false, bool bBlock = false) throw()
	{
		assert( hd.IsValid() );
		int res = 0;
		struct flock fl;
		fl.l_type   = bShare ? F_RDLCK : F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start  = iOffset;
		fl.l_len    = iLen;
		if( ::fcntl((int)(hd.GetHandle()), bBlock ? F_SETLKW : F_SETLK, &fl) == -1 )
			res = _OS_CR_FROM_ERRORNO();
		return call_result(res);
	}
	//unlock
	static void Unlock(io_handle& hd, int64 iOffset, int64 iLen) throw()
	{
		assert( hd.IsValid() );
		struct flock fl;
		fl.l_type   = F_UNLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start  = iOffset;
		fl.l_len    = iLen;
		int rv = ::fcntl((int)(hd.GetHandle()), F_SETLK, &fl);  //nonblocking
		(void)rv;
		assert( rv != -1 );
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
		m_fs.Close();
		m_uLength = 0;
	}

	bool IsValid() const throw()
	{
		return m_fs.IsValid();
	}

// Operations

	//strName: It can be a file name, or a directory without unnecessary trailing separator,
	//         or a directory with format '.../*'.
	bool Find(const const_string_s& strName) throw()
	{
		bool bRet = m_fs.Find(const_array_helper::GetInternalPointer(strName));
		if( bRet )
			m_uLength = calc_string_length(m_fs.GetFileName());
		return bRet;
	}
	bool FindNext() throw()
	{
		bool bRet = m_fs.FindNext();
		if( bRet )
			m_uLength = calc_string_length(m_fs.GetFileName());
		return bRet;
	}

// Attributes

	int64 GetFileSize() const throw()
	{
		return m_fs.GetFileSize();
	}
	const_string_s GetFileName() const throw()
	{
		return const_string_s(m_fs.GetFileName(), m_uLength);
	}
	void GetCreationTime(time_value& tv) const throw()
	{
		_os_timespec_to_timevalue(m_fs.GetCreationTime(), tv);
	}
	void GetAccessTime(time_value& tv) const throw()
	{
		_os_timespec_to_timevalue(m_fs.GetAccessTime(), tv);
	}
	void GetModifyTime(time_value& tv) const throw()
	{
		_os_timespec_to_timevalue(m_fs.GetModifyTime(), tv);
	}
	//It must be called after checking whether the file is a directory.
	bool IsDots() const throw()
	{
		return m_fs.IsDots();
	}
	bool IsDirectory() const throw()
	{
		return m_fs.IsDirectory();
	}

private:
	_os_file_searcher m_fs;
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
	inprocess_semaphore() throw() : m_bInitialized(false)
	{
	}
	~inprocess_semaphore() throw()
	{
		Term();
	}

	void Lock()  //may throw
	{
		assert( m_bInitialized );
		int res = ::sem_wait(&m_sem);
		if( res != 0 )
			throw exception_base(call_result(CR_FAIL));
	}
	void Unlock() throw()
	{
		assert( m_bInitialized );
		int res = ::sem_post(&m_sem);
		(void)res;
		assert( res == 0 );
	}
	bool TryLock() throw()
	{
		assert( m_bInitialized );
		int res = ::sem_trywait(&m_sem);
		return res == 0;
	}

	call_result Init(int iCount) throw()
	{
		assert( !m_bInitialized );
		int res = ::sem_init(&m_sem, 0, (unsigned int)iCount);
		if( res == -1 ) {
			res = _OS_CR_FROM_ERRORNO();
		}
		else if( res == 0 ) {
			m_bInitialized = true;
		}
		else {
			assert( false );
		}
		return call_result(res);
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			int res = ::sem_destroy(&m_sem);
			(void)res;
			assert( res == 0 );
			m_bInitialized = false;
		}
	}

private:
	sem_t  m_sem;
	bool   m_bInitialized;

private:
	//noncopyable
	inprocess_semaphore(const inprocess_semaphore&) throw();
	inprocess_semaphore& operator=(const inprocess_semaphore&) throw();
};

// interprocess_semaphore

class interprocess_semaphore
{
public:
	interprocess_semaphore() throw() : m_psem(NULL), m_szName(NULL), m_bExisting(false)
	{
	}
	~interprocess_semaphore() throw()
	{
		Term();
	}

	void Lock()  //may throw
	{
		assert( m_psem != NULL );
		int res = ::sem_wait(m_psem);
		if( res != 0 )
			throw exception_base(call_result(CR_FAIL));
	}
	void Unlock() throw()
	{
		assert( m_psem != NULL );
		int res = ::sem_post(m_psem);
		(void)res;
		assert( res == 0 );
	}
	bool TryLock() throw()
	{
		assert( m_psem != NULL );
		int res = ::sem_trywait(m_psem);
		return res == 0;
	}

	// name is case sensitive and limited to NAME_MAX-4 (251) characters.
	// recommend: maximum 240 characters
	//   bGlobal is unuseful ( LOCAL = GLOBAL )
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal, int iCount, bool& bExisting) throw()
	{
		assert( !str.IsNull() );
		assert( m_psem == NULL && m_szName == NULL );
		//name
		char_a* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), true);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		m_bExisting = false;
		//create
		int res = 0;
		int oflag   = O_CREAT | O_RDWR;
		mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;  //S_IRWXO is necessary for forked process
		sem_t* psem = ::sem_open(psz, oflag | O_EXCL, mode, (unsigned int)iCount);
		if( psem == SEM_FAILED ) {
			if( errno != EEXIST ) {
				res = _OS_CR_FROM_ERRORNO();
				_sync_helper::free_sync_name(psz, true);
				return call_result(res);
			}
			m_bExisting = true;
			psem = ::sem_open(psz, oflag, mode, (unsigned int)iCount);
			assert( psem != SEM_FAILED );
		}
		m_psem   = psem;
		m_szName = psz;
		bExisting = m_bExisting;
		return call_result(res);
	}
	call_result Open(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_psem == NULL && m_szName == NULL );
		//name
		char_a* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), true);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		int res = 0;
		sem_t* psem = ::sem_open(psz, O_RDWR);
		if( psem == SEM_FAILED ) {
			res = _OS_CR_FROM_ERRORNO();
			_sync_helper::free_sync_name(psz, true);
		}
		else {
			m_psem   = psem;
			m_szName = psz;
			m_bExisting = true;
		}
		return call_result(res);
	}
	void Term() throw()
	{
		if( m_psem != NULL ) {
			int res = ::sem_close(m_psem);
			(void)res;
			assert( res == 0 );
			m_psem = NULL;
			assert( m_szName != NULL );
			if( !m_bExisting )
				::sem_unlink(m_szName);  //no check, the return value may not be equal to zero.
			_sync_helper::free_sync_name(m_szName, true);
			m_szName = NULL;
		}
	}

private:
	sem_t*   m_psem;
	char_a*  m_szName;
	bool     m_bExisting;

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
	}
	~inprocess_mutex() throw()
	{
		Term();
	}

	void Lock() throw()
	{
		assert( m_bInitialized );
		::pthread_mutex_lock(&m_mtx);
	}
	void Unlock() throw()
	{
		assert( m_bInitialized );
		::pthread_mutex_unlock(&m_mtx);
	}
	bool TryLock() throw()
	{
		assert( m_bInitialized );
		return ::pthread_mutex_trylock(&m_mtx) == 0;
	}

	//methods
	call_result Init() throw()
	{
		assert( !m_bInitialized );

		int res = ::pthread_mutex_init(&m_mtx, NULL);
		if( res > 0 ) {
			res = CR_FROM_ERROR(res);
		}
		else if( res == 0 ) {
			m_bInitialized = true;
		}

		return call_result(res);
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			::pthread_mutex_destroy(&m_mtx);
			m_bInitialized = false;
		}
	}

private:
	pthread_mutex_t m_mtx;
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
	interprocess_mutex() throw()
	{
	}
	~interprocess_mutex() throw()
	{
	}

	void Lock()  //may throw
	{
		m_sem.Lock();
	}
	void Unlock() throw()
	{
		m_sem.Unlock();
	}
	bool TryLock() throw()
	{
		return m_sem.TryLock();
	}

	// name is case sensitive and limited to NAME_MAX-4 (251) characters.
	// recommend: maximum 240 characters
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal, bool& bExisting) throw()
	{
		return m_sem.Create(str, bGlobal, 1, bExisting);
	}
	call_result Open(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		return m_sem.Open(str, bGlobal);
	}
	void Term() throw()
	{
		m_sem.Term();
	}

private:
	interprocess_semaphore  m_sem;

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
		::pthread_cond_wait(&m_cv, &mtx.m_mtx);
	}
	// uTimeout: 0 or number (ms)
	bool TryWait(inprocess_mutex& mtx, uint uTimeout) throw()
	{
		assert( m_bInitialized );
		timeval now;
		::gettimeofday(&now, NULL);  //no check
		uint uSecond = uTimeout / 1000;
		uint uRest = uTimeout % 1000;
		timespec to;
		to.tv_sec = now.tv_sec + uSecond;
		to.tv_nsec = (now.tv_usec + uRest * 1000) * 1000;
		return ::pthread_cond_timedwait(&m_cv, &mtx.m_mtx, &to) == 0;  //ETIMEDOUT : failed
	}
	void Signal() throw()
	{
		assert( m_bInitialized );
		::pthread_cond_signal(&m_cv);
	}
	void SignalAll() throw()
	{
		assert( m_bInitialized );
		::pthread_cond_broadcast(&m_cv);
	}

	void Init() throw()
	{
		assert( !m_bInitialized );
		::pthread_cond_init(&m_cv, NULL);
		m_bInitialized = true;
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			int res = ::pthread_cond_destroy(&m_cv);
			(void)res;
			assert( res == 0 );
			m_bInitialized = false;
		}
	}

private:
	pthread_cond_t  m_cv;
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
		int res = ::pthread_rwlock_rdlock(&m_rw);
		(void)res;
		assert( res == 0 );
	}
	void LockExclusive() throw()
	{
		assert( m_bInitialized );
		int res = ::pthread_rwlock_wrlock(&m_rw);
		(void)res;
		assert( res == 0 );
	}
	void UnlockShared() throw()
	{
		assert( m_bInitialized );
		int res = ::pthread_rwlock_unlock(&m_rw);
		(void)res;
		assert( res == 0 );
	}
	void UnlockExclusive() throw()
	{
		assert( m_bInitialized );
		int res = ::pthread_rwlock_unlock(&m_rw);
		(void)res;
		assert( res == 0 );
	}
	bool TryLockShared() throw()
	{
		assert( m_bInitialized );
		return ::pthread_rwlock_tryrdlock(&m_rw) == 0;
	}
	bool TryLockExclusive() throw()
	{
		assert( m_bInitialized );
		return ::pthread_rwlock_trywrlock(&m_rw) == 0;
	}

	void Init() throw()
	{
		assert( !m_bInitialized );
		int res = ::pthread_rwlock_init(&m_rw, NULL);
		(void)res;
		assert( res == 0 );
		m_bInitialized = true;
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
			int res = ::pthread_rwlock_destroy(&m_rw);
			(void)res;
			assert( res == 0 );
			m_bInitialized = false;
		}
	}

private:
	pthread_rwlock_t  m_rw;
	bool  m_bInitialized;

private:
	//noncopyable
	inprocess_rwlock(const inprocess_rwlock&) throw();
	inprocess_rwlock& operator=(const inprocess_rwlock&) throw();
};

//------------------------------------------------------------------------------
// File Mapping

inline void _os_get_mmap_flags(int iMapType, int& prot) noexcept
{
	prot = PROT_NONE;
	if( iMapType & file_mapping_types::Read )
		prot |= PROT_READ;
	if( iMapType & file_mapping_types::Write )
		prot |= PROT_WRITE;
	if( iMapType & file_mapping_types::Execute )
		prot |= PROT_EXEC;
}
inline void _os_get_shm_flags(int iMapType, int& oflag) noexcept
{
	oflag = O_RDONLY;
	if( iMapType & file_mapping_types::Write )
		oflag = O_RDWR;
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
		int prot;
		_os_get_mmap_flags(iMapType, prot);
		return m_map.Map((int)hd.GetHandle(), uSize, iOffset, prot, MAP_SHARED, NULL);
	}
	void Unmap() noexcept
	{
		m_map.Unmap();
	}

	uintptr GetAddress() const noexcept
	{
		return (uintptr)m_map.GetAddress();
	}
	uintptr GetSize() const noexcept
	{
		return m_map.GetLength();
	}

private:
	_os_mmap m_map;

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
	//   bGlobal is unuseful ( LOCAL = GLOBAL )
	call_result Create(const const_string_s& strName, bool bGlobal, uintptr uSize, int iMapType, bool& bAlreadyExisted) noexcept
	{
		int prot, oflag;
		_os_get_mmap_flags(iMapType, prot);
		_os_get_shm_flags(iMapType, oflag);
		return m_shm.Open(const_array_helper::GetInternalPointer(strName),
						oflag, uSize, true, bAlreadyExisted, 0, prot);
	}
	call_result Open(const const_string_s& strName, bool bGlobal, int64 iOffset, uintptr uSize, int iMapType) noexcept
	{
		int prot, oflag;
		_os_get_mmap_flags(iMapType, prot);
		_os_get_shm_flags(iMapType, oflag);
		bool bAlreadyExisted;
		return m_shm.Open(const_array_helper::GetInternalPointer(strName),
						oflag, uSize, false, bAlreadyExisted, iOffset, prot);
	}
	void Destroy() noexcept
	{
		m_shm.Destroy();
	}

	uintptr GetAddress() const noexcept
	{
		return (uintptr)m_shm.GetAddress();
	}
	uintptr GetSize() const noexcept
	{
		return m_shm.GetLength();
	}

private:
	_os_shm m_shm;

private:
	//noncopyable
	shared_memory(const shared_memory&) noexcept;
	shared_memory& operator=(const shared_memory&) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
