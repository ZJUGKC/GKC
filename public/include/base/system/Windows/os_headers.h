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
// internal header
////////////////////////////////////////////////////////////////////////////////

//Windows

#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <ole2.h>
#include <Shellapi.h>

#pragma comment(lib, "rpcrt4.lib")

//CRT

#include <stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES

////////////////////////////////////////////////////////////////////////////////
