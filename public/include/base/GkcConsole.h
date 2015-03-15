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
This file contains console classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_CONSOLE_H__
#define __GKC_CONSOLE_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_CONST_H__
	#error GkcConsole.h requires GkcConst.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// Console

class Console
{
public:
	//output
	static void WriteLN() throw()
	{
		print_string(_S("\n"));
	}
	static void Write(const ConstStringS& str) throw()
	{
		print_string(ConstHelper::GetInternalPointer(str));
	}
	static void WriteLine(const ConstStringS& str) throw()
	{
		print_string(ConstHelper::GetInternalPointer(str));
		WriteLN();
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif  //__GKC_CONSOLE_H__
////////////////////////////////////////////////////////////////////////////////
