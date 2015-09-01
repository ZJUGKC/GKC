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
This file contains classes for Tables of Language Definition File.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LDF_TABLE_H__
#define __LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////

#include "parser.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// FsaTableInPool
//   FSA tables are defined in a pool

class FsaTableInPool
{
public:
	FsaTableInPool() throw() : m_pState(NULL), m_iMaxState(0), m_pMatch(NULL), m_iMaxMatch(0)
	{
	}
	~FsaTableInPool() throw()
	{
	}

	//state: 1---iMaxState
	//transition: the count of arrTransitionNum is iMaxState + 1
	//match: 0---iMaxMatch
	void Allocate(int iMaxState, const int* arrTransitionNum, int iMaxMatch)
	{
		assert( iMaxState > 1 && iMaxMatch > 0 );
		assert( m_pState == NULL && m_pMatch == NULL );
		//transition number
		int total_transition_num = 0;
		for( int i = 1; i <= iMaxState; i ++ ) {
			assert( arrTransitionNum[i] > 0 );
			total_transition_num += arrTransitionNum[i];
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxState + 1, (uint)sizeof(FSA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxMatch + 1, (uint)sizeof(FSA_MATCH_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(FSA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_alloc.Allocate(uBytes);  //may throw
		//pointers
		m_pState = (FSA_STATE_ITEM*)m_alloc.ToPtr(uStart);
		m_pState[0].iDefaultState = 0;
		m_pState[0].pTransition = NULL;
		m_pState[0].iMatchIndex = 0;
		m_pMatch = (FSA_MATCH_ITEM*)((byte*)m_pState + sizeof(FSA_STATE_ITEM) * (iMaxState + 1));
		m_pMatch[0].iMatch = 0;
		FSA_TRANSITION_ITEM* pItem = (FSA_TRANSITION_ITEM*)((byte*)m_pMatch + sizeof(FSA_MATCH_ITEM) * (iMaxMatch + 1));
		for( int i = 1; i <= iMaxState; i ++ ) {
			m_pState[i].pTransition = pItem;
			pItem += arrTransitionNum[i];
		}
		//parameters
		m_iMaxState = iMaxState;
		m_iMaxMatch = iMaxMatch;
	}
	//set state
	void SetState(int iState, int iDefaultState, int iMatchIndex) throw()
	{
		assert( iState > 0 && iState <= m_iMaxState );
		assert( m_pState != NULL );
		m_pState[iState].iDefaultState = iDefaultState;
		m_pState[iState].iMatchIndex = iMatchIndex;
	}
	//set match table
	void SetMatch(int iMatchIndex, int iMatch) throw()
	{
		assert( iMatchIndex > 0 && iMatchIndex <= m_iMaxMatch );
		assert( m_pMatch != NULL );
		m_pMatch[iMatchIndex].iMatch = iMatch;
	}

	const FSA_STATE_ITEM* GetStateTable(int& iMaxStateNo) const throw()
	{
		iMaxStateNo = m_iMaxState;
		return m_pState;
	}
	const FSA_MATCH_ITEM* GetMatchTable(int& iMaxMatchNo) const throw()
	{
		iMaxMatchNo = m_iMaxMatch;
		return m_pMatch;
	}

private:
	DataPoolAllocator m_alloc;  //pool
	//tables
	FSA_STATE_ITEM* m_pState;
	int m_iMaxState;
	FSA_MATCH_ITEM* m_pMatch;
	int m_iMaxMatch;

private:
	//noncopyable
};

// PdaTableInPool
//   PDA tables are defined in a pool

class PdaTableInPool
{
public:
	PdaTableInPool() throw() : m_pState(NULL), m_iMaxState(0), m_pRule(NULL), m_iMaxRule(0)
	{
	}
	~PdaTableInPool() throw()
	{
	}

	//state: 2---iMaxState
	//transition: the count of arrTransitionNum is iMaxState + 1
	//rule: 0---iMaxRule
	void Allocate(int iMaxState, const int* arrTransitionNum, int iMaxRule)
	{
		assert( iMaxState > 2 && iMaxRule > 0 );
		assert( m_pState == NULL && m_pRule == NULL );
		//transition number
		int total_transition_num = 0;
		for( int i = 2; i <= iMaxState; i ++ ) {
			assert( arrTransitionNum[i] > 0 );
			total_transition_num += arrTransitionNum[i];
		}
		//allocate
		uint uBytes, uBytes1, uBytes2;
		uBytes1 = SafeOperators::MultiplyThrow((uint)iMaxState + 1, (uint)sizeof(PDA_STATE_ITEM));  //may throw
		uBytes2 = SafeOperators::MultiplyThrow((uint)iMaxRule + 1, (uint)sizeof(PDA_RULE_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOperators::MultiplyThrow((uint)total_transition_num, (uint)sizeof(PDA_TRANSITION_ITEM));  //may throw
		uBytes  = SafeOperators::AddThrow(uBytes, uBytes1);  //may throw
		uint uStart = m_alloc.Allocate(uBytes);  //may throw
		//pointers
		m_pState = (PDA_STATE_ITEM*)m_alloc.ToPtr(uStart);
		m_pState[0].iDefaultState = 0;
		m_pState[0].pTransition = NULL;
		m_pState[1].iDefaultState = 0;
		m_pState[1].pTransition = NULL;
		m_pRule = (PDA_RULE_ITEM*)((byte*)m_pState + sizeof(PDA_STATE_ITEM) * (iMaxState + 1));
		m_pRule[0].uLeftEventNo = 0;
		m_pRule[0].uRightSymbolNumber = 0;
		PDA_TRANSITION_ITEM* pItem = (PDA_TRANSITION_ITEM*)((byte*)m_pRule + sizeof(PDA_RULE_ITEM) * (iMaxRule + 1));
		for( int i = 2; i <= iMaxState; i ++ ) {
			m_pState[i].pTransition = pItem;
			pItem += arrTransitionNum[i];
		}
		//parameters
		m_iMaxState = iMaxState;
		m_iMaxRule  = iMaxRule;
	}

	//set state
	void SetState(int iState, int iDefaultState) throw()
	{
		assert( iState > 1 && iState <= m_iMaxState );
		assert( m_pState != NULL );
		m_pState[iState].iDefaultState = iDefaultState;
	}
	//set rule
	void SetRule(int iRule, uint uLeftEventNo, uint uRightSymbolNumber) throw()
	{
		assert( iRule > 0 && iRule <= m_iMaxRule );
		assert( m_pRule != NULL );
		m_pRule[iRule].uLeftEventNo = uLeftEventNo;
		m_pRule[iRule].uRightSymbolNumber = uRightSymbolNumber;
	}

	const PDA_STATE_ITEM* GetStateTable(int& iMaxStateNo) const throw()
	{
		iMaxStateNo = m_iMaxState;
		return m_pState;
	}
	const PDA_RULE_ITEM* GetRuleTable(int& iMaxRuleNo) const throw()
	{
		iMaxRuleNo = m_iMaxRule;
		return m_pRule;
	}

private:
	DataPoolAllocator m_alloc;  //pool
	//tables
	PDA_STATE_ITEM* m_pState;
	int m_iMaxState;
	PDA_RULE_ITEM*  m_pRule;
	int m_iMaxRule;

private:
	//noncopyable
};

// LdfTableAnalyzer

class LdfTableAnalyzer
{
private:
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

	//FSA for lex & grammar file
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

public:
	LdfTableAnalyzer() throw()
	{
	}
	~LdfTableAnalyzer() throw()
	{
	}

	void Init()
	{
		m_ldfTokenTable.Init();  //may throw
		m_lexTable.SetFSA(RefPtrHelper::TypeCast<FiniteStateMachineT<_LDF_FsaTraits>, FiniteStateAutomata>(RefPtr<FiniteStateMachineT<_LDF_FsaTraits>>(m_fsm)));
		m_lexTable.SetTokenTable(RefPtrHelper::ToRefPtr(m_ldfTokenTable.GetTable()));

	}

	CallResult Process(IN const ConstStringS& strLexFile, IN const ConstStringS& strGraFile,
					OUT TokenTable& tokenTable, OUT FsaTableInPool& fsa,
					OUT TokenTable& actionTable, OUT PdaTableInPool& pda
					)
	{
		CallResult cr;

		cr = process_lex_file(strLexFile, tokenTable, fsa);
		if( cr.IsFailed() )
			return cr;

		return cr;
	}

private:
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

	//reduction action table for lex file
	class _LDF_LexActionTable
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

	//PDA for lex file
	class _LDF_LexPdaTraits
	{
	public:
		// state map
		PDA_BEGIN_TRAITS_STATE_MAP(_LDF_LexPdaTraits)
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
		PDA_BEGIN_TRAITS_RULE_MAP(_LDF_LexPdaTraits)
			PDA_RULE_ENTRY(0, 1)      // S -> lex_def $
			PDA_RULE_ENTRY(101, 2)    // lex_def -> TK_SEP rule_block
			PDA_RULE_ENTRY(102, 2)    // rule_block -> rule_block id
			PDA_RULE_ENTRY(102, 1)    // rule_block -> id
			PDA_RULE_ENTRY(103, 1)    // id -> TK_TOKEN
			PDA_RULE_ENTRY(103, 1)    // id -> TK_MACRO
		PDA_END_TRAITS_RULE_MAP()
	};

	//SymUserData
	class SymUserData
	{
	public:
	};

	// grammar actions
	class DoIdTokenAction : public IGrammarAction<SymUserData>
	{
	public:
		virtual void DoAction(INOUT SharedArray<RefPtr<SymbolDataT<SymUserData>>>& arr, INOUT SharedArray<StringS>& errorArray)
		{
		}
	};
	class DoIdMacroAction : public IGrammarAction<SymUserData>
	{
	public:
		virtual void DoAction(INOUT SharedArray<RefPtr<SymbolDataT<SymUserData>>>& arr, INOUT SharedArray<StringS>& errorArray)
		{
		}
	};

	CallResult process_lex_file(IN const ConstStringS& strLexFile, OUT TokenTable& tokenTable, OUT FsaTableInPool& fsa)
	{
		CallResult cr;

		//stream
		FileCharStream stream;
		cr = stream.Initialize(StringUtilHelper::To_C_Style(strLexFile));
		if( cr.IsFailed() )
			return cr;

		//lexer parser
		LexerParser lexParser;
		lexParser.SetLexerTable(RefPtrHelper::ToRefPtr(m_lexTable));
		lexParser.SetStream(RefPtrHelper::TypeCast<FileCharStream, ICharStream>(RefPtr<FileCharStream>(stream)));
		//actions
		CommentStartAction actionCommentStart;
		SpaceAction actionSpace;
		ReturnAction actionReturn;
		MacroTokenAction actionMacroToken;
		lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COMMENT_START"),
							RefPtrHelper::TypeCast<CommentStartAction, ILexerAction>(RefPtr<CommentStartAction>(actionCommentStart)));
		lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SPACE"),
							RefPtrHelper::TypeCast<SpaceAction, ILexerAction>(RefPtr<SpaceAction>(actionSpace)));
		lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RETURN"),
							RefPtrHelper::TypeCast<ReturnAction, ILexerAction>(RefPtr<ReturnAction>(actionReturn)));
		lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"),
							RefPtrHelper::TypeCast<MacroTokenAction, ILexerAction>(RefPtr<MacroTokenAction>(actionMacroToken)));
		lexParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"),
							RefPtrHelper::TypeCast<MacroTokenAction, ILexerAction>(RefPtr<MacroTokenAction>(actionMacroToken)));

		//action table
		_LDF_LexActionTable ldfActionTable;
		ldfActionTable.Init();  //may throw
		//PDA
		typedef PushDownMachineT<typename GrammarTable<SymUserData>::SymbolClass, _LDF_LexPdaTraits>  PdmClass;
		PdmClass pdm;
		//grammar table
		GrammarTable<SymUserData> graTable;
		graTable.SetPDA(RefPtrHelper::TypeCast<PdmClass, typename PdmClass::baseClass>(RefPtr<PdmClass>(pdm)));
		graTable.SetReductionActionTable(RefPtrHelper::ToRefPtr(ldfActionTable.GetTable()));
		//grammar parser
		GrammarParser<SymUserData> graParser;
		graParser.SetLexerParser(RefPtrHelper::ToRefPtr(lexParser));
		graParser.SetGrammarTable(RefPtrHelper::ToRefPtr(graTable));
		//actions
		DoIdTokenAction actionDoIdToken;
		DoIdMacroAction actionDoIdMacro;
		graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"),
							RefPtrHelper::TypeCast<DoIdTokenAction, IGrammarAction<SymUserData>>(RefPtr<DoIdTokenAction>(actionDoIdToken)));
		graParser.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"),
							RefPtrHelper::TypeCast<DoIdMacroAction, IGrammarAction<SymUserData>>(RefPtr<DoIdMacroAction>(actionDoIdMacro)));

		//loop
		graParser.Start();
		bool bEmpty;
		cr.SetResult(SystemCallResults::OK);
		while( cr.GetResult() != SystemCallResults::S_False ) {
			cr = graParser.Parse(bEmpty);
			if( cr.IsFailed() )
				break;
		}
		if( cr.IsFailed() )
			return cr;
		if( graParser.GetErrorArray().GetCount() > 0 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}

		cr.SetResult(SystemCallResults::OK);
		return cr;
	}

private:
	//token table
	_LDF_TokenTable m_ldfTokenTable;
	//FSA
	FiniteStateMachineT<_LDF_FsaTraits> m_fsm;
	//lexer table
	LexerTable m_lexTable;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
