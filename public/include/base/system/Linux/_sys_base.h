/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
//internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Synchronization

// helper

class _sync_helper
{
public:
	static char_a* gen_global_name(const char_a* pSrc) throw()
	{
		//global
		DECLARE_LOCAL_CONST_STRING(char_a, c_szGlobal, c_uGlobalLength, "/")
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

//------------------------------------------------------------------------------
// File System

class _os_file_searcher
{
public:
	_os_file_searcher() throw() : m_bFound(false), m_pDir(NULL), m_pDirent(NULL),
								m_szBuffer(NULL), m_uPathLen(0)
	{
	}
	~_os_file_searcher() throw()
	{
		Close();
	}

	void Close() throw()
	{
		if( m_bFound ) {
			if( m_pDir != NULL )
				close_dir();
			if( m_szBuffer != NULL )
				free_buffer();
			m_pDirent = NULL;
			m_bFound = false;
		}
	}

	bool IsValid() const throw()
	{
		return m_bFound;
	}

// Operations

	//szName: It can be a file name or a directory without unnecessary trailing separator,
	//        or a directory with format '.../*'.
	bool Find(const char_s* szName = NULL) throw()
	{
		assert( !IsValid() );

		if( szName == NULL )
			szName = _S("./*");
		//check format
		uintptr uOrgLen = calc_string_length(szName);
		if( uOrgLen == 0
			|| (uOrgLen == 1 && szName[uOrgLen - 1] == _S('*')) ) {
			szName = _S("./*");
			uOrgLen = 3;
		}

		//in directory
		if( szName[uOrgLen - 1] == _S('*') ) {
			if( !check_path_separator(szName[uOrgLen - 2]) )
				return false;
			if( !allocate_path_buffer(szName, uOrgLen - 1) )
				return false;
			m_pDir = ::opendir(m_szBuffer);
			if( m_pDir == NULL ) { //errno
				free_buffer();
				return false;
			}
			if( uOrgLen != 2 ) {
				char_s ch;
				get_path_separator(ch);
				m_szBuffer[uOrgLen - 2] = ch;
				m_szBuffer[uOrgLen - 1] = 0;
				m_uPathLen ++;
			}
			//first entry
			if( !fetch_next_valid_entry() ) {
				close_dir();
				free_buffer();
				return false;
			}
			m_bFound = true;
			return m_bFound;
		}

		//directory
		if( check_path_deletable_last_separator(szName, uOrgLen) ) {
			m_szBuffer = (char_s*)crt_alloc(uOrgLen * sizeof(char_s));
			if( m_szBuffer == NULL )
				return false;
			mem_copy(szName, (uOrgLen - 1) * sizeof(char_s), m_szBuffer);
			m_szBuffer[uOrgLen - 1] = 0;
			if( !fetch_file_info(m_szBuffer) ) {
				free_buffer();
				return false;
			}
			m_bFound = true;
			return m_bFound;
		}

		//file or directory
		m_bFound = fetch_file_info(szName);
		return m_bFound;
	}
	bool FindNext() throw()
	{
		assert( IsValid() );

		//file or directory
		if( m_pDir == NULL )
			return false;

		//next entry
		if( !fetch_next_valid_entry() )
			return false;

		return true;
	}

// Attributes

	int64 GetFileSize() const throw()
	{
		assert( IsValid() );
		return m_statbuf.st_size;
	}
	const char_s* GetFileName() const throw()
	{
		assert( IsValid() );
		return m_pDirent == NULL ? _S("") : m_pDirent->d_name;
	}
	const struct timespec& GetCreationTime() const throw()
	{
		assert( IsValid() );
		return m_statbuf.st_ctim;
	}
	const struct timespec& GetAccessTime() const throw()
	{
		assert( IsValid() );
		return m_statbuf.st_atim;
	}
	const struct timespec& GetModifyTime() const throw()
	{
		assert( IsValid() );
		return m_statbuf.st_mtim;
	}
	// Return true if the file name is "." or "..".
	// It must be called after checking whether the file is a directory.
	bool IsDots() const throw()
	{
		const char_s* sz = GetFileName();
		return sz[0] == _S('.') && (sz[1] == _S('\0') || (sz[1] == _S('.') && sz[2] == _S('\0')));
	}
	bool IsDirectory() const throw()
	{
		return matches_mask(S_IFDIR);
	}
	bool IsRegular() const throw()
	{
		return matches_mask(S_IFREG);
	}
	bool IsBlockDev() const throw()
	{
		return matches_mask(S_IFBLK);
	}
	bool IsCharDev() const throw()
	{
		return matches_mask(S_IFCHR);
	}
	bool IsFIFO() const throw()
	{
		return matches_mask(S_IFIFO);
	}
	bool IsLink() const throw()
	{
		return matches_mask(S_IFLNK);
	}
	bool IsSocket() const throw()
	{
		return matches_mask(S_IFSOCK);
	}

private:
	void close_dir() throw()
	{
		int res = ::closedir(m_pDir);
		(void)res;
		assert( res == 0 );  //-1, errno
		m_pDir = NULL;
	}
	void free_buffer() throw()
	{
		crt_free(m_szBuffer);
		m_szBuffer = NULL;
		m_uPathLen = 0;
	}
	//allocate path buffer
	bool allocate_path_buffer(const char_s* szName, uintptr uOrgLen) throw()
	{
		//assume 256
		m_uNameLen = 255 + 1;
		uintptr uLen, uBytes;
		call_result cr;
		cr = safe_operators::Add(uOrgLen, m_uNameLen, uLen);
		if( cr.IsFailed() )
			return false;
		cr = safe_operators::Multiply(uLen, (uintptr)sizeof(char_s), uBytes);
		if( cr.IsFailed() )
			return false;
		m_szBuffer = (char_s*)crt_alloc(uBytes);
		if( m_szBuffer == NULL )
			return false;
		if( uOrgLen == 1 )
			m_uPathLen = 1;
		else
			m_uPathLen = uOrgLen - 1;
		mem_copy(szName, m_uPathLen * sizeof(char_s), m_szBuffer);
		m_szBuffer[m_uPathLen] = 0;
		//actual name max
		{
			long name_max =::pathconf(m_szBuffer, _PC_NAME_MAX);
			if( name_max == -1 )
				name_max = 255;
			uintptr uMax = (uintptr)name_max + 1;  //no check
			if( uMax > m_uNameLen ) {
				cr = safe_operators::Add(uMax - m_uNameLen, uLen, uLen);
				if( cr.IsFailed() ) {
					free_buffer();
					return false;
				}
				cr = safe_operators::Multiply(uLen, (uintptr)sizeof(char_s), uBytes);
				if( cr.IsFailed() ) {
					free_buffer();
					return false;
				}
				void* p = crt_realloc(m_szBuffer, uBytes);
				if( p == NULL ) {
					free_buffer();
					return false;
				}
				m_szBuffer = (char_s*)p;
			}
			m_uNameLen = uMax;
		} //end block
		return true;
	}

	//fetch file information
	bool fetch_file_info(const char_s* szFile) throw()
	{
		return ::lstat(szFile, &m_statbuf) == 0;  //-1, errno
	}
	//fetch a file
	bool fetch_file() throw()
	{
		m_pDirent = ::readdir(m_pDir);
		return m_pDirent != NULL;  //NULL, end (errno is unchanged) or error (errno is set)
	}
	//fetch the next valid entry
	bool fetch_next_valid_entry() throw()
	{
		bool bRet = false;
		for( ; ; ) {
			if( !fetch_file() )
				break;
			mem_copy(m_pDirent->d_name, m_uNameLen * sizeof(char_s), m_szBuffer + m_uPathLen);
			if( !fetch_file_info(m_szBuffer) )
				continue;
			bRet = true;
			break;
		} //end for
		return bRet;
	}

	//mask
	bool matches_mask(mode_t mask) const throw()
	{
		assert( IsValid() );
		return (m_statbuf.st_mode & S_IFMT) == mask;
	}

private:
	bool m_bFound;  //is a flag for valid state

	//directory
	DIR* m_pDir;
	struct dirent* m_pDirent;
	//internal buffer
	struct stat m_statbuf;
	char_s* m_szBuffer;
	uintptr m_uPathLen;
	uintptr m_uNameLen;

private:
	//noncopyable
	_os_file_searcher(const _os_file_searcher&) throw();
	_os_file_searcher& operator=(const _os_file_searcher&) throw();
};

// _os_mmap

class _os_mmap
{
public:
	_os_mmap() throw() : m_pAddress(NULL), m_length(0)
	{
	}
	~_os_mmap() throw()
	{
		Unmap();
	}

	void Unmap() throw()
	{
		if( m_pAddress != NULL ) {
			int res = ::munmap(m_pAddress, m_length);
			(void)res;
			assert( res == 0 );  //-1, errno
			m_pAddress = NULL;
		}
	}

	//offset : must be a multiple of the page size as returned by ::sysconf(_SC_PAGE_SIZE).
	//prot (or operator) : PROT_EXEC, PROT_READ, PROT_WRITE, PROT_NONE
	//flags : MAP_SHARED, MAP_PRIVATE, ...
	call_result Map(int fd, size_t length,
					int64 offset = 0, int prot = PROT_READ, int flags = MAP_SHARED,
					void* pBase = NULL) throw()
	{
		assert( m_pAddress == NULL );
		assert( offset >= 0 );
		void* p = ::mmap(pBase, length, prot, flags, fd, offset);
		if( p == MAP_FAILED )
			return call_result(_OS_CR_FROM_ERRORNO());
		m_pAddress = p;
		m_length = length;
		return call_result();
	}

	//flags : MS_ASYNC, MS_SYNC (or operator : MS_INVALIDATE)
	call_result Flush(int flags = MS_SYNC) throw()
	{
		assert( m_pAddress != NULL );
		int res = ::msync(m_pAddress, m_length, flags);
		if( res == -1 )
			return call_result(_OS_CR_FROM_ERRORNO());
		return call_result();
	}

	const void* GetAddress() const throw()
	{
		return m_pAddress;
	}
	void* GetAddress() throw()
	{
		return m_pAddress;
	}
	size_t GetLength() const throw()
	{
		return m_length;
	}

private:
	void*   m_pAddress;
	size_t  m_length;

private:
	//noncopyable
	_os_mmap(const _os_mmap&) throw();
	_os_mmap& operator=(const _os_mmap&) throw();
};

// _os_shm

class _os_shm
{
public:
	_os_shm() throw() : m_name(NULL)
	{
	}
	~_os_shm() throw()
	{
		Destroy();
	}

	void Destroy() throw()
	{
		if( m_name != NULL ) {
			m_map.Unmap();
			int res = ::shm_unlink(m_name);
			(void)res;
			assert( res == 0 );  //-1, errno
			_sync_helper::free_global_name(m_name);
			m_name = NULL;
		}
	}

	//szName : XXXXX -> /XXXXX
	//oflag : O_RDONLY, O_RDWR (or operator : O_CREAT, O_EXCL, O_TRUNC, do not use them)
	call_result Open(const char* szName, int oflag, size_t length,
					bool bCreate, bool& bAlreadyExisted,
					int64 offset = 0, int prot = PROT_READ | PROT_WRITE) throw()
	{
		assert( m_name == NULL );
		assert( length > 0 && length <= limits_base<int64>::Max );

		char* pName = _sync_helper::gen_global_name(szName);
		if( pName == NULL )
			return call_result(CR_OUTOFMEMORY);

		bAlreadyExisted = false;
		int act_flag = oflag;
		if( bCreate )
			act_flag |= (O_CREAT | O_EXCL);

		call_result cr;

		//shm
		int fd = ::shm_open(pName, act_flag, S_IRWXU | S_IRWXG | S_IRWXO);
		if( fd == -1 ) {
			cr.SetResult(_OS_CR_FROM_ERRORNO());
			if( bCreate && errno == EEXIST )
				fd = ::shm_open(pName, oflag, S_IRWXU | S_IRWXG | S_IRWXO);
				if( fd == -1 ) {
					cr.SetResult(_OS_CR_FROM_ERRORNO());
					_sync_helper::free_global_name(pName);
					return cr;
				}
				bAlreadyExisted = true;
			}
			else {
				_sync_helper::free_global_name(pName);
				return cr;
			}
		}

		//size
		if( ::ftruncate(fd, (int64)length) == -1 ) {
			cr.SetResult(_OS_CR_FROM_ERRORNO());
			::close(fd);
			_sync_helper::free_global_name(pName);
			return cr;
		}

		//map
		cr = m_map.Map(fd, length, offset, prot, MAP_SHARED, NULL);
		if( cr.IsFailed() ) {
			::close(fd);
			_sync_helper::free_global_name(pName);
			return cr;
		}

		::close(fd);
		m_name = pName;
		return cr;
	}

	const void* GetAddress() const throw()
	{
		return m_map.GetAddress();
	}
	void* GetAddress() throw()
	{
		return m_map.GetAddress();
	}
	size_t GetLength() const throw()
	{
		return m_map.GetLength();
	}

private:
	char* m_name;
	_os_mmap m_map;

private:
	//noncopyable
	_os_shm(const _os_shm&) throw();
	_os_shm& operator=(const _os_shm&) throw();
};

////////////////////////////////////////////////////////////////////////////////
