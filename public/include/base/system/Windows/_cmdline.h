﻿/*
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
inline void _cmdline_to_strings(int argc, wchar_t *argv[], wchar_t *envp[],  //in
								_auto_mem& spArgs, _auto_mem& spEnv,   //out
								GKC::ConstArray<GKC::ConstStringS>& args, GKC::ConstArray<GKC::ConstStringS>& env)  //out
{
	assert( argc > 0 );

	spArgs.Free();
	spEnv.Free();

	//alloc
	uintptr uSize;
	uSize = GKC::SafeOperators::MultiplyThrow<uintptr>(sizeof(GKC::ConstStringS), (uintptr)argc);
	spArgs.Allocate(uSize);  //may throw
	GKC::ConstStringS* parr1 = (GKC::ConstStringS*)spArgs.GetAddress();

	uintptr env_num = 0;
	while( envp[env_num] != NULL ) {
		++ env_num;
	}
	GKC::ConstStringS* parr2 = NULL;
	if( env_num > 0 ) {
		uSize = GKC::SafeOperators::MultiplyThrow<uintptr>(sizeof(GKC::ConstStringS), rv_forward(env_num));
		spEnv.Allocate(uSize);  //may throw
		parr2 = (GKC::ConstStringS*)spEnv.GetAddress();
	}

	//fill
	for( uintptr i = 0; i < (uintptr)argc; i ++ ) {
		parr1[i].SetPointer(argv[i], ::lstrlenW(argv[i]));
	}
	args.SetPointer(parr1, (uintptr)argc);
	for( uintptr i = 0; i < env_num; i ++ ) {
		parr2[i].SetPointer(envp[i], ::lstrlenW(envp[i]));
	}
	env.SetPointer(parr2, env_num);
}

////////////////////////////////////////////////////////////////////////////////