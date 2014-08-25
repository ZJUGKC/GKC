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
This file contains main function for Shared Assembly.
*/

////////////////////////////////////////////////////////////////////////////////

// so init functions

__attribute__((constructor)) void my_init()
{
	if( !ProgramEntryPoint::SAMain(true) ) {
		throw GKC::Exception(GKC::CallResult(CR_SABAD));  //throw
	}
}

__attribute__((destructor)) void my_fini()
{
	ProgramEntryPoint::SAMain(false);
}

////////////////////////////////////////////////////////////////////////////////
