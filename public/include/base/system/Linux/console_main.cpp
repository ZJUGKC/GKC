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
This file contains main function for Console Application.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//signal
/*
The destructors of local objects are not called because ::exit() is called.
All allocated system resources (memory, io, kernel, ...) are freed by process manager.
*/
static
void __sig_int(int signo)
{
	//break key is pressed
	stdout_attr_helper::get_attr().Restore();
	::exit(255);
}
static
void _sig_term(int signo)
{
	//catched signal sent by kill(1) command
	stdout_attr_helper::get_attr().Restore();
	::exit(254);
}

//main

int main(int argc, char *argv[], char *envp[])
{
	const_array<const_string_s> args, env;

	//memory
	_auto_mem spArgs, spEnv;

	//convert
	_cmdline_to_strings(argc, argv, envp, spArgs, spEnv, args, env);  //may throw

	//stdout
	stdout_attr_helper::get_attr().Init();
	stdout_attr_restore sar(stdout_attr_helper::get_attr());

	//locale
	set_default_locale();

	//signal
	::signal(SIGINT, __sig_int);  //no check
	::signal(SIGTERM, _sig_term);  //no check

	//time
	time_initialize();

	//main
	return program_entry_point::ConsoleMain(args, env);
}

////////////////////////////////////////////////////////////////////////////////
