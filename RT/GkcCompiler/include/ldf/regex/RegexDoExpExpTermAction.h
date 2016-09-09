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
This file contains component class of Do-Exp-Exp-Term action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_EXP_EXP_TERM_ACTION_H__
#define __REGEX_DO_EXP_EXP_TERM_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoExpExpTermAction

class RegexDoExpExpTermAction : public _IGrammarAction,
								public _RegexAstActionBase
{
public:
	RegexDoExpExpTermAction() throw()
	{
	}
	~RegexDoExpExpTermAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//iterators
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[3].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter2(m_tree.Deref().GetAtPosition(spS.Deref().GetPosition()));
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[1].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter1(m_tree.Deref().GetAtPosition(spS.Deref().GetPosition()));
		//node
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		try {
			AstTree::Iterator iter(_RegexBinaryOperator_AST(iter1, iter2, REGEX_OP_OR, m_tree.Deref()));  //may throw
			spS.Deref().SetPosition(iter.GetPosition());
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
	RegexDoExpExpTermAction(const RegexDoExpExpTermAction&) throw();
	RegexDoExpExpTermAction& operator=(const RegexDoExpExpTermAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoExpExpTermAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
