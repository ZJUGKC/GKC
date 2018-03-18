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
	virtual void SetInput(const GKC::ShareCom<GKC::ITextStream>& sp) throw()
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
	static CallResult create_lexer_analyzer(const _WmarkUtility_Objects& objs, ShareCom<ILexerAnalyzer>& sp) throw()
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
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), objs.spReturn);
			if( cr.IsFailed() )
				return cr;
			cr = spLexerAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), objs.spCommentStart);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//output
		sp = spLexerAnalyzer;
		return cr;
	}
	//create grammar analyzer
	static CallResult create_grammar_analyzer(const _WmarkUtility_Objects& objs, const ShareCom<ILexerAnalyzer>& spLexerAnalyzer,
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
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_INDENT"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "document"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "block_list"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "block"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "block_body"), objs.spPositionFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "return_set"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
		} //end block
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
			//Do-Doc-Rs-Block-List
			cr = _Create_WmarkDoDocRsBlockListAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_doc_rs_block_list"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_doc_rs_block_list_rs"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Block-List-List-Block
			cr = _Create_WmarkDoBlockListListBlockAction(spAction);
			if( cr.IsFailed() )
				return cr;
			_COMPONENT_INSTANCE_INTERFACE(IGrammarAction, _ICplMetaDataActionUtility, spAction, spU, cr);
			if( cr.IsFailed() )
				return cr;
			arrUtility.Add(spU);  //may throw
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_block_list_list_block"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Block-Block-Body
			cr = _Create_WmarkDoBlockBlockBodyAction(spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_block_block_body"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_block_block_body_rt"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_block_list_block"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_doc_block_list"), spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_doc_block_list_rs"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Block-Body-Indent
			cr = _Create_WmarkDoBlockBodyIndentAction(spAction);
			if( cr.IsFailed() )
				return cr;
			_COMPONENT_INSTANCE_INTERFACE(IGrammarAction, _ICplMetaDataActionUtility, spAction, spU, cr);
			if( cr.IsFailed() )
				return cr;
			arrUtility.Add(spU);  //may throw
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_block_body_indent"), spAction);
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
	WmarkParser(const WmarkParser&) throw();
	WmarkParser& operator=(const WmarkParser&) throw();
};

DECLARE_COM_TYPECAST(WmarkParser)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
