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
// path

// get_exe_path_name
//   maximum buffer size: MAX_FULL_PATH characters
inline bool get_exe_path_name(char_s* szBuffer, uintptr uSize, uintptr& uChars) throw()
{
	assert( uSize > 0 );
	uChars = 0;
	ssize_t ret = ::readlink("/proc/self/exe", szBuffer, uSize - 1);
	if( ret == -1 )
		return false;  //errno
	szBuffer[ret] = 0;
	uChars = ret;
	return true;
}

// get_full_path_name
//   buffer size: MAX_FULL_PATH
//   szPath: use platform path prefix
//   this function can process the current directory.
inline bool get_full_path_name(const char_s* szPath, char_s* szBuffer) throw()
{
	char_s* p = (char_s*)::realpath(szPath, szBuffer);
	return p != NULL;  //errno
}

// get_current_directory
//   buffer size: MAX_FULL_PATH
inline bool get_current_directory(char_s* szBuffer, uintptr uSize) throw()
{
	assert( uSize > 0 );
	char_s* p = (char_s*)::getcwd(szBuffer, uSize);
	return p != NULL;  //errno
}

// set_current_directory
inline bool set_current_directory(const char_s* szPath) throw()
{
	return ::chdir(szPath) == 0;  //-1, errno
}

// check_file_exists
inline bool check_file_exists(const char_s* szFile) throw()
{
	return ::access(szFile, F_OK) == 0;  //-1, errno
}

// check_directory_exists
inline bool check_directory_exists(const char_s* szPath) throw()
{
	//stat
	struct stat st;
	if( ::stat(szPath, &st) == -1 )
		return false;  //errno
	return S_ISDIR(st.st_mode) ? true : false;
}

// create_directory
//   this function returns failure if the directory/file already exists.
inline bool create_directory(const char_s* szPath) throw()
{
	return ::mkdir(szPath, S_IRWXU | S_IRWXG | S_IRWXO) == 0;  //-1, errno, may be EEXIST
}
inline bool create_directory(const char_s* szPath, bool& bExisting) throw()
{
	bExisting = false;
	if( ::mkdir(szPath, S_IRWXU | S_IRWXG | S_IRWXO) == -1 ) { //0
		if( errno != EEXIST )
			return false;
		bExisting = true;
	}
	return true;
}

// delete_directory
inline bool delete_directory(const char_s* szPath) throw()
{
	return ::rmdir(szPath) == 0;  //-1, errno
}

// delete_file
inline bool delete_file(const char_s* szFile) throw()
{
	return ::unlink(szFile) == 0;  //-1, errno
}

// rename_file
//   rename directory/file
inline bool rename_file(const char_s* szOldName, const char_s* szNewName) throw()
{
	return ::rename(szOldName, szNewName) == 0;  //-1, errno
}

//home directory
#define ENVVAR_HOME_DIR  _S("HOME")

////////////////////////////////////////////////////////////////////////////////
