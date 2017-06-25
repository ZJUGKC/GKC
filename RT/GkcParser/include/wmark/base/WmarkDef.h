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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_DEF_H__
#define __WMARK_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _WmarkUtility_Objects

struct _WmarkUtility_Objects
{
	//tables
	GKC::ShareCom<GKC::ILexerTables>   spLexerTables;
	GKC::ShareCom<GKC::IGrammarTables> spGrammarTables;
	//lexer actions
	GKC::ShareCom<GKC::ILexerAction> spCommentStart;
};

// _IWmarkUtility_Access

class NOVTABLE _IWmarkUtility_Access
{
public:
	virtual void GetObjects(_WmarkUtility_Objects& obj) throw() = 0;
};

DECLARE_GUID(GUID__IWmarkUtility_Access)

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
