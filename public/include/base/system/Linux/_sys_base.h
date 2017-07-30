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
// Time

// _os_tm_to_system_time

inline void _os_tm_to_system_time(const struct tm* ptm, system_time& tm) throw()
{
	tm.uYear         = (ushort)(ptm->tm_year + 1900);
	tm.uMonth        = (ushort)(ptm->tm_mon + 1);
	tm.uDayOfWeek    = (ushort)(ptm->tm_wday);
	tm.uDay          = (ushort)(ptm->tm_mday);
	tm.uHour         = (ushort)(ptm->tm_hour);
	tm.uMinute       = (ushort)(ptm->tm_min);
	tm.uSecond       = (ushort)(ptm->tm_sec);
	tm.uMilliseconds = 0;
}

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
		if( m_pDir != NULL )
			close_dir();
		if( m_szBuffer != NULL )
			free_buffer();
		m_pDirent = NULL;
		m_bFound = false;
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
		if( check_path_deletable_last_separator(szName) ) {
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
		if( m_pDir == NULL ) {
			m_bFound = false;
			return m_bFound;
		}

		//next entry
		if( !fetch_next_valid_entry() ) {
			m_bFound = false;
			return m_bFound;
		}

		m_bFound = true;
		return m_bFound;
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
		res;
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
		if( uOrgLen == 1 ) {
			m_uPathLen = 1;
			mem_copy(szName, m_uPathLen * sizeof(char_s), m_szBuffer);
			m_szBuffer[m_uPathLen] = 0;
		}
		else {
			m_uPathLen = uOrgLen - 1;
			mem_copy(szName, m_uPathLen * sizeof(char_s), m_szBuffer);
			m_szBuffer[m_uPathLen] = 0;
		}
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
	bool m_bFound;

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

////////////////////////////////////////////////////////////////////////////////
