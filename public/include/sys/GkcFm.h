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
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_FM_H__
////////////////////////////////////////////////////////////////////////////////
