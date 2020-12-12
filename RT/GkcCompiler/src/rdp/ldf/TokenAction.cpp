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
Token Actions
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RScanner.h"

#include "rdp/ldf/token/TokenDef.h"
#include "rdp/ldf/token/TkAction.h"
#include "rdp/ldf/token/NtAction.h"
#include "rdp/ldf/token/TkidAction.h"
#include "rdp/ldf/token/ActidAction.h"

#include "rdp/ldf/TokenAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// global variables

_ScopeShareComObject<TkAction>     g_actionTk;
_ScopeShareComObject<NtAction>     g_actionNt;
_ScopeShareComObject<TkidAction>   g_actionTkid;
_ScopeShareComObject<ActidAction>  g_actionActid;

// set actions

void _Token_SetActions(_RScanner& rsc)
{
	CallResult cr;

	ShareCom<_IRdScannerAction> sp;

	rsc.ClearActions();
	//TkAction
	_CREATE_SCOPE_COMPONENT_INSTANCE(TkAction, _IRdScannerAction, g_actionTk, sp, cr);
	assert( cr.IsSucceeded() );
	rsc.AddAction(ACT_TK, sp);  //may throw
	//NtAction
	_CREATE_SCOPE_COMPONENT_INSTANCE(NtAction, _IRdScannerAction, g_actionNt, sp, cr);
	assert( cr.IsSucceeded() );
	rsc.AddAction(ACT_NT, sp);  //may throw
	//TkidAction
	_CREATE_SCOPE_COMPONENT_INSTANCE(TkidAction, _IRdScannerAction, g_actionTkid, sp, cr);
	assert( cr.IsSucceeded() );
	rsc.AddAction(ACT_TKID, sp);  //may throw
	//ActidAction
	_CREATE_SCOPE_COMPONENT_INSTANCE(ActidAction, _IRdScannerAction, g_actionActid, sp, cr);
	assert( cr.IsSucceeded() );
	rsc.AddAction(ACT_ACTID, sp);  //may throw
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
