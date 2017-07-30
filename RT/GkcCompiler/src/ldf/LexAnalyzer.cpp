/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains the functions for lex file.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/FsaTable.h"
#include "base/Lexer.h"
#include "base/Pda.h"
#include "base/PdaTable.h"
#include "base/Grammar.h"

#include "ldf/base/LdfFsa.h"
#include "ldf/base/LdfLexer.h"
#include "ldf/base/LdfGrammar.h"

#include "ldf/regex/RegexDef.h"
#include "ldf/regex/LdfRegexFsa.h"
#include "ldf/regex/LdfRegexPda.h"
#include "ldf/regex/RegexCharAction.h"
#include "ldf/regex/RegexCharSymbolData.h"
#include "ldf/regex/RegexCharSymbolData_Factory.h"
#include "ldf/regex/RegexCharSetSymbolData.h"
#include "ldf/regex/RegexCharSetSymbolData_Factory.h"
#include "ldf/regex/RegexPositionSymbolData.h"
#include "ldf/regex/RegexPositionSymbolData_Factory.h"
#include "ldf/regex/RegexDoCharAction.h"
#include "ldf/regex/RegexDoCharSAction.h"
#include "ldf/regex/RegexDoCharRangeAction.h"
#include "ldf/regex/RegexDoCharItemCharEAction.h"
#include "ldf/regex/RegexDoCharItemItemCharEAction.h"
#include "ldf/regex/RegexDoCharSetAction.h"
#include "ldf/regex/RegexDoCharSetUpAction.h"
#include "ldf/regex/RegexDoFactorCharSetAction.h"
#include "ldf/regex/RegexDoFactorCharAction.h"
#include "ldf/regex/RegexDoFactorParenExpAction.h"
#include "ldf/regex/RegexDoFactor1QuestionAction.h"
#include "ldf/regex/RegexDoFactor1StarAction.h"
#include "ldf/regex/RegexDoFactor1PlusAction.h"
#include "ldf/regex/RegexDoTermTermFactor1Action.h"
#include "ldf/regex/RegexDoExpExpTermAction.h"
#include "ldf/regex/RegexGrammarAccepted.h"

#include "ldf/lex/LexDef.h"
#include "ldf/lex/LdfLexPda.h"
#include "ldf/lex/MacroTokenAction.h"
#include "ldf/lex/DoIdTokenMacroAction.h"

#include "ldf/LexAction.h"
#include "ldf/RegexAction.h"
#include "ldf/RegexAnalyzer.h"
#include "ldf/RegexDotAlg.h"
#include "ldf/LexAnalyzer.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex file

// _Lex_Data

class _Lex_Data
{
public:
	explicit _Lex_Data(TokenTable& tt) throw() : m_token_table(tt)
	{
	}
	~_Lex_Data() throw()
	{
	}

	//called only once
	void Init()
	{
		if( m_token_regex.IsBlockNull() )
			m_token_regex = ShareArrayHelper::MakeShareArray<_LexerTokenString>(MemoryHelper::GetCrtMemoryManager());  //may throw
		if( m_macro_regex.IsBlockNull() )
			m_macro_regex = ShareArrayHelper::MakeShareArray<_LexerTokenString>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_token_id = CPL_TK_FIRST;
		m_macro_id = CPL_TK_FIRST;
	}
	void Finish()
	{
		m_token_table.Finish();  //may throw
		m_macro_table.Finish();  //may throw
	}

	RefPtr<TokenTable> GetTokenTable() const throw()
	{
		return RefPtr<TokenTable>(m_token_table);
	}
	RefPtr<TokenTable> GetMacroTable() const throw()
	{
		return RefPtr<TokenTable>(m_macro_table);
	}
	ShareArray<_LexerTokenString>& GetTokenRegex() throw()
	{
		return m_token_regex;
	}
	ShareArray<_LexerTokenString>& GetMacroRegex() throw()
	{
		return m_macro_regex;
	}
	RefPtr<uint> GetTokenId() const throw()
	{
		return RefPtr<uint>(m_token_id);
	}
	RefPtr<uint> GetMacroId() const throw()
	{
		return RefPtr<uint>(m_macro_id);
	}

	void ExpandTokenMacros()
	{
		for( auto iter(m_token_regex.GetBegin()); iter != m_token_regex.GetEnd(); iter.MoveNext() )
			expand_macro_one(iter.get_Value());  //may throw
	}

private:
	void expand_macro_one(_LexerTokenString& str)
	{
		//loop
		do {
			uintptr uLength = str.GetLength();
			//find
			uintptr uCount = 0;
			uintptr uStart = 0;
			do {
				if( uStart >= uLength )
					break;
				CharF ch;
				str.GetAt(uStart, ch);
				if( ch != '{' ) {
					uStart ++;
					continue;
				}
				if( uStart != 0 ) {
					str.GetAt(uStart - 1, ch);
					// \{
					if( ch == '\\' ) {
						uStart ++;
						continue;
					}
				}
				uintptr uLeftC = uStart;
				uCount ++;
				uStart ++;
				do {
					if( uStart >= uLength ) {
						uCount --;
						break;
					}
					str.GetAt(uStart, ch);
					if( ch != '}' ) {
						uStart ++;
						continue;
					}
					str.GetAt(uStart - 1, ch);
					// \}
					if( ch == '\\' ) {
						uStart ++;
						continue;
					}
					uintptr uLen = uStart - uLeftC - 1;
					uStart ++;
					//empty
					if( uLen == 0 ) {
						uCount --;
						break;
					}
					//macro name
					_LexerTokenString strMacro;
					str.ToSubString(uLeftC + 1, uLen, strMacro);  //may throw
					StringA strU(strMacro.ToUTF8());  //may throw
					ConstStringA c_macro_name(StringUtilHelper::To_ConstString(strU));
					RefPtr<_LexerTokenString> rStrV;
					uint id = m_macro_table.get_ID(c_macro_name);
					if( id == 0 ) {
						id = m_token_table.get_ID(c_macro_name);
						if( id == 0 ) {
							uCount --;
							break;
						}
						rStrV = RefPtr<_LexerTokenString>(m_token_regex[id - m_token_table.GetMinID()].get_Value());
					}
					else {
						rStrV = RefPtr<_LexerTokenString>(m_macro_regex[id - m_macro_table.GetMinID()].get_Value());
					}
					str.SetAt(uLeftC, '(');
					str.SetAt(uStart - 1, ')');
					str.Delete(uLeftC + 1, uLen);
					str.Insert(uLeftC + 1, rStrV.Deref());  //may throw
					//now uStart and uLength are invalid
					uStart = uLeftC + 1 + rStrV.Deref().GetLength() + 1;
					uLength = str.GetLength();
					break;
				} while(true);
			} while(true);
			if( uCount == 0 )
				break;
		} while(true);
	}

private:
	//table
	TokenTable& m_token_table;
	TokenTable m_macro_table;
	//regular expression
	ShareArray<_LexerTokenString> m_token_regex;
	ShareArray<_LexerTokenString> m_macro_regex;
	//id
	uint m_token_id;
	uint m_macro_id;

private:
	//noncopyable
	_Lex_Data(const _Lex_Data&) throw();
	_Lex_Data& operator=(const _Lex_Data&) throw();
};

CallResult _Generate_Lexer_Tables(const ShareCom<ITextStream>& sp, TokenTable& tokenTable, FsaTable& fsaTable)
{
	CallResult cr;

	//lexer
	_LdfLexerParser lexer;
	cr = lexer.Initialize();  //may throw
	if( cr.IsFailed() )
		return cr;
	//actions
	{
		ShareCom<_ILexerAction> spMacroToken;
		cr = _Create_MacroTokenAction(spMacroToken);
		if( cr.IsFailed() )
			return cr;
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"), spMacroToken);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"), spMacroToken);  //may throw
	} //end block
	//stream
	int iCharType;
	lexer.SetStream(sp, iCharType);

	//grammar
	_LdfGrammarParser grammar;
	grammar.SetLexerParser(lexer.GetLexerParser());
	//nonterminal
	TokenTable nt_table;
	uint uID = 101;
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "lex_def"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_block"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "id"), uID ++);  //may throw
	nt_table.Finish();  //may throw
	grammar.SetNonterminalTable(RefPtr<TokenTable>(nt_table));
	//reduction action table for lex file
	TokenTable ra_table;
	uID = 1;
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ref"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_block_id"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_id"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), uID ++);  //may throw
	ra_table.Finish();  //may throw
	grammar.SetReductionActionTable(RefPtr<TokenTable>(ra_table));
	//pda table
	grammar.SetPdaTable(LdfLexPdaTraits::GetTable());
	//factory
	{
		ShareCom<IComFactory> spCF;
		_BasicSymbolDataFactory_Create(spCF, cr);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEP"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "lex_def"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_block"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "id"), spCF);  //may throw
	} //end block
	//data
	_Lex_Data lex_data(tokenTable);
	lex_data.Init();  //may throw
	//actions
	{
		ShareCom<_IGrammarAction> spAction;
		//DoIdToken
		cr = _Create_DoIdTokenMacroAction(spAction);
		if( cr.IsFailed() )
			return cr;
		ShareCom<_I_IdTokenMacroAction_Utility> spUtility;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_IdTokenMacroAction_Utility, spAction, spUtility, cr);
		assert( cr.IsSucceeded() );
		spUtility.Deref().SetOutput(lex_data.GetTokenTable(), lex_data.GetTokenRegex(), lex_data.GetTokenId());
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), spAction);  //may throw
		//DoIdMacro
		cr = _Create_DoIdTokenMacroAction(spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_IdTokenMacroAction_Utility, spAction, spUtility, cr);
		assert( cr.IsSucceeded() );
		spUtility.Deref().SetOutput(lex_data.GetMacroTable(), lex_data.GetMacroRegex(), lex_data.GetMacroId());
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), spAction);  //may throw
	} //end block

	//execute
	cr = grammar.Execute();  //may throw
	if( cr.IsFailed() )
		return cr;

	lex_data.Finish();  //may throw
	lex_data.ExpandTokenMacros();  //may throw

	assert( lex_data.GetTokenRegex().GetCount() > 0 );

	//check overflow
	{
		//for debugging
		TokenTable& tt = lex_data.GetTokenTable().Deref();
		tt;
		assert( tt.GetMinID() == CPL_TK_FIRST
				&& tt.GetMaxID() < (uint)(Limits<int>::Max - 1) );  //with last NULL item
	} //end block

	//AST
	_Regex_AST rast;
	rast.Init();  //may throw
	cr = _Regex_Generate_AST(lex_data.GetTokenRegex(), iCharType, rast);  //may throw
	if( cr.IsFailed() )
		return cr;

	//Generate
	_Regex_Generate_Tables(rast, fsaTable);  //may throw

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
