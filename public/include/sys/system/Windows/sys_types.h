/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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

//Environment Variable

template <class TstringS>
inline bool get_environment_variable(const char_s* szName, TstringS& str)
{
	const uintptr c_uChars = 256;
	str.SetLength(c_uChars);  //may throw
	DWORD dwRet = ::GetEnvironmentVariableW(szName, &(str.GetBegin().get_Value()), (DWORD)c_uChars + 1);
	if( dwRet == 0 )
		return false;  //The return value of ::GetLastError() may be ERROR_ENVVAR_NOT_FOUND.
	if( dwRet > (DWORD)c_uChars ) {
		str.SetLength(dwRet - 1);  //may throw
		::GetEnvironmentVariableW(szName, &(str.GetBegin().get_Value()), dwRet);  //no check
	}
	else {
		str.SetLength(dwRet);
	} //end if
	return true;
}

// change, add, delete(szValue==NULL)
inline bool set_environment_variable(const char_s* szName, const char_s* szValue) throw()
{
	BOOL bRet = ::SetEnvironmentVariableW(szName, szValue);
	return bRet ? true : false;  //FALSE, ::GetLastError()
}

////////////////////////////////////////////////////////////////////////////////
