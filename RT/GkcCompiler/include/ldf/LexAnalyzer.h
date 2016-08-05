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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEX_ANALYZER_H__
#define __LEX_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex file

// _Lex_Data

class _Lex_Data
{
public:
	_Lex_Data() throw()
	{
	}
	~_Lex_Data() throw()
	{
	}

	//called only once
	void Init()
	{
		if( m_token_regex.IsBlockNull() )
			m_token_regex = ShareArrayHelper::MakeShareArray<StringA>(MemoryHelper::GetCrtMemoryManager());  //may throw
		if( m_macro_regex.IsBlockNull() )
			m_macro_regex = ShareArrayHelper::MakeShareArray<StringA>(MemoryHelper::GetCrtMemoryManager());  //may throw
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
	ShareArray<StringA>& GetTokenRegex() throw()
	{
		return m_token_regex;
	}
	ShareArray<StringA>& GetMacroRegex() throw()
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
	void expand_macro_one(StringA& str)
	{
		if( str.IsBlockNull() )
			return ;

		//loop
		do {
			//find
			uintptr uCount = 0;
			uintptr uStart = 0;
			do {
				auto iter(StringHelper::Find(str, '{', uStart));
				if( iter.IsNull() )
					break;
				intptr delta = iter.CalcDelta(str.GetAt(uStart));
				if( iter != str.GetBegin() ) {
					auto iter1(iter);
					iter1.MovePrev();
					// \{
					if( iter1.get_Value() == '\\' ) {
						uStart += (delta + 1);
						continue;
					}
				}
				uStart += (delta + 1);
				uCount ++;
				uintptr uLeftC = uStart;
				do {
					auto iterE(StringHelper::Find(str, '}', uStart));
					if( iterE.IsNull() ) {
						uCount --;
						break;
					}
					delta = iterE.CalcDelta(str.GetAt(uStart));
					auto iter1(iterE);
					iter1.MovePrev();
					// \}
					if( iter1.get_Value() == '\\' ) {
						uStart += (delta + 1);
						continue;
					}
					uStart += (delta + 1);
					intptr len = iterE.CalcDelta(iter);
					//empty
					if( len <= 1 ) {
						uCount --;
						break;
					}
					//macro name
					StringA strMacro(StringHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));  //may throw
					StringUtilHelper::Sub(str, uLeftC, len - 1, strMacro);  //may throw
					ConstStringA c_macro_name(StringUtilHelper::To_ConstString(strMacro));
					StringA strV;
					uint id = m_macro_table.get_ID(c_macro_name);
					if( id == 0 ) {
						id = m_token_table.get_ID(c_macro_name);
						if( id == 0 ) {
							uCount --;
							break;
						}
						strV = m_token_regex[id - m_token_table.GetMinID()].get_Value();
					}
					else {
						strV = m_macro_regex[id - m_macro_table.GetMinID()].get_Value();
					}
					iter.get_Value() = '(';
					iterE.get_Value() = ')';
					StringHelper::Delete(uLeftC, len - 1, str);
					StringUtilHelper::Insert(uLeftC, strV, str);  //may throw
					//now iterators are invalid
					uStart = uLeftC + strV.GetLength() + 1;
					break;
				} while(true);
			} while(true);
			if( uCount == 0 )
				break;
		} while(true);
	}

private:
	//table
	TokenTable m_token_table;
	TokenTable m_macro_table;
	//regular expression
	ShareArray<StringA> m_token_regex;
	ShareArray<StringA> m_macro_regex;
	//id
	uint m_token_id;
	uint m_macro_id;

private:
	//noncopyable
	_Lex_Data(const _Lex_Data&) throw();
	_Lex_Data& operator=(const _Lex_Data&) throw();
};

inline CallResult _Generate_Lexer_Tables(const ShareCom<ITextStream>& sp)
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
	lexer.SetStream(sp);

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
	_Lex_Data lex_data;
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

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LEX_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////
