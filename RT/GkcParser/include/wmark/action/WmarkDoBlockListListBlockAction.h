/*
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
This file contains component class of Do-Block-List-List-Block action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_DO_BLOCK_LIST_LIST_BLOCK_ACTION_H__
#define __WMARK_DO_BLOCK_LIST_LIST_BLOCK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkDoBlockListListBlockAction

class WmarkDoBlockListListBlockAction : public IGrammarAction,
										public _CplMetaDataActionBase
{
public:
	WmarkDoBlockListListBlockAction() throw()
	{
	}
	~WmarkDoBlockListListBlockAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//get positions
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[1].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		CplMetaDataPosition pos(spU.Deref().GetPosition());
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[3].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		CplMetaDataPosition posHead(spU.Deref().GetPosition());
		//set
		m_spMeta.Deref().SetAstNext(posHead, pos);
		//set position
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(posHead);
		return cr;
	}

private:
	//noncopyable
	WmarkDoBlockListListBlockAction(const WmarkDoBlockListListBlockAction&) throw();
	WmarkDoBlockListListBlockAction& operator=(const WmarkDoBlockListListBlockAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkDoBlockListListBlockAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
