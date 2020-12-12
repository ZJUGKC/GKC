/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
This file contains compression functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "compress/PkzCompressor.h"
#include "compress/PkzDecompressor.h"

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//functions

void _PkzCompressor_Create(_UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_UNIQUECOM_INSTANCE(GKC::PkzCompressor, sp, cr);
}

void _PkzDecompressor_Create(_UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_UNIQUECOM_INSTANCE(GKC::PkzDecompressor, sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
