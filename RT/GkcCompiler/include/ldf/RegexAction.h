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
#ifndef __REGEX_ACTION_H__
#define __REGEX_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// lexer actions

extern _ScopeShareComObject<RegexCharAction> g_actionRegexChar;

// RegexCharAction

inline CallResult _Create_RegexCharAction(ShareCom<_ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexCharAction, _ILexerAction, g_actionRegexChar, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
// factories

extern _ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexCharSymbolData)> g_factoryRegexCharSymbolData;
extern _ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexCharSetSymbolData)> g_factoryRegexCharSetSymbolData;
extern _ScopeShareComObject<USE_COM_FACTORY_CLASS_NAME(RegexPositionSymbolData)> g_factoryRegexPositionSymbolData;

// RegexCharSymbolDataFactory

inline CallResult _Create_RegexCharSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(USE_COM_FACTORY_CLASS_NAME(RegexCharSymbolData), IComFactory, g_factoryRegexCharSymbolData, sp, cr);
	return cr;
}

// RegexCharSetSymbolDataFactory

inline CallResult _Create_RegexCharSetSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(USE_COM_FACTORY_CLASS_NAME(RegexCharSetSymbolData), IComFactory, g_factoryRegexCharSetSymbolData, sp, cr);
	return cr;
}

// RegexPositionSymbolDataFactory

inline CallResult _Create_RegexPositionSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(USE_COM_FACTORY_CLASS_NAME(RegexPositionSymbolData), IComFactory, g_factoryRegexPositionSymbolData, sp, cr);
	return cr;
}

//------------------------------------------------------------------------------
// grammar actions

// _Regex_ActionSet

struct _Regex_ActionSet
{
	_ScopeShareComObject<RegexDoCharAction> actionRegexDoChar;
	_ScopeShareComObject<RegexDoCharSAction> actionRegexDoCharS;
	_ScopeShareComObject<RegexDoCharRangeAction> actionRegexDoCharRange;
	_ScopeShareComObject<RegexDoCharItemCharEAction> actionRegexDoCharItemCharE;
	_ScopeShareComObject<RegexDoCharItemItemCharEAction> actionRegexDoCharItemItemCharE;
	_ScopeShareComObject<RegexDoCharSetAction> actionRegexDoCharSet;
	_ScopeShareComObject<RegexDoCharSetUpAction> actionRegexDoCharSetUp;
	_ScopeShareComObject<RegexDoFactorCharSetAction> actionRegexDoFactorCharSet;
	_ScopeShareComObject<RegexDoFactorCharAction> actionRegexDoFactorChar;
	_ScopeShareComObject<RegexDoFactorParenExpAction> actionRegexDoFactorParenExp;
	_ScopeShareComObject<RegexDoFactor1QuestionAction> actionRegexDoFactor1Question;
	_ScopeShareComObject<RegexDoFactor1StarAction> actionRegexDoFactor1Star;
	_ScopeShareComObject<RegexDoFactor1PlusAction> actionRegexDoFactor1Plus;
	_ScopeShareComObject<RegexDoTermTermFactor1Action> actionRegexDoTermTermFactor1;
	_ScopeShareComObject<RegexDoExpExpTermAction> actionRegexDoExpExpTerm;
	_ScopeShareComObject<RegexGrammarAccepted> actionRegexGrammarAccepted;
};

// RegexDoCharAction

inline CallResult _Create_RegexDoCharAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharAction, _IGrammarAction, ras.actionRegexDoChar, sp, cr);
	return cr;
}

// RegexDoCharSAction

inline CallResult _Create_RegexDoCharSAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharSAction, _IGrammarAction, ras.actionRegexDoCharS, sp, cr);
	return cr;
}

// RegexDoCharRangeAction

inline CallResult _Create_RegexDoCharRangeAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharRangeAction, _IGrammarAction, ras.actionRegexDoCharRange, sp, cr);
	return cr;
}

// RegexDoCharItemCharEAction

inline CallResult _Create_RegexDoCharItemCharEAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharItemCharEAction, _IGrammarAction, ras.actionRegexDoCharItemCharE, sp, cr);
	return cr;
}

// RegexDoCharItemItemCharEAction

inline CallResult _Create_RegexDoCharItemItemCharEAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharItemItemCharEAction, _IGrammarAction, ras.actionRegexDoCharItemItemCharE, sp, cr);
	return cr;
}

// RegexDoCharSetAction

inline CallResult _Create_RegexDoCharSetAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharSetAction, _IGrammarAction, ras.actionRegexDoCharSet, sp, cr);
	return cr;
}

// RegexDoCharSetUpAction

inline CallResult _Create_RegexDoCharSetUpAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoCharSetUpAction, _IGrammarAction, ras.actionRegexDoCharSetUp, sp, cr);
	return cr;
}

// RegexDoFactorCharSetAction

inline CallResult _Create_RegexDoFactorCharSetAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactorCharSetAction, _IGrammarAction, ras.actionRegexDoFactorCharSet, sp, cr);
	return cr;
}

// RegexDoFactorCharAction

inline CallResult _Create_RegexDoFactorCharAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactorCharAction, _IGrammarAction, ras.actionRegexDoFactorChar, sp, cr);
	return cr;
}

// RegexDoFactorParenExpAction

inline CallResult _Create_RegexDoFactorParenExpAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactorParenExpAction, _IGrammarAction, ras.actionRegexDoFactorParenExp, sp, cr);
	return cr;
}

// RegexDoFactor1QuestionAction

inline CallResult _Create_RegexDoFactor1QuestionAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactor1QuestionAction, _IGrammarAction, ras.actionRegexDoFactor1Question, sp, cr);
	return cr;
}

// RegexDoFactor1StarAction

inline CallResult _Create_RegexDoFactor1StarAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactor1StarAction, _IGrammarAction, ras.actionRegexDoFactor1Star, sp, cr);
	return cr;
}

// RegexDoFactor1PlusAction

inline CallResult _Create_RegexDoFactor1PlusAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoFactor1PlusAction, _IGrammarAction, ras.actionRegexDoFactor1Plus, sp, cr);
	return cr;
}

// RegexDoTermTermFactor1Action

inline CallResult _Create_RegexDoTermTermFactor1Action(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoTermTermFactor1Action, _IGrammarAction, ras.actionRegexDoTermTermFactor1, sp, cr);
	return cr;
}

// RegexDoExpExpTermAction

inline CallResult _Create_RegexDoExpExpTermAction(const _Regex_ActionSet& ras, ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexDoExpExpTermAction, _IGrammarAction, ras.actionRegexDoExpExpTerm, sp, cr);
	return cr;
}

// RegexGrammarAccepted

inline CallResult _Create_RegexGrammarAccepted(const _Regex_ActionSet& ras, ShareCom<_IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(RegexGrammarAccepted, _IGrammarAccepted, ras.actionRegexGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
