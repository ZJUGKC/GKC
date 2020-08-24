/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains Wmark action functions.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//lexer functions

extern _ScopeShareComObject<WmarkReturnAction> g_actionWmarkReturn;
extern _ScopeShareComObject<WmarkCommentStartAction> g_actionWmarkCommentStart;

// WmarkReturnAction

inline CallResult _Create_WmarkReturnAction(ShareCom<ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkReturnAction, ILexerAction, g_actionWmarkReturn, sp, cr);
	return cr;
}

// WmarkCommentStartAction

inline CallResult _Create_WmarkCommentStartAction(ShareCom<ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkCommentStartAction, ILexerAction, g_actionWmarkCommentStart, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
//grammar functions

// _Wmark_ActionSet

struct _Wmark_ActionSet
{
	_ScopeShareComObject<WmarkDoDocRsBlockListAction> actionWmarkDoDocRsBlockList;
	_ScopeShareComObject<WmarkDoBlockListListBlockAction> actionWmarkDoBlockListListBlock;
	_ScopeShareComObject<WmarkDoBlockBlockBodyAction> actionWmarkDoBlockBlockBody;
	_ScopeShareComObject<WmarkDoBlockBodyCommentAction> actionWmarkDoBlockBodyComment;
	_ScopeShareComObject<WmarkDoBlockBodyIndentAction> actionWmarkDoBlockBodyIndent;
	_ScopeShareComObject<WmarkGrammarAccepted> actionWmarkGrammarAccepted;
};

// Do-Doc-Rs-Block-List action

inline CallResult _Create_WmarkDoDocRsBlockListAction(const _Wmark_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkDoDocRsBlockListAction, IGrammarAction, was.actionWmarkDoDocRsBlockList, sp, cr);
	return cr;
}

// Do-Block-List-List-Block action

inline CallResult _Create_WmarkDoBlockListListBlockAction(const _Wmark_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkDoBlockListListBlockAction, IGrammarAction, was.actionWmarkDoBlockListListBlock, sp, cr);
	return cr;
}

// Do-Block-Block-Body action

inline CallResult _Create_WmarkDoBlockBlockBodyAction(const _Wmark_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkDoBlockBlockBodyAction, IGrammarAction, was.actionWmarkDoBlockBlockBody, sp, cr);
	return cr;
}

// Do-Block-Body-Comment action

inline CallResult _Create_WmarkDoBlockBodyCommentAction(const _Wmark_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkDoBlockBodyCommentAction, IGrammarAction, was.actionWmarkDoBlockBodyComment, sp, cr);
	return cr;
}

// Do-Block-Body-Indent action

inline CallResult _Create_WmarkDoBlockBodyIndentAction(const _Wmark_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkDoBlockBodyIndentAction, IGrammarAction, was.actionWmarkDoBlockBodyIndent, sp, cr);
	return cr;
}

// grammar accepted

inline CallResult _Create_WmarkGrammarAccepted(const _Wmark_ActionSet& was, ShareCom<IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WmarkGrammarAccepted, IGrammarAccepted, was.actionWmarkGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
