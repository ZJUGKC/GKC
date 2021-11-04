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

//Windows

//------------------------------------------------------------------------------
// Thread

// thread_sleep
//  uTimeout: ms
inline void thread_sleep(uint uTimeout) throw()
{
	::Sleep(uTimeout);
}

// _os_crt_thread

class _os_crt_thread
{
public:
	_os_crt_thread() throw() : m_h(NULL), m_id(0)
	{
	}
	~_os_crt_thread() throw()
	{
		if ( !IsNull() )
			WaitForEnd();
		Close();
	}

	void WaitForEnd() throw()
	{
		assert( !IsNull() );
		//end
		DWORD dwRet = ::WaitForSingleObject(m_h, INFINITE);
		assert( dwRet == WAIT_OBJECT_0 );
		(void)dwRet;
	}

	bool IsNull() const throw()
	{
		return m_h == NULL;
	}
	bool IsRunning() const throw()
	{
		assert( !IsNull() );
		DWORD dwRet = ::WaitForSingleObject(m_h, 0);
		return dwRet != WAIT_OBJECT_0;
	}

	void Close() throw()
	{
		if ( m_h != NULL ) {
			//close
			BOOL bRet = ::CloseHandle(m_h);
			assert( bRet );
			(void)bRet;
			m_h = NULL;
		}
		m_id = 0;
	}

	bool Create(unsigned (__stdcall *start_address)(void*), void* arglist) throw()
	{
		assert( IsNull() );
		m_h = (HANDLE)::_beginthreadex(NULL, 0, start_address, arglist, 0, &m_id);
		return m_h != NULL;	 // NULL, errno and _doserrno
	}

	bool GetPriority() const throw()
	{
		assert( !IsNull() );
		int ret = ::GetThreadPriority(m_h);
		return ret != THREAD_PRIORITY_ERROR_RETURN;  // THREAD_PRIORITY_ERROR_RETURN, ::GetLastError()
	}
	bool SetPriority(int iPriority) throw()
	{
		assert( !IsNull() );
		BOOL bRet = ::SetThreadPriority(m_h, iPriority);
		return bRet;  // FALSE, ::GetLastError()
	}
	bool Suspend() throw()
	{
		assert( !IsNull() );
		DWORD dwRet = ::SuspendThread(m_h);
		return dwRet != (DWORD)-1;  // -1, ::GetLastError()
	}
	bool Resume() throw()
	{
		assert( !IsNull() );
		DWORD dwRet = ::ResumeThread(m_h);
		return dwRet != (DWORD)-1;  // -1, ::GetLastError()
	}

private:
	HANDLE m_h;
	DWORD  m_id;
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
		return !m_thread.IsNull();
	}

	void WaitForEnd() throw()
	{
		m_thread.WaitForEnd();
	}

	bool Create() throw()
	{
		return m_thread.Create(&thread_proc, (void*)this);
	}

protected:
	static unsigned __stdcall thread_proc(void* pv)
	{
		T* pT = static_cast<T*>((thisClass*)pv);
		pT->DoThreadProc();
		return 0;
	}

	//overrideable
	void DoThreadProc()
	{
		assert(false);
	}

private:
	_os_crt_thread  m_thread;
};

////////////////////////////////////////////////////////////////////////////////
