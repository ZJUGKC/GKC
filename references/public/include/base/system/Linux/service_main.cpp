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

volatile int g_ServiceExitFlag = 0;

//signal callbacks
static
void _sig_term(int signo)
{
	//catched signal sent by kill(1) command
	if( signo == SIGTERM ) {
		g_ServiceExitFlag = 1;
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
	if( pid == -1 ) {
		::perror("cannot fork!\n");
		exit(EXIT_FAILURE);
		//return -1;
	}
	if( pid != 0 ) {
		exit(EXIT_SUCCESS);  //parent process
		//return 0;
	}

//in child
	//setsid
	pid = ::setsid();
	if( pid == (pid_t)-1 ) {
		::perror("cannot initialize session!\n");
		return -1;
	}
	::signal(SIGHUP, SIG_IGN);

/*
//second
	//fork
	pid_t pid2 = ::fork();
	if( pid2 == -1 ) {
		::perror("cannot fork 2!\n");
		exit(EXIT_FAILURE);
		//return -1;
	}
	if( pid2 != 0 ) {
		exit(EXIT_SUCCESS);  //parent process
		//return 0;
	}
	::signal(SIGHUP, SIG_IGN);
*/

	//cwd
	if( ::chdir("/") == -1 ) {
		::perror("cannot change dir!\n");
		return -1;
	}
	//close handles
	const int c_MAXFILE = 65535;
	for( int i = 0; i < c_MAXFILE; i ++ )
		::close(i);  //no check
	/*
	//redirect IO
	int fdi = ::open("/dev/null", O_RDONLY);
	if( fdi == -1 ) {
		::perror("cannot open /dev/null!\n");
		return -1;
	}
	int fdo = ::open("/dev/null", O_RDWR);
	if( fdo == -1 ) {
		::perror("cannot open /dev/null!\n");
		return -1;
	}
	int fde = ::open("/dev/null", O_RDWR);
	if( fde == -1 ) {
		::perror("cannot open /dev/null!\n");
		return -1;
	}
	if( ::dup2(fdi, STDIN_FILENO) == -1 ) {
		::perror("cannot dup /dev/null!\n");
		return -1;
	}
	if( ::dup2(fdo, STDOUT_FILENO) == -1 ) {
		::perror("cannot dup /dev/null!\n");
		return -1;
	}
	if( ::dup2(fde, STDERR_FILENO) == -1 ) {
		::perror("cannot dup /dev/null!\n");
		return -1;
	}
	int ret_close;
	ret_close = ::close(fdi);
	assert( ret_close == 0 );
	ret_close = ::close(fdo);
	assert( ret_close == 0 );
	ret_close = ::close(fde);
	assert( ret_close == 0 );
	(void)ret_close;
	*/
	//mask
	::umask(0);
	//signal
	//::signal(SIGCHLD, SIG_IGN);  //no check

/*
	::signal(SIGINT, SIG_IGN);  //no check
	::signal(SIGQUIT, SIG_IGN);  //no check
	::signal(SIGWINCH, SIG_IGN);  //no check
	::signal(SIGCONT, SIG_IGN);  //no check
	::signal(SIGSTOP, SIG_IGN);  //no check
*/

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

//global
	//time
	time_initialize();

//loop
	report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_SUCCESS, _S("Start Service!"));
	service_main_loop sml;
	if( !sml.Prepare(args) ) {
		report_service_log(GKC_SERVICE_NAME, SERVICE_LOG_ERROR, _S("Initialize loop failed!"));
		return -1;
	}
	while( !g_ServiceExitFlag ) {
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
