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
#ifndef __LDF_LEXER_H__
#define __LDF_LEXER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for lex and gra file

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

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_LEXER_H__
////////////////////////////////////////////////////////////////////////////////
