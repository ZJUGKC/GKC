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
This file contains component class of regular expression grammar accepted action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __REGEX_GRAMMAR_ACCEPTED_H__
#define __REGEX_GRAMMAR_ACCEPTED_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RegexGrammarAccepted

class RegexGrammarAccepted : public _IGrammarAccepted,
							public _RegexAstActionBase
{
public:
	RegexGrammarAccepted() throw()
	{
	}
	~RegexGrammarAccepted() throw()
	{
	}

// _IGrammarAccepted methods
	virtual GKC::CallResult DoAccepted(INOUT GKC::ShareCom<_IGrammarSymbolData>& sym) throw()
	{
		CallResult cr;
		//iterator
		ShareCom<_I_RegexPositionSymbolData_Utility> spS;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarSymbolData, _I_RegexPositionSymbolData_Utility, sym, spS, cr);
		if( cr.IsFailed() )
			return cr;
		AstTree::Iterator iter(m_tree.Deref().ToIterator(spS.Deref().GetPosition()));
		//root
		AstTree::Iterator iterR(m_tree.Deref().GetRoot());
		m_tree.Deref().SetParent(iter, iterR);
		m_tree.Deref().SetChild(iterR, iter);
		return cr;
	}

private:
	//noncopyable
	RegexGrammarAccepted(const RegexGrammarAccepted&) throw();
	RegexGrammarAccepted& operator=(const RegexGrammarAccepted&) throw();
};

DECLARE_COM_TYPECAST(RegexGrammarAccepted)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
