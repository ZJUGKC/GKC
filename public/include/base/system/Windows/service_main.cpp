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
This file contains main function for Service program.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

// tools

SERVICE_STATUS_HANDLE g_hServiceStatus = NULL;
SERVICE_STATUS        g_status;
DWORD                 g_dwThreadID;

static
void _set_service_status(DWORD dwState) throw()
{
	::InterlockedExchange((LONG volatile *)(&g_status.dwCurrentState), (LONG)dwState);
	::SetServiceStatus(g_hServiceStatus, &g_status);  //no check
}

//callbacks
static
VOID WINAPI _Handler(_In_ DWORD dwOpcode) throw()
{
	switch( dwOpcode ) {
	case SERVICE_CONTROL_STOP:
		// stop
		_set_service_status(SERVICE_STOP_PENDING);
		::PostThreadMessage(g_dwThreadID, WM_QUIT, 0, 0);  //no check
		break;
	case SERVICE_CONTROL_PAUSE:
		// pause
		break;
	case SERVICE_CONTROL_CONTINUE:
		// continue
		break;
	case SERVICE_CONTROL_INTERROGATE:
		// interrogate
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		// shutdown
		break;
	default:
		// UnknownRequest
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Bad service request"));
		break;
	}  //end switch
}

static
VOID WINAPI _ServiceMain(
	_In_ DWORD dwArgc,
	_In_ LPWSTR* lpszArgv) throw()
{
//Register the control request handler
	g_status.dwCurrentState = SERVICE_START_PENDING;
	g_dwThreadID = ::GetCurrentThreadId();
	g_hServiceStatus = ::RegisterServiceCtrlHandlerW(L"", _Handler);
	if( g_hServiceStatus == NULL ) {
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Handler not installed"));
		return ;
	}
	_set_service_status(SERVICE_START_PENDING);

	g_status.dwWin32ExitCode = S_OK;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;

//command
	const_array<const_string_s> args;
	_auto_mem spArgs;
	//convert
	try {
		_cmdline_to_strings(dwArgc, lpszArgv, spArgs, args);  //may throw
	}
	catch(...) {
		_set_service_status(SERVICE_STOPPED);
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Out of memory"));
		return ;
	}

//COM
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if( FAILED(hr) ) {
		_set_service_status(SERVICE_STOPPED);
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Cannot initialize MTA!"));
		return ;
	}

//loop
	_os_create_thread_message_queue();  //message queue

	_set_service_status(SERVICE_RUNNING);
	report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_SUCCESS, _S("Service started/resumed"));

	service_main_loop sml;
	if( !sml.Prepare(args) ) {
		::CoUninitialize();
		_set_service_status(SERVICE_STOPPED);
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Initialize loop failed!"));
		return ;
	}

	while( true ) {
		MSG msg;
		BOOL bRet = ::PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE);
		if( bRet ) {
			bRet = ::GetMessageW(&msg, NULL, 0, 0);
			//WM_QUIT
			if( !bRet )
				break;
			::TranslateMessage(&msg);  //no check
			::DispatchMessageW(&msg);  //no check
		}
		else {
			if( !sml.OneLoop() )
				break;
			//to be continued
			thread_sleep(1);
		} //end if
	} //end while

//final
	g_status.dwWin32ExitCode = sml.Cleanup();

	::CoUninitialize();
	_set_service_status(SERVICE_STOPPED);
	report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_SUCCESS, _S("Service stopped!"));
}

//wWinMain

extern "C"
int APIENTRY wWinMain(IN HINSTANCE hInstance,
	IN HINSTANCE hPrevInstance,
	IN LPWSTR    lpCmdLine,
	IN int       nCmdShow)
{
//init
	g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_status.dwCurrentState = SERVICE_STOPPED;
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_status.dwWin32ExitCode = 0;
	g_status.dwServiceSpecificExitCode = 0;
	g_status.dwCheckPoint = 0;
	g_status.dwWaitHint = 0;

//dispatcher
	WCHAR sname[16];
	sname[0] = 0;
	SERVICE_TABLE_ENTRYW st[] = {
		{ sname, _ServiceMain },
		{ NULL, NULL }
	};
	if( !::StartServiceCtrlDispatcherW(st) )
		g_status.dwWin32ExitCode = ::GetLastError();

	return g_status.dwWin32ExitCode;
}

////////////////////////////////////////////////////////////////////////////////
