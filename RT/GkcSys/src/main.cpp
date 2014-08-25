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
This file contains main functions for SA.
*/

////////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "globals.h"

////////////////////////////////////////////////////////////////////////////////

// ProgramEntryPoint

class ProgramEntryPoint
{
public:
	static bool SAMain(bool bInitOrDump)
	{
		if( bInitOrDump ) {
			//init
			if( !init_globals() )
				return false;
		}
		else {
			//dump
			dump_globals();
		}
		return true;
	}
};

#include "base/GkcSAMain.cpp"

////////////////////////////////////////////////////////////////////////////////
