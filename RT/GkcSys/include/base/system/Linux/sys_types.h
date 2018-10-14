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

//Linux

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
	uintptr GetFunctionAddress(const char_a* szFunc) throw()
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

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
