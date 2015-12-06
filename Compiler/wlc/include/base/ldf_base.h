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
		// state map
		FSA_BEGIN_TRAITS_STATE_MAP(_LDF_FsaTraits)
			//transitions
			FSA_BEGIN_STATE_TRANSITION(FSA_STATE_START)
				FSA_STATE_TRANSITION_ENTRY('/', 2)
				FSA_STATE_TRANSITION_ENTRY(' ', 4)
				FSA_STATE_TRANSITION_ENTRY('\t', 4)
				FSA_STATE_TRANSITION_ENTRY('\r', 5)
				FSA_STATE_TRANSITION_ENTRY('\n', 7)
				FSA_STATE_TRANSITION_ENTRY('%', 8)
				FSA_STATE_TRANSITION_ENTRY('{', 10)
				FSA_STATE_TRANSITION_ENTRY('}', 11)
				FSA_STATE_TRANSITION_ENTRY('T', 12)
				FSA_STATE_TRANSITION_ENTRY('d', 16)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'c', 17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('e', 'z', 17)
				FSA_STATE_TRANSITION_ENTRY(':', 21)
				FSA_STATE_TRANSITION_ENTRY('|', 22)
				FSA_STATE_TRANSITION_ENTRY(';', 23)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(2)
				FSA_STATE_TRANSITION_ENTRY('*', 3)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(3)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(4)
				FSA_STATE_TRANSITION_ENTRY(' ', 4)
				FSA_STATE_TRANSITION_ENTRY('\t', 4)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(5)
				FSA_STATE_TRANSITION_ENTRY('\n', 6)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(6)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(7)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(8)
				FSA_STATE_TRANSITION_ENTRY('%', 9)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(9)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(10)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(11)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(12)
				FSA_STATE_TRANSITION_ENTRY('K', 13)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(13)
				FSA_STATE_TRANSITION_ENTRY('_', 14)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(14)
				FSA_STATE_TRANSITION_RANGE_ENTRY('A', 'Z', 15)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(15)
				FSA_STATE_TRANSITION_RANGE_ENTRY('A', 'Z', 15)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 15)
				FSA_STATE_TRANSITION_ENTRY('_', 15)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(16)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'n', 17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('p', 'z', 17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				FSA_STATE_TRANSITION_ENTRY('_', 17)
				FSA_STATE_TRANSITION_ENTRY('o', 18)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'z', 17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				FSA_STATE_TRANSITION_ENTRY('_', 17)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(18)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'z', 17)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				FSA_STATE_TRANSITION_ENTRY('_', 19)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(19)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'z', 20)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				FSA_STATE_TRANSITION_ENTRY('_', 17)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(20)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'z', 20)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 20)
				FSA_STATE_TRANSITION_ENTRY('_', 20)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(21)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(22)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(23)
			FSA_END_STATE_TRANSITION()

			//state
			FSA_BEGIN_STATE_SET()
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, FSA_STATE_START, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 2, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 3, -1)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 4, -2)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 5, -3)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 6, -3)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 7, -3)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 8, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 9, -4)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 10, -8)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 11, -9)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 12, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 13, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 14, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 15, -7)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 16, -6)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 17, -6)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 18, -6)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 19, -6)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 20, -5)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 21, -10)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 22, -11)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 23, -12)
			FSA_END_STATE_SET()
		FSA_END_TRAITS_STATE_MAP()

		// match map
		FSA_BEGIN_TRAITS_MATCH_MAP(_LDF_FsaTraits)
			FSA_STATE_MATCH_ENTRY(0)
			FSA_STATE_MATCH_ENTRY(1)   //TK_COMMENT_START
			FSA_STATE_MATCH_ENTRY(2)   //TK_SPACE
			FSA_STATE_MATCH_ENTRY(3)   //TK_RETURN
			FSA_STATE_MATCH_ENTRY(4)   //TK_SEP
			FSA_STATE_MATCH_ENTRY(5)   //TK_ACTION
			FSA_STATE_MATCH_ENTRY(6)   //TK_MACRO
			FSA_STATE_MATCH_ENTRY(7)   //TK_TOKEN
			FSA_STATE_MATCH_ENTRY(8)   //TK_LCURLY
			FSA_STATE_MATCH_ENTRY(9)   //TK_RCURLY
			FSA_STATE_MATCH_ENTRY(10)  //TK_COLON
			FSA_STATE_MATCH_ENTRY(11)  //TK_VERT
			FSA_STATE_MATCH_ENTRY(12)  //TK_SEMI
		FSA_END_TRAITS_MATCH_MAP()
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
			m_lexTable.SetTokenTable(RefPtrHelper::ToRefPtr(m_ldfTokenTable.GetTable()));
			m_lexParser.SetLexerTable(RefPtrHelper::ToRefPtr(m_lexTable));
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
