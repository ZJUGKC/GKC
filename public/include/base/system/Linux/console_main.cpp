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

#include "_cmdline.h"

////////////////////////////////////////////////////////////////////////////////

//signal
static
void __sig_int(int signo)
{
	//break key is pressed
	stdout_attr_helper::get_attr().Restore();
	::exit(0);
}

//main

int main(int argc, char *argv[], char *envp[])
{
	GKC::ConstArray<GKC::ConstStringS> args, env;

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

	//main
	return ProgramEntryPoint::ConsoleMain(args, env);
}

////////////////////////////////////////////////////////////////////////////////
