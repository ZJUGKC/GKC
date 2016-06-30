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
#ifndef ___LDF_BASE_H__
#define ___LDF_BASE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _ldf_helper

class _ldf_helper
{
//------------------------------------------------------------------------------
//for lex and gra file

private:
	// token table
	class _LDF_TokenTable
	{
	public:
		//called only once
		void Init()
		{
			uint uID = TK_FIRST;
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEP"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_ACTION"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LCURLY"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RCURLY"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COLON"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_VERT"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEMI"), uID ++);
		}

		const TokenTable& GetTable() const throw()
		{
			return m_table;
		}

	private:
		TokenTable m_table;
	};

	// FSA for lex & grammar file
	class _LDF_FsaTraits
	{
	public:
	};

	typedef FiniteStateMachineT<_LDF_FsaTraits>  _LDF_FSM;

	// lexer actions
	class Lexer_MacroTokenAction : public ILexerAction
	{
	public:
		virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info)
		{
			//receive the regular expression
			info.GetData().SetLength(0);
			CallResult cr;
			byte ch;
			//next char
			cr = _get_next_char(stream, ch);
			if( cr.IsFailed() )
				return ;
			//skip blanks
			while( ch == ' ' || ch == '\t' ) {
				(info.GetCharEnd().uCharIndex) ++;
				(info.GetCharEnd().uCol) ++;
				//next char
				cr = _get_next_char(stream, ch);
				if( cr.IsFailed() )
					return ;
			}
			//regex
			while( ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' ) {
				(info.GetCharEnd().uCharIndex) ++;
				(info.GetCharEnd().uCol) ++;
				StringUtilHelper::Append((CharA)ch, info.GetData());  //may throw
				//next char
				cr = _get_next_char(stream, ch);
				if( cr.IsFailed() )
					return ;
			}
			//back char
			cr = stream.Deref().UngetChar(1);
			assert( cr.IsOK() );
		}

	private:
		static CallResult _get_next_char(RefPtr<ICharStream>& stream, byte& ch) throw()
		{
			CallResult cr;
			cr = stream.Deref().GetChar(ch);
			if( cr.GetResult() == SystemCallResults::S_EOF )
				cr.SetResult(SystemCallResults::Fail);
			if( cr.IsFailed() )
				return cr;
			return cr;
		}
	};

public:
	// _LDF_LexerParser
	class _LDF_LexerParser
	{
	public:
		//called only once
		void Init()
		{
			m_ldfTokenTable.Init();  //may throw
			m_lexTable.SetFSA(RefPtrHelper::TypeCast<_LDF_FSM, FiniteStateAutomata>(RefPtr<_LDF_FSM>(m_fsm)));
			m_lexTable.SetTokenTable(RefPtrHelper::MakeRefPtr(m_ldfTokenTable.GetTable()));
			m_lexParser.SetLexerTable(RefPtrHelper::MakeRefPtr(m_lexTable));
			//actions
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"),
								RefPtrHelper::TypeCast<Lexer_CommentStartAction, ILexerAction>(RefPtr<Lexer_CommentStartAction>(m_actionCommentStart)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"),
								RefPtrHelper::TypeCast<Lexer_SpaceAction, ILexerAction>(RefPtr<Lexer_SpaceAction>(m_actionSpace)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"),
								RefPtrHelper::TypeCast<Lexer_ReturnAction, ILexerAction>(RefPtr<Lexer_ReturnAction>(m_actionReturn)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"),
								RefPtrHelper::TypeCast<Lexer_MacroTokenAction, ILexerAction>(RefPtr<Lexer_MacroTokenAction>(m_actionMacroToken)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"),
								RefPtrHelper::TypeCast<Lexer_MacroTokenAction, ILexerAction>(RefPtr<Lexer_MacroTokenAction>(m_actionMacroToken)));
		}

		void SetStream(const RefPtr<ICharStream>& stream) throw()
		{
			m_lexParser.SetStream(stream);
		}

		RefPtr<LexerParser> GetLexerParser() throw()
		{
			return RefPtr<LexerParser>(m_lexParser);
		}

	private:
		//token table
		_LDF_TokenTable m_ldfTokenTable;
		//FSA
		_LDF_FSM m_fsm;
		//lexer table
		LexerTable m_lexTable;
		//lexer parser
		LexerParser m_lexParser;
		//actions
		Lexer_CommentStartAction m_actionCommentStart;
		Lexer_SpaceAction m_actionSpace;
		Lexer_ReturnAction m_actionReturn;
		Lexer_MacroTokenAction m_actionMacroToken;
	};

private:
	// _LDF_GrammarParser<T>
	template <class T>
	class _LDF_GrammarParser
	{
	public:
		void SetLexerParser(const RefPtr<LexerParser>& parser) throw()
		{
			m_graParser.SetLexerParser(parser);
		}
		void SetGrammarTable(const RefPtr<GrammarTable<T>>& table) throw()
		{
			m_graParser.SetGrammarTable(table);
		}

		GrammarParser<T>& GetGrammarParser() throw()
		{
			return m_graParser;
		}

		CallResult Execute()
		{
			CallResult cr;
			//loop
			m_graParser.Start();
			bool bEmpty;
			cr.SetResult(SystemCallResults::OK);
			while( cr.GetResult() != SystemCallResults::S_False ) {
				cr = m_graParser.Parse(bEmpty);
				if( cr.IsFailed() )
					break;
			}
			if( cr.IsFailed() )
				return cr;
			if( m_graParser.GetErrorArray().GetCount() > 0 ) {
				cr.SetResult(SystemCallResults::Fail);
				return cr;
			}
			return cr;
		}

	private:
		GrammarParser<T> m_graParser;
	};
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //___LDF_BASE_H__
////////////////////////////////////////////////////////////////////////////////
