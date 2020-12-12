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
Scanner Actions
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"
#include "rdp/base/_RParser.h"

#include "rdp/ldf/token/TokenDef.h"

#include "rdp/ldf/scanner/ScannerDef.h"
#include "rdp/ldf/scanner/DoIdAction.h"

#include "rdp/ldf/ScannerAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// global variables

const RULEELEMENT g_Rules[] = {
//==============================================================================
//program : TK_SEP sec_tkid TK_SEP sec_actid
{ NT_program, LA_NULL }, { TK_SEP, LA_NULL }, { NT_sec_tkid, LA_NULL }, { TK_SEP, LA_NULL }, { NT_sec_actid, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//sec_tkid : TK_TK sec_tkid
{ NT_sec_tkid, LA_NULL }, { TK_TK, ACT_S_TK }, { NT_sec_tkid, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//sec_tkid : epsilon
{ NT_sec_tkid, LA_NULL }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//sec_actid : TK_ACT sec_actid
{ NT_sec_actid, LA_NULL }, { TK_ACT, ACT_S_ACT }, { NT_sec_actid, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//sec_actid : epsilon
{ NT_sec_actid, LA_NULL }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//==============================================================================
//end
{ CPL_TK_NULL, LA_NULL }
};

// functions

bool _Scanner_InitTable(_RTdTable& rt)
{
	return rt.Generate(g_Rules, TK_MAX);  //may throw
}

void _Scanner_InitActions(_RParser& rp, _Scanner_ActionSet& sas, _RIdTable& rtToken, _RIdTable& rtAction)
{
	CallResult cr;

	ShareCom<_IRdParserAction> sp;
	ShareCom<_I_Scanner_DataAction_Utility> spU;

	rp.ClearActions();
	//Do-Id
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoIdAction, _IRdParserAction, sas.actionDoTk, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Scanner_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetTable(RefPtr<_RIdTable>(rtToken));
	rp.AddAction(ACT_S_TK, sp);  //may throw
	//Do-Act
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoIdAction, _IRdParserAction, sas.actionDoAct, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Scanner_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetTable(RefPtr<_RIdTable>(rtAction));
	rp.AddAction(ACT_S_ACT, sp);  //may throw
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
