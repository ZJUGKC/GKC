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
#ifndef __BASE_H__
#define __BASE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex and gra file

// functions

inline CallResult _Create_MacroTokenAction(ShareCom<_ILexerAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(MacroTokenAction, _ILexerAction, sp, cr);
	return cr;
}
inline CallResult _Create_DoIdTokenMacroAction(ShareCom<_IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(DoIdTokenMacroAction, _IGrammarAction, sp, cr);
	return cr;
}

// _LdfLexerParser

class _LdfLexerParser
{
public:
	_LdfLexerParser() throw()
	{
	}
	~_LdfLexerParser() throw()
	{
	}

	//called only once
	CallResult Initialize()
	{
		//token table
		uint uID = CPL_TK_FIRST;
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEP"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_ACTION"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LCURLY"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RCURLY"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COLON"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_VERT"), uID ++);  //may throw
		m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEMI"), uID ++);  //may throw
		m_table.Finish();  //may throw

		//lexer
		m_lexer.SetTokenTable(RefPtr<TokenTable>(m_table));
		m_lexer.SetFsaTable(LdfFsaTraits::GetTable());

		CallResult cr;
		//actions
		{
			ShareCom<_ILexerAction> spAction;
			_CommentStartAction_Create(spAction, cr);
			if( cr.IsFailed() )
				return cr;
			m_lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), spAction);  //may throw
			_SpaceAction_Create(spAction, cr);
			if( cr.IsFailed() )
				return cr;
			m_lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), spAction);  //may throw
			_ReturnAction_Create(spAction, cr);
			if( cr.IsFailed() )
				return cr;
			m_lexer.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), spAction);  //may throw
		} //end block

		return cr;
	}

	void SetAction(const ConstStringA& strToken, const ShareCom<_ILexerAction>& spAction)
	{
		m_lexer.SetAction(strToken, spAction);  //may throw
	}
	void SetStream(const ShareCom<ITextStream>& stream) throw()
	{
		m_lexer.SetStream(stream);
	}
	RefPtr<LexerParser> GetLexerParser() const throw()
	{
		return RefPtr<LexerParser>(m_lexer);
	}

private:
	TokenTable m_table;

	//lexer parser
	LexerParser m_lexer;

private:
	//noncopyable
	_LdfLexerParser(const _LdfLexerParser&) throw();
	_LdfLexerParser& operator=(const _LdfLexerParser&) throw();
};

// _LdfGrammarParser

class _LdfGrammarParser
{
public:
	_LdfGrammarParser() throw()
	{
	}
	~_LdfGrammarParser() throw()
	{
	}

	void SetLexerParser(const RefPtr<LexerParser>& lexer) throw()
	{
		m_grammar.SetLexerParser(lexer);
	}
	void SetNonterminalTable(const RefPtr<TokenTable>& nt) throw()
	{
		m_grammar.SetNonterminalTable(nt);
	}
	void SetReductionActionTable(const RefPtr<TokenTable>& ra) throw()
	{
		m_grammar.SetReductionActionTable(ra);
	}
	void SetPdaTable(const PDA_TABLE& table) throw()
	{
		m_grammar.SetPdaTable(table);
	}
	void SetAction(const ConstStringA& strAction, const ShareCom<_IGrammarAction>& spAction)
	{
		m_grammar.SetAction(strAction, spAction);  //may throw
	}
	void SetFactory(const ConstStringA& strEvent, const ShareCom<IComFactory>& sp)
	{
		m_grammar.SetFactory(strEvent, sp);  //may throw
	}

	CallResult Execute()
	{
		CallResult cr;
		//loop
		m_grammar.Start(true);  //may throw
		while( true ) {
			uint uLastEventNo;
			cr = m_grammar.Parse(uLastEventNo);  //may throw
			if( cr.IsFailed() )
				break;
			if( cr.GetResult() == SystemCallResults::S_False ) {
				if( uLastEventNo == PDA_LAST_EVENT_NO )
					cr.SetResult(SystemCallResults::Fail);  //The empty source file is an error
				break;
			}
			if( cr.GetResult() == SystemCallResults::OK ) {
				if( uLastEventNo != PDA_NO_EVENT ) {
					//error state
					cr = m_grammar.Revert();  //may throw
					if( cr.IsFailed() )
						break;
				}
			}
		} //end while
		//check
		if( cr.IsFailed() )
			return cr;
		if( m_grammar.GetErrorArray().GetCount() > 0 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		//successful
		cr.SetResult(SystemCallResults::OK);
		return cr;
	}

private:
	//grammar parser
	GrammarParser m_grammar;

private:
	//noncopyable
	_LdfGrammarParser(const _LdfGrammarParser&) throw();
	_LdfGrammarParser& operator=(const _LdfGrammarParser&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__BASE_H__
////////////////////////////////////////////////////////////////////////////////
