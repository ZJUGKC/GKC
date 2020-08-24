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
This file contains action functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "action/CommentStartAction.h"
#include "action/LineCommentStartAction.h"
#include "action/SpaceAction.h"
#include "action/ReturnAction.h"

////////////////////////////////////////////////////////////////////////////////

namespace GKC {

//global variables

_ScopeShareComObject<CommentStartAction>      g_actionCommentStart;
_ScopeShareComObject<LineCommentStartAction>  g_actionLineCommentStart;
_ScopeShareComObject<SpaceAction>             g_actionSpace;
_ScopeShareComObject<ReturnAction>            g_actionReturn;

//functions

// CommentStartAction

inline void _Internal_CommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_SCOPE_COMPONENT_INSTANCE(CommentStartAction, _ILexerAction, g_actionCommentStart, sp, cr);
}

// LineCommentStartAction

inline void _Internal_LineCommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_SCOPE_COMPONENT_INSTANCE(LineCommentStartAction, _ILexerAction, g_actionLineCommentStart, sp, cr);
}

// SpaceAction

inline void _Internal_SpaceAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_SCOPE_COMPONENT_INSTANCE(SpaceAction, _ILexerAction, g_actionSpace, sp, cr);
}

// ReturnAction

inline void _Internal_ReturnAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_SCOPE_COMPONENT_INSTANCE(ReturnAction, _ILexerAction, g_actionReturn, sp, cr);
}

} //namespace

// CommentStartAction

void _CommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_CommentStartAction_Create(sp, cr);
}

// LineCommentStartAction

void _LineCommentStartAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_LineCommentStartAction_Create(sp, cr);
}

// SpaceAction

void _SpaceAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_SpaceAction_Create(sp, cr);
}

// ReturnAction

void _ReturnAction_Create(GKC::ShareCom<_ILexerAction>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_ReturnAction_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
