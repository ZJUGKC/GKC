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
This file contains component class of Do-Char-Set action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_SET_ACTION_H__
#define __REGEX_DO_CHAR_SET_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharSetAction

class RegexDoCharSetAction : public _IGrammarAction,
							public _I_RegexAstAction_Utility
{
public:
	RegexDoCharSetAction() throw()
	{
	}
	~RegexDoCharSetAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//range set
		ShareCom<_I_RegexCharSetSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSetSymbolData_Utility, arrSymbol[2].get_Value(), spU, cr);
		if( cr.IsFailed() )
			return cr;
		RefPtr<_RegexCharRangeSet> rs(spU.Deref().GetCharRangeSet());
		//node
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0].get_Value(), spS, cr);
		if( cr.IsFailed() )
			return cr;
		try {
			AstTree::Iterator iter(_RegexCharRangeSet_To_AST(rs.Deref(), m_tree.Deref()));  //may throw
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

// _I_RegexAstAction_Utility methods
	virtual void SetAST(const RefPtr<AstTree>& tree) throw()
	{
		m_tree = tree;
	}
	virtual RefPtr<AstTree> GetAST() throw()
	{
		return m_tree;
	}

private:
	RefPtr<AstTree> m_tree;

private:
	//noncopyable
	RegexDoCharSetAction(const RegexDoCharSetAction&) throw();
	RegexDoCharSetAction& operator=(const RegexDoCharSetAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharSetAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
