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
This file contains component class of grammar analyzer.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRAMMAR_ANALYZER_H__
#define __GRAMMAR_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GrammarAnalyzer

class GrammarAnalyzer : public _IGrammarAnalyzer
{
public:
	GrammarAnalyzer() throw() : m_bEmpty(false), m_bErrorState(false)
	{
	}
	~GrammarAnalyzer() throw()
	{
	}

// _IGrammarAnalyzer methods
	virtual GKC::CallResult SetTables(const GKC::ShareCom<_IGrammarTables>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		m_spTablesAccess = CALL_COM_TYPECAST(sp, _IGrammarTables, _IGrammarTablesAccess);
		if( m_spTablesAccess.IsBlockNull() )
			return CallResult(SystemCallResults::NotImpl);
		//set
		m_grammar.SetNonterminalTable(m_spTablesAccess.Deref().GetNonterminalTable());
		m_grammar.SetReductionActionTable(m_spTablesAccess.Deref().GetReductionActionTable());
		m_grammar.SetPdaTable(m_spTablesAccess.Deref().GetPdaTable());
		return CallResult();
	}
	virtual void SetLexerAnalyzer(const GKC::ShareCom<_ILexerAnalyzer>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		m_spLexerAccess = CALL_COM_TYPECAST(sp, _ILexerAnalyzer, _ILexerAnalyzerAccess);
		assert( !m_spLexerAccess.IsBlockNull() );
		m_grammar.SetLexerParser(m_spLexerAccess.Deref().GetLexerParser());
	}
	virtual GKC::CallResult SetAction(const GKC::ConstStringA& strAction, const GKC::ShareCom<_IGrammarAction>& spAction) throw()
	{
		CallResult cr;
		try {
			m_grammar.SetAction(strAction, spAction);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual void SetErrorAction(const GKC::ShareCom<_IGrammarError>& sp) throw()
	{
		m_grammar.SetErrorAction(sp);
	}
	virtual GKC::CallResult SetFactory(const GKC::ConstStringA& strEvent, const GKC::ShareCom<GKC::IComFactory>& sp) throw()
	{
		CallResult cr;
		try {
			m_grammar.SetFactory(strEvent, sp);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult Start(const bool& bUnexpectedEoeAsError) throw()
	{
		CallResult cr;
		try {
			m_grammar.Start(bUnexpectedEoeAsError);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult Parse() throw()
	{
		CallResult cr;
		uint uLastEventNo;
		try {
			cr = m_grammar.Parse(uLastEventNo);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		if( cr.IsFailed() )
			return cr;
		if( cr.GetResult() == SystemCallResults::S_False ) {
			m_bEmpty = (uLastEventNo == PDA_LAST_EVENT_NO);
			return cr;
		}
		if( cr.GetResult() == SystemCallResults::OK ) {
			m_bErrorState = (uLastEventNo != PDA_NO_EVENT);
			return cr;
		}
		return cr;
	}
	virtual const GKC::ShareArray<GKC::StringS> get_ErrorArray() throw()
	{
		return m_grammar.GetErrorArray();
	}
	virtual bool IsEmpty() throw()
	{
		return m_bEmpty;
	}
	virtual bool IsErrorState() throw()
	{
		return m_bErrorState;
	}
	virtual GKC::CallResult Revert() throw()
	{
		CallResult cr;
		try {
			cr = m_grammar.Revert();
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
	ShareCom<_IGrammarTablesAccess>  m_spTablesAccess;
	ShareCom<_ILexerAnalyzerAccess>  m_spLexerAccess;

	//grammar parser
	GrammarParser  m_grammar;

	bool m_bEmpty;
	bool m_bErrorState;

private:
	//noncopyable
	GrammarAnalyzer(const GrammarAnalyzer&) throw();
	GrammarAnalyzer& operator=(const GrammarAnalyzer&) throw();
};

DECLARE_COM_TYPECAST(GrammarAnalyzer)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
