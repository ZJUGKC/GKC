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
	template <typename Tchar>
	static void ConvertPathStringToPlatform(INOUT UniqueStringT<Tchar>& str) throw()
	{
		assert( !str.IsNull() );
		cvt_path_string_to_platform(UniqueArrayHelper::GetInternalPointer(str));
	}

	//separator
	template <class Tstring>
	static void AppendSeparator(Tstring& str)
	{
		uintptr uLength = str.GetLength();
		if( !check_path_last_separator(ShareArrayHelper::GetInternalPointer(str), uLength) ) {
			typename Tstring::EType ch;
			get_path_separator(ch);
			StringOpHelper::Append(ch, str);  //may throw
		}
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
	//  uPos: this value is returned by FindExtensionStart().
	template <typename Tchar, uintptr t_size>
	static void RemoveExtension(uintptr uPos, FixedStringT<Tchar, t_size>& str) throw()
	{
		str.SetLength(uPos);
	}
	template <typename Tchar>
	static void RemoveExtension(uintptr uPos, StringT<Tchar>& str) throw()
	{
		StringOpHelper::Delete(uPos, str.GetLength() - uPos, str);
	}
	template <typename Tchar>
	static void RemoveExtension(uintptr uPos, UniqueStringT<Tchar>& str) throw()
	{
		StringOpHelper::Delete(uPos, str.GetLength() - uPos, str);
	}

	//file part
	template <typename Tchar>
	static uintptr FindFilePartStart(IN const ConstStringT<Tchar>& str) throw()
	{
		const Tchar* szSrc = ConstArrayHelper::GetInternalPointer(str);
		return find_path_file_part_start(szSrc, str.GetCount()) - szSrc;
	}

	//path part
	//  The result string may have the trailing path separator.
	//  uPos: this value is returned by FindFilePartStart().
	template <typename Tchar, uintptr t_size>
	static void ToPathPart(uintptr uPos, FixedStringT<Tchar, t_size>& str) throw()
	{
		str.SetLength(uPos);
	}
	template <typename Tchar>
	static void ToPathPart(uintptr uPos, StringT<Tchar>& str) throw()
	{
		StringOpHelper::Delete(uPos, str.GetLength() - uPos, str);
	}
	template <typename Tchar>
	static void ToPathPart(uintptr uPos, UniqueStringT<Tchar>& str) throw()
	{
		StringOpHelper::Delete(uPos, str.GetLength() - uPos, str);
	}

	template <typename Tchar, uintptr t_size>
	static void RemovePathTrailingSeparator(FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_deletable_last_separator(FixedArrayHelper::GetInternalPointer(str), uLength) )
			str.SetLength(uLength - 1);
	}
	template <typename Tchar>
	static void RemovePathTrailingSeparator(StringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_deletable_last_separator(ShareArrayHelper::GetInternalPointer(str), uLength) )
			StringOpHelper::Delete(uLength - 1, 1, str);
	}
	template <typename Tchar>
	static void RemovePathTrailingSeparator(UniqueStringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_deletable_last_separator(UniqueArrayHelper::GetInternalPointer(str), uLength) )
			StringOpHelper::Delete(uLength - 1, 1, str);
	}

	//upper directory
	//  The result string may have the trailing path separator.
	//  The return value is the length of result string. It can be compared with the length of input string for determining whether the string is unchanged.
	template <typename Tchar, uintptr t_size>
	static uintptr ToUpperDirectory(FixedStringT<Tchar, t_size>& str) throw()
	{
		remove_separator(str);
		uintptr uPos = FindFilePartStart(StringUtilHelper::To_ConstString(str));
		ToPathPart(uPos, str);
		return uPos;
	}
	template <typename Tchar>
	static uintptr ToUpperDirectory(StringT<Tchar>& str) throw()
	{
		remove_separator(str);
		uintptr uPos = FindFilePartStart(StringUtilHelper::To_ConstString(str));
		ToPathPart(uPos, str);
		return uPos;
	}
	template <typename Tchar>
	static uintptr ToUpperDirectory(UniqueStringT<Tchar>& str) throw()
	{
		remove_separator(str);
		uintptr uPos = FindFilePartStart(StringUtilHelper::To_ConstString(str));
		ToPathPart(uPos, str);
		return uPos;
	}

	//path prefix modification
	template <class Tstring>
	static void AppendCurrentPathPrefix(Tstring& str)
	{
		uintptr uLength = str.GetLength();
		if( uLength == 0 )
			return ;
		for( auto iter(str.GetBegin()); iter != str.GetEnd(); iter.MoveNext() ) {
			const typename Tstring::EType& ch = iter.get_Value();
			if( check_path_separator(ch) || check_drive_separator(ch) )
				return ;
		}
		const typename Tstring::EType* sz;
		get_current_path_prefix(sz, uLength);
		ConstStringT<typename Tstring::EType> c_strPrefix(sz, uLength);
		StringOpHelper::Insert(0, c_strPrefix, str);  //may throw
	}
	//Called after ConvertPathStringToPlatform with absolute path
	//  Now this method can be ignored under the latest OS.
	template <class Tstring>
	static void AppendAbsolutePathPrefix(Tstring& str)
	{
		uintptr uLength = str.GetLength();
		if( uLength == 0 )
			return ;
		const typename Tstring::EType* sz;
		get_absolute_path_prefix(sz, uLength);
		ConstStringT<typename Tstring::EType> c_strPrefix(sz, uLength);
		StringOpHelper::Insert(0, c_strPrefix, str);  //may throw
	}

private:
	template <typename Tchar, uintptr t_size>
	static void remove_separator(FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_last_separator(FixedArrayHelper::GetInternalPointer(str), uLength) )
			str.SetLength(uLength - 1);
	}
	template <typename Tchar>
	static void remove_separator(StringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_last_separator(ShareArrayHelper::GetInternalPointer(str), uLength) )
			StringOpHelper::Delete(uLength - 1, 1, str);
	}
	template <typename Tchar>
	static void remove_separator(UniqueStringT<Tchar>& str) throw()
	{
		uintptr uLength = str.GetLength();
		if( check_path_last_separator(UniqueArrayHelper::GetInternalPointer(str), uLength) )
			StringOpHelper::Delete(uLength - 1, 1, str);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PATH_H__
////////////////////////////////////////////////////////////////////////////////
