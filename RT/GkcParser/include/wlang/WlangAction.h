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
This file contains WLang action functions.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//global variables

extern _ScopeShareComObject<WlangGrammarError> g_actionWlangGrammarError;

// grammar error

inline CallResult _Create_WlangGrammarError(ShareCom<IGrammarError>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WlangGrammarError, IGrammarError, g_actionWlangGrammarError, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
//classes

// _Wlang_ActionSet

struct _Wlang_ActionSet
{
	_ScopeShareComObject<WlangDoNsBodyAction> actionWlangDoNsBody;
	_ScopeShareComObject<WlangDoBodySemiAction> actionWlangDoBodySemi;
	_ScopeShareComObject<WlangGrammarAccepted> actionWlangGrammarAccepted;
};

//functions

// Do-Ns-Body action

inline CallResult _Create_WlangDoNsBodyAction(const _Wlang_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WlangDoNsBodyAction, IGrammarAction, was.actionWlangDoNsBody, sp, cr);
	return cr;
}

// Do-Body-Semi action

inline CallResult _Create_WlangDoBodySemiAction(const _Wlang_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WlangDoBodySemiAction, IGrammarAction, was.actionWlangDoBodySemi, sp, cr);
	return cr;
}

// grammar accepted

inline CallResult _Create_WlangGrammarAccepted(const _Wlang_ActionSet& was, ShareCom<IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WlangGrammarAccepted, IGrammarAccepted, was.actionWlangGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
