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
	if( pid < 0 ) {
		::perror("cannot fork!\n");
		return -1;
	}
	if( pid > 0 ) {
		//parent process
		return 0;
	}

//in child
	//setsid
	pid = ::setsid();
	if( pid < 0 ) {
		::perror("cannot initialize session!\n");
		return -1;
	}
	//mask
	::umask(0);
	//signal
	::signal(SIGCHLD, SIG_IGN);  //no check

//command
	const_array<const_string_s> args;
	_auto_mem spArgs;
	//convert
	_cmdline_to_strings(argc, argv, spArgs, args);  //may throw

	//main
	return program_entry_point::UIMain(args);
}

////////////////////////////////////////////////////////////////////////////////
