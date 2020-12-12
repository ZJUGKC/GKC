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
Rule-Item-End-Action
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"

#include "rdp/ldf/parser/ParserDef.h"
#include "rdp/ldf/parser/DoRuleItemEndAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoRuleItemEndAction

BEGIN_COM_TYPECAST(DoRuleItemEndAction)
	COM_TYPECAST_ENTRY(_IRdParserAction, _IRdParserAction)
	COM_TYPECAST_ENTRY(_I_Parser_DataAction_Utility, _ParserDataActionBase)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
