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
Id-Action
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RScanner.h"
#include "rdp/base/_RIdTable.h"

#include "rdp/ldf/scanner/ScannerDef.h"
#include "rdp/ldf/scanner/DoIdAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoIdAction

BEGIN_COM_TYPECAST(DoIdAction)
	COM_TYPECAST_ENTRY(_IRdParserAction, _IRdParserAction)
	COM_TYPECAST_ENTRY(_I_Scanner_DataAction_Utility, _ScannerDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
