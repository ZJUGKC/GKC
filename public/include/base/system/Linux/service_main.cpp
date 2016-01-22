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

volatile int g_ExitFlag = 0;

//signal callbacks
static
void _sig_term(int signo)
{
	//catched signal sent by kill(1) command
	if( signo == SIGTERM ) {
		g_ExitFlag = 1;
	} //end if
}

//main

int main(int argc, char *argv[], char *envp[])
{
//start
	//no check
	::signal(SIGTTOU, SIG_IGN);
	::signal(SIGTTIN, SIG_IGN);
	::signal(SIGTSTP, SIG_IGN);
	::signal(SIGHUP, SIG_IGN);
	//fork
	pid_t pid = ::fork();
	if( pid < 0 ) {
		::perror("cannot fork!\n");
		exit(EXIT_FAILURE);
		//return -1;
	}
	if( pid > 0 ) {
		exit(EXIT_SUCCESS);  //parent process
		//return 0;
	}

//in child
	//setsid
	pid = ::setsid();
	if( pid < 0 ) {
		::perror("cannot initialize session!\n");
		return -1;
	}
	//cwd
	if( ::chdir("/") < 0 ) {
		::perror("cannot change dir!\n");
		return -1;
	}
	//close handles
	const int c_MAXFILE = 65535;
	for( int i = 0; i < c_MAXFILE; i ++ )
		::close(i);  //no check
	//::open("/dev/null", O_RDONLY);
	//::open("/dev/null", O_RDWR);
	//::open("/dev/null", O_RDWR);
	//mask
	::umask(0);
	//signal
	::signal(SIGCHLD, SIG_IGN);  //no check

//command
	const_array<const_string_s> args;
	_auto_mem spArgs;
	//convert
	try {
		_cmdline_to_strings(argc, argv, spArgs, args);  //may throw
	}
	catch(...) {
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Out of memory!"));
		return -1;
	}

	//arrange to catch the signal
	::signal(SIGTERM, _sig_term);  //no check
//loop
	report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_SUCCESS, _S("Start Service!"));
	service_main_loop sml;
	if( !sml.Prepare(args) ) {
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Initialize loop failed!"));
		return -1;
	}
	while( !g_ExitFlag ) {
		if( !sml.OneLoop() )
			break;
		thread_sleep(1);
	} //end while

//final
	int ret = sml.Cleanup();
	report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_SUCCESS, _S("Service Stopped!"));

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
