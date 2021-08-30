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
This file contains component class of Do-Right action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_RIGHT_ACTION_H__
#define __GRA_DO_RIGHT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoRightAction

class GraDoRightAction : public _IGrammarAction,
						public _GraDataActionBase
{
public:
	GraDoRightAction() throw()
	{
	}
	~GraDoRightAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		ShareCom<_I_GraPositionSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraPositionSymbolData_Utility, arrSymbol[0], spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetPosition(m_data.Deref().GetRuleSet().GetCount() - 1);
		return cr;
	}

private:
	//noncopyable
	GraDoRightAction(const GraDoRightAction&) throw();
	GraDoRightAction& operator=(const GraDoRightAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoRightAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
