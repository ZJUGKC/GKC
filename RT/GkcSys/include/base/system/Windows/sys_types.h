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
// file

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

//------------------------------------------------------------------------------
// file IO

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
