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
This file contains component class of Do-Char-Set-Up action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_DO_CHAR_SET_UP_ACTION_H__
#define __REGEX_DO_CHAR_SET_UP_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexDoCharSetUpAction

class RegexDoCharSetUpAction : public _IGrammarAction,
							public _RegexAstActionBase
{
public:
	RegexDoCharSetUpAction() throw()
	{
	}
	~RegexDoCharSetUpAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		//range set
		ShareCom<_I_RegexCharSetSymbolData_Utility> spU;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexCharSetSymbolData_Utility, arrSymbol[3], spU, cr);
		if( cr.IsFailed() )
			return cr;
		RefPtr<_RegexCharRangeSet> rs(spU.Deref().GetCharRangeSet());
		//node
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, arrSymbol[0], spS, cr);
		if( cr.IsFailed() )
			return cr;
		try {
			_RegexCharRangeSet rsC;
			rs.Deref().CalcComplement(rsC);  //may throw
			AstTree::Iterator iter(_RegexCharRangeSet_To_AST(rsC, m_tree.Deref()));  //may throw
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
	RegexDoCharSetUpAction(const RegexDoCharSetUpAction&) throw();
	RegexDoCharSetUpAction& operator=(const RegexDoCharSetUpAction&) throw();
};

DECLARE_COM_TYPECAST(RegexDoCharSetUpAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
