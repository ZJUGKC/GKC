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
This file contains component class of Do-Doc-Rs-Block-List action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_DO_DOC_RS_BLOCK_LIST_ACTION_H__
#define __WMARK_DO_DOC_RS_BLOCK_LIST_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkDoDocRsBlockListAction

class WmarkDoDocRsBlockListAction : public IGrammarAction
{
public:
	WmarkDoDocRsBlockListAction() throw()
	{
	}
	~WmarkDoDocRsBlockListAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		//get position
		ShareCom<_ICplMetaDataPositionSymbolDataUtility> spU;
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[2].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		CplMetaDataPosition pos(spU.Deref().GetPosition());
		//set position
		_COMPONENT_INSTANCE_INTERFACE(IGrammarSymbolData, _ICplMetaDataPositionSymbolDataUtility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(pos);
		return cr;
	}

private:
	//noncopyable
	WmarkDoDocRsBlockListAction(const WmarkDoDocRsBlockListAction&) throw();
	WmarkDoDocRsBlockListAction& operator=(const WmarkDoDocRsBlockListAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkDoDocRsBlockListAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
