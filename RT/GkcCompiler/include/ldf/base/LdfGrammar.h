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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LDF_GRAMMAR_H__
#define __LDF_GRAMMAR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex and gra file

// _LdfGrammarParser

class _LdfGrammarParser
{
public:
	_LdfGrammarParser() throw()
	{
	}
	~_LdfGrammarParser() throw()
	{
	}

	void SetLexerParser(const RefPtr<LexerParser>& lexer) throw()
	{
		m_grammar.SetLexerParser(lexer);
	}
	void SetNonterminalTable(const RefPtr<TokenTable>& nt) throw()
	{
		m_grammar.SetNonterminalTable(nt);
	}
	void SetReductionActionTable(const RefPtr<TokenTable>& ra) throw()
	{
		m_grammar.SetReductionActionTable(ra);
	}
	void SetPdaTable(const PDA_TABLE& table) throw()
	{
		m_grammar.SetPdaTable(table);
	}
	void SetAction(const ConstStringA& strAction, const ShareCom<_IGrammarAction>& spAction)
	{
		m_grammar.SetAction(strAction, spAction);  //may throw
	}
	void SetFactory(const ConstStringA& strEvent, const ShareCom<IComFactory>& sp)
	{
		m_grammar.SetFactory(strEvent, sp);  //may throw
	}

	CallResult Execute()
	{
		CallResult cr;
		//loop
		m_grammar.Start(true);  //may throw
		while( true ) {
			uint uLastEventNo;
			cr = m_grammar.Parse(uLastEventNo);  //may throw
			if( cr.IsFailed() )
				break;
			if( cr.GetResult() == SystemCallResults::S_False ) {
				if( uLastEventNo == PDA_LAST_EVENT_NO )
					cr.SetResult(SystemCallResults::Fail);  //The empty source file is an error
				break;
			}
			if( cr.GetResult() == SystemCallResults::OK ) {
				if( uLastEventNo != PDA_NO_EVENT ) {
					//error state
					cr = m_grammar.Revert();  //may throw
					if( cr.IsFailed() )
						break;
				}
			}
		} //end while
		//check
		if( cr.IsFailed() )
			return cr;
		if( m_grammar.GetErrorArray().GetCount() > 0 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		//successful
		cr.SetResult(SystemCallResults::OK);
		return cr;
	}

private:
	//grammar parser
	GrammarParser m_grammar;

private:
	//noncopyable
	_LdfGrammarParser(const _LdfGrammarParser&) throw();
	_LdfGrammarParser& operator=(const _LdfGrammarParser&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
