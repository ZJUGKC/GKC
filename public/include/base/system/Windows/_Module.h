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
This file contains classes for Module of Assembly.

Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

#ifndef __MODULE_H__
#define __MODULE_H__

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////		

// _Module

class _Module
{
public:	
	_Module() throw();
	~_Module() throw()
	{
	}

	HINSTANCE GetModuleInstance() const throw()
	{
		return m_hInst;
	}
	HINSTANCE GetResourceInstance() const throw()
	{
		return m_hInstResource;
	}
	//set instance
	HINSTANCE SetResourceInstance(IN HINSTANCE hInst) throw()
	{
		return static_cast< HINSTANCE >(::InterlockedExchangePointer((void**)&m_hInstResource, hInst));
	}

private:
	HINSTANCE m_hInst;
	HINSTANCE m_hInstResource;
};

//global variable
extern _Module g_module;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__MODULE_H__
////////////////////////////////////////////////////////////////////////////////
