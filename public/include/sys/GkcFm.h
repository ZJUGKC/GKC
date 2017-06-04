/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains File Management classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_FM_H__
#define __GKC_FM_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcFm.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// FileManagementHelper

class FileManagementHelper
{
public:
	template <typename Tchar>
	static bool GetExePathName(StringT<Tchar>& str)
	{
		str.SetLength(MAX_FULL_PATH);  //may throw
		uintptr uChars;
		bool bRet = get_exe_path_name(ShareArrayHelper::GetInternalPointer(str), MAX_FULL_PATH, uChars);
		str.SetLength(uChars);
		return bRet;
	}
	template <typename Tchar>
	static bool GetFullPathName(const ConstStringT<Tchar>& str, StringT<Tchar>& strFull)
	{
		strFull.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_full_path_name(ConstArrayHelper::GetInternalPointer(str), ShareArrayHelper::GetInternalPointer(strFull));
		strFull.RecalcLength();
		return bRet;
	}

	//current directory
	template <typename Tchar>
	static bool GetCurrentDirectory(StringT<Tchar>& str)
	{
		str.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_current_directory(ShareArrayHelper::GetInternalPointer(str), MAX_FULL_PATH);
		str.RecalcLength();
		return bRet;
	}
	template <typename Tchar>
	static bool SetCurrentDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return set_current_directory(ConstArrayHelper::GetInternalPointer(str));
	}

	//check existing
	template <typename Tchar>
	static bool CheckFileExists(const ConstStringT<Tchar>& str) throw()
	{
		return check_file_exists(ConstArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static bool CheckDirectoryExists(const ConstStringT<Tchar>& str) throw()
	{
		return check_directory_exists(ConstArrayHelper::GetInternalPointer(str));
	}

	//this method returns failure if the directory already exists.
	template <typename Tchar>
	static bool CreateDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return create_directory(ConstArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static bool DeleteDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return delete_directory(ConstArrayHelper::GetInternalPointer(str));
	}
	//Create directory regardless of the missing of intermediate directories.
	template <typename Tchar, uintptr t_size>
	static bool ForceDirectory(FixedStringT<Tchar, t_size>& str) throw()
	{
		return force_directory(FixedArrayHelper::GetInternalPointer(str));
	}
	template <typename Tchar>
	static bool ForceDirectory(StringT<Tchar>& str) throw()
	{
		assert( !str.IsBlockNull() );
		return force_directory(ShareArrayHelper::GetInternalPointer(str));
	}

	template <typename Tchar>
	static bool DeleteFile(const ConstStringT<Tchar>& str) throw()
	{
		return delete_file(ConstArrayHelper::GetInternalPointer(str));
	}
	//Rename directory/file
	template <typename Tchar>
	static bool RenameFile(const ConstStringT<Tchar>& strOld, const ConstStringT<Tchar>& strNew) throw()
	{
		return rename_file(ConstArrayHelper::GetInternalPointer(strOld), ConstArrayHelper::GetInternalPointer(strNew));
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_FM_H__
////////////////////////////////////////////////////////////////////////////////
