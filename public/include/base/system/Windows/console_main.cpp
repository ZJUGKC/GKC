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

//main

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
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

	//time
	time_initialize();

	//main
	return program_entry_point::ConsoleMain(args, env);
}

////////////////////////////////////////////////////////////////////////////////
