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
This file contains component class of Do-Char-S action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_S_ACTION_H__
#define __REGEX_DO_CHAR_S_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharSAction

class RegexDoCharSAction : public _IGrammarAction
{
public:
	RegexDoCharSAction() throw()
	{
	}
	~RegexDoCharSAction() throw()
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
		//set value
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSymbolData_Utility, arrSymbol[0], spU, cr);
		if( cr.IsFailed() )
			return cr;
		spU.Deref().SetCharRange(rcr);
		return cr;
	}

private:
	//noncopyable
	RegexDoCharSAction(const RegexDoCharSAction&) throw();
	RegexDoCharSAction& operator=(const RegexDoCharSAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharSAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
