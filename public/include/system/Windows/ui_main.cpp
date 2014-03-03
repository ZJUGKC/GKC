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

	int ret = ProgramEntryPoint::UIMain();

	::CoUninitialize();

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
