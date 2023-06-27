/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
// Thread

// thread_sleep
//  uTimeout: ms
inline void thread_sleep(uint uTimeout) throw()
{
	struct timespec tms, tmd;
	tms.tv_sec = uTimeout / 1000;
	tms.tv_nsec = (uTimeout % 1000) * 1000 * 1000;
	::nanosleep(&tms, &tmd);  //no check
}

// _os_pthread_attr

class _os_pthread_attr
{
public:
	_os_pthread_attr() throw() : m_p(NULL)
	{
	}
	~_os_pthread_attr() throw()
	{
		Destroy();
	}

	void Destroy() throw()
	{
		if ( m_p != NULL ) {
			int ret = ::pthread_attr_destroy(&m_attr);
			assert( ret == 0 );  // or error number
			(void)ret;
			m_p = NULL;
		}
	}

	bool IsNull() const throw()
	{
		 return m_p == NULL;
	}
	const pthread_attr_t* GetAttr() const throw()
	{
		return m_p;
	}
	pthread_attr_t* GetAttr() throw()
	{
		return m_p;
	}

	bool Initialize() throw()
	{
		assert( IsNull() );
		if ( ::pthread_attr_init(&m_attr) == 0 ) // or error number
			m_p = &m_attr;
		return !IsNull();
	}

	bool GetStackSize(size_t& size) const throw()
	{
		assert( !IsNull() );
		return ::pthread_attr_getstacksize(&m_attr, &size) == 0;  // or error number
	}
	bool SetStackSize(size_t size) throw()
	{
		assert( !IsNull() );
		return ::pthread_attr_setstacksize(&m_attr, size) == 0;  // or error number
	}
	bool GetDetachState(int& state) const throw()
	{
		assert( !IsNull() );
		return ::pthread_attr_getdetachstate(&m_attr, &state) == 0;  // or error number
	}
	//state: PTHREAD_CREATE_DETACHED, PTHREAD_CREATE_JOINABLE
	bool SetDetachState(int state) throw()
	{
		assert( !IsNull() );
		return ::pthread_attr_setdetachstate(&m_attr, state) == 0;  // or error number
	}

private:
	_os_pthread_attr(const _os_pthread_attr&) throw();
	_os_pthread_attr& operator=(const _os_pthread_attr&) throw();

private:
	pthread_attr_t  m_attr;
	pthread_attr_t* m_p;
};

// _os_pthread

class _os_pthread
{
public:
	_os_pthread() throw() : m_bValid(false)
	{
	}
	~_os_pthread() throw()
	{
		if ( IsValid() )
			Join();
	}

	bool IsValid() const throw()
	{
		return m_bValid;
	}

	void Join() throw()
	{
		assert( IsValid() );
		void* result;
		int ret = ::pthread_join(m_id, &result);
		assert( ret == 0 );  // or error number
		(void)ret;
		m_bValid = false;
	}

	bool Create(void* (*start_routine)(void*), void* arg, size_t stacksize = 0, bool bDetach = false) throw()
	{
		assert( !IsValid() );
		_os_pthread_attr attr;
		if ( stacksize != 0 || bDetach ) {
			if ( !attr.Initialize() )
				return false;
			if ( stacksize == 0 )
				stacksize = 2 * 1024UL * 1024UL;  //2M
			if ( !attr.SetStackSize(stacksize) )
				return false;
			if ( !attr.SetDetachState(bDetach ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE) )
				return false;
		}
		int ret = ::pthread_create(&m_id, attr.GetAttr(), start_routine, arg);
		if ( ret == 0 )  // or error number
			m_bValid = true;
		return IsValid();
	}

	bool Detach() throw()
	{
		assert( IsValid() );
		return ::pthread_detach(m_id) == 0;
	}

private:
	_os_pthread(const _os_pthread&) throw();
	_os_pthread& operator=(const _os_pthread&) throw();

private:
	bool m_bValid;
	pthread_t m_id;
};

// thread_impl<T>

template <class T>
class NOVTABLE thread_impl
{
private:
	typedef thread_impl<T>  thisClass;

public:
	thread_impl() throw()
	{
	}
	~thread_impl() throw()
	{
	}

	bool IsValid() const throw()
	{
		return m_thread.IsValid();
	}

	void WaitForEnd() throw()
	{
		m_thread.Join();
	}

	bool Create(uint uStackSize = 0) throw()
	{
		return m_thread.Create(&thread_proc, (void*)this, uStackSize);
	}

protected:
	static void* thread_proc(void* pv)
	{
		T* pT = static_cast<T*>((thisClass*)pv);
		pT->DoThreadProc();
		return NULL;
	}

	//overrideable
	void DoThreadProc()
	{
		assert(false);
	}

private:
	_os_pthread  m_thread;
};

////////////////////////////////////////////////////////////////////////////////
