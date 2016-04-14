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

////////////////////////////////////////////////////////////////////////////////
