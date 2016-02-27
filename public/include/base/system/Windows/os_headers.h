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
// internal header
////////////////////////////////////////////////////////////////////////////////

//Windows

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <ole2.h>
#include <Shellapi.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

//CRT

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
