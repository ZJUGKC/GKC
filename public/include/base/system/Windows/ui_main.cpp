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
This file contains main function for UI program.
*/

////////////////////////////////////////////////////////////////////////////////

// tools

class _os_auto_com_final
{
public:
	_os_auto_com_final() throw()
	{
	}
	~_os_auto_com_final() throw()
	{
		::CoUninitialize();
	}
};

//wWinMain

int APIENTRY wWinMain(IN HINSTANCE hInstance,
	IN HINSTANCE hPrevInstance,
	IN LPWSTR    lpCmdLine,
	IN int       nCmdShow)
{
//command
	_os_auto_local_mem spLocal;
	const_array<const_string_s> args;
	_auto_mem spArgs;
	{
		LPWSTR lpszCommandLine = ::GetCommandLineW();
		LPWSTR* szArgList = NULL;
		int     iArgs = 0;
		szArgList = ::CommandLineToArgvW(lpszCommandLine, &iArgs);
		if( szArgList == NULL ) {
			::MessageBoxW(NULL, L"Cannot initialize command line!", L"Error", MB_OK);
			return -1;
		}
		spLocal.SetAddress(szArgList);
		//convert
		_cmdline_to_strings(iArgs, szArgList, spArgs, args);  //may throw
	} //end block

//COM
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if( FAILED(hr) ) {
		::MessageBoxW(NULL, L"Cannot initialize STA!", L"Error", MB_OK);
		return -1;
	}
	_os_auto_com_final _ac_final;  //auto call CoUninitialize
	// this statement solves a problem from ATL thunk technique which is not used in this library.
	::DefWindowProcW(NULL, 0, 0, 0);
	{
		INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), ICC_COOL_CLASSES | ICC_BAR_CLASSES };
		BOOL bRet = ::InitCommonControlsEx(&iccx);
		if( !bRet ) {
			::MessageBoxW(NULL, L"Cannot initialize common controls!", L"Error", MB_OK);
			return -1;
		}
	} //end block

//Show Flag is unuseful

	int ret = program_entry_point::UIMain(args);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
