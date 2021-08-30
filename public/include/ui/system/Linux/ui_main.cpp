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
This file contains main function for UI program.
*/

////////////////////////////////////////////////////////////////////////////////

//main

int main(int argc, char *argv[], char *envp[])
{
//start
	//fork
	pid_t pid = ::fork();
	if( pid == -1 ) {
		::perror("cannot fork!\n");
		return -1;
	}
	if( pid != 0 ) {
		//parent process
		return 0;
	}

//in child
	//setsid
	pid = ::setsid();
	if( pid == (pid_t)-1 ) {
		::perror("cannot initialize session!\n");
		return -1;
	}
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
	//mask
	::umask(0);
	//signal
	::signal(SIGCHLD, SIG_IGN);  //no check

//command
	const_array<const_string_s> args;
	_auto_mem spArgs;
	//convert
	_cmdline_to_strings(argc, argv, spArgs, args);  //may throw

//global
	//time
	time_initialize();

	//GTK+3
	::gtk_init(&argc, &argv);  //may terminate the program

	//main
	return program_entry_point::UIMain(args);
}

////////////////////////////////////////////////////////////////////////////////
