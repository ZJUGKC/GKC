﻿/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
This file contains component class of Do-Block-Body-Indent action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_DO_BLOCK_BODY_INDENT_ACTION_H__
#define __WMARK_DO_BLOCK_BODY_INDENT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkDoBlockBodyIndentAction

class WmarkDoBlockBodyIndentAction : public IGrammarAction,
									public _CplMetaDataActionBase
{
public:
	WmarkDoBlockBodyIndentAction() throw()
	{
	}
	~WmarkDoBlockBodyIndentAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			cr = do_action(arrSymbol, errorArray);  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

private:
	//do action
	CallResult do_action(ShareArray<ShareCom<IGrammarSymbolData>>& arrSymbol, ShareArray<StringS>& errorArray)
	{
		//indent
		CallResult cr;
		CplMetaDataPosition pos;
		cr = m_spMeta.Deref().InsertAstNode(WMARK_NOUN_INDENT, pos);
		if( cr.IsFailed() ) {
			StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			_CplErrorStringHelper::GenerateError(cr, strError);  //may throw
			errorArray.Add(strError);  //may throw
			return cr;
		}
		//set position
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[0], spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(pos);
		return cr;
	}

private:
	//noncopyable
	WmarkDoBlockBodyIndentAction(const WmarkDoBlockBodyIndentAction&) throw();
	WmarkDoBlockBodyIndentAction& operator=(const WmarkDoBlockBodyIndentAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkDoBlockBodyIndentAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
