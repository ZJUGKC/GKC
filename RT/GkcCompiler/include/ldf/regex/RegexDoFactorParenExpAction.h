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
This file contains component class of Do-Factor-Paren-Exp action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_FACTOR_PAREN_EXP_ACTION_H__
#define __REGEX_DO_FACTOR_PAREN_EXP_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoFactorParenExpAction

class RegexDoFactorParenExpAction : public _IGrammarAction,
									public _RegexAstActionBase
{
public:
	RegexDoFactorParenExpAction() throw()
	{
	}
	~RegexDoFactorParenExpAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//position
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[2], spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Position pos(spS.Deref().GetPosition());
		//set
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		spS.Deref().SetPosition(pos);
		return cr;
	}

private:
	//noncopyable
	RegexDoFactorParenExpAction(const RegexDoFactorParenExpAction&) throw();
	RegexDoFactorParenExpAction& operator=(const RegexDoFactorParenExpAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoFactorParenExpAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
