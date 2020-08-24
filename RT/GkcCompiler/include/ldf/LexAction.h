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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEX_ACTION_H__
#define __LEX_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// lexer actions

// global variables

extern _ScopeShareComObject<MacroTokenAction> g_actionMacroToken;

// MacroTokenAction

inline CallResult _Create_MacroTokenAction(ShareCom<_ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(MacroTokenAction, _ILexerAction, g_actionMacroToken, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
// grammar actions

// _Lex_ActionSet

struct _Lex_ActionSet
{
	_ScopeShareComObject<DoIdTokenMacroAction> actionDoIdTokenMacro;
};

// DoIdTokenMacroAction

inline CallResult _Create_DoIdTokenMacroAction(const _Lex_ActionSet& las, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoIdTokenMacroAction, _IGrammarAction, las.actionDoIdTokenMacro, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
