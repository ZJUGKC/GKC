﻿/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains Path of File System classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_PATH_H__
#define __GKC_PATH_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcPath.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// FsPathHelper

class FsPathHelper
{
public:
	//convert path string to platform
	template <typename Tchar, uintptr t_size>
	static void ConvertPathStringToPlatform(INOUT FixedStringT<Tchar, t_size>& str) throw()
	{
		cvt_path_string_to_platform(FixedArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static void ConvertPathStringToPlatform(INOUT StringT<Tchar>& str) throw()
	{
		assert( !str.IsBlockNull() );
		cvt_path_string_to_platform(ShareArrayHelper::GetInternalPointer(str));
	}

	//separator
	template <typename Tchar>
	static void AppendSeparator(StringT<Tchar>& str)
	{
		assert( !str.IsBlockNull() );
		uintptr uLength = str.GetLength();
		if( !check_path_last_separator(ShareArrayHelper::GetInternalPointer(str), uLength) ) {
			Tchar ch;
			get_path_separator(ch);
			StringHelper::Append(ch, str);  //may throw
		}
	}
	template <typename Tchar>
	static void RemoveSeparator(StringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_last_separator(ShareArrayHelper::GetInternalPointer(str), uLength) )
			StringHelper::Delete(uLength - 1, 1, str);
	}
	template <typename Tchar, uintptr t_size>
	static void RemoveSeparator(FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_last_separator(FixedArrayHelper::GetInternalPointer(str), uLength) )
			str.SetLength(uLength - 1);
	}

	//extension
	template <typename Tchar>
	static bool FindExtensionStart(IN const ConstStringT<Tchar>& str, OUT uintptr& uPos) throw()
	{
		uintptr uLength = str.GetCount();
		const Tchar* szSrc = ConstArrayHelper::GetInternalPointer(str);
		Tchar* szFind = find_path_extension_start(szSrc, uLength);
		if( szFind == NULL )
			return false;
		uPos = szFind - szSrc;
		return true;
	}
	//file part
	template <typename Tchar>
	static uintptr FindFilePartStart(IN const ConstStringT<Tchar>& str) throw()
	{
		const Tchar* szSrc = ConstArrayHelper::GetInternalPointer(str);
		return find_path_file_part_start(szSrc, str.GetCount()) - szSrc;
	}

	//path prefix modification
	template <typename Tchar>
	static void AppendCurrentPathPrefix(StringT<Tchar>& str)
	{
		assert( !str.IsBlockNull() );
		uintptr uLength = str.GetLength();
		if( uLength == 0 )
			return ;
		for( auto iter(str.GetBegin()); iter != str.GetEnd(); iter.MoveNext() ) {
			const Tchar& ch = iter.get_Value();
			if( check_path_separator(ch) || check_drive_separator(ch) )
				return ;
		}
		const Tchar* sz;
		get_current_path_prefix(sz, uLength);
		ConstStringT<Tchar> c_strPrefix(sz, uLength);
		StringHelper::Insert(0, c_strPrefix, str);  //may throw
	}
	//Called after ConvertPathStringToPlatform with absolute path
	//  Now this method can be ignored under the latest OS.
	template <typename Tchar>
	static void AppendAbsolutePathPrefix(StringT<Tchar>& str)
	{
		assert( !str.IsBlockNull() );
		uintptr uLength = str.GetLength();
		if( uLength == 0 )
			return ;
		const Tchar* sz;
		get_absolute_path_prefix(sz, uLength);
		ConstStringT<Tchar> c_strPrefix(sz, uLength);
		StringHelper::Insert(0, c_strPrefix, str);  //may throw
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PATH_H__
////////////////////////////////////////////////////////////////////////////////
