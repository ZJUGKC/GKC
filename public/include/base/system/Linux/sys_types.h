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

//Linux

//------------------------------------------------------------------------------
// Synchronization

// helper

class sync_helper
{
public:
	static CharA* gen_global_name(const CharA* pSrc) throw()
	{
		//global
		static const CharA c_szGlobal[] = "/";
		static const uintptr c_uGlobalLength = sizeof(c_szGlobal) / sizeof(CharA) - 1;
		uintptr uCount = calc_string_length(pSrc);
		uintptr uNewCount;
		call_result cr = GKC::SafeOperators::Add(uCount, c_uGlobalLength, uNewCount);
		if( cr.IsFailed() )
			return NULL;
		CharA* pNew = (CharA*)crt_alloc((uNewCount + 1) * sizeof(CharA));
		if( pNew == NULL )
			return NULL;
		//copy
		mem_copy(c_szGlobal, c_uGlobalLength * sizeof(CharA), pNew);
		mem_copy(pSrc, uCount * sizeof(CharA), pNew + c_uGlobalLength);
		pNew[uNewCount] = 0;
		return pNew;
	}
	static void free_global_name(const CharA* p) throw()
	{
		crt_free((uintptr)p);
	}
	//tools
	static CharA* gen_sync_name(const CharA* pSrc, bool bGlobal) throw()
	{
		CharA* psz = (CharA*)pSrc;
		if( bGlobal )
			psz = gen_global_name(psz);
		return psz;
	}
	static void free_sync_name(const CharA* p, bool bGlobal) throw()
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
			throw GKC::Exception(call_result(CR_FAIL));
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
			res = CR_FROM_ERRORNO();
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
			throw GKC::Exception(call_result(CR_FAIL));
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
	call_result Create(const GKC::RefPtr<CharS>& str, bool bGlobal, int iCount) throw()
	{
		assert( !str.IsNull() );
		assert( m_psem == NULL && m_szName == NULL );
		//name
		CharA* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), true);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//create
		int res = 0;
		sem_t* psem = ::sem_open(psz, O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG | (bGlobal ? S_IRWXO : 0), (unsigned int)iCount);
		if( psem == SEM_FAILED ) {
			res = CR_FROM_ERRORNO();
			sync_helper::free_sync_name(psz, true);
		}
		else {
			m_psem   = psem;
			m_szName = psz;
		}
		return call_result(res);
	}
	call_result Open(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
	{
		assert( !str.IsNull() );
		assert( m_psem == NULL && m_szName == NULL );
		//name
		CharA* psz = sync_helper::gen_sync_name(GKC::RefPtrHelper::GetInternalPointer(str), true);
		if( psz == NULL )
			return call_result(CR_OUTOFMEMORY);
		//open
		int res = 0;
		sem_t* psem = ::sem_open(psz, O_RDWR);
		if( psem == SEM_FAILED ) {
			res = CR_FROM_ERRORNO();
			sync_helper::free_sync_name(psz, true);
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
			sync_helper::free_sync_name(m_szName, true);
			m_szName = NULL;
		}
	}

private:
	sem_t*  m_psem;
	CharA*  m_szName;

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
	call_result Create(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
	{
		return m_sem.Create(str, bGlobal, 1);
	}
	call_result Open(const GKC::RefPtr<CharS>& str, bool bGlobal) throw()
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
		::gettimeofday(&now);  //no check
		uint uSecond = uTimeout / 1000;
		uint uRest = uTimeout % 1000;
		timespec to;
		to.tv_sec = now.tv_sec + uSecond;
		to.tv_nsec = (now.tv_usec + uRest * 1000) * 1000;
		return ::pthread_cond_timedwait(&m_cv, &mtx.m_mtx, &to) == 0;
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
	inprocess_condition(const inprocess_condition&) throw();
	inprocess_condition& operator=(const inprocess_condition&) throw();
};

////////////////////////////////////////////////////////////////////////////////
