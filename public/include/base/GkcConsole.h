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

#ifndef __GKC_STRING_H__
	#error GkcConsole.h requires GkcString.h to be included first.
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
	//write LF
	static void WriteLN() throw()
	{
		print_string(_S("\n"));
	}

	//write string
	static void Write(const ConstStringS& str) throw()
	{
		print_string(ConstHelper::GetInternalPointer(str));
	}
	template <uintptr t_size>
	static void Write(const FixedStringT<CharS, t_size>& str) throw()
	{
		print_string(FixedArrayHelper::GetInternalPointer(str));
	}
	static void Write(const StringS& str) throw()
	{
		print_string(SharedArrayHelper::GetInternalPointer(str));
	}

	//write with LF
	static void WriteLine(const ConstStringS& str) throw()
	{
		Write(str);
		WriteLN();
	}
	template <uintptr t_size>
	static void WriteLine(const FixedStringT<CharS, t_size>& str) throw()
	{
		Write(str);
		WriteLN();
	}
	static void WriteLine(const StringS& str) throw()
	{
		Write(str);
		WriteLN();
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif  //__GKC_CONSOLE_H__
////////////////////////////////////////////////////////////////////////////////
