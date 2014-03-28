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

//wWinMain

int APIENTRY wWinMain(IN HINSTANCE hInstance,
	IN HINSTANCE hPrevInstance,
	IN LPWSTR    lpCmdLine,
	IN int       nCmdShow)
{
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if( FAILED(hr) ) {
		::MessageBoxW(NULL, L"Cannot initialize STA!", L"Error", MB_OK);
		return -1;
	}
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProcW(NULL, 0, 0, 0);
	{
		INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), ICC_COOL_CLASSES | ICC_BAR_CLASSES };
		BOOL bRet = ::InitCommonControlsEx(&iccx);
		if( !bRet ) {
			::CoUninitialize();
			::MessageBoxW(NULL, L"Cannot initialize common controls!", L"Error", MB_OK);
			return -1;
		}
	} //end block

//command and ShowFlag
	int ret = ProgramEntryPoint::UIMain();

	::CoUninitialize();

	return ret;
}

//others
#include "_Module.cpp"

////////////////////////////////////////////////////////////////////////////////
