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
#ifndef __WLANG_DEF_H__
#define __WLANG_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _WlangUtility_Objects

struct _WlangUtility_Objects
{
	//lexer actions
	GKC::ShareCom<GKC::ILexerAction> spCommentStart;
	GKC::ShareCom<GKC::ILexerAction> spLineCommentStart;
	GKC::ShareCom<GKC::ILexerAction> spSpace;
	GKC::ShareCom<GKC::ILexerAction> spReturn;
	//factory
	GKC::ShareCom<GKC::IComFactory> spBasicFactory;
	//grammar actions
	GKC::ShareCom<GKC::IGrammarError> spGrammarError;
};

// _IWlangUtility_Access

class NOVTABLE _IWlangUtility_Access
{
public:
	virtual void GetObjects(_WlangUtility_Objects& obj) throw() = 0;
};

DECLARE_GUID(GUID__IWlangUtility_Access)

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
