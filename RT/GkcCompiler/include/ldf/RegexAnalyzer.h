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
#ifndef ___LDF_REGEX_H__
#define ___LDF_REGEX_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for regular expression

private:
	// token table
	class _LDF_regex_TokenTable
	{
	public:
		//called only once
		void Init()
		{
			uint uID = TK_FIRST;
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_BACKSLASH"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_HEX"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_CR"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_TAB"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_SPACE"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LBRACKET"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RBRACKET"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LPAREN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RPAREN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LCURLY"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RCURLY"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_QUESTION"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_STAR"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_PLUS"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_MINUS"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_UPARROW"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_VERT"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LBRACKET"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RBRACKET"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MINUS"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_UPARROW"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_QUESTION"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_STAR"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_PLUS"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_VERT"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LPAREN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RPAREN"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR"), uID ++);
		}

		const TokenTable& GetTable() const throw()
		{
			return m_table;
		}

	private:
		TokenTable m_table;
	};

	// fsa
	class _LDF_regex_FsaTraits
	{
	public:
	};

	typedef FiniteStateMachineT<_LDF_regex_FsaTraits>  _LDF_REGEX_FSM;

	// actions
	class Regexlex_CharAction : public ILexerAction
	{
	public:
		virtual void DoAction(INOUT RefPtr<ICharStream>& stream, INOUT LexerTokenInfo& info)
		{
			//fetch the actual character
			StringA& str = info.GetBuffer();
			assert( str.GetLength() > 1 );
			//value
			CharA ch = str.GetAt(1).get_Value();
			if( ch == 'r' )
				str.GetAt(0).set_Value('\r');
			else if( ch == 'n' )
				str.GetAt(0).set_Value('\n');
			else if( ch == 't' )
				str.GetAt(0).set_Value('\t');
			else if( ch == 's' )
				str.GetAt(0).set_Value(' ');
			else if( ch == 'x' ) {
				bool bOK;
				uint v;
				string_to_value(SharedArrayHelper::GetInternalPointer(str) + 2, 16, v, bOK);  //no check
				assert( bOK );
				str.GetAt(0).set_Value((CharA)v);
			}
			else
				str.GetAt(0).set_Value(ch);
			str.SetLength(1);
			//change ID
			info.SetID(29);  //TK_CHAR
		}
	};

	// _LDF_regex_LexerParser
	class _LDF_regex_LexerParser
	{
	public:
		//called only once
		void Init()
		{
			m_tokenTable.Init();  //may throw
			m_lexTable.SetFSA(RefPtrHelper::TypeCast<_LDF_REGEX_FSM, FiniteStateAutomata>(RefPtr<_LDF_REGEX_FSM>(m_fsm)));
			m_lexTable.SetTokenTable(RefPtrHelper::MakeRefPtr(m_tokenTable.GetTable()));
			m_lexParser.SetLexerTable(RefPtrHelper::MakeRefPtr(m_lexTable));
			//actions
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_BACKSLASH"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_HEX"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_CR"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LN"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_TAB"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_SPACE"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LBRACKET"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RBRACKET"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LPAREN"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RPAREN"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LCURLY"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RCURLY"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_QUESTION"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_STAR"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_PLUS"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_MINUS"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_UPARROW"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_VERT"),
								RefPtrHelper::TypeCast<Regexlex_CharAction, ILexerAction>(RefPtr<Regexlex_CharAction>(m_actionChar)));
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
		_LDF_regex_TokenTable m_tokenTable;
		//FSA
		_LDF_REGEX_FSM m_fsm;
		//lexer table
		LexerTable m_lexTable;
		//lexer parser
		LexerParser m_lexParser;
		//actions
		Regexlex_CharAction m_actionChar;
	};

	// _LDF_regex_SymUserData
	class _LDF_regex_SymUserData : public SymbolDataBase
	{
	public:
		void GetCharRange(uint& low, uint& high) const throw()
		{
			low  = m_iLow;
			high = m_iHigh;
		}
		void SetCharRange(const uint& low, const uint& high) throw()
		{
			m_iLow  = low;
			m_iHigh = high;
		}

	private:
		uint m_iLow;
		uint m_iHigh;
	};

	// reduction action table for regular expression
	class _LDF_regex_ActionTable
	{
	public:
		//called only once
		void Init()
		{
			uint uID = 1;
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_exp_exp_term"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_exp_term"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_term_term_factor_1"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_term_factor_1"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_plus"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_star"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_question"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_1_factor"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_paren_exp"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_char"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_factor_char_set"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_set"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_set_up"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_item_item_char_e"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_item_char_e"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_e_range"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_e_char"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_range"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char_s"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char"), uID ++);
			m_table.Finish();
		}

		const TokenTable& GetTable() const throw()
		{
			return m_table;
		}

	private:
		TokenTable m_table;
	};

	// PDA for regular expression
	class _LDF_regex_PdaTraits
	{
	public:
	};

	typedef PushDownMachineT<_LDF_regex_SymUserData, _LDF_regex_PdaTraits> _LDF_regex_PDM;

	// regex operators
	enum {
		REGEX_OP_LINK,
		REGEX_OP_OR,
		REGEX_OP_QUESTION,
		REGEX_OP_STAR,
		REGEX_OP_PLUS,
		REGEX_OP_MAX
	};

	// grammar actions
	class Regexgra_DoCharAction : public IGrammarAction<_LDF_regex_SymUserData>
	{
	public:
		virtual void DoAction(INOUT SharedArray<RefPtr<_LDF_regex_SymUserData>>& arr, INOUT SharedArray<StringS>& errorArray)
		{
		}
	};

	// _LDF_regex_GrammarTable
	class _LDF_regex_GrammarTable
	{
	public:
		//called only once
		void Init()
		{
			m_actTable.Init();  //may throw
			m_graTable.SetPDA(RefPtrHelper::TypeCast<_LDF_regex_PDM, typename _LDF_regex_PDM::baseClass>(RefPtr<_LDF_regex_PDM>(m_pdm)));
			m_graTable.SetReductionActionTable(RefPtrHelper::MakeRefPtr(m_actTable.GetTable()));
		}

		RefPtr<GrammarTable<_LDF_regex_SymUserData>> GetGrammarTable() throw()
		{
			return RefPtr<GrammarTable<_LDF_regex_SymUserData>>(m_graTable);
		}

	private:
		_LDF_regex_ActionTable m_actTable;
		_LDF_regex_PDM m_pdm;
		GrammarTable<_LDF_regex_SymUserData> m_graTable;
	};

	// _LDF_regex_GrammarAction
	class _LDF_regex_GrammarAction
	{
	public:
		_LDF_regex_GrammarAction() throw()
		{
		}

		void Apply(GrammarParser<_LDF_regex_SymUserData>& graParser)
		{
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_char"),
								RefPtrHelper::TypeCast<Regexgra_DoCharAction, IGrammarAction<_LDF_regex_SymUserData>>(RefPtr<Regexgra_DoCharAction>(m_actionDoChar)));
		}

	private:
		Regexgra_DoCharAction m_actionDoChar;
	};

	// _LDF_regex_GrammarParser
	typedef _LDF_GrammarParser<_LDF_regex_SymUserData>  _LDF_regex_GrammarParser;

public:
	// _LDF_regex_AST
	class _LDF_regex_AST
	{
	public:
		_LDF_regex_AST() throw()
		{
		}

		//no cleanup
		CallResult Generate(const SharedArray<StringA>& regex)
		{
			CallResult cr;
			for( auto iter(regex.GetBegin()); iter != regex.GetEnd(); iter.MoveNext() ) {
				cr = generate_one(iter.get_Value());
				if( cr.IsFailed() )
					return cr;
			}
			return cr;
		}

	private:
		CallResult generate_one(const StringA& str)
		{
			if( str.IsNull() )
				return CallResult(SystemCallResults::OK);

			CallResult cr;
			ConstArray<byte> buffer;
			ConstArrayHelper::SetInternalPointer((byte*)SharedArrayHelper::GetInternalPointer(str), str.GetLength(), buffer);
			//stream
			MemoryCharStream stream;
			stream.Initialize(buffer);

			//lexer parser
			_LDF_regex_LexerParser lexer;
			lexer.Init();
			lexer.SetStream(RefPtrHelper::TypeCast<MemoryCharStream, ICharStream>(RefPtr<MemoryCharStream>(stream)));

			//grammar parser
			_LDF_regex_GrammarParser parser;
			parser.SetLexerParser(lexer.GetLexerParser());
			// grammar table
			_LDF_regex_GrammarTable graTable;
			graTable.Init();
			parser.SetGrammarTable(graTable.GetGrammarTable());
			// action
			_LDF_regex_GrammarAction action;
			action.Apply(parser.GetGrammarParser());

			//execute
			cr = parser.Execute();
			if( cr.IsFailed() )
				return cr;

			cr.SetResult(SystemCallResults::OK);
			return cr;
		}

	private:
		SharedArray<AstTree> m_trees;
		DataPoolAllocator m_alloc;
	};


////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //___LDF_REGEX_H__
////////////////////////////////////////////////////////////////////////////////
