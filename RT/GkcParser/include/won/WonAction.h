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
This file contains Won action functions.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Won_ActionSet

struct _Won_ActionSet
{
	_ScopeShareComObject<WonDoCommonAction> actionWonDoCommon;
};

//functions

// Do-Common action

inline CallResult _Create_WonDoCommonAction(const _Won_ActionSet& was, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WonDoCommonAction, IGrammarAction, was.actionWonDoCommon, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
