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
This file contains component class of Do-Id-Macro action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_ID_MACRO_ACTION_H__
#define __GRA_DO_ID_MACRO_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoIdMacroAction

class GraDoIdMacroAction : public _IGrammarAction,
						public _GraDataActionBase
{
public:
	GraDoIdMacroAction() throw()
	{
	}
	~GraDoIdMacroAction() throw()
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
		StringA str(arrSymbol[1].get_Value().Deref().get_Buffer());
		//nonterminal
		uint uID = m_data.Deref().AddToNonterminalTable(StringUtilHelper::To_ConstString(str));  //may throw
		//left part
		ShareCom<_I_GraTokenSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_GraTokenSymbolData_Utility, arrSymbol[0].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		_GraToken tk;
		tk.SetID(uID);
		spU.Deref().SetToken(tk);
		return cr;
	}

private:
	//noncopyable
	GraDoIdMacroAction(const GraDoIdMacroAction&) throw();
	GraDoIdMacroAction& operator=(const GraDoIdMacroAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoIdMacroAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
