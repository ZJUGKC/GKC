﻿/*
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
This file contains component class of Do-Id-Block action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_ID_BLOCK_ACTION_H__
#define __GRA_DO_ID_BLOCK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoIdBlockAction

class GraDoIdBlockAction : public _IGrammarAction,
						public _GraDataActionBase
{
public:
	GraDoIdBlockAction() throw()
	{
	}
	~GraDoIdBlockAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			cr = do_action(arrSymbol);  //may throw
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
	CallResult do_action(ShareArray<ShareCom<_IGrammarSymbolData>>& arrSymbol)
	{
		CallResult cr;
		//token
		ShareCom<_I_GraTokenSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraTokenSymbolData_Utility, arrSymbol[2].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		_GraToken tk;
		spU.Deref().GetToken(tk);
		//rule
		_GraRuleSet& rs = m_data.Deref().GetRuleSet();
		rs.GetRule(rs.GetCount() - 1).Add(tk);  //may throw
		return cr;
	}

private:
	//noncopyable
	GraDoIdBlockAction(const GraDoIdBlockAction&) throw();
	GraDoIdBlockAction& operator=(const GraDoIdBlockAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoIdBlockAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
