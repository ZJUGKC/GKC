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

//------------------------------------------------------------------------------
// Factories

// global variables

extern _ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(GraTokenSymbolData)>     g_factoryGraTokenSymbolData;
extern _ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(GraPositionSymbolData)>  g_factoryGraPositionSymbolData;

// GraTokenSymbolDataFactory

inline CallResult _Create_GraTokenSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(USE_COM_FACTORY_CLASS_NAME(GraTokenSymbolData), IComFactory, g_factoryGraTokenSymbolData, sp, cr);
	return cr;
}

// GraPositionSymbolDataFactory

inline CallResult _Create_GraPositionSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(USE_COM_FACTORY_CLASS_NAME(GraPositionSymbolData), IComFactory, g_factoryGraPositionSymbolData, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
// Actions

// _Gra_ActionSet

struct _Gra_ActionSet
{
	_ScopeShareComObject<GraDoIdTokenAction> actionGraDoIdToken;
	_ScopeShareComObject<GraDoIdMacroAction> actionGraDoIdMacro;
	_ScopeShareComObject<GraDoIdAction> actionGraDoId;
	_ScopeShareComObject<GraDoIdBlockAction> actionGraDoIdBlock;
	_ScopeShareComObject<GraDoRightIdBlockAction> actionGraDoRightIdBlock;
	_ScopeShareComObject<GraDoRightAction> actionGraDoRight;
	_ScopeShareComObject<GraDoRightBlockAction> actionGraDoRightBlock;
	_ScopeShareComObject<GraDoRuleRightBlockAction> actionGraDoRuleRightBlock;
};

// GraDoIdTokenAction

inline CallResult _Create_GraDoIdTokenAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoIdTokenAction, _IGrammarAction, gas.actionGraDoIdToken, sp, cr);
	return cr;
}

// GraDoIdMacroAction

inline CallResult _Create_GraDoIdMacroAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoIdMacroAction, _IGrammarAction, gas.actionGraDoIdMacro, sp, cr);
	return cr;
}

// GraDoIdAction

inline CallResult _Create_GraDoIdAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoIdAction, _IGrammarAction, gas.actionGraDoId, sp, cr);
	return cr;
}

// GraDoIdBlockAction

inline CallResult _Create_GraDoIdBlockAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoIdBlockAction, _IGrammarAction, gas.actionGraDoIdBlock, sp, cr);
	return cr;
}

// GraDoRightIdBlockAction

inline CallResult _Create_GraDoRightIdBlockAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoRightIdBlockAction, _IGrammarAction, gas.actionGraDoRightIdBlock, sp, cr);
	return cr;
}

// GraDoRightAction

inline CallResult _Create_GraDoRightAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoRightAction, _IGrammarAction, gas.actionGraDoRight, sp, cr);
	return cr;
}

// GraDoRightBlockAction

inline CallResult _Create_GraDoRightBlockAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoRightBlockAction, _IGrammarAction, gas.actionGraDoRightBlock, sp, cr);
	return cr;
}

// GraDoRuleRightBlockAction

inline CallResult _Create_GraDoRuleRightBlockAction(const _Gra_ActionSet& gas, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(GraDoRuleRightBlockAction, _IGrammarAction, gas.actionGraDoRuleRightBlock, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
