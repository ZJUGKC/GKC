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
This file contains component class of Do-Factor-Char-Set action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_FACTOR_CHAR_SET_ACTION_H__
#define __REGEX_DO_FACTOR_CHAR_SET_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoFactorCharSetAction

class RegexDoFactorCharSetAction : public _IGrammarAction,
								public _RegexAstActionBase
{
public:
	RegexDoFactorCharSetAction() throw()
	{
	}
	~RegexDoFactorCharSetAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//position
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[1].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Position pos(spS.Deref().GetPosition());
		//set
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		spS.Deref().SetPosition(pos);
		return cr;
	}

private:
	//noncopyable
	RegexDoFactorCharSetAction(const RegexDoFactorCharSetAction&) throw();
	RegexDoFactorCharSetAction& operator=(const RegexDoFactorCharSetAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoFactorCharSetAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
