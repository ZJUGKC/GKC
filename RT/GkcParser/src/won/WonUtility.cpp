/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains global variables for Won utility component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "base/ParserDef.h"

#include "won/base/WonDef.h"
#include "won/base/WonLex.h"
#include "won/base/WonGra.h"

#include "won/WonUtility.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonUtility

BEGIN_COM_TYPECAST(WonUtility)
	COM_TYPECAST_ENTRY(_IWonUtility, _IWonUtility)
	COM_TYPECAST_ENTRY(_IWonUtility_Access, _IWonUtility_Access)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
