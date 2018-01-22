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

// WmarkCommentStartAction

inline CallResult _Create_WmarkCommentStartAction(ShareCom<ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WmarkCommentStartAction, ILexerAction, sp, cr);
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
