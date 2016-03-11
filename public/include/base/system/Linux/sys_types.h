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

	uintptr GetHandle() const throw()
	{
		return (uintptr)m_fd;
	}
	void Attach(uintptr h) throw()
	{
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
// Synchronization

// helper

class _sync_helper
{
public:
	static char_a* gen_global_name(const char_a* pSrc) throw()
	{
		//global
		DECLARE_LOCAL_STATIC_CONST_STRING(char_a, c_szGlobal, c_uGlobalLength, "/")
		//generate
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
	static void free_global_name(char_a* p) throw()
	{
		crt_free((void*)p);
	}
	//tools
	static char_a* gen_sync_name(const char_a* pSrc, bool bGlobal) throw()
	{
		char_a* psz = (char_a*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(char_a* p, bool bGlobal) throw()
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
