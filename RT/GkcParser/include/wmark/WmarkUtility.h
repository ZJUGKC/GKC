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
This file contains component class of Wmark utility.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_UTILITY_H__
#define __WMARK_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkUtility

class WmarkUtility : public _IWmarkUtility,
					public _IWmarkUtility_Access
{
public:
	WmarkUtility() throw()
	{
	}
	~WmarkUtility() throw()
	{
	}

// _IWmarkUtility methods
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
			cr = load_text_wmark_lex(spText);
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
			cr = load_text_wmark_gra(spText);
			if( cr.IsFailed() )
				return cr;
			// generate
			cr = spGrammarTables.Deref().GenerateTables(spText, m_spLexerTables);
			if( cr.IsFailed() )
				return cr;
			m_spGrammarTables = spGrammarTables;
		}
		//lexer actions
		if( m_spReturn.IsBlockNull() ) {
			cr = _Create_WmarkReturnAction(m_spReturn);
			if( cr.IsFailed() )
				return cr;
		}
		if( m_spCommentStart.IsBlockNull() ) {
			cr = _Create_WmarkCommentStartAction(m_spCommentStart);
			if( cr.IsFailed() )
				return cr;
		}
		//factory
		if( m_spBasicFactory.IsBlockNull() ) {
			cr = CplAnalyzerHelper::CreateBasicSymbolDataFactory(m_spBasicFactory);
			if( cr.IsFailed() )
				return cr;
		}
		if( m_spPositionFactory.IsBlockNull() ) {
			cr = CplMetaDataHelper::CreateCplMetaDataPositionSymbolDataFactory(m_spPositionFactory);
			if( cr.IsFailed() )
				return cr;
		}
		return cr;
	}

// _IWmarkUtility_Access methods
	virtual void GetObjects(_WmarkUtility_Objects& obj) throw()
	{
		//tables
		obj.spLexerTables     = m_spLexerTables;
		obj.spGrammarTables   = m_spGrammarTables;
		//lexer actions
		obj.spReturn          = m_spReturn;
		obj.spCommentStart    = m_spCommentStart;
		//factory
		obj.spBasicFactory    = m_spBasicFactory;
		obj.spPositionFactory = m_spPositionFactory;
	}

private:
	//load text
	static CallResult load_text_wmark_lex_buffer(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_wmark_lex::GetAddress(), g_wmark_lex::GetCount()), true, sp);
	}
	static CallResult load_text_wmark_gra_buffer(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_wmark_gra::GetAddress(), g_wmark_gra::GetCount()), true, sp);
	}
	static CallResult load_text_wmark_lex(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return load_text_wmark_lex_buffer(sp);
	}
	static CallResult load_text_wmark_gra(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return load_text_wmark_gra_buffer(sp);
	}

private:
	//tables
	ShareCom<ILexerTables>   m_spLexerTables;
	ShareCom<IGrammarTables> m_spGrammarTables;
	//lexer actions
	ShareCom<ILexerAction> m_spReturn;
	ShareCom<ILexerAction> m_spCommentStart;
	//factory
	ShareCom<IComFactory> m_spBasicFactory;
	ShareCom<IComFactory> m_spPositionFactory;

private:
	//noncopyable
	WmarkUtility(const WmarkUtility&) throw();
	WmarkUtility& operator=(const WmarkUtility&) throw();
};

DECLARE_COM_TYPECAST(WmarkUtility)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
