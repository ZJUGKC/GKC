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
This file contains component class of Wmark parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_PARSER_H__
#define __WMARK_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkParser

class WmarkParser : public _IWmarkParser
{
public:
	WmarkParser() throw() : m_uMaxErrorNumber(0)
	{
	}
	~WmarkParser() throw()
	{
	}

// _IWmarkParser methods
	virtual GKC::CallResult Initialize(const uint& uMaxErrorNumber, const GKC::ShareCom<_IWmarkUtility>& sp) throw()
	{
		CallResult cr;

		//access
		_WmarkUtility_Objects objs;
		{
			ShareCom<_IWmarkUtility_Access> spAccess;
			_COMPONENT_INSTANCE_INTERFACE(_IWmarkUtility, _IWmarkUtility_Access, sp, spAccess, cr);
			if( cr.IsFailed() )
				return cr;
			spAccess.Deref().GetObjects(objs);
		} //end block

		//lexer analyzer
		if( m_spLexerAnalyzer.IsBlockNull() ) {
			cr = create_lexer_analyzer(objs, m_spLexerAnalyzer);
			if( cr.IsFailed() )
				return cr;
		}

		//grammar analyzer
		if( m_spGrammarAnalyzer.IsBlockNull() ) {
			try {
				cr = create_grammar_analyzer(objs, m_spLexerAnalyzer, m_spGrammarAnalyzer, m_arrU);  //may throw
			}
			catch(Exception& e) {
				cr = e.GetResult();
			}
			catch(...) {
				cr.SetResult(SystemCallResults::Fail);
			}
			if( cr.IsFailed() )
				return cr;
		}

		//number
		m_uMaxErrorNumber = uMaxErrorNumber;

		return cr;
	}

	virtual void SetInput(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
	virtual void SetOutput(const GKC::ShareCom<GKC::ICplMetaData>& sp) throw() = 0;
	virtual GKC::CallResult Start() throw() = 0;
	// return value : SystemCallResults::OK, the call is successful.
	//                SystemCallResults::S_False, it reaches the end of stream.
	//                otherwise, this call is failed.
	virtual GKC::CallResult Parse() throw() = 0;
	virtual const GKC::ShareArray<GKC::StringS> get_ErrorArray() throw() = 0;

private:
	//create lexer analyzer
	static CallResult create_lexer_analyzer(const _WlangUtility_Objects& objs, ShareCom<ILexerAnalyzer>& sp) throw()
	{
		CallResult cr;
		//create
		ShareCom<ILexerAnalyzer> spLexerAnalyzer;
		cr = CplAnalyzerHelper::CreateLexerAnalyzer(spLexerAnalyzer);
		if( cr.IsFailed() )
			return cr;
		//lexer tables
		// set
		cr = spLexerAnalyzer.Deref().SetTables(objs.spLexerTables);
		if( cr.IsFailed() )
			return cr;
		//actions
		{
			// set
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), objs.spCommentStart);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//output
		sp = spLexerAnalyzer;
		return cr;
	}
	//create grammar analyzer
	static CallResult create_grammar_analyzer(const _WlangUtility_Objects& objs, const ShareCom<ILexerAnalyzer>& spLexerAnalyzer,
											ShareCom<IGrammarAnalyzer>& sp, ShareArray<ShareCom<_ICplMetaDataActionUtility>>& arrU)
	{
		//array
		ShareArray<ShareCom<_ICplMetaDataActionUtility>> arrUtility(ShareArrayHelper::MakeShareArray<ShareCom<_ICplMetaDataActionUtility>>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		//create
		CallResult cr;
		ShareCom<IGrammarAnalyzer> spGrammarAnalyzer;
		cr = CplAnalyzerHelper::CreateGrammarAnalyzer(spGrammarAnalyzer);
		if( cr.IsFailed() )
			return cr;
		//grammar tables
		// set
		cr = spGrammarAnalyzer.Deref().SetTables(objs.spGrammarTables);
		if( cr.IsFailed() )
			return cr;
		spGrammarAnalyzer.Deref().SetLexerAnalyzer(spLexerAnalyzer);
		//factory
		{
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "block"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "block_list"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;


		//actions
		{
			ShareCom<_ICplMetaDataActionUtility> spU;
			//grammar accepted
			ShareCom<IGrammarAccepted> spAccepted;
			cr = _Create_WmarkGrammarAccepted(spAccepted);
			if( cr.IsFailed() )
				return cr;
			_COMPONENT_INSTANCE_INTERFACE(IGrammarAccepted, _ICplMetaDataActionUtility, spAccepted, spU, cr);
			if( cr.IsFailed() )
				return cr;
			arrUtility.Add(spU);  //may throw
			ShareCom<IGrammarAction> spAction;

private:
	uint m_uMaxErrorNumber;

	ShareCom<ILexerAnalyzer> m_spLexerAnalyzer;
	ShareCom<IGrammarAnalyzer> m_spGrammarAnalyzer;

	ShareCom<ICplMetaData> m_spMeta;
	//for actions
	ShareArray<ShareCom<_ICplMetaDataActionUtility>> m_arrU;

private:
	//noncopyable
	WmarkParser(const WmarkParser&) throw();
	WmarkParser& operator=(const WmarkParser&) throw();
};

DECLARE_COM_TYPECAST(WmarkParser)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
