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

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LDF_TABLE_H__
////////////////////////////////////////////////////////////////////////////////
