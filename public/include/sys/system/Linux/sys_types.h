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

//Linux

//Environment Variable
template <class TstringS>
inline bool get_environment_variable(const char_s* szName, TstringS& str)
{
	char_s* p = (char_s*)::getenv(szName);
	if( p == NULL )
		return false;
	uintptr uLen = calc_string_length(p);
	str.SetLength(uLen);  //may throw
	mem_copy(p, uLen * sizeof(char_s), &(str.GetBegin().get_Value()));
	return true;
}

////////////////////////////////////////////////////////////////////////////////
