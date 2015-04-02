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
//OS

#if defined(OS_WINDOWS)
	#include "Windows/console_api.h"
#elif defined(OS_LINUX)
	#include "Linux/console_api.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//stdout tools

// stdout_attr_helper

class stdout_attr_helper
{
public:
	BEGIN_NOINLINE
	static stdout_attr& get_attr() throw()
	END_NOINLINE
	{
		static stdout_attr l_stdout_attr;
		return l_stdout_attr;
	}
};

// stdout_attr_restore

class stdout_attr_restore
{
public:
	stdout_attr_restore(stdout_attr& attr) throw() : m_attr(attr)
	{
	}
	~stdout_attr_restore() throw()
	{
		m_attr.Restore();
	}

private:
	stdout_attr&  m_attr;
};

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
