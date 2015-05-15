/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains classes for lexer parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEXER_PARSER_H__
#define __LEXER_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LexerTable
//   a general implementation
class LexerTable : public ILexerTable
{
public:
	LexerTable() throw()
	{
	}
	~LexerTable() throw()
	{
	}

public:
	//ILexerTable
	virtual void SetFSA(const SharedPtr<FiniteStateAutomata>& fsa) throw()
	{
		m_spFsa = fsa;
	}
	virtual const SharedPtr<FiniteStateAutomata>& GetFSA() const throw()
	{
		return m_fsa;
	}
	//token table
	virtual uint GetTokenID(IN const RefPtr<Char>& szToken) throw() = 0;
	virtual const RefPtr<CharA> GetTokenName(IN uint uID) throw() = 0;

private:
	//FSA
	SharedPtr<FiniteStateAutomata>  m_spFsa;
	//token table

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LEXER_PARSER_H__
////////////////////////////////////////////////////////////////////////////////
