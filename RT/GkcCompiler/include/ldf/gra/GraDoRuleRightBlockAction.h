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
This file contains component class of Do-Rule-Right-Block action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_RULE_RIGHT_BLOCK_ACTION_H__
#define __GRA_DO_RULE_RIGHT_BLOCK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoRuleRightBlockAction

class GraDoRuleRightBlockAction : public _IGrammarAction,
								public _GraDataActionBase
{
public:
	GraDoRuleRightBlockAction() throw()
	{
	}
	~GraDoRuleRightBlockAction() throw()
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
		//left part
		StringA str(arrSymbol[1].get_Value().Deref().get_Buffer());
		uint uID = m_data.Deref().AddToNonterminalTable(StringUtilHelper::To_ConstString(str));  //may throw
		//start index
		ShareCom<_I_GraPositionSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraPositionSymbolData_Utility, arrSymbol[3].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		uintptr uPos = spU.Deref().GetPosition();
		//loop
		_GraRuleSet& rs = m_data.Deref().GetRuleSet();
		for( uintptr i = uPos; i < rs.GetCount(); i ++ )
			rs.GetRule(i).GetToken(0).SetID(uID);
		return cr;
	}

private:
	//noncopyable
	GraDoRuleRightBlockAction(const GraDoRuleRightBlockAction&) throw();
	GraDoRuleRightBlockAction& operator=(const GraDoRuleRightBlockAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoRuleRightBlockAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
