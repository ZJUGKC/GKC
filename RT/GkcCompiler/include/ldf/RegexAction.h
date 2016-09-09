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

// RegexCharAction

inline CallResult _Create_RegexCharAction(ShareCom<_ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexCharAction, _ILexerAction, sp, cr);
	return cr;
}

// RegexCharSymbolDataFactory

inline CallResult _Create_RegexCharSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	return USE_COM_FACTORY_CLASS_NAME(RegexCharSymbolData)::Create(sp);
}

// RegexCharSetSymbolDataFactory

inline CallResult _Create_RegexCharSetSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	return USE_COM_FACTORY_CLASS_NAME(RegexCharSetSymbolData)::Create(sp);
}

// RegexPositionSymbolDataFactory

inline CallResult _Create_RegexPositionSymbolDataFactory(ShareCom<IComFactory>& sp) throw()
{
	return USE_COM_FACTORY_CLASS_NAME(RegexPositionSymbolData)::Create(sp);
}

// RegexDoCharAction

inline CallResult _Create_RegexDoCharAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharSAction

inline CallResult _Create_RegexDoCharSAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharSAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharRangeAction

inline CallResult _Create_RegexDoCharRangeAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharRangeAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharItemCharEAction

inline CallResult _Create_RegexDoCharItemCharEAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharItemCharEAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharItemItemCharEAction

inline CallResult _Create_RegexDoCharItemItemCharEAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharItemItemCharEAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharSetAction

inline CallResult _Create_RegexDoCharSetAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharSetAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoCharSetUpAction

inline CallResult _Create_RegexDoCharSetUpAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoCharSetUpAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactorCharSetAction

inline CallResult _Create_RegexDoFactorCharSetAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactorCharSetAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactorCharAction

inline CallResult _Create_RegexDoFactorCharAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactorCharAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactorParenExpAction

inline CallResult _Create_RegexDoFactorParenExpAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactorParenExpAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactor1QuestionAction

inline CallResult _Create_RegexDoFactor1QuestionAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactor1QuestionAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactor1StarAction

inline CallResult _Create_RegexDoFactor1StarAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactor1StarAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoFactor1PlusAction

inline CallResult _Create_RegexDoFactor1PlusAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoFactor1PlusAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoTermTermFactor1Action

inline CallResult _Create_RegexDoTermTermFactor1Action(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoTermTermFactor1Action, _IGrammarAction, sp, cr);
	return cr;
}

// RegexDoExpExpTermAction

inline CallResult _Create_RegexDoExpExpTermAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexDoExpExpTermAction, _IGrammarAction, sp, cr);
	return cr;
}

// RegexGrammarAccepted

inline CallResult _Create_RegexGrammarAccepted(ShareCom<_IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(RegexGrammarAccepted, _IGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
