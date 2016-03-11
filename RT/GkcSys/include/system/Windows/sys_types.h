/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
// file IO

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
	static void Flush(io_handle& hd) throw()
	{
		assert( hd.IsValid() );
		BOOL bRet = ::FlushFileBuffers((HANDLE)(hd.GetHandle()));
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

		FILETIME   ftA, ftM, ftC;
		SYSTEMTIME st;
		if( !::GetFileTime((HANDLE)(hd.GetHandle()), &ftC, &ftA, &ftM) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		//access
		if( !::FileTimeToSystemTime(&ftA, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmAccess);
		//modify
		if( !::FileTimeToSystemTime(&ftM, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmModify);
		//create
		if( !::FileTimeToSystemTime(&ftC, &st) ) {
			hRes = HRESULT_FROM_WIN32(::GetLastError());
			return call_result((int)hRes);
		}
		_os_cvt_system_time(&st, status.tmCreate);

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
// Shared Assembly

// sa_handle

class sa_handle
{
public:
	sa_handle() throw() : m_hd(NULL)
	{
	}
	sa_handle(sa_handle&& src) throw()
	{
		m_hd = src.m_hd;
		src.m_hd = NULL;
	}
	~sa_handle() throw()
	{
		Free();
	}

	sa_handle& operator=(sa_handle&& src) throw()
	{
		if( &src != this ) {
			if( m_hd != src.m_hd ) {
				Free();
				m_hd = src.m_hd;
				src.m_hd = NULL;
			}
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
		if( (uintptr)m_hd != hd )
			Free();
		m_hd = (HMODULE)hd;
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
			BOOL bRet = ::FreeLibrary(m_hd);
			assert( bRet );
			m_hd = NULL;
		}
	}

	bool Load(const char_s* szFile) throw()
	{
		assert( m_hd == NULL );
		m_hd = ::LoadLibraryW(szFile);
		return m_hd != NULL;
	}
	uintptr GetFunctionAddress(const char_a* szFunc) throw()
	{
		assert( m_hd != NULL );
		return (uintptr)::GetProcAddress(m_hd, szFunc);
	}

private:
	HMODULE m_hd;

private:
	//noncopyable
	sa_handle(const sa_handle& src) throw();
	sa_handle& operator=(const sa_handle& src) throw();
};

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
