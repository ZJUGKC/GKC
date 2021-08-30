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
This file contains component class of Do-Term-Term-Factor-1 action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_TERM_TERM_FACTOR_1_ACTION_H__
#define __REGEX_DO_TERM_TERM_FACTOR_1_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoTermTermFactor1Action

class RegexDoTermTermFactor1Action : public _IGrammarAction,
									public _RegexAstActionBase
{
public:
	RegexDoTermTermFactor1Action() throw()
	{
	}
	~RegexDoTermTermFactor1Action() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//iterators
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[2], spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter2(m_tree.Deref().ToIterator(spS.Deref().GetPosition()));
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[1], spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter1(m_tree.Deref().ToIterator(spS.Deref().GetPosition()));
		//node
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		try {
			AstTree::Iterator iter(_RegexBinaryOperator_AST(iter1, iter2, REGEX_OP_LINK, m_tree.Deref()));  //may throw
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
	RegexDoTermTermFactor1Action(const RegexDoTermTermFactor1Action&) throw();
	RegexDoTermTermFactor1Action& operator=(const RegexDoTermTermFactor1Action&) throw();
};

DECLARE_COM_TYPECAST(RegexDoTermTermFactor1Action)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
