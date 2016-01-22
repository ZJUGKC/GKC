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
This file contains main function for Shared Assembly.
*/

////////////////////////////////////////////////////////////////////////////////

//DllMain

extern "C"
BOOL APIENTRY DllMain(HMODULE hModule,
					DWORD  ul_reason_for_call,
					LPVOID lpReserved
					)
{
	BOOL bRet = TRUE;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		::DisableThreadLibraryCalls(hModule);
		bRet = program_entry_point::SAMain(true);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		bRet = program_entry_point::SAMain(false);
		break;
	default:
		break;
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
