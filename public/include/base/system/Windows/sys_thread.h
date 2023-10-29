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

	//initflag : 0, CREATE_SUSPENDED, STACK_SIZE_PARAM_IS_A_RESERVATION
	bool Create(unsigned (__stdcall *start_address)(void*), void* arglist, unsigned stack_size = 0, unsigned initflag = 0) throw()
	{
		assert( IsNull() );
		// _beginthreadex calls CreateThread which will set the last error value before it returns.
		m_h = (HANDLE)::_beginthreadex(NULL, stack_size, start_address, arglist, initflag, (unsigned int*)(void*)(&m_id));
		return m_h != NULL;  // NULL, errno and _doserrno
	}

	/*! \brief Retrieves the priority value for the specified thread.

	Retrieves the priority value for the specified thread.
	\return If the function succeeds, the return value is the thread's priority level.
	        If the function fails, the return value is THREAD_PRIORITY_ERROR_RETURN.
	*/
	int GetPriority() const throw()
	{
		assert( !IsNull() );
		int ret = ::GetThreadPriority(m_h);
		return ret;
		//bool bRet = ret != THREAD_PRIORITY_ERROR_RETURN;
		//THREAD_PRIORITY_ERROR_RETURN, ::GetLastError()
	}
	/*! \brief Sets the priority value for the specified thread.

	Sets the priority value for the specified thread.
	\param iPriority [in] The priority value for the thread.
	\return true for succeeded, false for otherwise.
	*/
	bool SetPriority(int iPriority) throw()
	{
		assert( !IsNull() );
		BOOL bRet = ::SetThreadPriority(m_h, iPriority);
		return bRet;  // FALSE, ::GetLastError()
	}
	/*! \brief Suspends the specified thread.

	Suspends the specified thread.
	\return If the function succeeds, the return value is the thread's previous suspend count, otherwise, it is (DWORD)-1.
	*/
	DWORD Suspend() throw()
	{
		assert( !IsNull() );
		DWORD dwRet = ::SuspendThread(m_h);
		return dwRet;
		//bool bRet = dwRet != (DWORD)-1;
		//-1, ::GetLastError()
	}
	/*! \brief Decrements a thread's suspend count.

	Decrements a thread's suspend count. When the suspend count is decremented to zero, the execution of the thread is resumed.
	\return If the function succeeds, the return value is the thread's previous suspend count.
	        If the function fails, the return value is (DWORD)-1.
	*/
	DWORD Resume() throw()
	{
		assert( !IsNull() );
		DWORD dwRet = ::ResumeThread(m_h);
		return dwRet;
		//bool bRet = dwRet != (DWORD)-1;
		//-1, ::GetLastError()
	}

private:
	_os_crt_thread(const _os_crt_thread&) throw();
	_os_crt_thread& operator=(const _os_crt_thread&) throw();

private:
	HANDLE m_h;
	DWORD  m_id;
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
		return !m_thread.IsNull();
	}

	void WaitForEnd() throw()
	{
		m_thread.WaitForEnd();
		m_thread.Close();
	}

	bool Create(uint uStackSize = 0) throw()
	{
		return m_thread.Create(&thread_proc, (void*)this, uStackSize);
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
