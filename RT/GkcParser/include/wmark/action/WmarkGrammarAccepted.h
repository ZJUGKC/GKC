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
This file contains component class of WMARK grammar accepted action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_GRAMMAR_ACCEPTED_H__
#define __WMARK_GRAMMAR_ACCEPTED_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkGrammarAccepted

class WmarkGrammarAccepted : public IGrammarAccepted,
							public _CplMetaDataActionBase
{
public:
	WmarkGrammarAccepted() throw()
	{
	}
	~WmarkGrammarAccepted() throw()
	{
	}

// IGrammarAccepted methods
	virtual GKC::CallResult DoAccepted(INOUT GKC::ShareCom<_IGrammarSymbolData>& sym) throw()
	{
		CallResult cr;
		//reverse link
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, sym, spU, cr);
		if( cr.IsFailed() )
			return cr;
		CplMetaDataPosition pos(spU.Deref().GetPosition());
		pos = m_spMeta.Deref().ReverseAstLink(pos);
		//root
		m_spMeta.Deref().SetAstLinkParent(pos, CplMetaDataPosition());
		m_spMeta.Deref().SetAstChild(CplMetaDataPosition(), pos);
		//start
		pos = m_spMeta.Deref().GetAstStart();
		m_spMeta.Deref().ResetAst();
		//symbol
		CplMetaDataPosition posDoc;
		cr = m_spMeta.Deref().InsertSymbol(DECLARE_TEMP_CONST_STRING(ConstStringA, WMARK_ROOT_SYMBOL), 0, false, posDoc);
		if( cr.IsFailed() )
			return cr;
		m_spMeta.Deref().SetData(posDoc, pos);
		//finish
		m_spMeta.Deref().FinishZeroLevel(false);
		return cr;
	}

private:
	//noncopyable
	WmarkGrammarAccepted(const WmarkGrammarAccepted&) throw();
	WmarkGrammarAccepted& operator=(const WmarkGrammarAccepted&) throw();
};

DECLARE_COM_TYPECAST(WmarkGrammarAccepted)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
