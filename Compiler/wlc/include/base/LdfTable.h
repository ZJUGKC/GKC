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

#include "_ldf_base.h"

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
public:
	LdfTableAnalyzer() throw()
	{
	}
	~LdfTableAnalyzer() throw()
	{
	}

	//called only once
	void Init()
	{
		m_lexer.Init();

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
	CallResult process_lex_file(IN const ConstStringS& strLexFile, OUT TokenTable& tokenTable, OUT FsaTableInPool& fsa)
	{
		CallResult cr;

		//stream
		FileCharStream stream;
		cr = stream.Initialize(StringUtilHelper::To_C_Style(strLexFile));
		if( cr.IsFailed() )
			return cr;

		//lexer parser
		m_lexer.SetStream(RefPtrHelper::TypeCast<FileCharStream, ICharStream>(RefPtr<FileCharStream>(stream)));

		//grammar parser
		_ldf_helper::_LDF_GrammarParser parser;
		parser.SetLexerParser(m_lexer.GetLexerParser());
		// grammar table
		_ldf_helper::_LDF_Lex_GrammarTable graTable;
		graTable.Init();
		parser.SetGrammarTable(graTable.GetGrammarTable());
		// data
		_ldf_helper::_LDF_Lex_GrammarData data(tokenTable);
		// action
		_ldf_helper::_LDF_Lex_GrammarAction action(data);
		action.Apply(parser.GetGrammarParser());

		//execute
		cr = parser.Execute();
		if( cr.IsFailed() )
			return cr;

		//expand macros
		data.ExpandTokenMacros();

		cr.SetResult(SystemCallResults::OK);
		return cr;
	}

private:
	//lexer parser
	_ldf_helper::_LDF_LexerParser m_lexer;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
