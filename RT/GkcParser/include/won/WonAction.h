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

//tools

inline CallResult _Won_SetActionData(int iType, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	ShareCom<_IWonActionUtility> spU;
	_COMPONENT_INSTANCE_INTERFACE(IGrammarAction, _IWonActionUtility, sp, spU, cr);
	if( cr.IsFailed() )
		return cr;
	spU.Deref().SetData(iType, data);
	return cr;
}

inline CallResult _Create_WonDoCommonAction(const _ScopeShareComObject<WonDoCommonAction>& sobj, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WonDoCommonAction, IGrammarAction, sobj, sp, cr);
	return cr;
}

inline CallResult _Won_CreateCommonAction(int iType, const RefPtr<_WonActionData>& data, const _ScopeShareComObject<WonDoCommonAction>& sobj, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	ShareCom<IGrammarAction> spAction;
	cr = _Create_WonDoCommonAction(sobj, spAction);
	if( cr.IsFailed() )
		return cr;
	cr = _Won_SetActionData(iType, data, spAction);
	if( cr.IsFailed() )
		return cr;
	sp = spAction;
	return cr;
}

// _Won_ActionSet

struct _Won_ActionSet
{
	_ScopeShareComObject<WonDoValueStringCatenationAction> actionWonDoValueStringCatenation;
	_ScopeShareComObject<WonDoStringCatenationAction> actionWonDoStringCatenation;
	_ScopeShareComObject<WonDoStringCatenationStringAction> actionWonDoStringCatenationString;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueNumberInteger;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueNumberFloat;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueNumberHexadecimal;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueBooleanTrue;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueBooleanFalse;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoValueNull;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoBeginObject;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoEndObject;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoEndPair;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoKey;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoBeginArray;
	_ScopeShareComObject<WonDoCommonAction> actionWonDoEndArray;
};

//functions

// Do-Value-String-Catenation action

inline CallResult _Create_WonDoValueStringCatenationAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	ShareCom<IGrammarAction> spAction;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WonDoValueStringCatenationAction, IGrammarAction, was.actionWonDoValueStringCatenation, spAction, cr);
	if( cr.IsFailed() )
		return cr;
	cr = _Won_SetActionData(_WonTokenTypes::ValueString, data, spAction);
	if( cr.IsFailed() )
		return cr;
	sp = spAction;
	return cr;
}

// Do-String-Catenation action

inline CallResult _Create_WonDoStringCatenationAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	ShareCom<IGrammarAction> spAction;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WonDoStringCatenationAction, IGrammarAction, was.actionWonDoStringCatenation, spAction, cr);
	if( cr.IsFailed() )
		return cr;
	cr = _Won_SetActionData(_WonTokenTypes::Unknown, data, spAction);
	if( cr.IsFailed() )
		return cr;
	sp = spAction;
	return cr;
}

// Do-String-Catenation-String action

inline CallResult _Create_WonDoStringCatenationStringAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	ShareCom<IGrammarAction> spAction;
	_CREATE_SCOPE_COMPONENT_INSTANCE(WonDoStringCatenationStringAction, IGrammarAction, was.actionWonDoStringCatenationString, spAction, cr);
	if( cr.IsFailed() )
		return cr;
	cr = _Won_SetActionData(_WonTokenTypes::Unknown, data, spAction);
	if( cr.IsFailed() )
		return cr;
	sp = spAction;
	return cr;
}

// Do-Value-Number-Integer

inline CallResult _Create_WonDoValueNumberIntegerAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueNumberInteger, data, was.actionWonDoValueNumberInteger, sp);
}

// Do-Value-Number-Float

inline CallResult _Create_WonDoValueNumberFloatAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueNumberFloat, data, was.actionWonDoValueNumberFloat, sp);
}

// Do-Value-Number-Hexadecimal

inline CallResult _Create_WonDoValueNumberHexadecimalAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueNumberHexadecimal, data, was.actionWonDoValueNumberHexadecimal, sp);
}

// Do-Value-Boolean-True

inline CallResult _Create_WonDoValueBooleanTrueAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueBooleanTrue, data, was.actionWonDoValueBooleanTrue, sp);
}

// Do-Value-Boolean-False

inline CallResult _Create_WonDoValueBooleanFalseAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueBooleanFalse, data, was.actionWonDoValueBooleanFalse, sp);
}

// Do-Value-Null

inline CallResult _Create_WonDoValueNullAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::ValueNull, data, was.actionWonDoValueNull, sp);
}

// Do-Begin-Object

inline CallResult _Create_WonDoBeginObjectAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::BeginObject, data, was.actionWonDoBeginObject, sp);
}

// Do-End-Object

inline CallResult _Create_WonDoEndObjectAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::EndObject, data, was.actionWonDoEndObject, sp);
}

// Do-Pair-Key-Value

inline CallResult _Create_WonDoEndPairAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::EndPair, data, was.actionWonDoEndPair, sp);
}

// Do-Key-String

inline CallResult _Create_WonDoKeyAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::Key, data, was.actionWonDoKey, sp);
}

// Do-Begin-Array

inline CallResult _Create_WonDoBeginArrayAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::BeginArray, data, was.actionWonDoBeginArray, sp);
}

// Do-End-Array

inline CallResult _Create_WonDoEndArrayAction(const _Won_ActionSet& was, const RefPtr<_WonActionData>& data, ShareCom<IGrammarAction>& sp) throw()
{
	return _Won_CreateCommonAction(_WonTokenTypes::EndArray, data, was.actionWonDoEndArray, sp);
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
