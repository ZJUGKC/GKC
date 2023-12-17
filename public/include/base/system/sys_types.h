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
//
//This file can use exceptions
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//internal headers

#include "_sys_base.h"

//------------------------------------------------------------------------------
//File

// file_open_types
/*! \brief File types for opening.

File types for opening.
*/
BEGIN_ENUM(file_open_types)
	ENUM_VALUE_ENTRY(Read,       0x00000000)  //!< Read
	ENUM_VALUE_ENTRY(Write,      0x00000001)  //!< Write
	ENUM_VALUE_ENTRY(ReadWrite,  0x00000002)  //!< Read and Write
END_ENUM()

// file_creation_types
/*! \brief File types for creation.

File types for creation.
\note
    They can combine with <or> operation.
*/
BEGIN_ENUM(file_creation_types)
	ENUM_VALUE_ENTRY(Create,     0x00001000)  //!< Create
	ENUM_VALUE_ENTRY(NoTruncate, 0x00002000)  //!< No truncate
END_ENUM()

// file_mapping_types
//   they can combine with <or> operation
BEGIN_ENUM(file_mapping_types)
	ENUM_VALUE_ENTRY(Read,    0x00000001)
	ENUM_VALUE_ENTRY(Write,   0x00000002)
	ENUM_VALUE_ENTRY(Execute, 0x00000004)
END_ENUM()

#pragma pack(push, 1)

// storage_status
/*! \brief A structure for storage status.

A structure for storage status.
*/
struct storage_status
{
	int64       iSize;     //!< storage size in bytes (such as file, memory)
	time_value  tmAccess;  //!< time of last access
	time_value  tmModify;  //!< time of last modification
	time_value  tmCreate;  //!< time of creation
};

#pragma pack(pop)

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/sys_types.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/sys_types.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------

#define program_entry_point  GKC::ProgramEntryPoint

////////////////////////////////////////////////////////////////////////////////
