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
This file contains component class of WLang parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_PARSER_H__
#define __WLANG_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangParser

class WlangParser : public _IWlangParser
{
public:
	WlangParser() throw() : m_uMaxErrorNumber(0)
	{
	}
	~WlangParser() throw()
	{
	}

// _IWlangParser methods
	virtual GKC::CallResult Initialize(const uint& uMaxErrorNumber) throw()
	{
		CallResult cr;

		//lexer analyzer
		ShareCom<ILexerAnalyzer> spLexerAnalyzer;
		cr = CplAnalyzerHelper::CreateLexerAnalyzer(spLexerAnalyzer);
		if( cr.IsFailed() )
			return cr;
		//lexer tables
		{
			ShareCom<ILexerTables> spLexerTables;
			cr = CplAnalyzerHelper::CreateLexerTables(spLexerTables);
			if( cr.IsFailed() )
				return cr;
			ShareCom<ITextStream> spText;
			cr = load_text(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.lex")), spText);
			if( cr.IsFailed() )
				return cr;
			// generate
			cr = spLexerTables.Deref().GenerateTables(spText);
			if( cr.IsFailed() )
				return cr;
			// set
			cr = spLexerAnalyzer.Deref().SetTables(spLexerTables);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//actions
		{
			ShareCom<ILexerAction> spAction;
			cr = LexerActionHelper::CreateCommentStartAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = LexerActionHelper::CreateLineCommentStartAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LINE_COMMENT_START"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = LexerActionHelper::CreateSpaceAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = LexerActionHelper::CreateReturnAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), spAction);
			if( cr.IsFailed() )
				return cr;
		} //end block

		//grammar analyzer
		ShareCom<IGrammarAnalyzer> spGrammarAnalyzer;
		cr = CplAnalyzerHelper::CreateGrammarAnalyzer(spGrammarAnalyzer);
		if( cr.IsFailed() )
			return cr;
		//grammar tables
		{
			ShareCom<IGrammarTables> spGrammarTables;
			cr = CplAnalyzerHelper::CreateGrammarTables(spGrammarTables);
			if( cr.IsFailed() )
				return cr;
			ShareCom<ITextStream> spText;
			cr = load_text(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.gra")), spText);
			if( cr.IsFailed() )
				return cr;
			// generate
			cr = spGrammarTables.Deref().GenerateTables(spText);
			if( cr.IsFailed() )
				return cr;
			// set
			cr = spGrammarAnalyzer.Deref().SetTables(spGrammarTables);
			if( cr.IsFailed() )
				return cr;
		} //end block

		spGrammarAnalyzer.Deref().SetLexerAnalyzer(spLexerAnalyzer);
		//actions
		{
			ShareCom<IGrammarError> spGrammarError;
			cr = _Create_WlangGrammarError(spGrammarError);
			if( cr.IsFailed() )
				return cr;
			spGrammarAnalyzer.Deref().SetErrorAction(spGrammarError);
		} //end block

		m_spLexerAnalyzer = spLexerAnalyzer;
		m_spGrammarAnalyzer = spGrammarAnalyzer;

		m_uMaxErrorNumber = uMaxErrorNumber;

		return cr;
	}
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStream>& sp) throw()
	{
		assert( !m_spLexerAnalyzer.IsBlockNull() );
		m_spLexerAnalyzer.Deref().SetStream(sp);
	}
	virtual GKC::CallResult Start() throw()
	{
		assert( !m_spGrammarAnalyzer.IsBlockNull() );
		return m_spGrammarAnalyzer.Deref().Start(true);
	}
	virtual GKC::CallResult Parse() throw()
	{
		assert( !m_spGrammarAnalyzer.IsBlockNull() );
		CallResult cr = m_spGrammarAnalyzer.Deref().Parse();
		if( cr.GetResult() == SystemCallResults::OK ) {
			if( m_spGrammarAnalyzer.Deref().IsErrorState() ) {
				cr = m_spGrammarAnalyzer.Deref().Revert();
				if( cr.IsFailed() )
					return cr;
			}
			const GKC::ShareArray<GKC::StringS> arrError(m_spGrammarAnalyzer.Deref().get_ErrorArray());
			uintptr uCount = arrError.GetCount();
			if( m_uMaxErrorNumber != 0 && uCount >= (uintptr)m_uMaxErrorNumber )
				cr.SetResult(SystemCallResults::S_False);
		}
		return cr;
	}
	virtual const GKC::ShareArray<GKC::StringS> get_ErrorArray() throw()
	{
		assert( !m_spGrammarAnalyzer.IsBlockNull() );
		return m_spGrammarAnalyzer.Deref().get_ErrorArray();
	}

private:
	CallResult load_text(const ConstStringS& str, ShareCom<ITextStream>& sp) throw()
	{
		CallResult cr;
		// stream
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(str, FileOpenTypes::Read, 0, spStream);
		if( cr.IsFailed() )
			return cr;
		ShareCom<ITextStream> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() )
			return cr;
		spText.Deref().SetStream(spStream);
		// BOM
		int iBOMType;
		cr = spText.Deref().CheckBOM(iBOMType);
		if( cr.IsFailed() )
			return cr;
		if( iBOMType != BOMTypes::UTF8 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		sp = spText;
		return cr;
	}

private:
	uint m_uMaxErrorNumber;

	ShareCom<ILexerAnalyzer> m_spLexerAnalyzer;
	ShareCom<IGrammarAnalyzer> m_spGrammarAnalyzer;

private:
	//noncopyable
	WlangParser(const WlangParser&) throw();
	WlangParser& operator=(const WlangParser&) throw();
};

DECLARE_COM_TYPECAST(WlangParser)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
