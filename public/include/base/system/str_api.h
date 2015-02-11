/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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

//------------------------------------------------------------------------------
//tools

// char_lower
inline CharA char_lower(CharA c) throw()
{
	return (CharA)::tolower((int)c);
}
inline CharH char_lower(CharH c) throw()
{
	return (CharH)::towlower((wint_t)c);
}
inline CharL char_lower(CharL c) throw()
{
	return (CharL)::towlower((wint_t)c);
}
// char_upper
inline CharA char_upper(CharA c) throw()
{
	return (CharA)::toupper((int)c);
}
inline CharH char_upper(CharH c) throw()
{
	return (CharH)::towupper((wint_t)c);
}
inline CharL char_upper(CharL c) throw()
{
	return (CharL)::towupper((wint_t)c);
}

//------------------------------------------------------------------------------
//OS

#if defined(OS_WINDOWS)
	#include "Windows/str_api.h"
#elif defined(OS_LINUX)
	#include "Linux/str_api.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//tools

// calc_string_length

inline uintptr calc_string_length(const CharA* s) throw()
{
	return ::strlen(s);
}

// compare_string

inline int compare_string(const CharA* s1, const CharA* s2) throw()
{
	return ::strcmp(s1, s2);
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
