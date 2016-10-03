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
#ifndef __GRA_ACTION_H__
#define __GRA_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraTokenSymbolDataFactory

inline CallResult _Create_GraTokenSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	return USE_COM_FACTORY_CLASS_NAME(GraTokenSymbolData)::Create(sp);
}

// GraPositionSymbolDataFactory

inline CallResult _Create_GraPositionSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	return USE_COM_FACTORY_CLASS_NAME(GraPositionSymbolData)::Create(sp);
}

// GraDoIdTokenAction

inline CallResult _Create_GraDoIdTokenAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoIdTokenAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoIdMacroAction

inline CallResult _Create_GraDoIdMacroAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoIdMacroAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoIdAction

inline CallResult _Create_GraDoIdAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoIdAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoIdBlockAction

inline CallResult _Create_GraDoIdBlockAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoIdBlockAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoRightIdBlockAction

inline CallResult _Create_GraDoRightIdBlockAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoRightIdBlockAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoRightAction

inline CallResult _Create_GraDoRightAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoRightAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoRightBlockAction

inline CallResult _Create_GraDoRightBlockAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoRightBlockAction, _IGrammarAction, sp, cr);
	return cr;
}

// GraDoRuleRightBlockAction

inline CallResult _Create_GraDoRuleRightBlockAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(GraDoRuleRightBlockAction, _IGrammarAction, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
