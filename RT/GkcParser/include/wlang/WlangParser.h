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
	virtual GKC::CallResult Initialize(const uint& uMaxErrorNumber, const GKC::ShareCom<_IWlangUtility>& sp) throw()
	{
		CallResult cr;

		//access
		_WlangUtility_Objects objs;
		{
			ShareCom<_IWlangUtility_Access> spAccess;
			_COMPONENT_INSTANCE_INTERFACE(_IWlangUtility, _IWlangUtility_Access, sp, spAccess, cr);
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
	virtual void SetInput(const GKC::ShareCom<GKC::ITextStreamRoot>& sp) throw()
	{
		assert( !m_spLexerAnalyzer.IsBlockNull() );
		int iCharType;
		m_spLexerAnalyzer.Deref().SetStream(sp, iCharType);
	}
	virtual void SetOutput(const GKC::ShareCom<GKC::ICplMetaData>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		if( sp != m_spMeta ) {
			m_spMeta = sp;
			//for actions
			if( m_arrU.GetCount() != 0 ) {
				for( auto iter(m_arrU.GetBegin()); iter != m_arrU.GetEnd(); iter.MoveNext() )
					iter.get_Value().Deref().SetMetaData(sp);
			}
		} //end if
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
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LINE_COMMENT_START"), objs.spLineCommentStart);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), objs.spSpace);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), objs.spReturn);
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
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_KEY_NAMESPACE"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_IDENTIFIER"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LCURLY"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RCURLY"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEMI"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "program"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "namespace_block_list"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "body"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "namespace_block"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//actions
		{
			//grammar error
			spGrammarAnalyzer.Deref().SetErrorAction(objs.spGrammarError);
			ShareCom<_ICplMetaDataActionUtility> spU;
			//grammar accepted
			ShareCom<IGrammarAccepted> spAccepted;
			cr = _Create_WlangGrammarAccepted(spAccepted);
			if( cr.IsFailed() )
				return cr;
			_COMPONENT_INSTANCE_INTERFACE(IGrammarAccepted, _ICplMetaDataActionUtility, spAccepted, spU, cr);
			if( cr.IsFailed() )
				return cr;
			arrUtility.Add(spU);  //may throw
			spGrammarAnalyzer.Deref().SetAcceptedAction(spAccepted);
			ShareCom<IGrammarAction> spAction;
			//Do-Ns-Body
			cr = _Create_WlangDoNsBodyAction(spAction);
			if( cr.IsFailed() )
				return cr;
			_COMPONENT_INSTANCE_INTERFACE(IGrammarAction, _ICplMetaDataActionUtility, spAction, spU, cr);
			if( cr.IsFailed() )
				return cr;
			arrUtility.Add(spU);  //may throw
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ns_body"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ns_null_body"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Body-Semi
			cr = _Create_WlangDoBodySemiAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_body_semi"), spAction);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//output
		sp = spGrammarAnalyzer;
		arrU = arrUtility;
		return cr;
	}

private:
	uint m_uMaxErrorNumber;

	ShareCom<ILexerAnalyzer> m_spLexerAnalyzer;
	ShareCom<IGrammarAnalyzer> m_spGrammarAnalyzer;

	ShareCom<ICplMetaData> m_spMeta;
	//for actions
	ShareArray<ShareCom<_ICplMetaDataActionUtility>> m_arrU;

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
