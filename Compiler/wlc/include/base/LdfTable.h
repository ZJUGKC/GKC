﻿/*
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
		uBytes1 = SafeOpertaor::MultiplyThrow((uint)iMaxState + 1, (uint)sizeof(FSA_STATE_ITEM));  //may throw
		uBytes2 = SafeOpertaor::MultiplyThrow((uint)iMaxMatch + 1, (uint)sizeof(FSA_MATCH_ITEM));  //may throw
		uBytes = SafeOpertaor::AddThrow(uBytes1, uBytes2);  //may throw
		uBytes1 = SafeOpertaor::MultiplyThrow((uint)total_transition_num, (uint)sizeof(FSA_TRANSITION_ITEM));  //may throw
		uBytes = SafeOpertaor::AddThrow(uBytes, uBytes1);  //may throw
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
		assert( iState > 0 );
		assert( m_pState != NULL );
		m_pState[iState].iDefaultState = iDefaultState;
		m_pState[iState].iMatchIndex = iMatchIndex;
	}
	//set match table
	void SetMatch(int iMatchIndex, int iMatch) throw()
	{
		assert( iMatchIndex > 0 );
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

// LdfTableHelper

class LdfTableHelper
{
private:
	//FSA for lex & grammar file
	class _LDF_FsaTraits
	{
	public:
		// state map
		BEGIN_FSA_TRAITS_STATE_MAP(_LDF_FsaTraits)
			//transitions
			BEGIN_STATE_TRANSITION(FSA_STATE_START)
				STATE_TRANSITION_ENTRY('/', 2)
				STATE_TRANSITION_ENTRY(' ', 4)
				STATE_TRANSITION_ENTRY('\t', 4)
				STATE_TRANSITION_ENTRY('\r', 5)
				STATE_TRANSITION_ENTRY('\n', 7)
				STATE_TRANSITION_ENTRY('%', 8)
				STATE_TRANSITION_ENTRY('{', 10)
				STATE_TRANSITION_ENTRY('}', 11)
				STATE_TRANSITION_ENTRY('T', 12)
				STATE_TRANSITION_ENTRY('d', 16)
				STATE_TRANSITION_RANGE_ENTRY('a', 'c', 17)
				STATE_TRANSITION_RANGE_ENTRY('e', 'z', 17)
				STATE_TRANSITION_ENTRY(':', 21)
				STATE_TRANSITION_ENTRY('|', 22)
				STATE_TRANSITION_ENTRY(';', 23)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(2)
				STATE_TRANSITION_ENTRY('*', 3)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(4)
				STATE_TRANSITION_ENTRY(' ', 4)
				STATE_TRANSITION_ENTRY('\t', 4)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(5)
				STATE_TRANSITION_ENTRY('\n', 6)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(8)
				STATE_TRANSITION_ENTRY('%', 9)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(12)
				STATE_TRANSITION_ENTRY('K', 13)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(13)
				STATE_TRANSITION_ENTRY('_', 14)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(14)
				STATE_TRANSITION_RANGE_ENTRY('A', 'Z', 15)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(15)
				STATE_TRANSITION_RANGE_ENTRY('A', 'Z', 15)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 15)
				STATE_TRANSITION_ENTRY('_', 15)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(16)
				STATE_TRANSITION_RANGE_ENTRY('a', 'n', 17)
				STATE_TRANSITION_RANGE_ENTRY('p', 'z', 17)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				STATE_TRANSITION_ENTRY('_', 17)
				STATE_TRANSITION_ENTRY('o', 18)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(17)
				STATE_TRANSITION_RANGE_ENTRY('a', 'z', 17)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				STATE_TRANSITION_ENTRY('_', 17)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(18)
				STATE_TRANSITION_RANGE_ENTRY('a', 'z', 17)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				STATE_TRANSITION_ENTRY('_', 19)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(19)
				STATE_TRANSITION_RANGE_ENTRY('a', 'z', 20)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 17)
				STATE_TRANSITION_ENTRY('_', 17)
			END_STATE_TRANSITION()
			BEGIN_STATE_TRANSITION(20)
				STATE_TRANSITION_RANGE_ENTRY('a', 'z', 20)
				STATE_TRANSITION_RANGE_ENTRY('0', '9', 20)
				STATE_TRANSITION_ENTRY('_', 20)
			END_STATE_TRANSITION()

			//state
			BEGIN_STATE_SET()
				STATE_SET_ENTRY(FSA_STATE_STOP, FSA_STATE_START, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 2, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 3, -1)
				STATE_SET_ENTRY(FSA_STATE_STOP, 4, -2)
				STATE_SET_ENTRY(FSA_STATE_STOP, 5, -3)
				STATE_SET_ENTRY(FSA_STATE_STOP, 6, -3)
				STATE_SET_ENTRY(FSA_STATE_STOP, 7, -3)
				STATE_SET_ENTRY(FSA_STATE_STOP, 8, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 9, -4)
				STATE_SET_ENTRY(FSA_STATE_STOP, 10, -8)
				STATE_SET_ENTRY(FSA_STATE_STOP, 11, -9)
				STATE_SET_ENTRY(FSA_STATE_STOP, 12, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 13, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 14, 100)
				STATE_SET_ENTRY(FSA_STATE_STOP, 15, -7)
				STATE_SET_ENTRY(FSA_STATE_STOP, 16, -6)
				STATE_SET_ENTRY(FSA_STATE_STOP, 17, -6)
				STATE_SET_ENTRY(FSA_STATE_STOP, 18, -6)
				STATE_SET_ENTRY(FSA_STATE_STOP, 19, -6)
				STATE_SET_ENTRY(FSA_STATE_STOP, 20, -5)
				STATE_SET_ENTRY(FSA_STATE_STOP, 21, -10)
				STATE_SET_ENTRY(FSA_STATE_STOP, 22, -11)
				STATE_SET_ENTRY(FSA_STATE_STOP, 23, -12)
			END_STATE_SET()
		END_FSA_TRAITS_STATE_MAP()

		// match map
		BEGIN_FSA_TRAITS_MATCH_MAP(_LDF_FsaTraits)
			STATE_MATCH_ENTRY(0)
			STATE_MATCH_ENTRY(-1)   //TK_COMMENT_START
			STATE_MATCH_ENTRY(-2)   //TK_SPACE
			STATE_MATCH_ENTRY(-3)   //TK_RETURN
			STATE_MATCH_ENTRY(-4)   //TK_SEP
			STATE_MATCH_ENTRY(-5)   //TK_ACTION
			STATE_MATCH_ENTRY(-6)   //TK_MACRO
			STATE_MATCH_ENTRY(-7)   //TK_TOKEN
			STATE_MATCH_ENTRY(-8)   //TK_LCURLY
			STATE_MATCH_ENTRY(-9)   //TK_RCURLY
			STATE_MATCH_ENTRY(-10)  //TK_COLON
			STATE_MATCH_ENTRY(-11)  //TK_VERT
			STATE_MATCH_ENTRY(-12)  //TK_SEMI
		END_FSA_TRAITS_MATCH_MAP()
	};

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

public:
	//process lex file
	static void ProcessLexFile(const ConstringS& strFile)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
