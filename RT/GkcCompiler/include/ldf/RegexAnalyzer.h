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
#ifndef __REGEX_ANALYZER_H__
#define __REGEX_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for regular expression

// _Regex_AST

class _Regex_AST
{
public:
	_Regex_AST() throw()
	{
	}
	~_Regex_AST() throw()
	{
	}

	//called only once
	void Init()
	{
		if( m_trees.IsBlockNull() )
			m_trees = ShareArrayHelper::MakeShareArray<AstTree>(MemoryHelper::GetCrtMemoryManager());  //may throw
	}

	void SetCount(uintptr uCount)
	{
		m_trees.SetCount(uCount, 0, RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_allocator)));  //may throw
	}

	AstTree& GetAST(uintptr uIndex) throw()
	{
		assert( uIndex < m_trees.GetCount() );
		return m_trees[uIndex].get_Value();
	}

private:
	ShareArray<AstTree> m_trees;
	ArrayPoolAllocator  m_allocator;

private:
	//noncopyable
	_Regex_AST(const _Regex_AST&) throw();
	_Regex_AST& operator=(const _Regex_AST&) throw();
};

inline CallResult _Regex_Generate_AST(const ShareArray<StringA>& arr, _Regex_AST& ast)
{
	CallResult cr;

	//lexer
	TokenTable tm_table;
	uint uID = CPL_TK_FIRST;
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_BACKSLASH"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_HEX"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_CR"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LN"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_TAB"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_SPACE"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LBRACKET"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RBRACKET"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LPAREN"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RPAREN"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LCURLY"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RCURLY"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_QUESTION"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_STAR"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_PLUS"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_MINUS"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_UPARROW"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_VERT"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LBRACKET"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RBRACKET"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MINUS"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_UPARROW"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_QUESTION"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_STAR"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_PLUS"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_VERT"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LPAREN"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RPAREN"), uID ++);  //may throw
	tm_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR"), uID ++);  //may throw
	tm_table.Finish();  //may throw
	LexerParser lexer;
	lexer.SetTokenTable(RefPtr<TokenTable>(tm_table));
	lexer.SetFsaTable(LdfRegexFsaTraits::GetTable());
	//actions
	{
		ShareCom<_ILexerAction> spAction;
		cr = _Create_RegexCharAction(spAction);
		if( cr.IsFailed() )
			return cr;
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_BACKSLASH"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_HEX"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_CR"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LN"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_TAB"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_SPACE"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LBRACKET"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RBRACKET"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LPAREN"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RPAREN"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LCURLY"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RCURLY"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_QUESTION"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_STAR"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_PLUS"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_MINUS"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_UPARROW"), spAction);  //may throw
		lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_VERT"), spAction);  //may throw
	} //end block
	//stream
	ShareCom<IBufferUtility> spBU;
	{
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateBufferStream(1, 1, spStream);  //any value
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(IByteStream, IBufferUtility, spStream, spBU, cr);
		if( cr.IsFailed() )
			return cr;
		ShareCom<ITextStream> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() )
			return cr;
		spText.Deref().SetStream(spStream);
		lexer.SetStream(spText);
	} //end block

	//grammar
	_LdfGrammarParser grammar;
	grammar.SetLexerParser(RefPtr<LexerParser>(lexer));
	//nonterminal
	TokenTable nt_table;
	uID = 101;
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_exp"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_term"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_factor_1"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_factor"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_charset"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char_item"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char_e"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char_range"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char_s"), uID ++);  //may throw
	nt_table.Finish();  //may throw
	grammar.SetNonterminalTable(RefPtr<TokenTable>(nt_table));
	//reduction action table for regular expression
	TokenTable ra_table;
	uID = 1;
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_exp_exp_term"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_exp_term"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_term_term_factor_1"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_term_factor_1"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_plus"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_star"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_question"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_factor"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_paren_exp"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_char"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_char_set"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_set"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_set_up"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_item_item_char_e"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_item_char_e"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_e_range"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_e_char"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_range"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_s"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char"), uID ++);  //may throw
	ra_table.Finish();  //may throw
	grammar.SetReductionActionTable(RefPtr<TokenTable>(ra_table));
	//pda table
	grammar.SetPdaTable(LdfRegexPdaTraits::GetTable());
	//factory
	{
		ShareCom<IComFactory> spCF;
		_BasicSymbolDataFactory_Create(spCF, cr);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR"), spCF);  //may throw
		cr = _Create_RegexCharSymbolDataFactory(spCF);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "regex_char_s"), spCF);  //may throw
	} //end block
	//actions
	{
		ShareCom<_IGrammarAction> spAction;
		//DoChar
		cr = _Create_RegexDoCharAction(spAction);
		if( cr.IsFailed() )
			return cr;
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char"), spAction);  //may throw
	} //end block

	//loop
	ast.SetCount(arr.GetCount());  //may throw
	uintptr index = 0;
	for( auto iter(arr.GetBegin()); iter != arr.GetEnd(); iter.MoveNext() ) {
		StringA& str = iter.get_Value();
		if( str.IsBlockNull() )
			continue;
		//buffer
		cr = spBU.Deref().SetBuffer((uintptr)ShareArrayHelper::GetInternalPointer(str), str.GetLength());
		if( cr.IsFailed() )
			return cr;
		//AST
		AstTree& atree = ast.GetAST(index);

		//execute
		cr = grammar.Execute();  //may throw
		if( cr.IsFailed() )
			return cr;

		//next
		index ++;
	} //end for

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__REGEX_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////
