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
	io_handle(io_handle&& src) throw()
	{
		m_fd = src.m_fd;
		src.m_fd = -1;
	}
	~io_handle() throw()
	{
		Close();
	}

	io_handle& operator=(io_handle&& src) throw()
	{
		if( &src != this ) {
			if( src.m_fd != m_fd ) {
				Close();
				m_fd = src.m_fd;
				src.m_fd = -1;
			}
		}
		return *this;
	}

	bool IsValid() const throw()
	{
		return m_fd >= 0;
	}

	//close
	void Close() throw()
	{
		if( IsValid() ) {
#ifdef DEBUG
			int res =
#endif
			::close(m_fd);
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
		if( uRet < 0 )
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
		if( uRet < 0 )
			res = _OS_CR_FROM_ERRORNO();
		else
			uWritten = (uint)uRet;
		return call_result(res);
	}

protected:
	int m_fd;

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
		int fd = ::open(szFile, flags, S_IRWXU | S_IRWXG | S_IRWXO);
		if( fd < 0 )
			res = _OS_CR_FROM_ERRORNO();
		else
			hd.m_fd = fd;

		return call_result(res);
	}
	//seek
	//  uMethod: IO_SEEK_*
	//  If uMethod = IO_SEEK_BEGIN, iOffset is regarded as an unsigned value.
	static call_result Seek(io_handle& hd, int64 iOffset, int64& iNewPos, uint uMethod) throw()
	{
		assert( hd.IsValid() );
		assert( sizeof(off_t) == 8 );
		off_t npos = ::lseek(hd.m_fd, (off_t)iOffset, (int)uMethod);  //64 bits
		int res = 0;
		if( npos < 0 )
			res = _OS_CR_FROM_ERRORNO();
		else
			iNewPos = npos;
		return call_result(res);
	}
	//flush
	static void Flush(io_handle& hd) throw()
	{
		assert( hd.IsValid() );
#ifdef DEBUG
		int res =
#endif
		::fsync(hd.m_fd);
		assert( res == 0 );
	}
	//set size
	static call_result SetSize(io_handle& hd, int64 iSize) throw()
	{
		assert( hd.IsValid() );
		int res = 0;
		if( ::ftruncate(hd.m_fd, iSize) < 0 )
			res = _OS_CR_FROM_ERRORNO();
		return call_result(res);
	}
	//get status
	static call_result GetStatus(io_handle& hd, file_status& status) throw()
	{
		assert( hd.IsValid() );

		int res = 0;
		//stat
		struct stat st;
		if( ::fstat(hd.m_fd, &st) < 0 ) {
			res = _OS_CR_FROM_ERRORNO();
			return call_result(res);
		}
		//size
		status.iSize = st.st_size;  //64 bits

		struct tm tmz;
		struct tm* ptm;
		//access
		ptm = ::gmtime_r(&st.st_atime, &tmz);
		if( ptm == NULL ) {
			res = CR_FAIL;
			return call_result(res);
		}
		_os_tm_to_system_time(&tmz, status.tmAccess);
		status.tmAccess.uMilliseconds = (ushort)(st.st_atim.tv_nsec / 1000000);
		//modify
		ptm = ::gmtime_r(&st.st_mtime, &tmz);
		if( ptm == NULL ) {
			res = CR_FAIL;
			return call_result(res);
		}
		_os_tm_to_system_time(&tmz, status.tmModify);
		status.tmModify.uMilliseconds = (ushort)(st.st_mtim.tv_nsec / 1000000);
		//create
		ptm = ::gmtime_r(&st.st_ctime, &tmz);
		if( ptm == NULL ) {
			res = CR_FAIL;
			return call_result(res);
		}
		_os_tm_to_system_time(&tmz, status.tmCreate);
		status.tmCreate.uMilliseconds = (ushort)(st.st_ctim.tv_nsec / 1000000);

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
		if( ::fcntl(hd.m_fd, bBlock ? F_SETLKW : F_SETLK, &fl) < 0 )
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
#ifdef DEBUG
		int rv =
#endif
		::fcntl(hd.m_fd, F_SETLK, &fl);  //nonblocking
		assert( rv >= 0 );
	}
};

//------------------------------------------------------------------------------
// Synchronization

// helper

class _sync_helper
{
public:
	static char_a* gen_global_name(const char_a* pSrc) throw()
	{
		//global
		static const char_a c_szGlobal[] = "/";
		static const uintptr c_uGlobalLength = sizeof(c_szGlobal) / sizeof(char_a) - 1;
		uintptr uCount = calc_string_length(pSrc);
		uintptr uNewCount;
		call_result cr = safe_operators::Add(uCount, c_uGlobalLength, uNewCount);
		if( cr.IsFailed() )
			return NULL;
		char_a* pNew = (char_a*)crt_alloc((uNewCount + 1) * sizeof(char_a));
		if( pNew == NULL )
			return NULL;
		//copy
		mem_copy(c_szGlobal, c_uGlobalLength * sizeof(char_a), pNew);
		mem_copy(pSrc, uCount * sizeof(char_a), pNew + c_uGlobalLength);
		pNew[uNewCount] = 0;
		return pNew;
	}
	static void free_global_name(const char_a* p) throw()
	{
		crt_free(p);
	}
	//tools
	static char_a* gen_sync_name(const char_a* pSrc, bool bGlobal) throw()
	{
		char_a* psz = (char_a*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(const char_a* p, bool bGlobal) throw()
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
#ifdef DEBUG
		int res =
#endif
		::sem_post(&m_sem);
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
		if( res < 0 ) {
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
#ifdef DEBUG
			int res =
#endif
			::sem_destroy(&m_sem);
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
	interprocess_semaphore() throw() : m_psem(NULL), m_szName(NULL)
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
#ifdef DEBUG
		int res =
#endif
		::sem_post(m_psem);
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
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal, int iCount) throw()
	{
		assert( !str.IsNull() );
		assert( m_psem == NULL && m_szName == NULL );
		//name
		char_a* psz = _sync_helper::gen_sync_name(ref_ptr_helper::GetInternalPointer(str), true);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		int res = 0;
		sem_t* psem = ::sem_open(psz, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | (bGlobal ? S_IRWXO : 0), (unsigned int)iCount);
		if( psem == SEM_FAILED ) {
			res = _OS_CR_FROM_ERRORNO();
			_sync_helper::free_sync_name(psz, true);
		}
		else {
			m_psem   = psem;
			m_szName = psz;
		}
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
		}
		return call_result(res);
	}
	void Term() throw()
	{
		if( m_psem != NULL ) {
#ifdef DEBUG
			int res =
#endif
			::sem_close(m_psem);
			assert( res == 0 );
			m_psem = NULL;
			assert( m_szName != NULL );
			::sem_unlink(m_szName);  //no check, the return value may not be equal to zero.
			_sync_helper::free_sync_name(m_szName, true);
			m_szName = NULL;
		}
	}

private:
	sem_t*   m_psem;
	char_a*  m_szName;

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
	call_result Create(const ref_ptr<char_s>& str, bool bGlobal) throw()
	{
		return m_sem.Create(str, bGlobal, 1);
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
#ifdef DEBUG
			int res =
#endif
			::pthread_cond_destroy(&m_cv);
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
#ifdef DEBUG
		int res =
#endif
		::pthread_rwlock_rdlock(&m_rw);
		assert( res == 0 );
	}
	void LockExclusive() throw()
	{
		assert( m_bInitialized );
#ifdef DEBUG
		int res =
#endif
		::pthread_rwlock_wrlock(&m_rw);
		assert( res == 0 );
	}
	void UnlockShared() throw()
	{
		assert( m_bInitialized );
#ifdef DEBUG
		int res =
#endif
		::pthread_rwlock_unlock(&m_rw);
		assert( res == 0 );
	}
	void UnlockExclusive() throw()
	{
		assert( m_bInitialized );
#ifdef DEBUG
		int res =
#endif
		::pthread_rwlock_unlock(&m_rw);
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
#ifdef DEBUG
		int res =
#endif
		::pthread_rwlock_init(&m_rw, NULL);
		assert( res == 0 );
		m_bInitialized = true;
	}
	void Term() throw()
	{
		if( m_bInitialized ) {
#ifdef DEBUG
			int res =
#endif
			::pthread_rwlock_destroy(&m_rw);
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
// Thread

// thread_sleep
//  uTimeout: ms
inline void thread_sleep(uint uTimeout) throw()
{
	uint uSecond = uTimeout / 1000;
	uint uRest = uTimeout % 1000;
	//no check
	if( uSecond != 0 )
		::sleep(uSecond);
	::usleep(uRest * 1000);
}

////////////////////////////////////////////////////////////////////////////////
