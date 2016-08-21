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

// MacroTokenAction

inline CallResult _Create_MacroTokenAction(ShareCom<_ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(MacroTokenAction, _ILexerAction, sp, cr);
	return cr;
}

// DoIdTokenMacroAction

inline CallResult _Create_DoIdTokenMacroAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(DoIdTokenMacroAction, _IGrammarAction, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
