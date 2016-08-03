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

		//lexer tables
		ShareCom<ILexerTables> spLexerTables;
		cr = CplAnalyzerHelper::CreateLexerTables(spLexerTables);
		if( cr.IsFailed() )
			return cr;
		// stream
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.lex")), FileOpenTypes::Read, 0, spStream);
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
		// generate
		cr = spLexerTables.Deref().GenerateTables(spText);
		if( cr.IsFailed() )
			return cr;

		//grammar tables
		ShareCom<IGrammarTables> spGrammarTables;
		cr = CplAnalyzerHelper::CreateGrammarTables(spGrammarTables);
		if( cr.IsFailed() )
			return cr;
		// stream
		cr = StreamHelper::CreateFileStream(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("wlang.gra")), FileOpenTypes::Read, 0, spStream);
		if( cr.IsFailed() )
			return cr;
		spText.Deref().SetStream(spStream);
		// BOM
		cr = spText.Deref().CheckBOM(iBOMType);
		if( cr.IsFailed() )
			return cr;
		if( iBOMType != BOMTypes::UTF8 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		// generate
		cr = spGrammarTables.Deref().GenerateTables(spText);
		if( cr.IsFailed() )
			return cr;

		//lexer analyzer
		ShareCom<ILexerAnalyzer> spLexerAnalyzer;
		cr = CplAnalyzerHelper::CreateLexerAnalyzer(spLexerAnalyzer);
		if( cr.IsFailed() )
			return cr;
		cr = spLexerAnalyzer.Deref().SetTables(spLexerTables);
		if( cr.IsFailed() )
			return cr;
		//actions
		ShareCom<ILexerAction> spCommentStart;
		cr = LexerActionHelper::CreateCommentStartAction(spCommentStart);
		if( cr.IsFailed() )
			return cr;
		cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), ShareComHelper::ToWeakCom(spCommentStart));
		if( cr.IsFailed() )
			return cr;
		ShareCom<ILexerAction> spLineCommentStart;
		cr = LexerActionHelper::CreateLineCommentStartAction(spLineCommentStart);
		if( cr.IsFailed() )
			return cr;
		cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LINE_COMMENT_START"), ShareComHelper::ToWeakCom(spLineCommentStart));
		if( cr.IsFailed() )
			return cr;
		ShareCom<ILexerAction> spSpace;
		cr = LexerActionHelper::CreateSpaceAction(spSpace);
		if( cr.IsFailed() )
			return cr;
		cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), ShareComHelper::ToWeakCom(spSpace));
		if( cr.IsFailed() )
			return cr;
		ShareCom<ILexerAction> spReturn;
		cr = LexerActionHelper::CreateReturnAction(spReturn);
		if( cr.IsFailed() )
			return cr;
		cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), ShareComHelper::ToWeakCom(spReturn));
		if( cr.IsFailed() )
			return cr;

		//grammar analyzer
		ShareCom<IGrammarAnalyzer> spGrammarAnalyzer;
		cr = CplAnalyzerHelper::CreateGrammarAnalyzer(spGrammarAnalyzer);
		if( cr.IsFailed() )
			return cr;
		cr = spGrammarAnalyzer.Deref().SetTables(spGrammarTables);
		if( cr.IsFailed() )
			return cr;
		spGrammarAnalyzer.Deref().SetLexerAnalyzer(spLexerAnalyzer);
		//actions
		ShareCom<IGrammarError> spGrammarError;
		cr = _Create_WlangGrammarError(spGrammarError);
		if( cr.IsFailed() )
			return cr;
		spGrammarAnalyzer.Deref().SetErrorAction(ShareComHelper::ToWeakCom(spGrammarError));

		m_spCommentStart = spCommentStart;
		m_spLineCommentStart = spLineCommentStart;
		m_spSpace = spSpace;
		m_spReturn = spReturn;
		m_spLexerAnalyzer = spLexerAnalyzer;
		m_spGrammarError = spGrammarError;
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
	uint m_uMaxErrorNumber;

	ShareCom<ILexerAction> m_spCommentStart;
	ShareCom<ILexerAction> m_spLineCommentStart;
	ShareCom<ILexerAction> m_spSpace;
	ShareCom<ILexerAction> m_spReturn;
	ShareCom<ILexerAnalyzer> m_spLexerAnalyzer;
	ShareCom<IGrammarError> m_spGrammarError;
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
