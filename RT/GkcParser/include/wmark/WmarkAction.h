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

//functions

// WmarkReturnAction

inline CallResult _Create_WmarkReturnAction(ShareCom<ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkReturnAction, ILexerAction, sp, cr);
	return cr;
}

// WmarkCommentStartAction

inline CallResult _Create_WmarkCommentStartAction(ShareCom<ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkCommentStartAction, ILexerAction, sp, cr);
	return cr;
}

// Do-Doc-Rs-Block-List action

inline CallResult _Create_WmarkDoDocRsBlockListAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkDoDocRsBlockListAction, IGrammarAction, sp, cr);
	return cr;
}

// Do-Block-List-List-Block action

inline CallResult _Create_WmarkDoBlockListListBlockAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkDoBlockListListBlockAction, IGrammarAction, sp, cr);
	return cr;
}

// Do-Block-Block-Body action

inline CallResult _Create_WmarkDoBlockBlockBodyAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkDoBlockBlockBodyAction, IGrammarAction, sp, cr);
	return cr;
}

// Do-Block-Body-Indent action

inline CallResult _Create_WmarkDoBlockBodyIndentAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkDoBlockBodyIndentAction, IGrammarAction, sp, cr);
	return cr;
}

// grammar accepted

inline CallResult _Create_WmarkGrammarAccepted(ShareCom<IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkGrammarAccepted, IGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
