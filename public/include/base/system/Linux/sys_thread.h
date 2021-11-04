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
	uint uSecond = uTimeout / 1000;
	uint uRest = uTimeout % 1000;
	//no check
	if( uSecond != 0 )
		::sleep(uSecond);
	::usleep(uRest * 1000);
}

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

	bool Create(void* (*start_routine)(void*), void* arg) throw()
	{
		assert( !IsValid() );
		int ret = ::pthread_create(&m_id, NULL, start_routine, arg);
		if ( ret == 0 )  // or error number
			m_bValid = true;
		return IsValid();
	}

private:
	bool m_bValid;
	pthread_t m_id;
};

// thread_impl<T>

template <class T>
class NO_VTABLE thread_impl
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

	bool Create() throw()
	{
		return m_thread.Create(&thread_proc, (void*)this);
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
