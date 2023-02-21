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

//Linux

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <dlfcn.h>
#include <dirent.h>

#include <sys/mman.h>

////////////////////////////////////////////////////////////////////////////////
