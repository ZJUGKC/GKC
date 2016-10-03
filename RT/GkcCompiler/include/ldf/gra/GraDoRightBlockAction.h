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
This file contains component class of Do-Right-Block action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_RIGHT_BLOCK_ACTION_H__
#define __GRA_DO_RIGHT_BLOCK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoRightBlockAction

class GraDoRightBlockAction : public _IGrammarAction
{
public:
	GraDoRightBlockAction() throw()
	{
	}
	~GraDoRightBlockAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		ShareCom<_I_GraPositionSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraPositionSymbolData_Utility, arrSymbol[1].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		uintptr uPos = spU.Deref().GetPosition();
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraPositionSymbolData_Utility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(uPos);
		return cr;
	}

private:
	//noncopyable
	GraDoRightBlockAction(const GraDoRightBlockAction&) throw();
	GraDoRightBlockAction& operator=(const GraDoRightBlockAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoRightBlockAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
