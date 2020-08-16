/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains component class of Won utility.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_UTILITY_H__
#define __WON_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonUtility

class WonUtility : public _IWonUtility,
				public _IWonUtility_Access
{
public:
	WonUtility() throw()
	{
	}
	~WonUtility() throw()
	{
	}

// _IWonUtility methods
	virtual GKC::CallResult Initialize() throw()
	{
		CallResult cr;
		//tables
		if( m_spLexerTables.IsBlockNull() ) {
			ShareCom<ILexerTables> spLexerTables;
			cr = CplAnalyzerHelper::CreateLexerTables(spLexerTables);
			if( cr.IsFailed() )
				return cr;
			ShareCom<ITextStreamRoot> spText;
			cr = load_text_won_lex(spText);
			if( cr.IsFailed() )
				return cr;
			// generate
			cr = spLexerTables.Deref().GenerateTables(spText);
			if( cr.IsFailed() )
				return cr;
			m_spLexerTables = spLexerTables;
		}
		if( m_spGrammarTables.IsBlockNull() ) {
			ShareCom<IGrammarTables> spGrammarTables;
			cr = CplAnalyzerHelper::CreateGrammarTables(spGrammarTables);
			if( cr.IsFailed() )
				return cr;
			ShareCom<ITextStreamRoot> spText;
			cr = load_text_won_gra(spText);
			if( cr.IsFailed() )
				return cr;
			// generate
			cr = spGrammarTables.Deref().GenerateTables(spText, m_spLexerTables);
			if( cr.IsFailed() )
				return cr;
			m_spGrammarTables = spGrammarTables;
		}
		//lexer actions
		if( m_spCommentStart.IsBlockNull() ) {
			cr = LexerActionHelper::CreateCommentStartAction(m_spCommentStart);
			if( cr.IsFailed() )
				return cr;
		}
		if( m_spSpace.IsBlockNull() ) {
			cr = LexerActionHelper::CreateSpaceAction(m_spSpace);
			if( cr.IsFailed() )
				return cr;
		}
		if( m_spReturn.IsBlockNull() ) {
			cr = LexerActionHelper::CreateReturnAction(m_spReturn);
			if( cr.IsFailed() )
				return cr;
		}
		//factory
		if( m_spBasicFactory.IsBlockNull() ) {
			cr = CplAnalyzerHelper::CreateBasicSymbolDataFactory(m_spBasicFactory);
			if( cr.IsFailed() )
				return cr;
		}
		return cr;
	}

// _IWonUtility_Access methods
	virtual void GetObjects(_WonUtility_Objects& obj) throw()
	{
		//tables
		obj.spLexerTables   = m_spLexerTables;
		obj.spGrammarTables = m_spGrammarTables;
		//lexer actions
		obj.spCommentStart  = m_spCommentStart;
		obj.spSpace         = m_spSpace;
		obj.spReturn        = m_spReturn;
		//factory
		obj.spBasicFactory  = m_spBasicFactory;
	}

private:
	static CallResult load_text_won_lex(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_won_lex::GetAddress(), g_won_lex::GetCount()), true, sp);
	}
	static CallResult load_text_won_gra(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_won_gra::GetAddress(), g_won_gra::GetCount()), true, sp);
	}

private:
	//tables
	ShareCom<ILexerTables>   m_spLexerTables;
	ShareCom<IGrammarTables> m_spGrammarTables;
	//lexer actions
	ShareCom<ILexerAction> m_spCommentStart;
	ShareCom<ILexerAction> m_spSpace;
	ShareCom<ILexerAction> m_spReturn;
	//factory
	ShareCom<IComFactory> m_spBasicFactory;

private:
	//noncopyable
	WonUtility(const WonUtility&) throw();
	WonUtility& operator=(const WonUtility&) throw();
};

DECLARE_COM_TYPECAST(WonUtility)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
