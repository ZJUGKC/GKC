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
This file contains global variables for RD parser component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"
#include "rdp/base/_RParser.h"

#include "rdp/RdDef.h"
#include "rdp/RdParser.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdParser

BEGIN_COM_TYPECAST(RdParser)
	COM_TYPECAST_ENTRY(_IRdParser, _IRdParser)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
