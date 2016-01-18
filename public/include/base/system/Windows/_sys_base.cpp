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

/*
This file contains classes for global variables of _os_module.

Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////

extern "C" IMAGE_DOS_HEADER __ImageBase;

////////////////////////////////////////////////////////////////////////////////

//_os_module

_os_module::_os_module() throw()
{
	m_hInst = m_hInstResource = reinterpret_cast<HINSTANCE>(&__ImageBase);
}

//global
_os_module _os_g_module;

////////////////////////////////////////////////////////////////////////////////
