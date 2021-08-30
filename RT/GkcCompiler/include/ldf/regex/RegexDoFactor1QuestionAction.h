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
This file contains component class of Do-Factor-1-Question action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_FACTOR_1_QUESTION_ACTION_H__
#define __REGEX_DO_FACTOR_1_QUESTION_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoFactor1QuestionAction

class RegexDoFactor1QuestionAction : public _IGrammarAction,
									public _RegexAstActionBase
{
public:
	RegexDoFactor1QuestionAction() throw()
	{
	}
	~RegexDoFactor1QuestionAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//iterator
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[1], spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter(m_tree.Deref().ToIterator(spS.Deref().GetPosition()));
		//node
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		try {
			AstTree::Iterator iterOP(_RegexUnaryOperator_AST(iter, REGEX_OP_QUESTION, m_tree.Deref()));  //may throw
			spS.Deref().SetPosition(iterOP.GetPosition());
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
	RegexDoFactor1QuestionAction(const RegexDoFactor1QuestionAction&) throw();
	RegexDoFactor1QuestionAction& operator=(const RegexDoFactor1QuestionAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoFactor1QuestionAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
