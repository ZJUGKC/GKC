/*
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
This file contains Path of FileSystem classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_PATH_H__
#define __GKC_PATH_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_STRING_H__
	#error GkcPath.h requires GkcString.h to be included first.
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
		assert( !str.IsNull() );
		cvt_path_string_to_platform(FixedArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static void ConvertPathStringToPlatform(INOUT StringT<Tchar>& str) throw()
	{
		assert( !str.IsNull() );
		cvt_path_string_to_platform(SharedArrayHelper::GetInternalPointer(str));
	}

	//relative
	template <typename Tchar>
	static bool IsRelative(IN const ConstStringT<Tchar>& str) throw()
	{
		assert( !str.IsNull() );
		return path_is_relative(ConstHelper::GetInternalPointer(str));
	}

	//separator
	template <typename Tchar>
	static void AppendSeparator(StringT<Tchar>& str)
	{
		assert( !str.IsNull() );
		uintptr uLength = str.GetLength();
		if( uLength == 0 || !check_path_separator(str.GetAt(uLength - 1).get_Value()) ) {
			Tchar ch;
			get_path_separator(ch);
			StringUtilHelper::Append(str, ch);
		}
	}
	template <typename Tchar>
	static void RemoveSeparator(StringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( uLength > 0 && check_path_separator(str.GetAt(uLength - 1).get_Value()) ) {
			StringUtilHelper::Delete(uLength - 1, 1, str);
		}
	}

	//extension
	template <typename Tchar>
	static bool FindExtensionStart(IN const ConstStringT<Tchar>& str, OUT uintptr& uPos) throw()
	{
		uintptr uLength = str.GetCount();
		if( uLength == 0 )
			return false;
		auto iterB(str.GetReverseBegin());
		auto iter(iterB);
		for( ; iter != str.GetReverseEnd(); iter.MoveNext() ) {
			const Tchar& ch = iter.get_Value();
			if( check_path_separator(ch) )
				return false;
			if( check_path_extension_start(ch) ) {
				uPos = uLength + iter.CalcDelta(iterB) - 1;
				return true;
			}
		}
		return false;
	}
	//file part
	template <typename Tchar>
	static uintptr FindFilePartStart(IN const ConstStringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetCount();
		if( uLength == 0 )
			return 0;
		auto iterB(str.GetReverseBegin());
		auto iter(iterB);
		for( ; iter != str.GetReverseEnd(); iter.MoveNext() ) {
			const Tchar& ch = iter.get_Value();
			if( check_path_separator(ch) || check_drive_separator(ch) )
				return uLength + iter.CalcDelta(iterB);
		}
		return 0;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PATH_H__
////////////////////////////////////////////////////////////////////////////////
