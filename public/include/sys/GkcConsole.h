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

// ConsoleHelper

class ConsoleHelper
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

//attribute
	//set text attribute (one or more STDOUT_ATTR_*)
	static void SetTextAttribute(uint uAttrs) throw()
	{
		stdout_attr_helper::get_attr().SetAttribute(uAttrs);
	}

//input
	template <uintptr t_size>
	static void Read(FixedStringT<CharS, t_size>& str) throw()
	{
		assert( t_size > 0 );
		str.SetLength(0);
		CharS szFormat[256];
		szFormat[0] = 0;
		int ret = value_to_string(szFormat, sizeof(szFormat) / sizeof(CharS), _S("%%%Ius"), t_size - 1);
		if( ret >= 0 )
			szFormat[ret] = 0;
		//no check
		scan_format(szFormat, FixedArrayHelper::GetInternalPointer(str));
	}
	template <uintptr t_size>
	static void ReadLine(FixedStringT<CharS, t_size>& str) throw()
	{
		assert( t_size > 0 );
		str.SetLength(0);
		CharS szFormat[256];
		szFormat[0] = 0;
		int ret = value_to_string(szFormat, sizeof(szFormat) / sizeof(CharS), _S("%%%Iu[^\n]"), t_size - 1);
		if( ret >= 0 )
			szFormat[ret] = 0;
		//no check
		scan_format(szFormat, FixedArrayHelper::GetInternalPointer(str));
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif  //__GKC_CONSOLE_H__
////////////////////////////////////////////////////////////////////////////////
