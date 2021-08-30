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
This file contains parser functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "base/ParserDef.h"

//Wlang

#include "wlang/base/WlangDef.h"
#include "wlang/base/WlangLex.h"
#include "wlang/base/WlangGra.h"

#include "wlang/action/WlangGrammarError.h"
#include "wlang/action/WlangGrammarAccepted.h"
#include "wlang/action/WlangDoNsBodyAction.h"
#include "wlang/action/WlangDoBodySemiAction.h"

#include "wlang/WlangAction.h"
#include "wlang/WlangUtility.h"
#include "wlang/WlangParser.h"

//Won

#include "won/base/WonDef.h"
#include "won/base/WonLex.h"
#include "won/base/WonGra.h"

#include "won/action/WonDoCommonAction.h"
#include "won/action/WonDoValueStringCatenationAction.h"
#include "won/action/WonDoStringCatenationAction.h"
#include "won/action/WonDoStringCatenationStringAction.h"

#include "won/WonAction.h"
#include "won/WonUtility.h"
#include "won/WonParser.h"

//Wmark

#include "wmark/base/WmarkDef.h"
#include "wmark/base/WmarkLex.h"
#include "wmark/base/WmarkGra.h"

#include "wmark/action/WmarkReturnAction.h"
#include "wmark/action/WmarkCommentStartAction.h"
#include "wmark/action/WmarkGrammarAccepted.h"
#include "wmark/action/WmarkDoDocRsBlockListAction.h"
#include "wmark/action/WmarkDoBlockListListBlockAction.h"
#include "wmark/action/WmarkDoBlockBlockBodyAction.h"
#include "wmark/action/WmarkDoBlockBodyCommentAction.h"
#include "wmark/action/WmarkDoBlockBodyIndentAction.h"
#include "wmark/action/WmarkDoBlockBodyTextAction.h"

#include "wmark/WmarkAction.h"
#include "wmark/WmarkUtility.h"
#include "wmark/WmarkParser.h"

////////////////////////////////////////////////////////////////////////////////

//functions

namespace GKC {

// WlangUtility

inline void _Internal_WlangUtility_Create(GKC::ShareCom<_IWlangUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WlangUtility, _IWlangUtility, sp, cr);
}

// WlangParser

inline void _Internal_WlangParser_Create(GKC::ShareCom<_IWlangParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WlangParser, _IWlangParser, sp, cr);
}

// WonUtility

inline void _Internal_WonUtility_Create(GKC::ShareCom<_IWonUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WonUtility, _IWonUtility, sp, cr);
}

// WonParser

inline void _Internal_WonParser_Create(GKC::ShareCom<_IWonParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WonParser, _IWonParser, sp, cr);
}

// Parse won to meta data

inline void _delete_quotes(StringA& str) throw()
{
	assert( str.GetLength() >= 2 );
	StringOpHelper::Delete(str.GetLength() - 1, 1, str);
	StringOpHelper::Delete(0, 1, str);
}
inline bool _add_node(ShareCom<ICplMetaData>& spM, const ConstStringA& cstrToken, int iType, const CplMetaDataPosition& posCur, CplMetaDataPosition& pos, CallResult& cr) throw()
{
	CplMetaDataPosition posSymbol(spM.Deref().Find(cstrToken));
	if( posSymbol.IsNull() ) {
		cr = spM.Deref().InsertSymbol(cstrToken, 0, false, posSymbol);
		if( cr.IsFailed() )
			return false;
	}
	cr = spM.Deref().InsertAstNode((uint)iType, pos);
	if( cr.IsFailed() )
		return false;
	spM.Deref().SetAstData(pos, posSymbol);
	spM.Deref().SetAstParent(pos, posCur);
	CplMetaDataPosition posParent(posCur);
	CplAstNodeInfo info;
	if( posCur.IsNull() ) {
		spM.Deref().GetAstNodeInfo(pos, info);
		posParent = info.posParent;
		assert( !posParent.IsNull() );
	}
	spM.Deref().GetAstNodeInfo(posParent, info);
	if( info.uType == _WonTokenTypes::BeginObject || info.uType == _WonTokenTypes::BeginArray ) {
		if( info.posChild.IsNull() ) {
			spM.Deref().SetAstChild(posParent, pos);
		}
		else {
			CplMetaDataPosition pos1(info.posChild);
			spM.Deref().GetAstNodeInfo(pos1, info);
			CplMetaDataPosition pos2(info.posNext);
			while( !pos2.IsNull() ) {
				spM.Deref().GetAstNodeInfo(pos2, info);
				pos1 = pos2;
				pos2 = info.posNext;
			}
			spM.Deref().SetAstNext(pos1, pos);
		}
	}
	else {
		assert( info.posChild.IsNull() );
		spM.Deref().SetAstChild(posParent, pos);
	}
	return true;
}
inline void _leave_current_level(ShareCom<ICplMetaData>& spM, CplMetaDataPosition& pos) throw()
{
	assert( !pos.IsNull() );
	CplAstNodeInfo info;
	spM.Deref().GetAstNodeInfo(pos, info);
	assert( !info.posParent.IsNull() );
	pos = info.posParent;
}

inline void _Internal_Won_Parse(const GKC::ShareCom<_IWonParser>& spParser, const GKC::ShareCom<GKC::ICplMetaData>& spMeta, GKC::CallResult& cr) throw()
{
	ShareCom<_IWonParser> spP(spParser);
	ShareCom<ICplMetaData> spM(spMeta);
	//start
	cr = spP.Deref().Start();
	if( cr.IsFailed() )
		return ;
	//loop
	CplMetaDataPosition posCurrent;
	CplMetaDataPosition pos;
	StringA strToken;
	while( true ) {
		int iType;
		cr = spP.Deref().GetNextToken(iType, strToken);
		if( cr.IsFailed() )
			return ;
		if( cr.GetResult() == SystemCallResults::S_False )
			break;
		//types
		switch( iType ) {
		case _WonTokenTypes::ValueString:
			_delete_quotes(strToken);
		case _WonTokenTypes::ValueNumberInteger:
		case _WonTokenTypes::ValueNumberFloat:
		case _WonTokenTypes::ValueNumberHexadecimal:
		case _WonTokenTypes::ValueBooleanTrue:
		case _WonTokenTypes::ValueBooleanFalse:
		case _WonTokenTypes::ValueNull:
			if( !_add_node(spM, StringUtilHelper::To_ConstString(strToken), iType, posCurrent, pos, cr) )
				return ;
			break;
		case _WonTokenTypes::Key:
			_delete_quotes(strToken);
		case _WonTokenTypes::BeginObject:
		case _WonTokenTypes::BeginArray:
			if( !_add_node(spM, StringUtilHelper::To_ConstString(strToken), iType, posCurrent, pos, cr) )
				return ;
			//enter
			posCurrent = pos;
			break;
		case _WonTokenTypes::EndObject:
		case _WonTokenTypes::EndPair:
		case _WonTokenTypes::EndArray:
			_leave_current_level(spM, posCurrent);
			break;
		default:
			cr.SetResult(SystemCallResults::Fail);
			return ;
			break;
		}
	} //end while
	//won
	ConstStringA cstr(DECLARE_TEMP_CONST_STRING(ConstStringA, WON_ROOT_SYMBOL));
	pos = spM.Deref().Find(cstr);
	if( pos.IsNull() ) {
		cr = spM.Deref().InsertSymbol(cstr, 0, false, pos);
		if( cr.IsFailed() )
			return ;
	}
	spM.Deref().SetData(pos, spM.Deref().GetAstStart());
}

// WmarkUtility

inline void _Internal_WmarkUtility_Create(GKC::ShareCom<_IWmarkUtility>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WmarkUtility, _IWmarkUtility, sp, cr);
}

// WmarkParser

inline void _Internal_WmarkParser_Create(GKC::ShareCom<_IWmarkParser>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(WmarkParser, _IWmarkParser, sp, cr);
}

} //namespace

// WlangUtility

void _WlangUtility_Create(GKC::ShareCom<_IWlangUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WlangUtility_Create(sp, cr);
}

// WlangParser

void _WlangParser_Create(GKC::ShareCom<_IWlangParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WlangParser_Create(sp, cr);
}

// WonUtility

void _WonUtility_Create(GKC::ShareCom<_IWonUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WonUtility_Create(sp, cr);
}

// WonParser

void _WonParser_Create(GKC::ShareCom<_IWonParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WonParser_Create(sp, cr);
}

// Parse won to meta data
void _Won_Parse(const GKC::ShareCom<_IWonParser>& spParser, const GKC::ShareCom<GKC::ICplMetaData>& spMeta, GKC::CallResult& cr) throw()
{
	GKC::_Internal_Won_Parse(spParser, spMeta, cr);
}

// WmarkUtility

void _WmarkUtility_Create(GKC::ShareCom<_IWmarkUtility>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WmarkUtility_Create(sp, cr);
}

// WmarkParser

void _WmarkParser_Create(GKC::ShareCom<_IWmarkParser>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_WmarkParser_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
