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

//Wlang

#include "wlang/base/WlangDef.h"
#include "wlang/base/WlangLex.h"
#include "wlang/base/WlangGra.h"

#include "wlang/action/WlangGrammarError.h"
#include "wlang/action/WlangGrammarAccepted.h"
#include "wlang/action/WlangDoNsBodyAction.h"
#include "wlang/action/WlangDoBodySemiAction.h"

#include "wlang/WlangAction.h"
#include "wlang/WlangUtility.h"
#include "wlang/WlangParser.h"

//Won

#include "won/base/WonDef.h"
#include "won/base/WonLex.h"
#include "won/base/WonGra.h"

#include "won/action/WonDoCommonAction.h"

#include "won/WonAction.h"
#include "won/WonUtility.h"
#include "won/WonParser.h"

//Wmark

#include "wmark/base/WmarkDef.h"
#include "wmark/base/WmarkLex.h"
#include "wmark/base/WmarkGra.h"

#include "wmark/action/WmarkReturnAction.h"
#include "wmark/action/WmarkCommentStartAction.h"
#include "wmark/action/WmarkGrammarAccepted.h"
#include "wmark/action/WmarkDoDocRsBlockListAction.h"
#include "wmark/action/WmarkDoBlockListListBlockAction.h"
#include "wmark/action/WmarkDoBlockBlockBodyAction.h"
#include "wmark/action/WmarkDoBlockBodyIndentAction.h"

#include "wmark/WmarkAction.h"
#include "wmark/WmarkUtility.h"
#include "wmark/WmarkParser.h"

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

// WmarkUtility

inline void _Internal_WmarkUtility_Create(GKC::ShareCom<_IWmarkUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WmarkUtility, _IWmarkUtility, sp, cr);
}

// WmarkParser

inline void _Internal_WmarkParser_Create(GKC::ShareCom<_IWmarkParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WmarkParser, _IWmarkParser, sp, cr);
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

// WmarkUtility

void _WmarkUtility_Create(GKC::ShareCom<_IWmarkUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WmarkUtility_Create(sp, cr);
}

// WmarkParser

void _WmarkParser_Create(GKC::ShareCom<_IWmarkParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WmarkParser_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
