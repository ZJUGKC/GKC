/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for text stream component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "stream/TextStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// text stream

BEGIN_COM_TYPECAST(TextStream)
	COM_TYPECAST_ENTRY(_ITextStreamRoot, _ITextStreamRootImpl)
	COM_TYPECAST_ENTRY(_ITextStreamString, _ITextStreamString)
	COM_TYPECAST_ENTRY(_ITextUtility, _ITextUtility)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
