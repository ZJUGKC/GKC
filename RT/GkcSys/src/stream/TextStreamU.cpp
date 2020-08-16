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
This file contains global variables for unique text stream component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "stream/TextStreamU.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// unique text stream

BEGIN_COM_TYPECAST(TextStreamU)
	COM_TYPECAST_ENTRY(_ITextStreamRoot, _ITextStreamRootImpl)
	COM_TYPECAST_ENTRY(_ITextStreamStringU, _ITextStreamStringU)
	COM_TYPECAST_ENTRY(_ITextUtilityU, _ITextUtilityU)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
