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
This file contains global variables for unique memory stream component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "stream/MemoryStreamU.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// unique memory stream

BEGIN_COM_TYPECAST(MemoryStreamU)
	COM_TYPECAST_ENTRY(_IByteStream, _IByteStream)
	COM_TYPECAST_ENTRY(_IMemoryUtilityU, _IMemoryUtilityU)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
