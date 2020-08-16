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
This file contains component class of WLang utility.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_UTILITY_H__
#define __WLANG_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangUtility

class WlangUtility : public _IWlangUtility,
					public _IWlangUtility_Access
{
public:
	WlangUtility() throw()
	{
	}
	~WlangUtility() throw()
	{
	}

// _IWlangUtility methods
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
			cr = load_text_wlang_lex(spText);
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
			cr = load_text_wlang_gra(spText);
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
		if( m_spLineCommentStart.IsBlockNull() ) {
			cr = LexerActionHelper::CreateLineCommentStartAction(m_spLineCommentStart);
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
		if( m_spPositionFactory.IsBlockNull() ) {
			cr = CplMetaDataHelper::CreateCplMetaDataPositionSymbolDataFactory(m_spPositionFactory);
			if( cr.IsFailed() )
				return cr;
		}
		//grammar actions
		if( m_spGrammarError.IsBlockNull() ) {
			cr = _Create_WlangGrammarError(m_spGrammarError);
			if( cr.IsFailed() )
				return cr;
		}
		return cr;
	}

// _IWlangUtility_Access methods
	virtual void GetObjects(_WlangUtility_Objects& obj) throw()
	{
		//tables
		obj.spLexerTables      = m_spLexerTables;
		obj.spGrammarTables    = m_spGrammarTables;
		//lexer actions
		obj.spCommentStart     = m_spCommentStart;
		obj.spLineCommentStart = m_spLineCommentStart;
		obj.spSpace            = m_spSpace;
		obj.spReturn           = m_spReturn;
		//factory
		obj.spBasicFactory     = m_spBasicFactory;
		obj.spPositionFactory  = m_spPositionFactory;
		//grammar actions
		obj.spGrammarError     = m_spGrammarError;
	}

private:
	//load text
	static CallResult load_text_wlang_lex_file(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromFile(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.lex")), true, sp);
	}
	static CallResult load_text_wlang_gra_file(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromFile(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.gra")), true, sp);
	}
	static CallResult load_text_wlang_lex_buffer(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_wlang_lex::GetAddress(), g_wlang_lex::GetCount()), true, sp);
	}
	static CallResult load_text_wlang_gra_buffer(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return _ParserInputHelper::LoadTextFromBuffer(ConstStringA(g_wlang_gra::GetAddress(), g_wlang_gra::GetCount()), true, sp);
	}
	static CallResult load_text_wlang_lex(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return load_text_wlang_lex_buffer(sp);
	}
	static CallResult load_text_wlang_gra(ShareCom<ITextStreamRoot>& sp) throw()
	{
		return load_text_wlang_gra_buffer(sp);
	}

private:
	//tables
	ShareCom<ILexerTables>   m_spLexerTables;
	ShareCom<IGrammarTables> m_spGrammarTables;
	//lexer actions
	ShareCom<ILexerAction> m_spCommentStart;
	ShareCom<ILexerAction> m_spLineCommentStart;
	ShareCom<ILexerAction> m_spSpace;
	ShareCom<ILexerAction> m_spReturn;
	//factory
	ShareCom<IComFactory> m_spBasicFactory;
	ShareCom<IComFactory> m_spPositionFactory;
	//grammar actions
	ShareCom<IGrammarError> m_spGrammarError;

private:
	//noncopyable
	WlangUtility(const WlangUtility&) throw();
	WlangUtility& operator=(const WlangUtility&) throw();
};

DECLARE_COM_TYPECAST(WlangUtility)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
