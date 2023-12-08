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
	/*! \brief Get executable file name.

	Get executable file name.
	\tparam Tchar Character type.
	\param str [out] Receive the file name.
	\return true for succeeded, false for otherwise.
	*/
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
	static bool GetExePathName(UniqueStringT<Tchar>& str)
	{
		str.SetLength(MAX_FULL_PATH);  //may throw
		uintptr uChars;
		bool bRet = get_exe_path_name(UniqueArrayHelper::GetInternalPointer(str), MAX_FULL_PATH, uChars);
		str.SetLength(uChars);
		return bRet;
	}

	/*! \brief Get full path by current device and directory.

	Get full path by current device and directory.
	\tparam Tchar Character type.
	\param str [in] The file name, can be an absolute path or a relative path.
	\param strFull [out] Receive the full absoulte path.
	\return true for succeeded, false for otherwise.
	*/
	template <typename Tchar>
	static bool GetFullPathName(const ConstStringT<Tchar>& str, StringT<Tchar>& strFull)
	{
		strFull.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_full_path_name(ConstArrayHelper::GetInternalPointer(str), ShareArrayHelper::GetInternalPointer(strFull));
		strFull.RecalcLength();
		return bRet;
	}
	template <typename Tchar>
	static bool GetFullPathName(const ConstStringT<Tchar>& str, UniqueStringT<Tchar>& strFull)
	{
		strFull.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_full_path_name(ConstArrayHelper::GetInternalPointer(str), UniqueArrayHelper::GetInternalPointer(strFull));
		strFull.RecalcLength();
		return bRet;
	}

	//current directory
	/*! \brief Get current directory.

	Get current directory.
	\tparam Tchar Character type.
	\param str [out] Receive the current directory.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool GetCurrentDirectory(StringT<Tchar>& str)
	{
		str.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_current_directory(ShareArrayHelper::GetInternalPointer(str), MAX_FULL_PATH);
		str.RecalcLength();
		return bRet;
	}
	template <typename Tchar>
	static bool GetCurrentDirectory(UniqueStringT<Tchar>& str)
	{
		str.SetLength(MAX_FULL_PATH);  //may throw
		bool bRet = get_current_directory(UniqueArrayHelper::GetInternalPointer(str), MAX_FULL_PATH);
		str.RecalcLength();
		return bRet;
	}
	/*! \brief Set current directory.

	Set current directory.
	\param str [in] The directory string.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool SetCurrentDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return set_current_directory(ConstArrayHelper::GetInternalPointer(str));
	}

	//check existing
	/*! \brief Check whether the file exists.

	Check whether the file exists.
	\tparam Tchar Character type.
	\param str [in] Specify the file name.
	\return true for existing, false for missing.
	*/
	template <typename Tchar>
	static bool CheckFileExists(const ConstStringT<Tchar>& str) throw()
	{
		return check_file_exists(ConstArrayHelper::GetInternalPointer(str));
	}
	/*! \brief Check whether the directory exists.

	Check whether the directory exists.
	\tparam Tchar Character type.
	\param str [in] Specify the directory name.
	\return true for existing, false for missing.
	*/
	template <typename Tchar>
	static bool CheckDirectoryExists(const ConstStringT<Tchar>& str) throw()
	{
		return check_directory_exists(ConstArrayHelper::GetInternalPointer(str));
	}

	/*! \brief Create a directory.

	Create a directory.
	\tparam Tchar Character type.
	\param str [in] The directory string.
	\return true for succeeded, false for failed.
	\note If the specified directory already exists, this function will return failure.
	*/
	template <typename Tchar>
	static bool CreateDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return create_directory(ConstArrayHelper::GetInternalPointer(str));
	}
	/*! \brief Create a directory.

	Create a directory.
	\tparam Tchar Character type.
	\param str [in] The directory string.
	\param bExisting [out] Receive the flag whether the directory already exists.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool CreateDirectory(const ConstStringT<Tchar>& str, bool& bExisting) throw()
	{
		return create_directory(ConstArrayHelper::GetInternalPointer(str), bExisting);
	}
	/*! \brief Delete a directory.

	Delete a directory.
	\tparam Tchar Character type.
	\param str [in] The directory string.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool DeleteDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return delete_directory(ConstArrayHelper::GetInternalPointer(str));
	}
	/*! \brief Create directory regardless of the missing of intermediate directories.

	Create directory regardless of the missing of intermediate directories.
	\tparam Tchar Character type.
	\param str [in] The directory to be created.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool ForceDirectory(const ConstStringT<Tchar>& str) throw()
	{
		return force_directory(ConstArrayHelper::GetInternalPointer(str));
	}
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
	static bool ForceDirectory(UniqueStringT<Tchar>& str) throw()
	{
		assert( !str.IsNull() );
		return force_directory(UniqueArrayHelper::GetInternalPointer(str));
	}

	/*! \brief Delete a file.

	Delete a file.
	\tparam Tchar Character type.
	\param str [in] The string of file name.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool DeleteFile(const ConstStringT<Tchar>& str) throw()
	{
		return delete_file(ConstArrayHelper::GetInternalPointer(str));
	}
	//Rename directory/file
	/*! \brief Rename a directory or file.

	Rename a directory or file.
	\tparam Tchar Character type.
	\param strOld [in] The string of current dir/file name.
	\param strNew [in] The string of new dir/file name.
	\return true for succeeded, false for failed.
	*/
	template <typename Tchar>
	static bool RenameFile(const ConstStringT<Tchar>& strOld, const ConstStringT<Tchar>& strNew) throw()
	{
		return rename_file(ConstArrayHelper::GetInternalPointer(strOld), ConstArrayHelper::GetInternalPointer(strNew));
	}

	//Home directory
	template <class Tstring>
	static bool GetHomeDirectory(Tstring& str)
	{
		return EnvironmentVariableHelper::Query<typename Tstring::EType>(DECLARE_TEMP_CONST_STRING(ConstStringT<typename Tstring::EType>, ENVVAR_HOME_DIR), str);  //may throw
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_FM_H__
////////////////////////////////////////////////////////////////////////////////
