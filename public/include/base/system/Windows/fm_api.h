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
// path

// get_exe_path_name
//   maximum buffer size: MAX_FULL_PATH characters
inline bool get_exe_path_name(char_s* szBuffer, uintptr uSize, uintptr& uChars) throw()
{
	assert( uSize > 0 && uSize <= limits_base<uint>::Max );
	uChars = 0;
	char_s* p;
	errno_t ret = ::_get_wpgmptr(&p);
	if( ret == 0 ) {
		uintptr uLen = calc_string_length(p);
		if( uLen > uSize - 1 )
			uLen = uSize - 1;
		mem_copy(p, uLen * sizeof(char_s), szBuffer);
		szBuffer[uLen] = 0;
		uChars = uLen;
		return true;
	}
	DWORD dwRet = ::GetModuleFileNameW(NULL, szBuffer, (DWORD)uSize);
	if( dwRet == 0 )
		return false;  //::GetLastError()
	if( dwRet == (DWORD)uSize ) {
		uChars = uSize - 1;
		return false;  //::GetLastError() == ERROR_INSUFFICIENT_BUFFER
	}
	uChars = (uintptr)dwRet;
	return true;
}

// get_full_path_name
//   buffer size: MAX_FULL_PATH
//   szPath: use platform path prefix
//   this function is not thread-safe.
//   this function can process the current directory.
inline bool get_full_path_name(const char_s* szPath, char_s* szBuffer) throw()
{
	char_s* szFilePart = NULL;
	DWORD dwRet = ::GetFullPathNameW(szPath, MAX_FULL_PATH, szBuffer, &szFilePart);
	return dwRet != 0;  //::GetLastError()
}

// get_current_directory
//   buffer size: MAX_FULL_PATH
//   this function is not thread-safe.
inline bool get_current_directory(char_s* szBuffer, uintptr uSize) throw()
{
	assert( uSize > 0 && uSize <= limits_base<uint>::Max );
	DWORD dwRet = ::GetCurrentDirectoryW((DWORD)uSize, szBuffer);
	if( dwRet == 0 )
		return false;  //::GetLastError()
	if( dwRet >= (DWORD)uSize )
		return false;
	return true;
}

// set_current_directory
//   maximum path length : MAX_PATH
//   this function is not thread-safe.
inline bool set_current_directory(const char_s* szPath) throw()
{
	return ::SetCurrentDirectoryW(szPath) != FALSE;  //::GetLastError()
}

// check_file_exists
//   szFile: use platform path prefix
inline bool check_file_exists(const char_s* szFile) throw()
{
	WIN32_FIND_DATAW findFileData;
	HANDLE hFind = ::FindFirstFileW(szFile, &findFileData);
	if( hFind == INVALID_HANDLE_VALUE )
		return false;  //::GetLastError()
	BOOL bRet = ::FindClose(hFind);
	assert( bRet );  //::GetLastError()
	return true;
}

// check_directory_exists
//   szPath: use platform path prefix
inline bool check_directory_exists(const char_s* szPath) throw()
{
	DWORD dwCode = ::GetFileAttributesW(szPath);
	if( dwCode == INVALID_FILE_ATTRIBUTES )
		return false;  //::GetLastError()
	return (FILE_ATTRIBUTE_DIRECTORY & dwCode) ? true : false;  //CO_E_BAD_PATH
}

// create_directory
//   szPath: use platform path prefix
//   this function returns failure if the directory already exists.
inline bool create_directory(const char_s* szPath) throw()
{
	return ::CreateDirectoryW(szPath, NULL) ? true : false;  //::GetLastError(), may be ERROR_ALREADY_EXISTS
}
inline bool create_directory(const char_s* szPath, bool& bExisting) throw()
{
	bExisting = false;
	if( !::CreateDirectoryW(szPath, NULL) ) {
		if( ::GetLastError() != ERROR_ALREADY_EXISTS )
			return false;
		bExisting = true;
	}
	return true;
}

// delete_directory
//   szPath: use platform path prefix
//
//   To recursively delete the files in a directory,
//   use the SHFileOperationW function.
//
inline bool delete_directory(const char_s* szPath) throw()
{
	return ::RemoveDirectoryW(szPath) ? true : false;  //::GetLastError()
}

// delete_file
//   szFile: use platform path prefix
inline bool delete_file(const char_s* szFile) throw()
{
	return ::DeleteFileW(szFile) != FALSE;  //::GetLastError()
}

// rename_file
//   rename directory/file
//   szOldName, szNewName: use platform path prefix
inline bool rename_file(const char_s* szOldName, const char_s* szNewName) throw()
{
	return ::MoveFileW(szOldName, szNewName) != FALSE;  //::GetLastError()
}

//home directory
#define ENVVAR_HOME_DIR  _S("USERPROFILE")

////////////////////////////////////////////////////////////////////////////////
