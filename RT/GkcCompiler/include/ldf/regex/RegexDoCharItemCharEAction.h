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
This file contains component class of Do-Char-Item-Char-E action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_ITEM_CHAR_E_ACTION_H__
#define __REGEX_DO_CHAR_ITEM_CHAR_E_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharItemCharEAction

class RegexDoCharItemCharEAction : public _IGrammarAction
{
public:
	RegexDoCharItemCharEAction() throw()
	{
	}
	~RegexDoCharItemCharEAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//get value
		ShareCom<_I_RegexCharSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[1], spU, cr);
		if( cr.IsFailed() )
			return cr;
		_RegexCharRange rcr;
		spU.Deref().GetCharRange(rcr);
		//add value
		ShareCom<_I_RegexCharSetSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSetSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		RefPtr<_RegexCharRangeSet> rs(spS.Deref().GetCharRangeSet());
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
	RegexDoCharItemCharEAction(const RegexDoCharItemCharEAction&) throw();
	RegexDoCharItemCharEAction& operator=(const RegexDoCharItemCharEAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharItemCharEAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
