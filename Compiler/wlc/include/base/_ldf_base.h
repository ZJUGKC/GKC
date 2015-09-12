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
	// _LDF_SymUserData
	class _LDF_SymUserData : public SymbolDataBase
	{
	public:
	};

public:
	// _LDF_GrammarParser
	class _LDF_GrammarParser
	{
	public:
		void SetLexerParser(const RefPtr<LexerParser>& parser) throw()
		{
			m_graParser.SetLexerParser(parser);
		}
		void SetGrammarTable(const RefPtr<GrammarTable<_LDF_SymUserData>>& table) throw()
		{
			m_graParser.SetGrammarTable(table);
		}

		GrammarParser<_LDF_SymUserData>& GetGrammarParser() throw()
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
		GrammarParser<_LDF_SymUserData> m_graParser;
	};

//------------------------------------------------------------------------------
//for lex file

private:
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

	typedef PushDownMachineT<_LDF_SymUserData, _LDF_Lex_PdaTraits> _LDF_Lex_PDM;

	// grammar actions
	class DoIdTokenMacroAction : public IGrammarAction<_LDF_SymUserData>
	{
	public:
		DoIdTokenMacroAction(TokenTable& tt, SharedArray<StringA>& arr) throw() : m_tt(tt), m_arr(arr), m_uCurrentID(TK_FIRST)
		{
		}
		virtual void DoAction(INOUT SharedArray<RefPtr<_LDF_SymUserData>>& arr, INOUT SharedArray<StringS>& errorArray)
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

		RefPtr<GrammarTable<_LDF_SymUserData>> GetGrammarTable() throw()
		{
			return RefPtr<GrammarTable<_LDF_SymUserData>>(m_graTable);
		}

	private:
		_LDF_Lex_ActionTable m_actTable;
		_LDF_Lex_PDM m_pdm;
		GrammarTable<_LDF_SymUserData> m_graTable;
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

		void Apply(GrammarParser<_LDF_SymUserData>& graParser)
		{
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"),
								RefPtrHelper::TypeCast<DoIdTokenMacroAction, IGrammarAction<_LDF_SymUserData>>(RefPtr<DoIdTokenMacroAction>(m_actionDoIdToken)));
			graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"),
								RefPtrHelper::TypeCast<DoIdTokenMacroAction, IGrammarAction<_LDF_SymUserData>>(RefPtr<DoIdTokenMacroAction>(m_actionDoIdMacro)));
		}

	private:
		DoIdTokenMacroAction m_actionDoIdToken;
		DoIdTokenMacroAction m_actionDoIdMacro;
	};

//------------------------------------------------------------------------------
//for gra file

};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //___LDF_BASE_H__
////////////////////////////////////////////////////////////////////////////////
