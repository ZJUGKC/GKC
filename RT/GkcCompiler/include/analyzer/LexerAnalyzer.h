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
This file contains component class of lexer analyzer.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEXER_ANALYZER_H__
#define __LEXER_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LexerAnalyzer

class LexerAnalyzer : public _ILexerAnalyzer,
					public _ILexerAnalyzerAccess
{
public:
	LexerAnalyzer() throw()
	{
	}
	~LexerAnalyzer() throw()
	{
	}

// _ILexerAnalyzer methods
	virtual GKC::ShareCom<_ILexerTables> GetTables() throw()
	{
		return CALL_COM_TYPECAST(m_spAccess, _ILexerTablesAccess, _ILexerTables);
	}
	virtual GKC::CallResult SetTables(const GKC::ShareCom<_ILexerTables>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		m_spAccess = CALL_COM_TYPECAST(sp, _ILexerTables, _ILexerTablesAccess);
		if( m_spAccess.IsBlockNull() )
			return CallResult(SystemCallResults::NotImpl);
		//set
		m_lexer.SetTokenTable(m_spAccess.Deref().GetTokenTable());
		m_lexer.SetFsaTable(m_spAccess.Deref().GetFsaTable());
		return CallResult();
	}
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStream>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		m_lexer.SetStream(sp);
	}
	virtual GKC::CallResult SetAction(const GKC::ConstStringA& strToken, const GKC::ShareCom<_ILexerAction>& spAction) throw()
	{
		CallResult cr;
		try {
			m_lexer.SetAction(strToken, spAction);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual void Start() throw()
	{
		m_lexer.Start();
	}
	virtual GKC::CallResult Parse() throw()
	{
		CallResult cr;
		try {
			cr = m_lexer.Parse();
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual RefPtr<_LexerTokenInfo> GetTokenInfo() throw()
	{
		return RefPtr<_LexerTokenInfo>(m_lexer.GetTokenInfo());
	}

// _ILexerAnalyzerAccess methods
	virtual GKC::RefPtr<GKC::LexerParser> GetLexerParser() throw()
	{
		return RefPtr<LexerParser>(m_lexer);
	}

private:
	bool is_valid() const throw()
	{
		return !(m_lexer.GetStream().IsBlockNull()) && !(m_spAccess.IsBlockNull());
	}

private:
	ShareCom<_ILexerTablesAccess>  m_spAccess;

	//lexer parser
	LexerParser  m_lexer;

private:
	//noncopyable
	LexerAnalyzer(const LexerAnalyzer&) throw();
	LexerAnalyzer& operator=(const LexerAnalyzer&) throw();
};

DECLARE_COM_TYPECAST(LexerAnalyzer)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
