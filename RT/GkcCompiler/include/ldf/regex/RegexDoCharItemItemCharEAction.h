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
This file contains component class of Do-Char-Item-Item-Char-E action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_ITEM_ITEM_CHAR_E_ACTION_H__
#define __REGEX_DO_CHAR_ITEM_ITEM_CHAR_E_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharItemItemCharEAction

class RegexDoCharItemItemCharEAction : public _IGrammarAction
{
public:
	RegexDoCharItemItemCharEAction() throw()
	{
	}
	~RegexDoCharItemItemCharEAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//get value
		ShareCom<_I_RegexCharSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[2], spU, cr);
		if( cr.IsFailed() )
			return cr;
		_RegexCharRange rcr;
		spU.Deref().GetCharRange(rcr);
		//move
		ShareCom<_I_RegexCharSetSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSetSymbolData_Utility, arrSymbol[1], spS, cr);
		if( cr.IsFailed() )
			return cr;
		RefPtr<_RegexCharRangeSet> rs1(spS.Deref().GetCharRangeSet());
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSetSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		RefPtr<_RegexCharRangeSet> rs(spS.Deref().GetCharRangeSet());
		rs1.Deref().MoveTo(rs.Deref());
		//add value
		try {
			rs.Deref().AddRangeByCombination(rcr);  //may throw
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
	//noncopyable
	RegexDoCharItemItemCharEAction(const RegexDoCharItemItemCharEAction&) throw();
	RegexDoCharItemItemCharEAction& operator=(const RegexDoCharItemItemCharEAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharItemItemCharEAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
