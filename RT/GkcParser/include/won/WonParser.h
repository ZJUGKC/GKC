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
This file contains component class of Won parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_PARSER_H__
#define __WON_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonParser

class WonParser : public _IWonParser
{
public:
	WonParser() throw()
	{
	}
	~WonParser() throw()
	{
	}

// _IWonParser methods
	virtual GKC::CallResult Initialize(const GKC::ShareCom<_IWonUtility>& sp) throw()
	{
		CallResult cr;

		//access
		_WonUtility_Objects objs;
		{
			ShareCom<_IWonUtility_Access> spAccess;
			_COMPONENT_INSTANCE_INTERFACE(_IWonUtility, _IWonUtility_Access, sp, spAccess, cr);
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
			cr = create_grammar_analyzer(objs, m_spLexerAnalyzer, RefPtr<_WonActionData>(m_data), m_action_set, m_spGrammarAnalyzer);
			if( cr.IsFailed() )
				return cr;
		}

		return cr;
	}
	virtual void SetInput(const GKC::ShareCom<GKC::ITextStreamRoot>& sp) throw()
	{
		assert( !m_spLexerAnalyzer.IsBlockNull() );
		int iCharType;
		m_spLexerAnalyzer.Deref().SetStream(sp, iCharType);
	}
	virtual GKC::CallResult Start() throw()
	{
		assert( !m_spGrammarAnalyzer.IsBlockNull() );
		return m_spGrammarAnalyzer.Deref().Start(true);
	}
	virtual GKC::CallResult GetNextToken(int& iType, GKC::StringA& str) throw()
	{
		assert( !m_spGrammarAnalyzer.IsBlockNull() );
		CallResult cr;
		m_data.set_Type(_WonTokenTypes::Unknown);
		do {
			//parse
			cr = m_spGrammarAnalyzer.Deref().Parse();
			//end
			if( cr.GetResult() == SystemCallResults::S_False )
				break;
			//error string
			const GKC::ShareArray<GKC::StringS> arrError(m_spGrammarAnalyzer.Deref().get_ErrorArray());
			uintptr uCount = arrError.GetCount();
			StringA strError;
			if( uCount > 0 ) {
				CallResult crCvt(convert_error_string(arrError[uCount - 1], strError));
				if( crCvt.IsFailed() ) {
					iType = _WonTokenTypes::Unknown;
					cr = crCvt;
					break;
				}
			}
			if( cr.IsFailed() ) {
				iType = _WonTokenTypes::Error;
				str = strError;
				break;
			}
			//unexpected
			if( m_spGrammarAnalyzer.Deref().IsErrorState() ) {
				iType = _WonTokenTypes::Unexpected;
				str = strError;
				break;
			}
			//unknown
			if( m_data.get_Type() == _WonTokenTypes::Unknown )
				continue;
			//normal
			iType = m_data.get_Type();
			str = m_data.get_String();
			break;
		} while( true );
		return cr;
	}

private:
	//error string
	static CallResult convert_error_string(const StringS& strError, StringA& str) throw()
	{
		CallResult cr;
		try {
			str = CS_S2U(StringUtilHelper::To_ConstString(strError)).GetV();  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	//create lexer analyzer
	static CallResult create_lexer_analyzer(const _WonUtility_Objects& objs, ShareCom<ILexerAnalyzer>& sp) throw()
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
	static CallResult create_grammar_analyzer(const _WonUtility_Objects& objs, const ShareCom<ILexerAnalyzer>& spLexerAnalyzer,
											const RefPtr<_WonActionData>& data, const _Won_ActionSet& was,
											ShareCom<IGrammarAnalyzer>& sp) throw()
	{
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
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_STRING"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_NUMBER_INTEGER"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_NUMBER_FLOAT"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_NUMBER_HEXADECIMAL"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_BOOLEAN_TRUE"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_BOOLEAN_FALSE"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_KEY_NULL"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LCURLY"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RCURLY"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LBRACKET"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RBRACKET"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COLON"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMA"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "object"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "array"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "value"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "begin_object"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "end_object"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "pair_list"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "pair"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "key"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "begin_array"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "end_array"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "value_list"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "string_catenation"), objs.spBasicFactory);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//actions
		{
			ShareCom<IGrammarAction> spAction;
			//Do-Value-String-Catenation
			cr = _Create_WonDoValueStringCatenationAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_string_catenation"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-String-Catenation
			cr = _Create_WonDoStringCatenationAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_string_catenation"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-String-Catenation-String
			cr = _Create_WonDoStringCatenationStringAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_string_catenation_string"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Number-Integer
			cr = _Create_WonDoValueNumberIntegerAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_number_integer"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Number-Float
			cr = _Create_WonDoValueNumberFloatAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_number_float"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Number-Hexadecimal
			cr = _Create_WonDoValueNumberHexadecimalAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_number_hexadecimal"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Boolean-True
			cr = _Create_WonDoValueBooleanTrueAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_boolean_true"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Boolean-False
			cr = _Create_WonDoValueBooleanFalseAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_boolean_false"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Value-Null
			cr = _Create_WonDoValueNullAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_value_null"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Begin-Object
			cr = _Create_WonDoBeginObjectAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_begin_object"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-End-Object
			cr = _Create_WonDoEndObjectAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_end_object"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Pair-Key-Value
			cr = _Create_WonDoEndPairAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_pair_key_value"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Key-String
			cr = _Create_WonDoKeyAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_key_string"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-Begin-Array
			cr = _Create_WonDoBeginArrayAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_begin_array"), spAction);
			if( cr.IsFailed() )
				return cr;
			//Do-End-Array
			cr = _Create_WonDoEndArrayAction(was, data, spAction);
			if( cr.IsFailed() )
				return cr;
			cr = spGrammarAnalyzer.Deref().SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_end_array"), spAction);
			if( cr.IsFailed() )
				return cr;
		} //end block
		//output
		sp = spGrammarAnalyzer;
		return cr;
	}

private:
	ShareCom<ILexerAnalyzer> m_spLexerAnalyzer;
	ShareCom<IGrammarAnalyzer> m_spGrammarAnalyzer;

	_WonActionData m_data;

	//for actions
	_Won_ActionSet m_action_set;

private:
	//noncopyable
	WonParser(const WonParser&) throw();
	WonParser& operator=(const WonParser&) throw();
};

DECLARE_COM_TYPECAST(WonParser)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
