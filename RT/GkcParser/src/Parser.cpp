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
This file contains parser functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "base/ParserDef.h"

#include "wlang/base/WlangDef.h"
#include "wlang/base/WlangLex.h"
#include "wlang/base/WlangGra.h"

#include "wlang/action/WlangGrammarError.h"
#include "wlang/action/WlangGrammarAccepted.h"
#include "wlang/action/WlangPositionSymbolData.h"
#include "wlang/action/WlangPositionSymbolData_Factory.h"
#include "wlang/action/WlangDoNsBodyAction.h"
#include "wlang/action/WlangDoBodySemiAction.h"

#include "wlang/WlangAction.h"
#include "wlang/WlangUtility.h"
#include "wlang/WlangParser.h"

#include "won/base/WonDef.h"
#include "won/base/WonLex.h"
#include "won/base/WonGra.h"

#include "won/action/WonDoCommonAction.h"

#include "won/WonAction.h"
#include "won/WonUtility.h"
#include "won/WonParser.h"

////////////////////////////////////////////////////////////////////////////////

//functions

namespace GKC {

// WlangUtility

inline void _Internal_WlangUtility_Create(GKC::ShareCom<_IWlangUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WlangUtility, _IWlangUtility, sp, cr);
}

// WlangParser

inline void _Internal_WlangParser_Create(GKC::ShareCom<_IWlangParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WlangParser, _IWlangParser, sp, cr);
}

// WonUtility

inline void _Internal_WonUtility_Create(GKC::ShareCom<_IWonUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WonUtility, _IWonUtility, sp, cr);
}

// WonParser

inline void _Internal_WonParser_Create(GKC::ShareCom<_IWonParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WonParser, _IWonParser, sp, cr);
}

} //namespace

// WlangUtility

void _WlangUtility_Create(GKC::ShareCom<_IWlangUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WlangUtility_Create(sp, cr);
}

// WlangParser

void _WlangParser_Create(GKC::ShareCom<_IWlangParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WlangParser_Create(sp, cr);
}

// WonUtility

void _WonUtility_Create(GKC::ShareCom<_IWonUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WonUtility_Create(sp, cr);
}

// WonParser

void _WonParser_Create(GKC::ShareCom<_IWonParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WonParser_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
