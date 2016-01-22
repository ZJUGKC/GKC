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

////////////////////////////////////////////////////////////////////////////////
//internal header
////////////////////////////////////////////////////////////////////////////////

//convert

inline void _cmdline_to_strings(int argc, char *argv[],  //in
								_auto_mem& spArgs,  //out
								const_array<const_string_s>& args)  //out
{
	assert( argc > 0 );

	spArgs.Free();

	//alloc
	uintptr uSize;
	uSize = safe_operators::MultiplyThrow<uintptr>(sizeof(const_string_s), (uintptr)argc);
	spArgs.Allocate(uSize);  //may throw
	const_string_s* parr1 = (const_string_s*)spArgs.GetAddress();

	//fill
	for( uintptr i = 0; i < (uintptr)argc; i ++ ) {
		const_array_helper::SetInternalPointer(argv[i], calc_string_length(argv[i]), parr1[i]);
	}
	const_array_helper::SetInternalPointer(parr1, (uintptr)argc, args);
}

inline void _cmdline_to_strings(int argc, char *argv[], char *envp[],  //in
								_auto_mem& spArgs, _auto_mem& spEnv,  //out
								const_array<const_string_s>& args, const_array<const_string_s>& env)  //out
{
	assert( argc > 0 );

	spArgs.Free();
	spEnv.Free();

	//alloc
	uintptr uSize;
	uSize = safe_operators::MultiplyThrow<uintptr>(sizeof(const_string_s), (uintptr)argc);
	spArgs.Allocate(uSize);  //may throw
	const_string_s* parr1 = (const_string_s*)spArgs.GetAddress();

	uintptr env_num = 0;
	while( envp[env_num] != NULL ) {
		++ env_num;
	}
	const_string_s* parr2 = NULL;
	if( env_num > 0 ) {
		uSize = safe_operators::MultiplyThrow<uintptr>(sizeof(const_string_s), env_num);
		spEnv.Allocate(uSize);  //may throw
		parr2 = (const_string_s*)spEnv.GetAddress();
	}

	//fill
	for( uintptr i = 0; i < (uintptr)argc; i ++ ) {
		const_array_helper::SetInternalPointer(argv[i], calc_string_length(argv[i]), parr1[i]);
	}
	const_array_helper::SetInternalPointer(parr1, (uintptr)argc, args);
	for( uintptr i = 0; i < env_num; i ++ ) {
		const_array_helper::SetInternalPointer(envp[i], calc_string_length(envp[i]), parr2[i]);
	}
	const_array_helper::SetInternalPointer(parr2, env_num, env);
}

////////////////////////////////////////////////////////////////////////////////
