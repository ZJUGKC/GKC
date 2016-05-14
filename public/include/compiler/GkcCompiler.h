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
This file contains GkcCompiler helper classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_COMPILER_H__
#define __GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcCompiler.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

#include "_GkcCompiler.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// LEXER_CHAR_INFO
typedef _LEXER_CHAR_INFO  LEXER_CHAR_INFO;

// LexerTokenInfo
typedef _LexerTokenInfo  LexerTokenInfo;

// ILexerAction
typedef _ILexerAction  ILexerAction;

#define GUID_ILexerAction  GUID__ILexerAction

// LexerActionHelper

class LexerActionHelper
{
public:
	// comment start action
	static CallResult CreateCommentStartAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_CommentStartAction_Create(sp, cr);
		return cr;
	}
	// space action
	static CallResult CreateSpaceAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_SpaceAction_Create(sp, cr);
		return cr;
	}
	// return action
	static CallResult CreateReturnAction(ShareCom<ILexerAction>& sp) throw()
	{
		CallResult cr;
		::_ReturnAction_Create(sp, cr);
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COMPILER_H__
////////////////////////////////////////////////////////////////////////////////
