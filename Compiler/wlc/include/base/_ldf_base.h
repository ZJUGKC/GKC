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
	class MacroTokenAction : public ILexerAction
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
								RefPtrHelper::TypeCast<CommentStartAction, ILexerAction>(RefPtr<CommentStartAction>(m_actionCommentStart)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"),
								RefPtrHelper::TypeCast<SpaceAction, ILexerAction>(RefPtr<SpaceAction>(m_actionSpace)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"),
								RefPtrHelper::TypeCast<ReturnAction, ILexerAction>(RefPtr<ReturnAction>(m_actionReturn)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"),
								RefPtrHelper::TypeCast<MacroTokenAction, ILexerAction>(RefPtr<MacroTokenAction>(m_actionMacroToken)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"),
								RefPtrHelper::TypeCast<MacroTokenAction, ILexerAction>(RefPtr<MacroTokenAction>(m_actionMacroToken)));
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
		CommentStartAction m_actionCommentStart;
		SpaceAction m_actionSpace;
		ReturnAction m_actionReturn;
		MacroTokenAction m_actionMacroToken;
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

//------------------------------------------------------------------------------
//for lex file

private:
	// _LDF_lex_SymUserData
	class _LDF_lex_SymUserData : public SymbolDataBase
	{
	public:
	};

	// reduction action table for lex file
	class _LDF_Lex_ActionTable
	{
	public:
		//called only once
		void Init()
		{
			uint uID = 1;
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ref"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_block_id"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_id"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), uID ++);
			m_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), uID ++);
		}

		const TokenTable& GetTable() const throw()
		{
			return m_table;
		}

	private:
		TokenTable m_table;
	};

	// PDA for lex file
	class _LDF_Lex_PdaTraits
	{
	public:
		// state map
		PDA_BEGIN_TRAITS_STATE_MAP(_LDF_Lex_PdaTraits)
			//transitions
			PDA_BEGIN_STATE_TRANSITION(PDA_STATE_START)
				PDA_STATE_TRANSITION_ENTRY(101, 3)  //lex_def
				PDA_STATE_TRANSITION_ENTRY(4, 4)    //TK_SEP
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(3)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, PDA_STATE_ACCEPTED)  //$
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(4)
				PDA_STATE_TRANSITION_ENTRY(7, 5)    //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, 6)    //TK_MACRO
				PDA_STATE_TRANSITION_ENTRY(103, 7)  //id
				PDA_STATE_TRANSITION_ENTRY(102, 8)  //rule_block
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(5)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -4)  //$
				PDA_STATE_TRANSITION_ENTRY(7, -4)    //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, -4)    //TK_MACRO
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(6)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -5)  //$
				PDA_STATE_TRANSITION_ENTRY(7, -5)    //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, -5)    //TK_MACRO
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(7)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -3)  //$
				PDA_STATE_TRANSITION_ENTRY(7, -3)    //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, -3)    //TK_MACRO
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(8)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -1)  //$
				PDA_STATE_TRANSITION_ENTRY(7, 5)     //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, 6)     //TK_MACRO
				PDA_STATE_TRANSITION_ENTRY(103, 9)   //id
			PDA_END_STATE_TRANSITION()
			PDA_BEGIN_STATE_TRANSITION(9)
				PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -2)  //$
				PDA_STATE_TRANSITION_ENTRY(7, -2)    //TK_TOKEN
				PDA_STATE_TRANSITION_ENTRY(6, -2)    //TK_MACRO
			PDA_END_STATE_TRANSITION()
			//state
			PDA_BEGIN_STATE_SET()
				PDA_STATE_SET_ENTRY(PDA_STATE_START)
				PDA_STATE_SET_ENTRY(3)
				PDA_STATE_SET_ENTRY(4)
				PDA_STATE_SET_ENTRY(5)
				PDA_STATE_SET_ENTRY(6)
				PDA_STATE_SET_ENTRY(7)
				PDA_STATE_SET_ENTRY(8)
				PDA_STATE_SET_ENTRY(9)
			PDA_END_STATE_SET()
		PDA_END_TRAITS_STATE_MAP()

		// rule map
		PDA_BEGIN_TRAITS_RULE_MAP(_LDF_Lex_PdaTraits)
			PDA_RULE_ENTRY(0, 1)      // S -> lex_def $
			PDA_RULE_ENTRY(101, 2)    // lex_def -> TK_SEP rule_block
			PDA_RULE_ENTRY(102, 2)    // rule_block -> rule_block id
			PDA_RULE_ENTRY(102, 1)    // rule_block -> id
			PDA_RULE_ENTRY(103, 1)    // id -> TK_TOKEN
			PDA_RULE_ENTRY(103, 1)    // id -> TK_MACRO
		PDA_END_TRAITS_RULE_MAP()
	};

	typedef PushDownMachineT<_LDF_lex_SymUserData, _LDF_Lex_PdaTraits> _LDF_Lex_PDM;

	// grammar actions
	class DoIdTokenMacroAction : public IGrammarAction<_LDF_lex_SymUserData>
	{
	public:
		DoIdTokenMacroAction(TokenTable& tt, SharedArray<StringA>& arr) throw() : m_tt(tt), m_arr(arr), m_uCurrentID(TK_FIRST)
		{
		}
		virtual void DoAction(INOUT SharedArray<RefPtr<_LDF_lex_SymUserData>>& arr, INOUT SharedArray<StringS>& errorArray)
		{
			StringA& strToken = arr[1].get_Value().Deref().GetBuffer();
			ConstStringA cs_token = StringUtilHelper::To_ConstString(strToken);
			if( m_tt.GetTokenID(cs_token) > 0 ) {
				StringS strError(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				CplErrorBuffer tmp;
				int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), CplErrorBuffer::c_size,
										_S("Error (%u) : (%u) %s redifinition"),
										arr[1].get_Value().Deref().GetCharStart().uRow + 1,
										arr[1].get_Value().Deref().GetCharStart().uCol + 1,
										ConstHelper::GetInternalPointer(CS_U2S(cs_token).GetC())
										);
				if( ret >= 0 )
					tmp.SetLength(ret);
				StringUtilHelper::MakeString(tmp, strError);  //may throw
				errorArray.Add(strError);
				return ;
			}
			uint uLeastCount = SafeOperators::AddThrow(m_uCurrentID, (uint)1);  //may throw
			if( m_arr.GetCount() < (uintptr)uLeastCount )
				m_arr.SetCount((uintptr)uLeastCount, 0);
			m_arr.SetAt(m_uCurrentID, arr[1].get_Value().Deref().GetAux());
			m_tt.InsertToken(cs_token, m_uCurrentID ++);
		}

	private:
		TokenTable&            m_tt;
		SharedArray<StringA>&  m_arr;
		uint m_uCurrentID;
	};

public:
	// _LDF_Lex_GrammarTable
	class _LDF_Lex_GrammarTable
	{
	public:
		//called only once
		void Init()
		{
			m_actTable.Init();  //may throw
			m_graTable.SetPDA(RefPtrHelper::TypeCast<_LDF_Lex_PDM, typename _LDF_Lex_PDM::baseClass>(RefPtr<_LDF_Lex_PDM>(m_pdm)));
			m_graTable.SetReductionActionTable(RefPtrHelper::ToRefPtr(m_actTable.GetTable()));
		}

		RefPtr<GrammarTable<_LDF_lex_SymUserData>> GetGrammarTable() throw()
		{
			return RefPtr<GrammarTable<_LDF_lex_SymUserData>>(m_graTable);
		}

	private:
		_LDF_Lex_ActionTable m_actTable;
		_LDF_Lex_PDM m_pdm;
		GrammarTable<_LDF_lex_SymUserData> m_graTable;
	};

	class _LDF_Lex_GrammarAction;
	// _LDF_Lex_GrammarData
	class _LDF_Lex_GrammarData
	{
	public:
		_LDF_Lex_GrammarData(TokenTable& tt) : m_tokenTable(tt),
											m_tokenRegex(SharedArrayHelper::MakeSharedArray<StringA>(MemoryHelper::GetCrtMemoryManager())),
											m_macroRegex(SharedArrayHelper::MakeSharedArray<StringA>(MemoryHelper::GetCrtMemoryManager()))
		{
		}

		SharedArray<StringA>& GetTokenRegex() throw()
		{
			return m_tokenRegex;
		}

		void ExpandTokenMacros()
		{
			for( auto iter(m_tokenRegex.GetBegin()); iter != m_tokenRegex.GetEnd(); iter.MoveNext() )
				expand_macro_one(iter.get_Value());
		}

	private:
		void expand_macro_one(StringA& str)
		{
			if( str.IsNull() )
				return ;

			//loop
			do {
				//find
				uintptr uCount = 0;
				uintptr uStart = 0;
				do {
					auto iter(StringUtilHelper::Find(str, '{', uStart));
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
						auto iterE(StringUtilHelper::Find(str, '}', uStart));
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
						StringA strMacro(StringUtilHelper::MakeEmptyString<CharA>(MemoryHelper::GetCrtMemoryManager()));
						StringUtilHelper::Sub(str, uLeftC, len - 1, strMacro);
						ConstStringA c_macro_name = StringUtilHelper::To_ConstString(strMacro);
						uint id = m_macroTable.GetTokenID(c_macro_name);
						StringA strV;
						if( id == 0 ) {
							id = m_tokenTable.GetTokenID(c_macro_name);
							if( id == 0 ) {
								uCount --;
								break;
							}
							strV = m_tokenRegex[id].get_Value();
						}
						else {
							strV = m_macroRegex[id].get_Value();
						}
						iter.get_Value() = '(';
						iterE.get_Value() = ')';
						StringUtilHelper::Delete(uLeftC, len - 1, str);
						StringUtilHelper::Insert(uLeftC, strV, str);
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
		TokenTable& m_tokenTable;
		TokenTable  m_macroTable;
		SharedArray<StringA> m_tokenRegex;  //regular expression
		SharedArray<StringA> m_macroRegex;  //regular expression

	private:
		friend class _LDF_Lex_GrammarAction;
	};

	// _LDF_Lex_GrammarAction
	class _LDF_Lex_GrammarAction
	{
	public:
		_LDF_Lex_GrammarAction(_LDF_Lex_GrammarData& data) throw() : m_actionDoIdToken(data.m_tokenTable, data.m_tokenRegex),
																	m_actionDoIdMacro(data.m_macroTable, data.m_macroRegex)
		{
		}

		void Apply(GrammarParser<_LDF_lex_SymUserData>& graParser)
		{
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"),
								RefPtrHelper::TypeCast<DoIdTokenMacroAction, IGrammarAction<_LDF_lex_SymUserData>>(RefPtr<DoIdTokenMacroAction>(m_actionDoIdToken)));
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"),
								RefPtrHelper::TypeCast<DoIdTokenMacroAction, IGrammarAction<_LDF_lex_SymUserData>>(RefPtr<DoIdTokenMacroAction>(m_actionDoIdMacro)));
		}

	private:
		DoIdTokenMacroAction m_actionDoIdToken;
		DoIdTokenMacroAction m_actionDoIdMacro;
	};

	// _LDF_lex_GrammarParser
	typedef _LDF_GrammarParser<_LDF_lex_SymUserData>  _LDF_lex_GrammarParser;

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
		// state map
		FSA_BEGIN_TRAITS_STATE_MAP(_LDF_regex_FsaTraits)
			//transitions
			FSA_BEGIN_STATE_TRANSITION(FSA_STATE_START)
				FSA_STATE_TRANSITION_ENTRY('\\', 2)
				FSA_STATE_TRANSITION_ENTRY('[', 3)
				FSA_STATE_TRANSITION_ENTRY(']', 4)
				FSA_STATE_TRANSITION_ENTRY('-', 5)
				FSA_STATE_TRANSITION_ENTRY('^', 6)
				FSA_STATE_TRANSITION_ENTRY('?', 7)
				FSA_STATE_TRANSITION_ENTRY('*', 8)
				FSA_STATE_TRANSITION_ENTRY('+', 9)
				FSA_STATE_TRANSITION_ENTRY('|', 10)
				FSA_STATE_TRANSITION_ENTRY('(', 11)
				FSA_STATE_TRANSITION_ENTRY(')', 12)
				FSA_STATE_TRANSITION_RANGE_ENTRY(0x21, 0x7E, 13)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(2)
				FSA_STATE_TRANSITION_ENTRY('\\', 14)
				FSA_STATE_TRANSITION_ENTRY('r', 15)
				FSA_STATE_TRANSITION_ENTRY('n', 16)
				FSA_STATE_TRANSITION_ENTRY('t', 17)
				FSA_STATE_TRANSITION_ENTRY('s', 18)
				FSA_STATE_TRANSITION_ENTRY('[', 19)
				FSA_STATE_TRANSITION_ENTRY(']', 20)
				FSA_STATE_TRANSITION_ENTRY('(', 21)
				FSA_STATE_TRANSITION_ENTRY(')', 22)
				FSA_STATE_TRANSITION_ENTRY('{', 23)
				FSA_STATE_TRANSITION_ENTRY('}', 24)
				FSA_STATE_TRANSITION_ENTRY('?', 25)
				FSA_STATE_TRANSITION_ENTRY('*', 26)
				FSA_STATE_TRANSITION_ENTRY('+', 27)
				FSA_STATE_TRANSITION_ENTRY('-', 28)
				FSA_STATE_TRANSITION_ENTRY('^', 29)
				FSA_STATE_TRANSITION_ENTRY('|', 30)
				FSA_STATE_TRANSITION_ENTRY('x', 31)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(3)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(4)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(5)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(6)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(7)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(8)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(9)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(10)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(11)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(12)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(13)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(14)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(15)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(16)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(17)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(18)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(19)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(20)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(21)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(22)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(23)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(24)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(25)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(26)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(27)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(28)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(29)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(30)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(31)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 32)
				FSA_STATE_TRANSITION_RANGE_ENTRY('A', 'F', 32)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'f', 32)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(32)
				FSA_STATE_TRANSITION_RANGE_ENTRY('0', '9', 33)
				FSA_STATE_TRANSITION_RANGE_ENTRY('A', 'F', 33)
				FSA_STATE_TRANSITION_RANGE_ENTRY('a', 'f', 33)
			FSA_END_STATE_TRANSITION()
			FSA_BEGIN_STATE_TRANSITION(33)
			FSA_END_STATE_TRANSITION()

			//state
			FSA_BEGIN_STATE_SET()
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, FSA_STATE_START, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 2, -29)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 3, -19)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 4, -20)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 5, -21)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 6, -22)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 7, -23)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 8, -24)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 9, -25)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 10, -26)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 11, -27)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 12, -28)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 13, -29)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 14, -1)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 15, -3)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 16, -4)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 17, -5)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 18, -6)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 19, -7)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 20, -8)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 21, -9)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 22, -10)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 23, -11)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 24, -12)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 25, -13)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 26, -14)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 27, -15)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 28, -16)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 29, -17)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 30, -18)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 31, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 32, 100)
				FSA_STATE_SET_ENTRY(FSA_STATE_STOP, 33, -2)
			FSA_END_STATE_SET()
		FSA_END_TRAITS_STATE_MAP()

		// match map
		FSA_BEGIN_TRAITS_MATCH_MAP(_LDF_regex_FsaTraits)
			FSA_STATE_MATCH_ENTRY(0)
			FSA_STATE_MATCH_ENTRY(1)   //TK_CHAR_BACKSLASH
			FSA_STATE_MATCH_ENTRY(2)   //TK_CHAR_HEX
			FSA_STATE_MATCH_ENTRY(3)   //TK_CHAR_CR
			FSA_STATE_MATCH_ENTRY(4)   //TK_CHAR_LN
			FSA_STATE_MATCH_ENTRY(5)   //TK_CHAR_TAB
			FSA_STATE_MATCH_ENTRY(6)   //TK_CHAR_SPACE
			FSA_STATE_MATCH_ENTRY(7)   //TK_CHAR_LBRACKET
			FSA_STATE_MATCH_ENTRY(8)   //TK_CHAR_RBRACKET
			FSA_STATE_MATCH_ENTRY(9)   //TK_CHAR_LPAREN
			FSA_STATE_MATCH_ENTRY(10)  //TK_CHAR_RPAREN
			FSA_STATE_MATCH_ENTRY(11)  //TK_CHAR_LCURLY
			FSA_STATE_MATCH_ENTRY(12)  //TK_CHAR_RCURLY
			FSA_STATE_MATCH_ENTRY(13)  //TK_CHAR_QUESTION
			FSA_STATE_MATCH_ENTRY(14)  //TK_CHAR_STAR
			FSA_STATE_MATCH_ENTRY(15)  //TK_CHAR_PLUS
			FSA_STATE_MATCH_ENTRY(16)  //TK_CHAR_MINUS
			FSA_STATE_MATCH_ENTRY(17)  //TK_CHAR_UPARROW
			FSA_STATE_MATCH_ENTRY(18)  //TK_CHAR_VERT
			FSA_STATE_MATCH_ENTRY(19)  //TK_LBRACKET
			FSA_STATE_MATCH_ENTRY(20)  //TK_RBRACKET
			FSA_STATE_MATCH_ENTRY(21)  //TK_MINUS
			FSA_STATE_MATCH_ENTRY(22)  //TK_UPARROW
			FSA_STATE_MATCH_ENTRY(23)  //TK_QUESTION
			FSA_STATE_MATCH_ENTRY(24)  //TK_STAR
			FSA_STATE_MATCH_ENTRY(25)  //TK_PLUS
			FSA_STATE_MATCH_ENTRY(26)  //TK_VERT
			FSA_STATE_MATCH_ENTRY(27)  //TK_LPAREN
			FSA_STATE_MATCH_ENTRY(28)  //TK_RPAREN
			FSA_STATE_MATCH_ENTRY(29)  //TK_CHAR
		FSA_END_TRAITS_MATCH_MAP()
	};

	typedef FiniteStateMachineT<_LDF_regex_FsaTraits>  _LDF_REGEX_FSM;

	// actions
	class CharAction : public ILexerAction
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
				string_to_value(SharedArrayHelper::GetInternalPointer(str) + 2, 16, v, bOK);
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
			m_lexTable.SetTokenTable(RefPtrHelper::ToRefPtr(m_tokenTable.GetTable()));
			m_lexParser.SetLexerTable(RefPtrHelper::ToRefPtr(m_lexTable));
			//actions
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_BACKSLASH"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_HEX"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_CR"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LN"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_TAB"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_SPACE"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LBRACKET"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RBRACKET"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LPAREN"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RPAREN"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_LCURLY"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_RCURLY"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_QUESTION"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_STAR"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_PLUS"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_MINUS"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_UPARROW"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
			m_lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_CHAR_VERT"),
								RefPtrHelper::TypeCast<CharAction, ILexerAction>(RefPtr<CharAction>(m_actionChar)));
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
		CharAction m_actionChar;
	};

public:
	// _LDF_regex_AST
	class _LDF_regex_AST
	{
	public:
		_LDF_regex_AST() throw()
		{
		}

		//no cleanup
		void Generate(const SharedArray<StringA>& regex)
		{
			for( auto iter(regex.GetBegin()); iter != regex.GetEnd(); iter.MoveNext() )
				generate_one(iter.get_Value());
		}

	private:
		void generate_one(const StringA& str)
		{
			if( str.IsNull() )
				return ;

			CallResult cr;
			ConstArray<byte> buffer;
			ConstHelper::SetInternalPointer((byte*)SharedArrayHelper::GetInternalPointer(str), str.GetLength(), buffer);
			//stream
			MemoryCharStream stream;
			stream.Initialize(buffer);

			//lexer parser
			_LDF_regex_LexerParser lexer;
			lexer.Init();
			lexer.SetStream(RefPtrHelper::TypeCast<MemoryCharStream, ICharStream>(RefPtr<MemoryCharStream>(stream)));

			//grammar parser
			//_LDF_regex_GrammarParser parser;

		}

	private:
		SharedArray<AstTree> m_trees;
		DataPoolAllocator m_alloc;
	};

//------------------------------------------------------------------------------
//for gra file

};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //___LDF_BASE_H__
////////////////////////////////////////////////////////////////////////////////
